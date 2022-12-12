# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   OpGenerator.py
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

from .codetemplate.depthwiseTemplate import depthwiseInplace
from .codetemplate.depthwiseTemplate_bitmask import depthwiseInplace_bitmask
from .codetemplate.depthwiseTemplate_mask import depthwiseInplace_mask


class OpGenerator:
    def __init__(self, incpath, srcpath, layers, fp_requantize=False):
        self.incpath = incpath
        self.srcpath = srcpath
        self.layers = layers
        self.fp_requantize = fp_requantize

    def genOpcode(self):
        # find all conv ops
        op_list = []
        for op in self.layers:
            layer_info = op.get_layer_info()
            if layer_info["op"] == "CONV_2D" or layer_info["op"] == "DEPTHWISE_CONV_2D":
                op = convOp(layer_info)
                if op not in op_list:
                    op_list.append(op)

        # go through and generate all ops
        incfile = includeFile(self.incpath)
        for op in op_list:
            if op.isDepthwise:
                if op.kernel_h > op.kernel_w:
                    depthwise_template = depthwiseInplace(
                        op.kernel_h, op.kernel_w, op.pad_h, op.pad_w, op.stride, "CWH", self.fp_requantize
                    )
                else:
                    depthwise_template = depthwiseInplace(
                        op.kernel_h, op.kernel_w, op.pad_h, op.pad_w, op.stride, "CHW", self.fp_requantize
                    )
                    depthwise_template_mask = depthwiseInplace_mask(
                        op.kernel_h, op.kernel_w, op.pad_h, op.pad_w, op.stride, "CHW", self.fp_requantize
                    )
                    depthwise_template_bitmask = depthwiseInplace_bitmask(
                        op.kernel_h, op.kernel_w, op.pad_h, op.pad_w, op.stride, "CHW", self.fp_requantize
                    )
                depthwise_template.genFile(self.srcpath)
                incfile.addDefine(depthwise_template.genFuncDefine())
                depthwise_template_mask.genFile(self.srcpath)
                incfile.addDefine(depthwise_template_mask.genFuncDefine())
                depthwise_template_bitmask.genFile(self.srcpath)
                incfile.addDefine(depthwise_template_bitmask.genFuncDefine())

        incfile.writeFile()


class convOp:
    def __init__(self, layer_info):
        if layer_info["op"] == "CONV_2D":
            isDepthwise = False
        elif layer_info["op"] == "DEPTHWISE_CONV_2D":
            isDepthwise = True
        kernel_h = layer_info["kernel_h"]
        kernel_w = layer_info["kernel_w"]
        pad_h = (kernel_h - 1) // 2
        pad_w = (kernel_w - 1) // 2
        stride = layer_info["stride_h"]
        self.inchannel = layer_info["input_c"]
        self.isDepthwise = isDepthwise
        self.kernel_h = kernel_h
        self.kernel_w = kernel_w
        self.stride = stride
        self.pad_h = pad_h
        self.pad_w = pad_w

    def __eq__(self, other):
        if isinstance(other, convOp):
            if (
                self.isDepthwise == other.isDepthwise
                and self.kernel_h == other.kernel_h
                and self.kernel_w == other.kernel_w
                and self.stride == other.stride
                and self.pad_h == other.pad_h
                and self.pad_w == other.pad_w
            ):
                return True
            else:
                return False
        return NotImplemented


class includeFile:
    def __init__(self, path):
        self.path = path
        self.defstring = ""

    def addDefine(self, defstr):
        self.defstring += defstr + ";\n"

    def writeFile(self):
        import os

        outpath = os.path.join(self.path, "genInclude.h")
        outf = open(outpath, "w")
        outf.write(self.defstring)
        outf.close()
