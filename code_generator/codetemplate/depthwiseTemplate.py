# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   depthwiseTemplate.py
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


class depthwiseInplace:
    def __init__(
        self,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride,
        dataflow="CHW",
        fp_requantize=False,
    ):
        self.pad_h = pad_h
        self.pad_w = pad_w
        self.kernel_h = kernel_h
        self.kernel_w = kernel_w
        self.stride = stride
        self.arch = "ARMv7E-M"  # by default
        self.dataflow = dataflow
        self.fp_requantize = fp_requantize

    def setArch(self, arch):
        self.arch = arch

    def _genCode(self):
        retString = self._genHeader()
        retString += self._genKernelDefine() + ";\n"
        retString += self.genFuncDefine() + "\n{\n"
        if self.dataflow == "CHW":
            retString += self._genBufferInitialization()
        elif self.dataflow == "CWH":
            retString += self._genBufferInitializationCWH()

        if self.dataflow == "CHW":
            retString += self._genInplaceKernel()
        elif self.dataflow == "CWH":
            retString += self._genInplaceKernelCWH()

        retString += "    return STATE_SUCCESS;"
        retString += self._genEndStr()
        if self.dataflow == "CHW":
            retString += "\n" + self._genKernel()
        elif self.dataflow == "CWH":
            retString += "\n" + self._genKernelCWH()

        return retString

    def _getFunctionName(self):
        if self.fp_requantize:
            return (
                "depthwise_kernel"
                + str(self.kernel_h)
                + "x"
                + str(self.kernel_w)
                + "_stride"
                + str(self.stride)
                + "_inplace_"
                + self.dataflow
                + "_fpreq"
            )
        else:
            return (
                "depthwise_kernel"
                + str(self.kernel_h)
                + "x"
                + str(self.kernel_w)
                + "_stride"
                + str(self.stride)
                + "_inplace_"
                + self.dataflow
            )

    def _getKernelName(self):
        if self.fp_requantize:
            return (
                "depthwise_kernel"
                + str(self.kernel_h)
                + "x"
                + str(self.kernel_w)
                + "_stride"
                + str(self.stride)
                + "_inplace_kernel_"
                + self.dataflow
                + "_fpreq"
            )
        else:
            return (
                "depthwise_kernel"
                + str(self.kernel_h)
                + "x"
                + str(self.kernel_w)
                + "_stride"
                + str(self.stride)
                + "_inplace_kernel_"
                + self.dataflow
            )

    def genFuncDefine(self):
        retString = ""
        function_name = self._getFunctionName()
        retString += "tinyengine_status " + function_name
        if self.fp_requantize:
            retString += """(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const float *scales,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value)"""
        else:
            retString += """(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const int32_t *output_shift, const int32_t *output_mult,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value)"""

        return retString

    def _genKernelDefine(self):
        retString = ""
        if self.fp_requantize:
            retString += (
                "void "
                + self._getKernelName()
                + """(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
        q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset)"""
            )
        else:
            retString += (
                "void "
                + self._getKernelName()
                + """(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const int32_t *multiplier,
        const int32_t *shift, q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset)"""
            )

        return retString

    def _genHeader(self):
        retString = (
            """/* This file is automatically generated */
/* ----------------------------------------------------------------------
 * Project:      TinyEngine
 * Description:  for sparse in-place """
            + str(self.kernel_h)
            + "x"
            + str(self.kernel_w)
            + """ depth-wise convolution (HWC->CHW->HWC)
 * Target ISA:  """
            + self.arch
            + """
 * Author: wmchen@mit.edu
 * -------------------------------------------------------------------- */
#include "tinyengine_function.h"\n"""
        )

        return retString

    def _genBufferInitialization(self):
        retString = (
            """
    uint16_t c,i,j;
    q7_t *cols_8b_start = (q7_t *)runtime_buf;
    q7_t* cols_8b = (q7_t* )cols_8b_start;

    //Set padding value
    q7_t PAD8 = pad_value;
    /* setup the padding regions for Im2col buffers */
    //top region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
    for(i = 0; i < input_x + """
            + str(self.pad_w * 2)
            + """; i++){"""
        )
        for i in range(self.pad_h):
            retString += """
        *cols_8b++ = PAD8;"""
        retString += """
    }

    //middle regions: left and right regions
    for(i = 0; i < input_y; i++){"""
        for i in range(self.pad_w):
            retString += """
        *cols_8b++ = PAD8;//left"""
        retString += """
        cols_8b += input_x; //skip middle"""
        for i in range(self.pad_w):
            retString += """
        *cols_8b++ = PAD8;//right"""
        retString += (
            """
    }

    //bottom region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
    for(i = 0; i < input_x + """
            + str(self.pad_w * 2)
            + """; i++){"""
        )
        for i in range(self.pad_h):
            retString += """
        *cols_8b++ = PAD8;"""
        retString += """
    }

    const q7_t *src;
    const q7_t *ksrc = kernel;
"""
        return retString

    def _genBufferInitializationCWH(self):
        # HWC to CWH data flow
        retString = (
            """
    uint16_t c,i,j;
    q7_t *cols_8b_start = (q7_t *)runtime_buf;
    q7_t* cols_8b = (q7_t* )cols_8b_start;

    //HWC to CWH data flow
    //Set padding value
    q7_t PAD8 = pad_value;
    /* setup the padding regions for Im2col buffers */
    //top region: 8bit x (input_y + pad_h * 2) x pad_w: unroll by pad value
    for(i = 0; i < input_y + """
            + str(self.pad_h * 2)
            + """; i++){"""
        )
        for i in range(self.pad_w):
            retString += """
        *cols_8b++ = PAD8;"""
        retString += """
    }

    //middle regions: left and right regions
    for(i = 0; i < input_x; i++){"""
        for i in range(self.pad_h):
            retString += """
        *cols_8b++ = PAD8;//left"""
        retString += """
        cols_8b += input_y; //skip middle"""
        for i in range(self.pad_h):
            retString += """
        *cols_8b++ = PAD8;//right"""
        retString += (
            """
    }

    //bottom region: 8bit x (input_y + pad_h * 2) x pad_w: unroll by pad value
    for(i = 0; i < input_y + """
            + str(self.pad_h * 2)
            + """; i++){"""
        )
        for i in range(self.pad_w):
            retString += """
        *cols_8b++ = PAD8;"""
        retString += """
    }

    const q7_t *src;
    const q7_t *ksrc = kernel;
"""
        return retString

    def _genInplaceKernelCWH(self):
        retString = ""
        retString += """
    for (c = 0; c < input_ch; c++){"""
        retString += self._genFixedLoadHWC2CWH()
        retString += self._assignInplaceOut(2, "input")
        retString += self._genHandle1CH_CWH(2, "input_ch")
        retString += """
    }\n"""
        return retString

    def _genFixedLoadHWC2CWH(self):
        retString = (
            """
        cols_8b = (q7_t*)(cols_8b_start + """
            + str(self.pad_w)
            + """ * (input_y) + """
            + str(self.pad_h * self.pad_w * 2)
            + """); //skip """
            + str(self.pad_w)
            + """ rows
        src = input;
        for(i = 0; i < input_x; i++){
            cols_8b += """
            + str(self.pad_h)
            + """;//skip front
            for(j = 0; j < input_y; j++){
                *cols_8b++ = src[input_x * j * input_ch];// + input_offset;
            }
            src += input_ch;
            cols_8b += """
            + str(self.pad_h)
            + """;//skip end
        }\n"""
        )

        return retString

    def _genInplaceKernel(self):
        retString = ""
        retString += """
    for (c = 0; c < input_ch; c++){"""
        retString += self._genFixedLoadHWC2CHW()
        retString += self._assignInplaceOut(2, "input")
        retString += self._genHandle1CH(2, "input_ch")
        retString += """
    }\n"""
        return retString

    def _genFixedLoadHWC2CHW(self):
        retString = (
            """
        cols_8b = (q7_t*)(cols_8b_start + """
            + str(self.pad_h)
            + """ * (input_x) + """
            + str(self.pad_h * self.pad_w * 2)
            + """); //skip """
            + str(self.pad_h)
            + """ rows
        src = input;
        for(i = 0; i < input_y; i++){
            cols_8b += """
            + str(self.pad_w)
            + """;//skip front
            for(j = 0; j < input_x; j++){
                *cols_8b++ = *src;// + input_offset;
                src += input_ch;
            }
            cols_8b += """
            + str(self.pad_w)
            + """;//skip end
        }\n"""
        )

        return retString

    def _genEndStr(self):
        return "\n}"

    def _assignInplaceOut(self, pre_indent, out_str):
        return "    " * pre_indent + "q7_t *inplace_out = " + out_str + ";\n"

    def _genHandle1CH(self, pre_indent, out_offset_str):
        if self.fp_requantize:
            retString = (
                "    " * pre_indent
                + self._getKernelName()
                + "(output_y, output_x, bias++, biasR++, ksrc, scales++, inplace_out, output_offset,"
                + "output_activation_min, output_activation_max,cols_8b_start, input_x, "
                + out_offset_str
                + ");\n"
            )
        else:
            retString = (
                "    " * pre_indent
                + self._getKernelName()
                + "(output_y, output_x, bias++, biasR++, ksrc, output_mult++, output_shift++, inplace_out,"
                + "output_offset,output_activation_min, output_activation_max,cols_8b_start, input_x, "
                + out_offset_str
                + ");\n"
            )
        retString += "    " * pre_indent + "ksrc += " + str(self.kernel_h * self.kernel_w) + ";\n"
        retString += "    " * pre_indent + "input++;"
        return retString

    def _genHandle1CH_CWH(self, pre_indent, out_offset_str):
        if self.fp_requantize:
            retString = (
                "    " * pre_indent
                + self._getKernelName()
                + "(output_y, output_x, bias++, biasR++, ksrc, scales++, inplace_out,"
                + "output_offset,output_activation_min, output_activation_max,cols_8b_start, input_y, "
                + out_offset_str
                + ");\n"
            )
        else:
            retString = (
                "    " * pre_indent
                + self._getKernelName()
                + "(output_y, output_x, bias++, biasR++, ksrc, output_mult++, output_shift++, inplace_out,"
                + " output_offset,output_activation_min, output_activation_max,cols_8b_start, input_y, "
                + out_offset_str
                + ");\n"
            )
        retString += "    " * pre_indent + "ksrc += " + str(self.kernel_h * self.kernel_w) + ";\n"
        retString += "    " * pre_indent + "input++;"
        return retString

    def genFile(self, path):
        import os

        outpath = os.path.join(path, self._getFunctionName() + ".c")
        outf = open(outpath, "w")
        outf.write(self._genCode())
        outf.close()

    # ********************* KERNEL ********************* #
    def _genMACStr(self, input_cnt, kernel_cnt, stride):
        inbuf = "cols_8b"
        kerbuf = "ksrc"
        sum0 = "sum0"
        sum1 = "sum1"

        ret = (
            "            "
            + sum0
            + " += "
            + inbuf
            + "["
            + str(input_cnt)
            + "]"
            + "*"
            + kerbuf
            + "["
            + str(kernel_cnt)
            + "]"
            + ";\n"
        )
        ret += (
            "            "
            + sum1
            + " += "
            + inbuf
            + "["
            + str(input_cnt + stride)
            + "]"
            + "*"
            + kerbuf
            + "["
            + str(kernel_cnt)
            + "]"
            + ";\n"
        )

        return ret

    def _genConvString(self):
        retString = ""
        kercnt = 0
        for i in range(self.kernel_h):
            if i > 0:
                retString += "            cols_8b += column_x + " + str(self.pad_w * 2) + ";\n"
            for j in range(self.kernel_w):
                retString += self._genMACStr(j, kercnt, self.stride)
                kercnt += 1

        return retString

    def _genConvStringCWH(self):
        retString = ""
        kercnt = 0
        for i in range(self.kernel_w):
            if i > 0:
                retString += "            cols_8b += column_x + " + str(self.pad_h * 2) + ";\n"
            for j in range(self.kernel_h):
                retString += self._genMACStr(j, kercnt, self.stride)
                kercnt += 1

        return retString

    def _genConvLeftStringCWH(self):
        retString = ""
        kercnt = 0
        for i in range(self.kernel_w):
            if i > 0:
                retString += "            cols_8b += column_x + " + str(self.pad_h * 2) + ";\n"
            for j in range(self.kernel_h):
                retString += self._genLeftMACStr(j, kercnt, self.stride)
                kercnt += 1

        return retString

    def _genLeftMACStr(self, input_cnt, kernel_cnt, stride):
        inbuf = "cols_8b"
        kerbuf = "ksrc"
        sum0 = "sum"

        ret = (
            "            "
            + sum0
            + " += "
            + inbuf
            + "["
            + str(input_cnt)
            + "]"
            + "*"
            + kerbuf
            + "["
            + str(kernel_cnt)
            + "]"
            + ";\n"
        )

        return ret

    def _genConvLeftString(self):
        retString = ""
        kercnt = 0
        for i in range(self.kernel_h):
            if i > 0:
                retString += "            cols_8b += column_x + " + str(self.pad_w * 2) + ";\n"
            for j in range(self.kernel_w):
                retString += self._genLeftMACStr(j, kercnt, self.stride)
                kercnt += 1

        return retString

    def _genKernel(self):
        retString = ""
        # function name
        retString += self._genKernelDefine() + "\n{\n"
        retString += "    #define STRIDE " + str(self.stride) + "\n"
        retString += "    int i, j;\n"

        # initialize accumulators as bias
        retString += """    /* MACs for each output */
    for (i = 0; i < output_y; i++) {
        for (j = 0; j < output_x / 2; j++) {
            q7_t *cols_8b = cols_8b_iterptr;

            q31_t sum0 = bias[0] + biasR[0];
            q31_t sum1 = bias[0] + biasR[0];
            """
        # computation
        retString += """
            /* computation */\n"""
        retString += self._genConvString()

        # requantize
        if self.fp_requantize:
            retString += """
            /* requantize */
            sum0 = (float) sum0 * *scales;
            sum0 += output_offset;
            sum0 = TN_MAX(sum0, activation_min);
            sum0 = TN_MIN(sum0, activation_max);
            output[(i * output_x + j * 2) * channel_offset] = sum0;

            sum1 = (float) sum1 * *scales;
            sum1 += output_offset;
            sum1 = TN_MAX(sum1, activation_min);
            sum1 = TN_MIN(sum1, activation_max);
            output[(i * output_x + (j * 2 + 1)) * channel_offset] = sum1;

            cols_8b_iterptr += STRIDE * 2;
        }
        """
        else:
            retString += """
            /* requantize */
            sum0 = arm_nn_requantize(sum0 + biasR[0], *multiplier, *shift);
            sum0 += output_offset;
            sum0 = TN_MAX(sum0, activation_min);
            sum0 = TN_MIN(sum0, activation_max);
            output[(i * output_x + j * 2) * channel_offset] = sum0;

            sum1 = arm_nn_requantize(sum1 + biasR[0], *multiplier, *shift);
            sum1 += output_offset;
            sum1 = TN_MAX(sum1, activation_min);
            sum1 = TN_MIN(sum1, activation_max);
            output[(i * output_x + (j * 2 + 1)) * channel_offset] = sum1;

            cols_8b_iterptr += STRIDE * 2;
        }
        """

        # left over for odd dimension
        retString += """if (output_x & 1) {
            q7_t * cols_8b = cols_8b_iterptr;
            q31_t sum = bias[0] + biasR[0];\n"""

        retString += self._genConvLeftString()

        if self.fp_requantize:
            retString += """
            sum = (float) sum * *scales;
            sum += output_offset;
            sum = TN_MAX(sum, activation_min);
            sum = TN_MIN(sum, activation_max);
            output[(i * output_x + output_x - 1) * channel_offset] = sum;

            cols_8b_iterptr += STRIDE;
        }
        """
        else:
            retString += """
            sum = arm_nn_requantize(sum + biasR[0], *multiplier, *shift);
            sum += output_offset;
            sum = TN_MAX(sum, activation_min);
            sum = TN_MIN(sum, activation_max);
            output[(i * output_x + output_x - 1) * channel_offset] = sum;

            cols_8b_iterptr += STRIDE;
        }
        """
        if self.stride != 1:
            retString += "cols_8b_iterptr += " + str(self.pad_w) + " * 2 - (column_x & 1);\n"
            retString += "        cols_8b_iterptr += (STRIDE - 1) * (column_x + " + str(self.pad_w) + " * 2);\n"
        else:
            retString += "cols_8b_iterptr += " + str(self.pad_w) + " * 2;\n"

        # end of function
        retString += """    }
}\n"""
        return retString

    def _genKernelCWH(self):
        retString = ""
        # function name
        retString += self._genKernelDefine() + "\n{\n"
        retString += "    #define STRIDE " + str(self.stride) + "\n"
        retString += "    int i, j;\n"

        # initialize accumulators as bias
        retString += """    /* MACs for each output */
    for (j = 0; j < output_x; j++) {
        for (i = 0; i < output_y / 2; i++) {
            q7_t *cols_8b = cols_8b_iterptr;

            q31_t sum0 = bias[0] + biasR[0];
            q31_t sum1 = bias[0] + biasR[0];
            """
        # computation
        retString += """
            /* computation */\n"""
        retString += self._genConvStringCWH()

        # requantize
        if self.fp_requantize:
            retString += """
            /* requantize */
            sum0 = (float) sum0 * *scales;
            sum0 += output_offset;
            sum0 = TN_MAX(sum0, activation_min);
            sum0 = TN_MIN(sum0, activation_max);
            output[(i * output_x + j * 2) * channel_offset] = sum0;

            sum1 = (float) sum1 * *scales;
            sum1 += output_offset;
            sum1 = TN_MAX(sum1, activation_min);
            sum1 = TN_MIN(sum1, activation_max);
            output[(i * output_x + (j * 2 + 1)) * channel_offset] = sum1;

            cols_8b_iterptr += STRIDE * 2;
        }
        """
        else:
            retString += """
            /* requantize */
            sum0 = arm_nn_requantize(sum0 + biasR[0], *multiplier, *shift);
            sum0 += output_offset;
            sum0 = TN_MAX(sum0, activation_min);
            sum0 = TN_MIN(sum0, activation_max);
            output[(i * output_x + j * 2) * channel_offset] = sum0;

            sum1 = arm_nn_requantize(sum1 + biasR[0], *multiplier, *shift);
            sum1 += output_offset;
            sum1 = TN_MAX(sum1, activation_min);
            sum1 = TN_MIN(sum1, activation_max);
            output[(i * output_x + (j * 2 + 1)) * channel_offset] = sum1;

            cols_8b_iterptr += STRIDE * 2;
        }
        """

        # left over for odd dimension
        retString += """if (output_y & 1) {
            q7_t * cols_8b = cols_8b_iterptr;
            q31_t sum = bias[0] + biasR[0];\n"""

        retString += self._genConvLeftStringCWH()

        if self.fp_requantize:
            retString += """
            sum = (float) sum * *scales;
            sum += output_offset;
            sum = TN_MAX(sum, activation_min);
            sum = TN_MIN(sum, activation_max);
            output[(i * output_x + output_x - 1) * channel_offset] = sum;

            cols_8b_iterptr += STRIDE;
        }
        """
        else:
            retString += """
            sum = arm_nn_requantize(sum + biasR[0], *multiplier, *shift);
            sum += output_offset;
            sum = TN_MAX(sum, activation_min);
            sum = TN_MIN(sum, activation_max);
            output[(i * output_x + output_x - 1) * channel_offset] = sum;

            cols_8b_iterptr += STRIDE;
        }
        """
        if self.stride != 1:
            retString += "cols_8b_iterptr += " + str(self.pad_h) + " * 2 - (column_x & 1);\n"
            retString += "        cols_8b_iterptr += (STRIDE - 1) * (column_x + " + str(self.pad_h) + " * 2);\n"
        else:
            retString += "cols_8b_iterptr += " + str(self.pad_h) + " * 2;\n"

        # end of function
        retString += """    }
}\n"""
        return retString


class includeFile(object):
    def _init_(self, path):
        self.path = path
        self.defstring = ""

    def addDefine(self, defstr):
        self.defstring += defstr + ";\n"

    def writeFile(self):
        import os

        outpath = os.path.join(self.path, "genInclude.h")
        outf = open(outpath, "w")
        outf.write(self.defstring)
