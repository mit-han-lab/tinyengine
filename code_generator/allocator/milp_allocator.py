# ----------------------------------------------------------------------
# Project: Right In-Place Convolution
# Title:   milp_allocator.py
#
# Reference papers:
#  - Right In-Place (RiP) Convolution: A Simple and General Strategy for Memory-Efficient CNN Inference, arXiv 2026
#    https://arxiv.org/abs/2610.00586
# Contact authors:
#  - Opegbemi Matthias Busoye, matthias@powerlabstech.com
#  - Tolulope Matthew Busoye, matthew@powerlabstech.com
#  - Eghonghon-aye Eigbe, eghonghon@powerlabstech.com
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

"""
MILP activation allocator
============================================================

Assigns every activation tensor a byte offset in the single SRAM buffer, so
that tensors alive at the same time never share an address and the peak
address touched is minimal. Replaces the greedy FirstFit pass, which reaches
the lower bound on MCUNet-shaped graphs but not on arbitrary ones.

Sets
----
    t, t1, t2   model.tensors                       one index per rectangle
    (t1, t2)    model.tensors_combination           unordered pairs, t1 < t2
    i           model.potential_inplace_tensors_in  inputs an op may overwrite
    o           model.potential_inplace_tensors_out outputs that may overwrite one

Constants, off the rectangle t unless noted
-------------------------------------------
    start_t     rec["start"]            layer t is produced at
    end_t       rec["end"]              layer t is last read at, end exclusive
    size_t      rec["size"]             bytes t holds
    gap_i       rec["gap"]              right-inplace headroom for i
    out_i       rec["inplace_tensor_out_idx"]   the output that overwrites i
    shift_i     rec["op_needs_right_shift"]     whether i's op needs its output
                                        below its input (conv) or shares the
                                        base with it (add, depthwise, upsample)
    N           local, from align_to_n_bytes     address alignment, (defualt: 4)
    M           local                   address big-M, the smaller of the SRAM
                                        and every rectangle stacked end to end
    L           local                   layer big-M, the last end
    C           self.RIGHT_SHIFT_COST   bytes of peak a byte of memmove is worth
    c           self.INPLACE_COST       bytes of peak a byte overwritten in place is worth

Decisions
---------
    x_t     model.placement                             byte offset of t
    a_t     model.placement_allign                      x_t in N-byte words
    z_t     model.effective_size                        bytes t occupies
    e_t     model.tensor_end                            layer t expires at
    Z       model.largest_space_in_memory               peak address touched
    b_t1t2  model.tensor_spatial_overlap_order          1 if t1 sits below t2
    p_t1t2  model.tensor_temporal_overlap               1 if alive together
    q_t1t2  model.tensor_spatial_overlap                1 if free to share an address
    d_i     model.decision_inplace_tensor_in            1 if i is overwritten
    r_i     model.decision_right_shift_inplace_tensor_in  1 if i is memmoved

Model, each row against the rule that builds it
-----------------------------------------------
    min  Z - sum_i c size_i d_i + sum_i C size_i r_i

    Z    >= x_t + z_t                        largest_space_in_memory_rule
    x_t   = N a_t                            data_alligned_rule
    z_t   = size_t                           effective_size_rule, t not a candidate
    z_i  >= size_i + gap_i r_i               effective_size_rule, candidate i, shift_i
    z_i  >= size_i + gap_i d_i               effective_size_rule, candidate i, not shift_i
    e_t   = end_t                            tensor_end_rule, t not a candidate
    e_i   = start_out_i d_i + end_i (1 - d_i)      tensor_end_rule, candidate i
    e_t1 - start_t2 <= L p_t1t2              no_temporal_overlap_rule
    p_t1t2 + q_t1t2 <= 1                     tensor_overlap_rule
    x_t1 + z_t1 <= x_t2 + M q_t1t2 + M(1 - b_t1t2)         no_spatial_overlap_rule
    x_t2 + z_t2 <= x_t1 + M q_t1t2 + M b_t1t2              no_spatial_overlap_rule
    sum_{i overwritten by o} d_i <= 1        inplace_tensor_decision_rule
    r_i  <= d_i                              inplace_tensor_right_shift_rule
    |x_out_i - xstart_i| <= M(1 - d_i)       inplace_tensor_placement_rule, as a
                                             big-M pair, where xstart_i is the
                                             address the output must start at:
        xstart_i = x_i - gap_i(1 - r_i)          shift_i, output below the input
        xstart_i = x_i                           not shift_i, output shares the base

tensor_overlap_fix_rule pins p, q and b on the pairs whose answer is already
known. A candidate's end is one of two layers rather than an unknown, so a pair
evaluated at both tells whether d can change the answer: disjoint even at their
longest, or overlapping even at their shortest, and it is decided. The ends move
by a single layer, so nearly every pair falls one way or the other, and a fixed
binary is substituted out before the solver sees it.

The two no_spatial_overlap_rule rows form a disjunction: whichever way b_t1t2
is set, one binds and the other goes slack, so t1 lies wholly below t2 or wholly
above it. An ILP cannot state "or" directly, hence the binary and the slack
constant.

M has to be as small as it can be while still switching those rows off. The
SRAM limit is the obvious candidate and the wrong one: it is a user setting, so
it can be fifty times any address the model will ever use, and a slack constant
that loose flattens the relaxation until the solver starts returning solutions
well off the optimum and reporting lower == upper on them, which reads as proof.

Whether two tensors are alive together is itself a decision, because a candidate
dies where its output begins only if that aliasing happens. So e_t is a variable,
p_t1t2 records the overlap, and p_t1t2 + q_t1t2 <= 1 turns it into the rule that
matters: never alive together and sharing an address. The two are opposites --
tensors that never coexist are precisely the ones that may reuse each other's
bytes, which is the whole of what the allocator is looking for. Rectangles are
registered in layer order, so tensors_combination is ordered by start too, and
no_temporal_overlap_rule needs only one row -- with start_t1 < start_t2, "t2 ends
before t1 starts" is impossible, and the test reduces to whether t1 is still
alive at start_t2. Layer 0 is the one place two tensors share a start, and there
neither direction can be ruled out.

Touching lifetimes leave the input dead at the very layer its buffer is being
read, which looks like a hole and is not one. Anything live at that layer either
started there, and the only tensor an op produces is the output itself, or
started earlier -- in which case it was live one layer before too, alongside the
input, and no_spatial_overlap_rule already holds it clear of the whole workspace.
The protection is written a layer early, not missing.

Which input an op overwrites is itself a decision. Most ops have one candidate,
but an Add reads both operands at an index before writing that index, so either
will do -- and the choice sets the peak, because a residual block that keeps its
result in the freshly written buffer leaves the older one free and settles one
slot higher than the block before it. Picking it here rather than by a rule
means it is solved for, not guessed.

The d_i sum is at most 1, not exactly 1, so the op may also decline and take a
buffer of its own. That is only safe because e_i grew a decision of its own: a
declined pair keeps the input alive through the layer, p_t1t2 sees the two
overlapping, and no_spatial_overlap_rule holds them apart. With a fixed lifetime
the same freedom would let a kernel overwrite its own input while still reading it.

How the pair is separated is the second decision, and only some ops have it.
shift_i says which. A convolution must start its output gap_i below where the
input is read from, and there are two ways to arrange that: memmove the input up
by gap_i and write the output from x_i (r_i = 1), or leave the input alone and
start the output at x_i - gap_i (r_i = 0). Both give the kernel the same
geometry. The first costs a copy, the second costs the freedom to place the
output, so r_i is priced into the objective at C per byte moved and the solver
pays for a memmove only where placing the output low would cost more peak than
the copy is worth. The kernel is told which it got, as the right_shift argument
beside gap_i, and shifts only under that flag.

The rest -- add, depthwise, upsample -- have no such choice: the output shares
the base, x_out_i = x_i, and r_i does not appear in their placement row at all.
An upsample still carries a gap_i, since its output is larger than its input and
the workspace runs that much past it, but the whole of it sits above x_i rather
than below.

That difference is what splits z_i in two. Where the output goes below, the input
reaches gap_i past its own bytes only when it is the input that moved, so z_i
follows r_i. Where the output shares the base, the workspace is above the input
whenever the aliasing happens, so z_i follows d_i instead. Both collapse to
size_i when the candidate is not the one overwritten: r_i <= d_i carries that for
the first, d_i is written out in the second, which has no r_i to imply it. Every
one of these rows would otherwise be a product of two decisions -- x_i d_i to
select an address, gap_i d_i r_i to size the workspace -- so each is a big-M pair
or an implication instead. An integer program is linear.

Both freedoms can be taken away, to measure what they are worth or to fall back
on the older behaviour. optimize_right_shift other than flexible_right_shift adds
inplace_tensor_always_right_shift_rule, pinning r_i per output: force_right_shift
separates every conv pair by a memmove as it was before r_i existed,
force_not_right_shift never moves one.
optimize_inplace_flexible=False adds not_inplace_flexible_rule, pinning each
candidate's lifetime to the aliased one so no op may decline its buffer.
"""

