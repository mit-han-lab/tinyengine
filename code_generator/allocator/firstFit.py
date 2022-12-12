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

__all__ = ["FirstFit"]


class slot:
    def __init__(self, y0: int, y1: int) -> None:
        self.y0 = y0
        self.y1 = y1

    def size(self):
        return self.y1 - self.y0


class FirstFit(BaseAllocator):
    def fit(self, rec) -> int:
        start, end, size = rec["start"], rec["end"], rec["size"]
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
                    if rec["stride2_inplace_idx"] is not None and start > rec["stride2_inplace_idx"]:
                        y1 = y0 + math.ceil(rec["size"] / 4)
                    else:
                        y1 = y0 + rec["size"]

                    for s in slots:
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
