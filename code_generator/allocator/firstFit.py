# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   firstFit.py
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

import math

from .base_allocator import BaseAllocator
from ..operators.basic_utils import op_inplace_type

__all__ = ["FirstFit"]


class slot:
    def __init__(self, y0: int, y1: int) -> None:
        self.y0 = y0
        self.y1 = y1

    def size(self):
        return self.y1 - self.y0


class FirstFit(BaseAllocator):
    """
    First fit, largest effective_size (size + gap) first, that places each inplace input with its output
    without a right shift: a conv output sits gap below its input, any other op shares its input's address.
    """

    def allocate(self):
        super().allocate()
        # the sort reads effective_size, so it has to be set first
        for rec in self.rectangles: rec["effective_size"] = rec["size"] + rec.get("gap", 0)

        # Reorder the rectangles to decide which tensor needs to be scheduled first
        sorted_index = self.sortSizeIndex()

        for rec_index in sorted_index:
            rec = self.rectangles[rec_index]

            # if the rec has not been placed and it can be done inplace and its output is yet to be place
            # do it inplace else allocate it without making its output inplace
            if (rec["inplace"] is not None and
                rec["inplace"] != op_inplace_type.force_not_inplace and
                self.rectangles[rec["inplace_tensor_out_idx"]]["placement"] == -1):

                output_index = rec["inplace_tensor_out_idx"]
                output_rec = self.rectangles[output_index]

                if rec["placement"] == -1:
                    rec["inplace_decision"] = 1
                    rec["right_shift_decision"] = 0
                    rec["effective_size"] = rec["size"] + rec["gap"]

                    # The output is never fitted, it takes the input's address. So the slot has to hold
                    # both of them from the input's start to the output's end, and be as large as the larger.
                    rec["placement"] = self.fit(
                        rec, end=max(rec["end"], output_rec["end"]), size=max(rec["effective_size"], output_rec["size"])
                    )

                    # left aligning the output and right aligning the input: a conv input sits gap above its output,
                    # that distance is now its headroom so the kernel is handed no gap. Any other op shares the address
                    output_rec["placement"] = rec["placement"]
                    if rec["op_needs_right_shift"]:
                        rec["placement"] += rec["gap"]
                        rec["gap"] = 0
                        rec["effective_size"] = rec["size"]

                    # the output keeps its gap, it may be the inplace input of the next op when the loop reaches it
                    output_rec["inplace_decision"] = 0
                    output_rec["right_shift_decision"] = 0
                    output_rec["effective_size"] = output_rec["size"]

                else:
                    # rec was placed as the output of an earlier block and is never moved, so its output can only take
                    # the address that follows from it (a conv output gap below, any other op over it), and only if
                    # nothing else uses that memory for the whole of the output's life
                    base = rec["placement"] - rec["gap"] if rec["op_needs_right_shift"] else rec["placement"]
                    size = max(output_rec["size"], rec["gap"])
                    if (
                        base >= 0 and base + size <= self.SRAM
                        and self.isRegionFree(base, size, output_rec["start"], output_rec["end"], ignore=rec["idx"])
                    ):
                        rec["inplace_decision"] = 1
                        rec["right_shift_decision"] = 0
                        rec["gap"] = 0

                        output_rec["placement"] = base
                        output_rec["inplace_decision"] = 0
                        output_rec["right_shift_decision"] = 0
                        output_rec["effective_size"] = output_rec["size"]
                    else:
                        # its output is fitted on its own when the loop reaches it
                        rec["gap"] = 0

            elif rec["placement"] == -1:
                rec["inplace_decision"] = 0
                rec["right_shift_decision"] = 0
                rec["gap"] = 0
                rec["effective_size"] = rec["size"]

                # fit each tensor into the memmory
                rec["placement"] = self.fit(rec)

            else:
                # rec was placed as the output of an earlier block and its own output is already placed, not inplace
                rec["gap"] = 0

    def fit(self, rec, end=None, size=None) -> int:
        """end and size override the rectangle's own, to reserve a slot an aliased output also uses."""
        start = rec["start"]
        end = rec["end"] if end is None else end
        size = rec["size"] if size is None else size
        slots = []
        slots.append(slot(0, self.SRAM))

        for _, rec in enumerate(self.rectangles):
            if rec["placement"] != -1:  # this each placed rectangle
                if (
                    start < rec["start"] < end
                    or start < rec["end"] < end
                    or rec["start"] < start < rec["end"]
                    or rec["start"] < end < rec["end"]
                ):  # overlap with the insert block
                    # placement of the rectatngle
                    y0 = rec["placement"]
                    # align memory to n bytes
                    y1 = y0 + math.ceil(rec["effective_size"] / self.align_to_n_bytes) * self.align_to_n_bytes

                    for s in list(slots):
                        if (
                            s.y0 <= y0 <= s.y1 or s.y0 <= y1 <= s.y1 or y0 <= s.y0 <= y1 or y0 <= s.y1 <= y1
                        ):  # overlap with the insert block
                            # upper block
                            if s.y1 > y1:
                                upper_block_botttom = y1
                                slots.append(slot(upper_block_botttom, s.y1))
                            # lower block
                            if s.y0 < y0:
                                lower_block_top = y0
                                slots.append(slot(s.y0, lower_block_top))
                            # slots.append(slot(s.y0, y0))
                            # slots.append(slot(y1, s.y1))
                            slots.remove(s)
                            # break

        # use the first available
        lowest_slot = None
        lowest_slot_starting = 2**31 - 1
        for s in slots:
            if s.size() >= size:  # take this slot
                if lowest_slot_starting > s.y0:
                    lowest_slot_starting = s.y0
                    lowest_slot = s

        assert lowest_slot is not None, "no available slot, memory exceed MAX SRAM setting"
        return lowest_slot_starting

    def isRegionFree(self, y0, size, start, end, ignore) -> bool:
        """
        [y0, y0 + size) is not used by any placed rectangle live in [start, end).
        ignore is the inplace input itself, which the region always covers, that overlap is the inplace.
        """
        for rec in self.rectangles:
            if rec["placement"] == -1 or rec["idx"] == ignore:
                continue
            if (rec["start"] < end and start < rec["end"] and
                    rec["placement"] < y0 + size and y0 < rec["placement"] + rec["effective_size"]):
                return False
        return True

    def sortSizeIndex(self):
        """
        Returns the rectangle indices ordered by effective_size or lifetime, largest first.
        """
        sorted_index = []
        rectangles = list(self.rectangles)
        while len(rectangles) > 0:
            max_life = 0
            max_size = 0
            max_rectangle = None
            for rec in rectangles:
                if self.sort_by_lifetime:
                    life = rec["end"] - rec["start"]
                    if life > max_life:
                        max_life = life
                        max_rectangle = rec
                else:
                    if rec["effective_size"] > max_size:
                        max_size = rec["effective_size"]
                        max_rectangle = rec
            assert max_rectangle is not None
            sorted_index.append(max_rectangle["idx"])
            rectangles.remove(max_rectangle)
        return sorted_index