import os
import itertools
import pyomo.environ as pyo

from .base_allocator import BaseAllocator
from ..operators.basic_utils import op_inplace_type, RightShift

__all__ = ["MILPAllocator"]

# Any solver pyomo drives; the executable is looked up on PATH unless TINYENGINE_MILP_SOLVER_PATH
# names the directory it is in.
SOLVER = os.environ.get("TINYENGINE_MILP_SOLVER", "scip")
SOLVER_PATH = os.environ.get("TINYENGINE_MILP_SOLVER_PATH")

# What a byte of memmove is worth in bytes of peak. At 1e-4 a 154,880 byte copy
# prices at ~15 bytes, so the shift is taken unless placing the output low saves
# more. Raise it to trade peak for fewer copies, lower it for the reverse.
RIGHT_SHIFT_COST = 1e-4
# What a byte overwritten in place is worth in bytes of peak, so among layouts of the
# same peak the one aliasing the most is taken. Kept small: no MCUNet peak moved.
INPLACE_COST = 1e-5



class MILPAllocator(BaseAllocator):

    def __init__(
        self,
        SRAM,
        model_name="mcunet_model",
        optimize_right_shift=RightShift.flexible_right_shift,
        optimize_inplace_flexible=True,
        RIGHT_SHIFT_COST=RIGHT_SHIFT_COST,
        INPLACE_COST=INPLACE_COST,
        align_to_n_bytes=4
    ):
        super().__init__(SRAM, model_name=model_name, align_to_n_bytes=align_to_n_bytes)
        self.optimize_right_shift = optimize_right_shift
        self.optimize_inplace_flexible = optimize_inplace_flexible
        self.INPLACE_COST = INPLACE_COST
        self.RIGHT_SHIFT_COST = RIGHT_SHIFT_COST

    def define_model(self):
        """Build the model above over self.rectangles. See module docstring for notation."""
        N = self.align_to_n_bytes

        # Every rectangle stacked end to end is itself a schedule, so no address can
        # be higher, and nothing may exceed the SRAM either. See the module docstring
        # for why this is not just the SRAM limit.
        M = min(self.SRAM, sum(rec["size"] + rec.get("gap", 0) for rec in self.rectangles))
        # The layer big-M. No lifetime reaches past the last end, so e_t1 - start_t2
        # can never exceed it and this is enough to switch a temporal row off.
        L = max(rec["end"] for rec in self.rectangles)

        potential_inplace_inputs_register = dict()
        for t in range(len(self.rectangles)):
            potential_inplace_inputs = []
            idx = self.rectangles[t]["idx"]
            assert t == idx, f"Delete this dont forget before pushing"
            for i, rec in enumerate(self.rectangles):
                if rec["inplace"] != op_inplace_type.force_not_inplace and rec["inplace_tensor_out_idx"] == idx:
                    potential_inplace_inputs.append(i)
            potential_inplace_inputs_register[t] = potential_inplace_inputs

        # ==================================================================
        # Constraint rules
        # ==================================================================

        def data_alligned_rule(model, tensor_set):
            """x_t = N*a_t. Every tensor starts on an N-byte boundary."""
            constraint_list = list()
            for t in tensor_set:
                constraint_list.append(model.placement[t] == model.placement_allign[t] * N)
            return constraint_list
        
        def largest_space_in_memory_rule(model, tensor_set):
            """Z >= x_t + z_t. Z is pushed to the highest address any tensor reaches."""
            constraint_list = list()
            for t in tensor_set:
                constraint_list.append(model.largest_space_in_memory >= model.placement[t] + model.effective_size[t])
            return constraint_list

        def tensor_overlap_rule(model, tensor_set):
            """p_t1t2 + q_t1t2 <= 1. Either the two tensors are never alive together or they do not share an address."""
            constraint_list = list()
            for t1, t2 in tensor_set:
                constraint_list.append(
                    model.tensor_temporal_overlap[t1, t2] + model.tensor_spatial_overlap[t1, t2] <= 1
                )
            return constraint_list

        def tensor_overlap_fix_rule(model, tensor_set):
            """
            Set p, q and b where the answer is already known. A actual tensor's end might unknown because
            the inplace decision determines its actual end but we know the range of its end, we can use
            that to evaluate the potential of there being a guaranteed overlap or a guaranteed disjoint.
            These can be evaluated based the range of the ends which [short_end, long_end].
            """
            NO_POSSIBLE_INTERCEPTION = 0
            DEFINITE_INTERCEPTION = 1
            UNDETERMINABLE_INTERCEPTION = 2
            def short_long_end(t):
                inplace_tensor_out_idx = self.rectangles[t]["inplace_tensor_out_idx"]
                short_end = self.rectangles[inplace_tensor_out_idx]["start"] if inplace_tensor_out_idx is not None\
                        else self.rectangles[t]["end"]
                return short_end, self.rectangles[t]["end"]

            def tensors_pair_interception(t1, t2):
                # we know when the tensor is created and the longest time it dies so we can possibly
                # eliminate tensor pairs that will never intercept or are assumed to intercept
                start1 = self.rectangles[t1]["start"]
                short_end1, long_end1 = short_long_end(t1)
                start2 = self.rectangles[t2]["start"]
                short_end2, long_end2 = short_long_end(t2)

                if start1 != 0 and start2 != 0:
                    assert start1 != start2 , (
                        "With the current implementation, no 2 tensors should be created at "
                        f"at the same time, got {t1} and {t2} created at {start1}, with the "
                        "exception of the input at 0."
                    )
                if ((start1 < start2 and long_end1 <= start2) or (start2 < start1 and long_end2 <= start1)):
                    return NO_POSSIBLE_INTERCEPTION     # disjoint even at their longest
                if ((start1 < start2 and short_end1 > start2) or (start2 < start1 and short_end2 > start1)):
                    return DEFINITE_INTERCEPTION        # overlapping even at their shortest
                # Neither verdict held, so the pair overlaps on the long end and not on the short one.
                return UNDETERMINABLE_INTERCEPTION

            for t1, t2 in tensor_set:
                pair_interception = tensors_pair_interception(t1, t2)
                if pair_interception == NO_POSSIBLE_INTERCEPTION:
                    # The two tensors do not overlap temporally
                    model.tensor_temporal_overlap[t1, t2].fix(0)
                    # so they can be in either overlap spatially or not
                    model.tensor_spatial_overlap[t1, t2].fix(1)
                    # Nothing to order once the pair is free to share an address.
                    model.tensor_spatial_overlap_order[t1, t2].fix(0)
                if pair_interception == DEFINITE_INTERCEPTION:
                    # Alive together whatever the model decides, so they have to held
                    # apart. b_t1t2 stays free to choose which one sits underneath.
                    model.tensor_temporal_overlap[t1, t2].fix(1)
            return []

        def tensor_end_rule(model, tensor_set):
            """
            e_i = start_out_i d_i + end_i (1 - d_i). A tensor dies where its output begins if it is done inplace
            happens. If all the outputs of an input is not inplace then the end of the input is know and it is fixed.
            """
            constraint_list = list()
            for t in tensor_set:
                if t in model.potential_inplace_tensors_in:
                    inplace_tensor_out_idx = self.rectangles[t]["inplace_tensor_out_idx"]
                    constraint_list.append(
                        model.tensor_end[t] ==
                        self.rectangles[inplace_tensor_out_idx]["start"] * model.decision_inplace_tensor_in[t] +
                        self.rectangles[t]["end"] * (1- model.decision_inplace_tensor_in[t])
                    )
                else:
                    model.tensor_end[t].fix(self.rectangles[t]["end"])
            return constraint_list

        def not_inplace_flexible_rule(model, tensor_set):
            """sum_i d_i = 1 per output, so every op overwrites one of its inputs, forced to do inplace."""
            return [
                pyo.quicksum([
                    model.decision_inplace_tensor_in[i]
                    for i in potential_inplace_inputs_register[t]
                ]) == 1 for t in tensor_set
            ]

        def no_temporal_overlap_rule(model, tensor_set):
            """
            e_a - start_b <= L p_t1t2, where a is whichever of the pair starts first and b the
            other, so p_t1t2 = 0 (no temporal overlap) forces a to end before b starts.
            Because all tensors are created at different points, we only need to check the end,
            so b cannot end before a begins.
            """
            constraint_list = list()
            for t1, t2 in tensor_set:
                start1 = self.rectangles[t1]["start"]
                end1 = model.tensor_end[t1]
                start2 = self.rectangles[t2]["start"]
                end2 = model.tensor_end[t2]
                if start1 != 0 and start2 != 0:
                    assert start1 != start2 , (
                        "With the current implementation, no 2 tensors should be created at "
                        f"at the same time, got {t1} and {t2} created at {start1}, with the "
                        "exception of the input at 0."
                    )
                if start1 < start2:
                    constraint_list.append(
                        end1 - start2 <= L * model.tensor_temporal_overlap[t1, t2]
                    )
                elif start2 < start1:
                    constraint_list.append(
                        end2 - start1 <= L * model.tensor_temporal_overlap[t1, t2]
                    )
                else:  # start1 == start2 == 0, layer 0 only
                    # if end1 + end2 > 1; model.tensor_temporal_overlap[t1, t2] = 1
                    # else; model.tensor_temporal_overlap[t1, t2] = 0
                    assert start1 == 0 and start2 == 0, (
                        "both tensors should start at the begining of the model timeline, "
                        f"but got {t1} starting at {start1} and {t2} starting at {start2}"
                    )
                    constraint_list.append(
                        end1 + end2 <= 1 + 2 * L * model.tensor_temporal_overlap[t1, t2]
                    )
                    constraint_list.append(
                        end1 + end2 >= 2 - L * (1 - model.tensor_temporal_overlap[t1, t2])
                    )
            return constraint_list

        def no_spatial_overlap_rule(model, tensor_set):
            """
            x_t1 + z_t1 <= x_t2 + M q_t1t2 + M(1 - b_t1t2), and the mirror.
            q_t1t2 = 1 mean both tensor can share the same working memory.
            q_t1t2 = 0 mean both will not overlap in memory
            b_t1t2 sets the order of the two tensors,
            if b_t1t2 = 1, t1 sits earlier in memory than t2
            """
            constraint_list = list()
            for t1, t2 in tensor_set:
                constraint_list.append(
                    model.placement[t1] + model.effective_size[t1] <=\
                    model.placement[t2] + M * model.tensor_spatial_overlap[t1, t2] +\
                                          M * (1 - model.tensor_spatial_overlap_order[(t1, t2)])
                )
                constraint_list.append(
                    model.placement[t2] + model.effective_size[t2] <=\
                    model.placement[t1] + M * model.tensor_spatial_overlap[t1, t2] +\
                                          M * model.tensor_spatial_overlap_order[(t1, t2)]
                )
            return constraint_list

        def inplace_tensor_decision_rule(model, tensor_set):
            """sum_i d_i <= 1. At most one of the inputs of t may be overwritten by t,
            and the op may also decline them all and take a buffer of its own (the optimizer decides)."""
            constraint_list = list()
            for t in tensor_set:
                potential_inplace_inputs = potential_inplace_inputs_register[t]
                assert len(potential_inplace_inputs) != 0, (
                    "for a potential inplace output tensor its potential inplace input tensor "
                    f"must not empty, this is the case for tensor {t}"
                )
                forced_potential_inputs = [
                    i for i in potential_inplace_inputs
                    if self.rectangles[i]["inplace"] == op_inplace_type.force_inplace
                ]
                assert len(forced_potential_inputs) <= 1, (
                    f"potential output tensor {t} has {len(forced_potential_inputs)} forced potential inputs ,"
                    f"({forced_potential_inputs}); it should be at most one."
                )
                constraint_list.append(
                    pyo.quicksum([
                        model.decision_inplace_tensor_in[i] for i in potential_inplace_inputs
                    ]) <= 1
                )
            return constraint_list

        def force_inplace_tensor_decision_in_rule(model, tensor_set):
            for t in tensor_set:
                if self.rectangles[t]["inplace"] == op_inplace_type.force_inplace:
                    model.decision_inplace_tensor_in[t].fix(1)
            return []


        def inplace_tensor_right_shift_rule(model, tensor_set):
            """s_i <= d_i. An input that is not the one being overwritten is never shifted."""
            return [
                model.decision_right_shift_inplace_tensor_in[i] <= model.decision_inplace_tensor_in[i]
                for i in tensor_set
            ]

        def inplace_tensor_always_right_shift_rule(model, tensor_set):
            """
            sum_i r_i = 1, per output. With r_i <= d_i and sum_i d_i = 1 this pins
            r to d, so the chosen input is shifted and no output is placed low.
            """
            # if a layer does not need a right shift we set it to 0
            constraint_list = list()
            for t in tensor_set:
                op_needs_right_shift = {self.rectangles[i]["op_needs_right_shift"] for i in potential_inplace_inputs_register[t]}
                assert len(op_needs_right_shift) == 1, \
                    f"every input has to have the same op, hence op_needs_right_shift"
                assert self.optimize_right_shift in (RightShift.force_right_shift, RightShift.force_not_right_shift), (
                    f"Invalid optimize_right_shift, expected {(RightShift.force_right_shift, RightShift.force_not_right_shift)}, "
                    f"received {self.optimize_right_shift}"
                )
                # For every inplace op, we force that the chosen input to be aliased is right_shifted for ops that support it
                if self.optimize_right_shift is RightShift.force_right_shift:
                    right_shift_decision = (1 if next(iter(op_needs_right_shift)) else 0)
                else:
                    # we force that no right shifting be done
                    right_shift_decision = 0
                constraint_list.append(
                    pyo.quicksum([
                        model.decision_right_shift_inplace_tensor_in[i]
                        for i in potential_inplace_inputs_register[t]
                    ]) == right_shift_decision
                )
            return constraint_list


        def inplace_tensor_placement_rule(model, tensor_set):
            """
            x_out(i) = x_i - Delta_i(1 - r_i), as a big-M pair so it switches off
            for the candidates that are not chosen.

            The output has to start Delta below where its input is read from. A
            shifted input is memmoved up to x_i + Delta and the output takes the
            base, so the two placements coincide; an unshifted one stays put and
            the output starts Delta lower instead. Both leave the kernel the same
            geometry, which is why it can tell them apart on pointers alone.
            """
            constraint_list = list()
            for t in tensor_set:
                potential_inplace_inputs = potential_inplace_inputs_register[t]
                assert len(potential_inplace_inputs) != 0
                for i in potential_inplace_inputs:

                    # The start of the output is dependent on if a layer does a right_shift or
                    # not, for conv, the output has to be placed below the input
                    # but for layers like add, depthwise, upsample the output shares the same
                    # memory start as the input
                    if self.rectangles[i]["op_needs_right_shift"]:
                        output_start_referenced_to_input_right_shift_decision = (
                            model.placement[i] - self.rectangles[i]["gap"] *
                            (1 - model.decision_right_shift_inplace_tensor_in[i])
                        )
                    else:
                        output_start_referenced_to_input_right_shift_decision = model.placement[i]

                    constraint_list.append(
                        model.placement[t] - output_start_referenced_to_input_right_shift_decision <=\
                        M * (1 - model.decision_inplace_tensor_in[i])
                    )
                    constraint_list.append(
                        output_start_referenced_to_input_right_shift_decision - model.placement[t] <=\
                        M * (1 - model.decision_inplace_tensor_in[i])
                    )
            return constraint_list

        def effective_size_rule(model, tensor_set):
            """
            z_t = n_t for anything that is not a candidate, and for a candidate one
            of two rows depending on where its op puts the output.

                z_i >= n_i + Delta_i r_i    op_needs_right_shift, i.e. conv
                z_i >= n_i + Delta_i d_i    everything else

            An op that needs its output below its input reaches Delta past its own
            bytes only when it is the input that moved; left unshifted the output
            sits below it, covered by that rectangle. An op that shares the base
            instead has the whole workspace above its input, so it reaches Delta
            whenever the aliasing happens at all. Both collapse to n_i when the
            candidate is not the one overwritten -- r_i <= d_i carries that for the
            first row, d_i is written out in the second, which has no r_i to imply it.
            """
            constraint_list = list()
            for t in tensor_set:
                if t in model.potential_inplace_tensors_in:
                    # if the effective_size of a tensor is dependent on if a right_shift
                    # is done, it is currently only dependent for just conv
                    if self.rectangles[t]["op_needs_right_shift"]:
                        constraint_list.append(
                            model.effective_size[t] >= (
                                self.rectangles[t]["size"] + self.rectangles[t]["gap"] *
                                model.decision_right_shift_inplace_tensor_in[t]
                            )
                        )
                    else:
                        # the tensor that can be chosen for inplace but with no right shift
                        # when it is chosen, the gap is added but when it is not, it has to be 0
                        constraint_list.append(
                            model.effective_size[t] >= (
                                self.rectangles[t]["size"] + self.rectangles[t]["gap"] *
                                model.decision_inplace_tensor_in[t]
                            )
                        )
                else:
                    model.effective_size[t].fix(self.rectangles[t]["size"])
            return constraint_list


        # ==================================================================
        # Model
        # ==================================================================

        model = pyo.ConcreteModel()

        # placement parameters
        model.tensors = pyo.Set(initialize=range(len(self.rectangles)))
        model.potential_inplace_tensors_in = pyo.Set(
            initialize=[
                t for t in model.tensors
                if self.rectangles[t]["inplace"] is not None # Final output inplace is never set
                and self.rectangles[t]["inplace"] != op_inplace_type.force_not_inplace
            ]
        )
        model.potential_inplace_tensors_out = pyo.Set(
            initialize=list({
                self.rectangles[t]["inplace_tensor_out_idx"] for t in model.tensors
                if self.rectangles[t]["inplace_tensor_out_idx"] is not None})
        )
        model.tensors_combination = pyo.Set(initialize=list(itertools.combinations(model.tensors, 2)))

        # placement variables, word alligned
        model.placement = pyo.Var(model.tensors, within=pyo.NonNegativeIntegers, bounds=(0, M))
        # a_t. Alignment is expressed as a variable rather than rounded after
        # the solve, since rounding a solved placement would move tensors into
        # each other.
        model.placement_allign = pyo.Var(model.tensors, within=pyo.NonNegativeIntegers, bounds=(0, M//N))
        model.largest_space_in_memory = pyo.Var(within=pyo.NonNegativeIntegers, bounds=(0, M))
        # e_t. Fixed for everything except tensor whose output can potentially do an inplace operation so
        # the end is not certain.
        model.tensor_end = pyo.Var(model.tensors, within=pyo.NonNegativeIntegers, bounds=(0, L))
        # z_t. size_t, plus gap_i when the input is actually right shift.
        model.effective_size = pyo.Var(model.tensors, within=pyo.NonNegativeIntegers, bounds=(0, M))
        # b_t1t2. Which way round the pair is ordered, once they must be apart.
        model.tensor_spatial_overlap_order = pyo.Var(model.tensors_combination, within=pyo.Binary)
        # p_t1t2. 1 if the two are alive at the same time/layer. A variable because a
        # tensor's end depends on whether its op output is done in place and it chooses this input for its location.
        model.tensor_temporal_overlap = pyo.Var(model.tensors_combination, within=pyo.Binary)
        # q_t1t2. 1 if the two tensors can share an address, 0 if they must be held apart spatial (in memory)
        # The opposite of p_t1t2: a pair that never coexists is exactly the pair that may reuse the same memory.
        model.tensor_spatial_overlap = pyo.Var(model.tensors_combination, within=pyo.Binary)
        # d_i. For output with more than one input, the model chooses the best input to overwrite for its inplace operation.
        model.decision_inplace_tensor_in = pyo.Var(model.potential_inplace_tensors_in, within=pyo.Binary)
        # r_i. 1 if the model chooses to right_shift `memmoves` its input for inplace operation, 0 if not, this is an
        # optimization to reduce the cost of inplace operation.
        model.decision_right_shift_inplace_tensor_in = pyo.Var(model.potential_inplace_tensors_in, within=pyo.Binary)

        # Constraints
        model.word_alligned_constraint = pyo.ConstraintList(rule=data_alligned_rule(model, model.tensors))
        model.largest_space_in_memory_constraint = pyo.ConstraintList(rule=largest_space_in_memory_rule(model, model.tensors))
        model.tensor_end_constraint = pyo.ConstraintList(rule=tensor_end_rule(model, model.tensors))
        model.effective_size_constraint = pyo.ConstraintList(rule=effective_size_rule(model, model.tensors))
        model.inplace_tensor_right_shift_constraint = pyo.ConstraintList(rule=inplace_tensor_right_shift_rule(model, model.potential_inplace_tensors_in))
        if self.optimize_right_shift !=  RightShift.flexible_right_shift:
            model.inplace_tensor_always_right_shift_constraint = pyo.ConstraintList(rule=inplace_tensor_always_right_shift_rule(model, model.potential_inplace_tensors_out))
        if not self.optimize_inplace_flexible:
            model.not_inplace_flexible_constraint = pyo.ConstraintList(rule=not_inplace_flexible_rule(model, model.potential_inplace_tensors_out))
        model.no_spatial_overlap_constraint = pyo.ConstraintList(rule=no_spatial_overlap_rule(model, model.tensors_combination))
        model.no_temporal_overlap_constraint = pyo.ConstraintList(rule=no_temporal_overlap_rule(model, model.tensors_combination))
        model.tensor_overlap_constraint = pyo.ConstraintList(rule=tensor_overlap_rule(model, model.tensors_combination))
        model.tensor_overlap_fix_constraint = pyo.Constraint(rule=tensor_overlap_fix_rule(model, model.tensors_combination))
        model.inplace_tensor_placement_constraint = pyo.ConstraintList(rule=inplace_tensor_placement_rule(model, model.potential_inplace_tensors_out))
        model.inplace_tensor_decision_constraint = pyo.ConstraintList(rule=inplace_tensor_decision_rule(model, model.potential_inplace_tensors_out))
        model.force_inplace_tensor_decision_in_rule_constraint = pyo.ConstraintList(rule=force_inplace_tensor_decision_in_rule(model, model.potential_inplace_tensors_in))

        # model objective, the largest space in memory touched
        inplace_tensor_cost = pyo.quicksum([
            self.INPLACE_COST * model.decision_inplace_tensor_in[i]
            * self.rectangles[i]["size"]
            for i in model.potential_inplace_tensors_in
        ])
        inplace_tensor_right_shift_cost = pyo.quicksum([
            self.RIGHT_SHIFT_COST * model.decision_right_shift_inplace_tensor_in[i]
            * self.rectangles[i]["size"]
            for i in model.potential_inplace_tensors_in
        ])
        objective_fun = model.largest_space_in_memory - inplace_tensor_cost + inplace_tensor_right_shift_cost
        model.objective = pyo.Objective(expr=objective_fun, sense=pyo.minimize)

        return model
    

    def allocate(self):
        """
        Solve and write x_t back to each rectangle's "placement". get_peak()
        then reads Z off those placements.
        """
        super().allocate()

        model = self.define_model()
        if SOLVER_PATH:
            solver = pyo.SolverFactory(SOLVER, executable=os.path.join(SOLVER_PATH, SOLVER))
        else:
            solver = pyo.SolverFactory(SOLVER)

        results = solver.solve(model, tee=False)
        # Check before reading the variables: an unsolved model leaves every
        # placement at None, which would surface as a TypeError from int(None)
        # rather than the real cause. Infeasible normally means two constraints
        # contradict, e.g. an in-place pair left with overlapping lifetimes.
        condition = results.solver.termination_condition
        if condition != pyo.TerminationCondition.optimal:
            raise RuntimeError(
                f"{self.__class__.__name__} could not place the {len(self.rectangles)} tensors: "
                f"solver terminated with '{condition}' (status '{results.solver.status}'). "
                f"SRAM limit is {self.SRAM} bytes."
            )

        def solved_int(var):
            """A solver returns 0.9999999 as often as 1.0, and int() of that is 0."""
            return int(round(pyo.value(var)))

        for t in model.tensors:
            rec = self.rectangles[t]
            rec["placement"] = solved_int(pyo.value(model.placement[t]))
            if self.rectangles[t]["inplace"] is not None and self.rectangles[t]["inplace"] != op_inplace_type.force_not_inplace:
                # skip for a pure output tenosr
                rec["inplace_decision"] = bool(solved_int(model.decision_inplace_tensor_in[t]))
                rec["right_shift_decision"] = bool(solved_int(model.decision_right_shift_inplace_tensor_in[t]))
                if rec["right_shift_decision"]:
                    assert rec["inplace"] != op_inplace_type.force_not_inplace, \
                        "When the rec is right shifted, the inplace type must not be force_not_inplace"
                    assert rec["inplace_decision"], "When the rec is right shifted, the inplace decision has to be True"
            else:
                rec["inplace_decision"] = rec["right_shift_decision"] = 0
            
            # input tensors
            if t in model.potential_inplace_tensors_in:                    
                if rec["inplace"] == op_inplace_type.force_not_inplace:
                    # if the tensor is force to use dual buffer
                    rec["gap"] = 0
                else:
                    if rec["inplace_decision"]:
                        if rec["op_needs_right_shift"]:
                            # when an op can choose to right shift or not like conv
                            # if it does not the the output already position behind it so we do not add the gap
                            # the selected inplace tensnor that was also shifted gets to keep its gap else it
                            # is set to 0
                            rec["gap"] = (rec["gap"] if rec["right_shift_decision"] else 0)
                        else:
                            # when an op has gap and cannot can not choose if it shifts or not like upSample
                            # it keeps it gap
                            pass
                    else:
                        # zero out the gap for potential inplace tensors that were not selected
                        # input tensors not selected to be inplace
                        rec["gap"] = 0
            else:
                # output only tensors
                rec["gap"] = 0

            rec["effective_size"] = rec["size"] + rec["gap"]

        # a basic sanity check
        assert int(round(pyo.value(model.largest_space_in_memory))) == int(round(self.get_peak())), (
            f"the reported peak for the model is not the same as what the allocator determined ,"
            f"this will an error and should be checked"
        )
