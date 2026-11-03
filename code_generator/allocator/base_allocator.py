# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   base_allocator.py
#
# Reference papers:
#  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
#  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
#  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
# Contact authors:
#  - Wei-Ming Chen, wmchen@mit.edu
#  - Wei-Chen Wang, wweichen@mit.edu
#  - Ji Lin, jilin@mit.edu
#  - Ligeng Zhu, ligeng@mit.edu
#  - Song Han, songhan@mit.edu
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------

from enum import Enum
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

from code_generator.constant import (
    FIGURE_CONFIG,
    TTYPE_INFERNECE,
    TTYPE_STATIC_WEIGHT,
    TTYPE_TRAINING_ACTIVATION,
    TTYPE_TRAINING_GRADIENT,
    TTYPE_TRAINING_WEIGHTS,
)
from ..operators.basic_utils import op_inplace_type, tensor_io


class AllocatorType(Enum):
    """Select different allocator types"""
    MILP = "MILP"
    first_fit = "first_fit"


class BaseAllocator:
    def __init__(self, SRAM, model_name="mcunet_model", sort_by_lifetime=False, align_to_n_bytes=4):
        self.rectangles = []
        self.SRAM = SRAM
        self.model_name = model_name
        self.sort_by_lifetime = sort_by_lifetime
        self.align_to_n_bytes = align_to_n_bytes
    
    # Description: add a tensor to schedule, return the index of the rectangle
    # Note: placement -1 indicates no placed yet
    def addTensor(
        self,
        start,
        end,
        size,
        tio,
        op,
        placement=-1,
        name=None,
        type="activation",
        inplace=None,
        inplace_tensor_out_idx=None,
    ) -> int:
        tensor_idx = len(self.rectangles)
        # Every rectangle is created plain, so we cannot know if it can be inplace except we
        # are sure that no other tensor needs it and we do this after all tensors are created
        if tio != tensor_io.out_:
            # Only input tensor (not output) can have a inplace type, the output derives their from their
            # corresponding inputs
            assert inplace is not None, (
                f"inplace should be set before adding the tensor, inplace not set for tensor {tensor_idx}"
            )
            assert isinstance(inplace, op_inplace_type), \
                f"inplace must be a type of op_inplace_type, got {type(inplace)} for tensor {tensor_idx}"
        assert inplace_tensor_out_idx is None, (
            "inplace_tensor_out_idx is decided by markPotentialInplace(), after every rectangle exists"
            " that is when we can know its output allocated idx"
        )

        self.rectangles.append(
            {
                "start": start,
                "end": end,
                "size": size,
                "placement": placement,
                "name": name,
                "op": op,
                "type": type,
                "idx": tensor_idx,
                "inplace": inplace,
                "inplace_tensor_out_idx": inplace_tensor_out_idx,
                # Memory actually touched while this tensor is live. Same as "size"
                # for an ordinary tensor. This is to accomodate for layers that require
                # right shift for their inplace operations like convolution.
                # It is set by the allocator
                "effective_size": None
            }
        )
        return tensor_idx

    def markPotentialInplace(self, tensor):
        """
        Copy a tensor's inplace type, gap and the output tensor idx that may overwrite it
        (inplace_tensor_out_idx) onto its rectangle. Whether that output actually takes it
        is the allocator's to decide; this only records that it may.

        Called once every rectangle exists, because the [output] tensor idx [that it may overwrite]
        is created after it is registered in the allocator,so its index is not knowable at registration.
        Tensors sharing a graph_idx map to one rectangle, so this runs more than once for it, and the
        second pass asserts the two agree rather than overwriting.
        """
        tensor_idx = tensor.allocator_idx
        inplace = tensor.inplace
        gap = tensor.gap
        op_needs_right_shift = tensor.op_needs_right_shift
        inplace_tensor_out_idx = None if tensor.inplace_tensor_out is None else tensor.inplace_tensor_out.allocator_idx
        rec = self.rectangles[tensor_idx]

        if tensor.inplace == op_inplace_type.force_not_inplace:
            assert inplace_tensor_out_idx is None, (
                f"if tensor is forced to not be inplace, no inplace_tensor_out_idx should be set, "
                f"received tensor {inplace_tensor_out_idx} for {tensor.allocator_idx}"
            )
        else:
            assert isinstance(inplace_tensor_out_idx, int) and inplace_tensor_out_idx >= 0, \
                f"the inplace output index must be a not negative integer, got {inplace_tensor_out_idx}"
            assert inplace_tensor_out_idx > tensor_idx, \
                f"the inplace output rectangle {inplace_tensor_out_idx} must be registered after the source tensor {tensor_idx}"

        if "gap" in rec:
            # which means that the corresponding rectangle for this tensor has been assigned it inplace_tenosr_idx earlier
            # as gap field is only set here
            assert rec["idx"] == tensor_idx, \
                f"for an already assigned rectangle, the idx must match, saw {tensor_idx} expected {rec['idx']}"
            assert rec["inplace"] == inplace, \
                f"for an already assigned rectangle, the inplace must match, but for tensor {tensor_idx} saw {inplace} expected {rec['inplace']}"
            assert rec["gap"] == gap, \
                f"for an already assigned rectangle, the gap must match, but for tensor {tensor_idx} saw {gap} expected {rec['gap']}"
            assert rec["inplace_tensor_out_idx"] == inplace_tensor_out_idx, (
                f"for an already assigned rectangle, the inplace_tensor_out_idx must match, "
                f"but for tensor {tensor_idx} saw {inplace_tensor_out_idx} expected {rec['inplace_tensor_out_idx']}"
            )
        else:
            rec["inplace"] = inplace
            rec["gap"] = gap
            rec["inplace_tensor_out_idx"] = inplace_tensor_out_idx
            rec["op_needs_right_shift"] = op_needs_right_shift

    def getIdxAddress(self, idx):
        target_rec = None
        for rec in self.rectangles:
            if rec["idx"] == idx:
                target_rec = rec
        assert target_rec is not None
        return target_rec["placement"]

    def allocate(self):
        # place each rectangle
        print(f"Deriving the memory schedule for {len(self.rectangles)} activation tensors.")

    def get_peak(self):
        peak = 0
        for rec in self.rectangles:
            # effective_size, not size: the buffer has to be large enough for the
            # right-inplace workspace (gap + input) too, otherwise the generated
            # buffer is short by exactly the amount the memmove overruns.
            rec_size = rec["placement"] + rec["effective_size"]
            if peak < rec_size:
                peak = rec_size
        return peak

    def visualize(self, path, train_start_idx=-1, scale=1024):
        fig, ax = plt.subplots(
            figsize=(FIGURE_CONFIG["FIGURE_W_INCH"], FIGURE_CONFIG["FIGURE_H_INCH"])
        )
        font_size = FIGURE_CONFIG["FONT_SIZE"]

        # effective_size exclude outputs (they are not set by the milp allocator for tensors which are never input)
        rectangles = [rec for rec in self.rectangles if (rec["start"] != rec["end"])]

        # The axes span the whole schedule: the last lifetime to end on x, and the
        # peak (placement + effective_size, the same quantity get_peak reports) on y.
        max_x = max(rec["end"] for rec in rectangles)
        max_y = max(rec["placement"] + rec["effective_size"] for rec in rectangles) / scale
        ax.set_xlim([0, max_x])
        # A little headroom so the rectangle that sets the peak does not sit flush
        # against the frame and become invisible.
        y_range = max_y * 1.05
        ax.set_ylim([0, y_range])

        # Height of the axes box in points, to convert a tensor's size into the
        # units the outline width is given in. 0.72 is what the title, legend and
        # tick labels leave of the figure once tight bounds are applied.
        axes_height_pt = FIGURE_CONFIG["FIGURE_H_INCH"] * 72 * 0.72

        # Let matplotlib choose the step. Models in the zoo run from 51 to 83 layers
        # and from 36 KB to 300 KB, so a fixed step is either unreadably dense on one
        # end of that range or featureless on the other.
        ax.xaxis.set_major_locator(MaxNLocator(nbins=12, integer=True))
        ax.yaxis.set_major_locator(MaxNLocator(nbins=10))
        ax.tick_params(labelsize=font_size)

        ax.set_xlabel("Life cycle (operator)", fontsize=font_size)
        ax.set_ylabel("Memory Footprint (KB)", fontsize=font_size)
        ax.set_title(
            f"{self.model_name} — peak {max_y:.1f} KB",
            fontsize=font_size,
            # Clears the legend strip, which sits between the axes and the title.
            pad=font_size * 1.7,
        )
        ax.set_axisbelow(True)
        ax.yaxis.grid(color="gray", linestyle="dashed")
        ax.xaxis.grid(color="gray", linestyle="dashed")
        ax.patch.set_edgecolor("black")
        ax.patch.set_linewidth(2)

        # One entry per operator type actually present, in the order the network
        # first reaches it. Rectangles go on with add_patch, which does not register
        # a legend handle, so the swatches have to be built by hand.
        legend_handles = {}
        for rec in rectangles:
            op_name = rec["op"].params["op"]
            if op_name not in legend_handles:
                legend_handles[op_name] = matplotlib.patches.Patch(
                    facecolor=rec["op"].params["color"], edgecolor="black", label=op_name
                )
        ax.legend(
            handles=list(legend_handles.values()),
            loc="lower left",
            bbox_to_anchor=(0, 1.005),
            ncol=len(legend_handles),
            frameon=False,
            fontsize=font_size * 0.75,
        )

        for cnt, rec in enumerate(rectangles):
            start, end, placement, size, color = (
                rec["start"],
                rec["end"],
                rec["placement"],
                rec["effective_size"],
                rec["op"].params["color"]
            )
            hatch = None

            # Rectangles that abut share a boundary, so the outline is what tells
            # two neighbours apart from one block.
            height_pt = (size / scale) / y_range * axes_height_pt
            linewidth = min(1.6, max(0.15, height_pt / 2))
            rect = matplotlib.patches.Rectangle(
                (start, placement / scale),
                end -1- start,
                size / scale,
                facecolor=color,
                edgecolor="#1a1a19",
                linewidth=linewidth,
                hatch=hatch,
            )

            ax.add_patch(rect)
            # Annotate index
            if FIGURE_CONFIG["SHOW_INDEX"]:
                cx = (start + end) / 2
                cy = (placement / scale) + (size / scale) / 2
                ax.annotate(
                    str(rec["idx"]),
                    (cx, cy),
                    color="b",
                    fontsize=_get_index_font_size(
                        FIGURE_CONFIG["INDEX_FONT_SIZE"], (size / scale), FIGURE_CONFIG["Y_STEP"]
                    ),
                    weight="bold",
                    ha="center",
                    va="center",
                )

        # tight: the legend and title live outside the axes and would be cropped.
        fig.savefig(path, dpi=FIGURE_CONFIG["DPI"], bbox_inches="tight")
        plt.close(fig)


def _get_index_font_size(origin_font_size, y_size, y_block_size):
    y_bound = y_block_size / 4
    if y_size > y_bound:
        return origin_font_size
    else:
        return int(origin_font_size * (y_size / y_bound))
