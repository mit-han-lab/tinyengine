# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   CodeGenerator.py
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

import os

from .constant import FUSE_SGD_UPDATE_STR, FUSHION_CONFIG
from .OpGenerator import OpGenerator

Codegen_root = "./codegen/"
include_path = Codegen_root + "Include/"
source_path = Codegen_root + "Source/"

use_hard_switsh = False
gen_kernels = True
use_aggressive_unroll = True


class CodeGenerator:
    """Provide utilities to generate C code for a given model and memory schdeule."""

    parse_count = 0
    header_handle = None
    source_handle = None

    def __init__(
        self,
        memsche,
        inplace,
        precision=8,
        unsigned_input=False,
        patch_params=None,
        FP_output=False,
        profile_mode=False,
        fp_requantize=False,
        tflite_op=False,
        dummy_address=False,
        outputTables=None,
        detectionUtils=None,
        is_training=False,
    ):
        self.MemSche = memsche

        # Check if path exists, create it if not
        if not os.path.exists(include_path):
            os.makedirs(include_path)
        if not os.path.exists(source_path):
            os.makedirs(source_path)

        self.header_handle = open(include_path + "genModel.h", "w")
        self.source_handle = open(source_path + "genModel.c", "w")
        self.inplace = inplace
        self.BIT = precision
        self.unsigned_input = unsigned_input
        self.patch_params = patch_params
        self.FP_output = FP_output
        self.profile_mode = profile_mode
        self.fp_requantize = fp_requantize
        self.tflite_op = tflite_op
        self.dummy_address = dummy_address
        self.trainSRAMTable = []
        self.outputTables = outputTables
        self.detectionUtils = detectionUtils
        self.is_training = is_training

    def _readOnly(self, name):
        if self.outputTables is None or name is None:
            return True
        else:
            for o in self.outputTables:
                if o.name in name:
                    return False
        return True

    def _generateSGD(self):
        if self.outputTables is None or FUSHION_CONFIG[FUSE_SGD_UPDATE_STR]:
            return
        else:
            fp = self.source_handle
            # generate anchors
            string = """/* SGD functions */
void update_SGD(float learning_rate){\n"""
            for output in self.outputTables:
                if "bias" not in output.name and "weight" not in output.name:
                    continue
                address_str = None
                # backtrace the location of each gradient tensors
                for lay in self.MemSche.layer:
                    # only need to find the output
                    if len(lay.output_tensors) > 0 and lay.output_tensors[0].graph_idx == output.idx:
                        address_str = lay._getBufferstr(
                            lay.params["output_buf_add"],
                            lay.params["output_buf_add_offset"],
                        )
                        break
                assert address_str is not None
                # construct the string for SGD
                string += f"fptr = (float*){address_str}; for(i=0;i < {output.len};i++) v{output.name}[i] += "
                string += "learning_rate * fptr[i];\n"
            string += "}\n"
            fp.write(string)

    def codeGeneration(self):
        # buffer in SRAM
        self._genMemBuffer()

        # parse trainable parameters & assign the corresponding buffers for layers
        self._parseTrainable()

        # include all headers
        self._includeHeaders()

        # generate detection output if any
        self._genDetprocessing()

        # generate patch-based
        self._genPatchInference()

        # generate invoke function
        self._genInvoke()

        # generate inference-only invoke function
        self._genInvokeInf()

        # generate SGD update if any
        self._generateSGD()

        self._closefp()

        # generate operatior kernels
        if gen_kernels:
            op_gen = OpGenerator(include_path, source_path, self.MemSche.layer, self.fp_requantize)
            op_gen.genOpcode()

    def _genDetprocessing(self):
        if self.detectionUtils is not None:
            fp = self.source_handle
            fp.write(self.detectionUtils.genPostProcessing())

    def _genOpstr(self, op, *args):
        if self.profile_mode:
            if len(args) > 0:
                return op.generate_profiling_str(*args)
            else:
                return op.generate_profiling_str()
        else:
            if len(args) > 0:
                return op.generate_inference_str(*args)
            else:
                return op.generate_inference_str()

    def _genPatchInference(self):
        schedule = self.MemSche

        # Find out the first layer for normal infernece
        first_normal_op = None
        last_patch_op = None
        for i, op in enumerate(schedule.layer):
            layer_info = op.get_layer_info()
            if "is_patch" not in layer_info or not layer_info["is_patch"]:
                first_normal_op = op
                break  # end of patch-based
            last_patch_op = op
        assert first_normal_op, "Cannot find the first op for normal inference."

        layer_info = schedule.layer[0].get_layer_info()
        if "is_patch" in layer_info and layer_info["is_patch"]:
            last_patch_op_output_buffer_str_for_patch_inference = last_patch_op._getBufferstr(
                last_patch_op.params["output_buf_add"], last_patch_op.params["output_buf_add_offset"]
            )
            first_bufferstr_for_normal_inference = first_normal_op._getBufferstr(
                first_normal_op.params["input_buf_add"], first_normal_op.params["input_buf_add_offset"]
            )
            assert last_patch_op
            fp = self.source_handle
            string = ""
            first_height = layer_info["input_h"]
            first_width = layer_info["input_w"]
            img_w = (first_width - self.patch_params["pad_l"] - self.patch_params["pad_r"]) * self.patch_params[
                "n_patch"
            ]
            # by default, we go three stride 2 conv in the patch-based inference
            # patch_out_w = int((first_width - self.patch_params["pad_l"]) / 8)
            patch_out_w = last_patch_op.params["output_w"]
            # by default, we go three stride 2 conv in the patch-based inference
            # patch_out_h = int((first_height - self.patch_params["pad_l"]) / 8)
            patch_out_h = last_patch_op.params["output_h"]
            out_w = self.patch_params["output_w"]
            # output_idx for data movement
            output_idx_str = (
                f"((w + j * {patch_out_w}) + (h + i * {patch_out_h}) * {out_w}) * {self.patch_params['output_c']} + c;"
            )
            # generate code for testing whole inference time
            string += (
                """void end2endinference(q7_t* img){
    //stage 1
    int i, j, h, w, c;
    for (i = 0; i < """
                + str(self.patch_params["n_patch"])
                + """; i++){
        uint16_t pad_t=0,pad_b=0;
        if (i == 0){
            pad_t = """
                + str(self.patch_params["pad_l"])
                + """;
        }
        else if (i == """
                + str(self.patch_params["n_patch"] - 1)
                + """){
            pad_b = """
                + str(self.patch_params["pad_r"])
                + """;
        }
        for (j = 0; j < """
                + str(self.patch_params["n_patch"])
                + """; j++){
            uint16_t pad_l=0,pad_r=0;
            if (j == 0){
                pad_l = """
                + str(self.patch_params["pad_l"])
                + """;
            }
            else if (j == """
                + str(self.patch_params["n_patch"] - 1)
                + """){
                pad_r = """
                + str(self.patch_params["pad_r"])
                + """;
            }
            /* load partial input from the img */
            q7_t* patch_input = getInput(); // for partial input
            int start_x = TN_MAX("""
                + str(first_width - self.patch_params["pad_l"] - self.patch_params["pad_r"])
                + """ * j - """
                + str(self.patch_params["pad_l"])
                + """,0);
            int start_y = TN_MAX("""
                + str(first_height - self.patch_params["pad_l"] - self.patch_params["pad_r"])
                + """ * i - """
                + str(self.patch_params["pad_l"])
                + """,0);
            q7_t* img_ptr = &img[(start_x + start_y * """
                + str(img_w)
                + """) * 3];

            //skip top
            patch_input += pad_t * """
                + str(first_width)
                + """ * 3;
            for (h = pad_t; h < """
                + str(first_height)
                + """ - pad_b; h++){
                //skip left
                patch_input += pad_l * 3;
                //fill middle
                int bytes = ("""
                + str(first_width)
                + """ - (pad_l + pad_r)) * 3;
                memcpy (patch_input, img_ptr, bytes);
                img_ptr += """
                + str(img_w)
                + """ * 3;
                patch_input += bytes;
                //skip right
                patch_input += pad_r * 3;
            }
            invoke_1patch(pad_t,pad_b,pad_l,pad_r);
            /* concat the output from buffer0 (this is set manually for now) */
            q7_t* output_ptr = """
                + f"{first_bufferstr_for_normal_inference};"
                + """;
            q7_t* patch_output = """
                + f"{last_patch_op_output_buffer_str_for_patch_inference}"
                + """;
            for (h = 0; h < """
                + str(patch_out_h)
                + """; h++){
                for (w = 0; w < """
                + str(patch_out_w)
                + """; w++){
                    for (c = 0; c < """
                + str(self.patch_params["output_c"])
                + """; c++){
                        int output_idx = """
                + f"{output_idx_str};"
                + """
                        output_ptr[output_idx] = patch_output[(w + h * """
                + str(patch_out_w)
                + """) * """
                + str(self.patch_params["output_c"])
                + """ + c];
                    }
                }
            }
        }
    }
    //stage 2
    invoke(NULL);
}"""
            )
            string += """

void invoke_1patch(uint16_t pad_t, uint16_t pad_b, uint16_t pad_l ,uint16_t pad_r){
"""
            fp.write(string)

            # gen patch-based inference code
            patch_layers = []
            layercnt = 0
            for i, op in enumerate(schedule.layer):
                layer_info = op.get_layer_info()
                if "is_patch" not in layer_info or not layer_info["is_patch"]:
                    break  # end of patch-based
                string = "/* layer " + str(layercnt) + ":" + layer_info["op"] + " */\n"
                layercnt += 1
                fp.write(string)
                if layer_info["op"] == "CONV_2D":
                    if self.unsigned_input:
                        raise Exception("unsigned input is not supported by patch-based yet")

                    string = self._genOpstr(
                        op,
                        False,
                        self.FP_output,
                        use_aggressive_unroll,
                        use_hard_switsh,
                        self.fp_requantize,
                    )
                    fp.write(string)

                elif layer_info["op"] == "DEPTHWISE_CONV_2D":
                    string = self._genOpstr(op, self.fp_requantize)
                    fp.write(string)

                elif layer_info["op"] == "ADD":
                    string = self._genOpstr(op)
                    fp.write(string)

                patch_layers.append(schedule.layer[i])

            # remove these layers for patching for the following code gen
            for layer in patch_layers:
                schedule.layer.remove(layer)

            string = "}\n\n"

            fp.write(string)
        else:  # not patch-based
            string = """void end2endinference(q7_t* img){
    invoke(NULL);
}
"""
            fp = self.source_handle
            fp.write(string)

    def _genInvoke(self):
        fp = self.source_handle
        string = "void invoke(float* labels){\n"
        fp.write(string)

        schedule = self.MemSche
        for i, op in enumerate(schedule.layer):
            layer_info = op.get_layer_info()
            string = "/* layer " + str(i) + ":" + layer_info["op"] + " */\n"
            fp.write(string)

            if layer_info["op"] == "CONV_2D":
                if (
                    self.FP_output
                    and "effective_scale" in layer_info
                    and layer_info["output_scale"] is not None
                    and layer_info["effective_scale"] is not None
                ):
                    use_fp = True
                else:
                    use_fp = False
                string = self._genOpstr(
                    op,
                    self.unsigned_input,
                    use_fp,
                    use_aggressive_unroll,
                    use_hard_switsh,
                    self.fp_requantize,
                    self.tflite_op,
                    self.dummy_address,
                )
                fp.write(string)
            elif layer_info["op"] == "DEPTHWISE_CONV_2D":
                string = self._genOpstr(op, self.fp_requantize)
                fp.write(string)
            elif layer_info["op"] == "GROUP_CONV":
                string = self._genOpstr(op, self.tflite_op, self.dummy_address)
                fp.write(string)
            elif layer_info["op"] == "TRANSPOSE_CONV_2D":
                string = self._genOpstr(op, self.tflite_op, self.dummy_address)
                fp.write(string)
            else:
                string = self._genOpstr(op)
                fp.write(string)

        string = "}\n"
        fp.write(string)

    def _genInvokeInf(self):
        fp = self.source_handle
        string = "void invoke_inf(){\n"
        fp.write(string)

        schedule = self.MemSche
        for i, op in enumerate(schedule.layer):
            layer_info = op.get_layer_info()
            string = "/* layer " + str(i) + ":" + layer_info["op"] + " */\n"
            fp.write(string)

            if layer_info["op"] == "CONV_2D":
                if (
                    self.FP_output
                    and "effective_scale" in layer_info
                    and layer_info["output_scale"] is not None
                    and layer_info["effective_scale"] is not None
                ):
                    use_fp = True
                else:
                    use_fp = False
                string = self._genOpstr(
                    op,
                    self.unsigned_input,
                    use_fp,
                    use_aggressive_unroll,
                    use_hard_switsh,
                    self.fp_requantize,
                    self.tflite_op,
                    self.dummy_address,
                )
                fp.write(string)

                if (
                    layer_info["output_h"] == 1
                    and layer_info["output_w"] == 1
                    and (layer_info["output_c"] == 2 or layer_info["output_c"] == 10)
                ):
                    string = "}\n"
                    fp.write(string)
                    return

            elif layer_info["op"] == "DEPTHWISE_CONV_2D":
                string = self._genOpstr(op, self.fp_requantize)
                fp.write(string)
            else:
                string = self._genOpstr(op)
                fp.write(string)

        string = "}\n"
        fp.write(string)

    def _getBufferIndex(self, location):
        if location == "front":
            return 0
        elif location == "end":
            return 0
        elif location == "residual":
            return 1
        return None

    def _genMemBuffer(self):
        schedule = self.MemSche
        # define output tensor
        string = "#define NNoutput &buffer0[" + str(_findtheinferenceOutput(schedule.layer)) + "];"
        fp = self.header_handle
        fp.write("\n" + string + "\n")

        # activation buffers
        string = "\n/* sram:" + str(schedule.peakmem) + ", flash:" + str(schedule.flash) + " */\n"
        fp.write(string + "\n")

        string = "static signed char buffer[" + str(schedule.peakmem) + "];\n"
        fp.write(string)
        accumulate_ptr = 0
        string = "static signed char *buffer0 = &buffer[" + str(accumulate_ptr) + "];\n"
        accumulate_ptr += int(schedule.buffers["input_output"])
        fp.write(string)
        string = "static signed char *buffer1 = &buffer[" + str(accumulate_ptr) + "];\n"
        accumulate_ptr += int(schedule.buffers["residual"])
        fp.write(string)

        string = "static int16_t *sbuf = (int16_t *)&buffer[" + str(accumulate_ptr) + "];\n"
        accumulate_ptr += int(schedule.buffers["im2col"])
        fp.write(string)
        string = "static int32_t *kbuf = (int32_t *)&buffer[" + str(accumulate_ptr) + "];\n"
        accumulate_ptr += int(schedule.buffers["kernel"])
        fp.write(string)
        string = "const int SBuffer_size = " + str(int(schedule.buffers["im2col"])) + ";\n"
        fp.write(string)
        string = "const int KBuffer_size = " + str(int(schedule.buffers["kernel"])) + ";\n"
        fp.write(string + "\n")

    def _includeHeaders(self):
        include_string = """/* Automatically generated source file */
#include <float.h>
#include "arm_nnfunctions.h"

#include "genNN.h"
#include "genModel.h"

#include "tinyengine_function.h"
#include "tinyengine_function_fp.h"

"""
        if self.profile_mode:
            include_string += '#include "profile.h"\n'

        include_string += (
            """
/* Variables used by all ops */
ADD_params add_params;
//Conv_Params conv_params;
//Depthwise_Params dpconv_params;
int i;
int8_t *int8ptr,*int8ptr2;
int32_t *int32ptr;
float *fptr,*fptr2,*fptr3;

signed char* getInput() {
    return &buffer0["""
            + f"{self.MemSche.layer[0].params['input_buf_add_offset']}"
            + """];
}
signed char* getOutput() {
    return NNoutput;
}\n"""
        )
        fp = self.source_handle
        fp.write(include_string)

    def _parseTrainable(self):
        schedule = self.MemSche
        for i, op in enumerate(schedule.layer):
            layer_info = op.get_layer_info()
            if layer_info["op"] == "CONV_2D":
                if isinstance(layer_info["weight_value"], str):
                    memstr = None
                    for t in op.input_tensors:
                        if t.graph_idx == layer_info["weight_value"]:
                            memstr = f"buffer[{t.buffer_address}]"
                            break
                    assert memstr is not None
                    self._parseWeightSRAM(self.parse_count, memstr)
                else:
                    if layer_info["first_k_channel"] is not None:
                        self._parseWeightPartial(
                            self.parse_count,
                            layer_info["weight_value"],
                            layer_info["first_k_channel"],
                            layer_info["weight_name"],
                            self._readOnly(layer_info["weight_name"]),
                        )
                    else:
                        self._parseWeight(
                            self.parse_count,
                            layer_info["weight_value"].flatten(),
                            layer_info["weight_name"],
                            self._readOnly(layer_info["weight_name"]),
                        )

                if "bias_name" in layer_info:
                    self._parseBias(
                        self.parse_count,
                        layer_info["bias"].flatten(),
                        layer_info["bias_name"],
                        self._readOnly(layer_info["bias_name"]),
                    )
                else:
                    self._parseBias(self.parse_count, layer_info["bias"].flatten())
                self._parseEffectivescales(self.parse_count, layer_info["effective_scale"].flatten())
                self._parseRequantize(
                    self.parse_count,
                    layer_info["shift"].flatten(),
                    layer_info["multiplier"].flatten(),
                )

                layer_info["parsed_trainable"] = self.parse_count
                self.parse_count += 1
            elif layer_info["op"] == "TRANSPOSE_CONV_2D":
                if isinstance(layer_info["weight_value"], str):
                    memstr = None
                    for t in op.input_tensors:
                        if t.graph_idx == layer_info["weight_value"]:
                            memstr = f"buffer[{t.buffer_address}]"
                            break
                    assert memstr is not None
                    self._parseWeightSRAM(self.parse_count, memstr)
                else:
                    self._parseWeight(
                        self.parse_count,
                        layer_info["weight_value"].flatten(),
                        layer_info["weight_name"],
                        self._readOnly(layer_info["weight_name"]),
                    )
                self._parseBias(self.parse_count, layer_info["bias"].flatten())
                layer_info["parsed_trainable"] = self.parse_count
                self.parse_count += 1
            elif layer_info["op"] == "DEPTHWISE_CONV_2D":
                if layer_info["kernel_h"] > layer_info["kernel_w"]:
                    self._parseCWHWeight(
                        self.parse_count,
                        layer_info["weight_value"].flatten(),
                        layer_info["kernel_h"],
                        layer_info["kernel_w"],
                        layer_info["input_c"],
                    )
                else:
                    if layer_info["input_dtype"] == "int8" and layer_info["output_dtype"] == "int8":
                        if "weight_name" in layer_info:
                            self._parseCHWWeight(
                                self.parse_count,
                                layer_info["weight_value"].flatten(),
                                layer_info["input_c"],
                            )
                            # we may need the them in the OHWI format for bp
                            if self.is_training:
                                self._parseWeight(
                                    self.parse_count,
                                    layer_info["weight_value"].flatten(),
                                    layer_info["weight_name"],
                                    self._readOnly(layer_info["weight_name"]),
                                )
                        else:
                            self._parseCHWWeight(
                                self.parse_count,
                                layer_info["weight_value"].flatten(),
                                layer_info["input_c"],
                            )
                        if "bias_name" in layer_info:
                            self._parseoffsetBias(
                                self.parse_count,
                                layer_info["bias"].flatten(),
                                layer_info["input_zero_point"] * -1,
                                layer_info["weight_value"].flatten(),
                                layer_info["input_c"],
                                layer_info["bias_name"],
                                self._readOnly(layer_info["bias_name"]),
                            )
                        else:
                            self._parseoffsetBias(
                                self.parse_count,
                                layer_info["bias"].flatten(),
                                layer_info["input_zero_point"] * -1,
                                layer_info["weight_value"].flatten(),
                                layer_info["input_c"],
                            )
                        self._parseEffectivescales(self.parse_count, layer_info["effective_scale"].flatten())
                        self._parseRequantize(
                            self.parse_count,
                            layer_info["shift"].flatten(),
                            layer_info["multiplier"].flatten(),
                        )
                    else:  # float32
                        if isinstance(layer_info["weight_value"], str):
                            memstr = None
                            for t in op.input_tensors:
                                if t.graph_idx == layer_info["weight_value"]:
                                    memstr = f"buffer[{t.buffer_address}]"
                                    break
                            assert memstr is not None
                            self._parseWeightSRAM(self.parse_count, memstr)
                        else:
                            self._parseWeight(
                                self.parse_count,
                                layer_info["weight_value"].flatten(),
                                layer_info["weight_name"],
                                self._readOnly(layer_info["weight_name"]),
                            )
                        if "bias_name" in layer_info:
                            self._parseBias(
                                self.parse_count,
                                layer_info["bias"].flatten(),
                                layer_info["bias_name"],
                                self._readOnly(layer_info["bias_name"]),
                            )
                        else:
                            self._parseBias(self.parse_count, layer_info["bias"].flatten())

                layer_info["parsed_trainable"] = self.parse_count
                self.parse_count += 1
            elif layer_info["op"] in {"DENSE", "MAT_MUL"}:
                # mapping to some buffer in SRAM
                if isinstance(layer_info["weight_value"], str):
                    memstr = None
                    for t in op.input_tensors:
                        if t.graph_idx == layer_info["weight_value"]:
                            memstr = f"buffer[{t.buffer_address}]"
                            break
                    assert memstr is not None
                    self._parseWeightSRAM(self.parse_count, memstr)
                else:
                    self._parseWeight(
                        self.parse_count,
                        layer_info["weight_value"].flatten(),
                        layer_info["weight_name"],
                        self._readOnly(layer_info["weight_name"]),
                    )

                layer_info["parsed_trainable"] = self.parse_count
                self.parse_count += 1
            elif layer_info["op"] == "BIAS_ADD":
                if "bias_name" in layer_info:
                    self._parseBias(
                        self.parse_count,
                        layer_info["bias"].flatten(),
                        layer_info["bias_name"],
                        self._readOnly(layer_info["bias_name"]),
                    )
                else:
                    self._parseBias(self.parse_count, layer_info["bias"].flatten())
                layer_info["parsed_trainable"] = self.parse_count
                self.parse_count += 1
            elif layer_info["op"] == "GROUP_CONV":
                if layer_info["input_dtype"] == "int8" and layer_info["output_dtype"] == "int8":
                    # TODO: check this is correct
                    if isinstance(layer_info["weight_value"], str):
                        memstr = None
                        for t in op.input_tensors:
                            if t.graph_idx == layer_info["weight_value"]:
                                memstr = f"buffer[{t.buffer_address}]"
                                break
                        assert memstr is not None
                        self._parseWeightSRAM(self.parse_count, memstr)
                    else:
                        self._parseWeight(
                            self.parse_count,
                            layer_info["weight_value"].flatten(),
                            layer_info["weight_name"],
                            self._readOnly(layer_info["weight_name"]),
                        )
                    self._parseBias(self.parse_count, layer_info["bias"].flatten())

                    layer_info["parsed_trainable"] = self.parse_count
                    self.parse_count += 1
                else:
                    if isinstance(layer_info["weight_value"], str):
                        memstr = None
                        for t in op.input_tensors:
                            if t.graph_idx == layer_info["weight_value"]:
                                memstr = f"buffer[{t.buffer_address}]"
                                break
                        assert memstr is not None
                        self._parseWeightSRAM(self.parse_count, memstr)
                    else:
                        self._parseWeight(
                            self.parse_count,
                            layer_info["weight_value"].flatten(),
                            layer_info["weight_name"],
                            self._readOnly(layer_info["weight_name"]),
                        )
                    self._parseBias(self.parse_count, layer_info["bias"].flatten())

                    layer_info["parsed_trainable"] = self.parse_count
                    self.parse_count += 1

    def _parseCWHWeight(self, Lindex, weight, height, width, channel):
        fp = self.header_handle
        # 8bit implementation
        if self.BIT == 8:
            string = "const unsigned char CWHweight" + str(Lindex) + "[" + str(len(weight)) + "] = {"
            fp.write(string)
            for j in range(channel):
                for w in range(width):
                    for h in range(height):
                        value = weight[(h * width + w) * channel + j]
                        if value < 0:
                            value += 256
                        fp.write(str(format(value, "#04x")) + ", ")
        else:
            raise NotImplementedError

        fp.write("};\n")

    def _parseCHWWeight(self, Lindex, weight, channel):
        fp = self.header_handle
        kernelsize = int(len(weight) / channel)
        # 8bit implementation
        if self.BIT == 8:
            string = "const unsigned char CHWweight" + str(Lindex) + "[" + str(len(weight)) + "] = {"
            fp.write(string)
            for j in range(channel):
                for i in range(kernelsize):
                    value = int(weight[i * channel + j])
                    if value < 0:
                        value += 256
                    fp.write(str(format(value, "#04x")) + ", ")
        else:
            raise NotImplementedError

        fp.write("};\n")

    def _parseEffectivescales(self, Lindex, scales):
        fp = self.header_handle
        string = "const float scales" + str(Lindex) + "[" + str(len(scales)) + "] = {"
        fp.write(string)
        for _, value in enumerate(scales):
            fp.write(str(value) + ", ")
        fp.write("};\n")

    def _parseWeight(self, Lindex, weight, weight_name=None, is_const=True):
        fp = self.header_handle
        const_str = "const " if is_const else ""
        string = f"{const_str}unsigned char weight" + str(Lindex) + "[" + str(len(weight)) + "] = {"
        fp.write(string)
        for _, value in enumerate(weight):
            value = int(value)
            if value < 0:
                value += 256
            fp.write(str(format(value, "#04x")) + ", ")
        fp.write("};\n")

        if self.is_training:
            string = f"{const_str}float weight_fp" + str(Lindex) + "[" + str(len(weight)) + "] = {"
            fp.write(string)
            for _, w in enumerate(weight):
                value = float(w)
                fp.write(str(value) + ", ")
            fp.write("};\n")

        if weight_name is not None:
            for r in self.trainSRAMTable:
                if r.name == weight_name:
                    return
            self.trainSRAMTable.append(tensorRecorder(weight_name, len(weight), "unknown"))

            if weight.dtype == "int8":
                string = f"{const_str}unsigned char* {weight_name}=weight" + str(Lindex) + ";\n"
            else:
                raise NotImplementedError
            fp.write(string)

    def _parseWeightPartial(self, Lindex, weight, first_k_channel=None, weight_name=None, is_const=True):
        fp = self.header_handle
        assert not is_const
        weight_SRAM = weight[:, :, :, 0:first_k_channel]
        # weight in SRAM
        string = "unsigned char weight" + str(Lindex) + "[" + str(len(weight_SRAM.flatten())) + "] = {"
        fp.write(string)
        for i in range(len(weight_SRAM.flatten())):
            value = int(weight_SRAM.flatten()[i])
            if value < 0:
                value += 256
            fp.write(str(format(value, "#04x")) + ", ")
        fp.write("};\n")

        weight_Flash = weight[:, :, :, first_k_channel:]
        # weight in Flash
        string = "const unsigned char weight" + str(Lindex) + "Flash[" + str(len(weight_Flash.flatten())) + "] = {"
        fp.write(string)
        for i in range(len(weight_Flash.flatten())):
            value = int(weight_Flash.flatten()[i])
            if value < 0:
                value += 256
            fp.write(str(format(value, "#04x")) + ", ")
        fp.write("};\n")

        if weight_name is not None:
            for r in self.trainSRAMTable:
                if r.name == weight_name:
                    return
            self.trainSRAMTable.append(tensorRecorder(weight_name, len(weight), "unknown"))

            if weight.dtype == "int8":
                string = f"unsigned char* {weight_name}=weight" + str(Lindex) + ";\n"
                string += f"const unsigned char* {weight_name}Flash=weight" + str(Lindex) + "Flash;\n"
            else:
                raise NotImplementedError
            fp.write(string)

    def _parseWeightSRAM(self, Lindex, mem_str):
        fp = self.header_handle
        string = f"signed char *weight{str(Lindex)} = &{mem_str};\n"
        string += f"float *weight_fp{str(Lindex)} = (float *)&{mem_str};\n"
        fp.write(string)

    def _parseoffsetBias(self, Lindex, bias, input_offset, weight, channel, bias_name=None, is_const=True):
        fp = self.header_handle
        const_str = "const " if is_const else ""
        string = f"{const_str}int32_t offsetBias" + str(Lindex) + "[" + str(len(bias)) + "] = {"
        fp.write(string)
        kernelsize = int(len(weight) / channel)
        # fuse the offset into bias
        for i in range(channel):
            tmpW = 0
            for j in range(kernelsize):
                tmpW += weight[j * channel + i]
            fp.write(str(self.int32_clip(bias[i] + tmpW * input_offset)) + ", ")
        fp.write("};\n")
        string = f"{const_str}int32_t offsetRBias" + str(Lindex) + "[" + str(len(bias)) + "] = {"
        fp.write(string)
        kernelsize = int(len(weight) / channel)
        for i in range(channel):
            tmpW = 0
            for j in range(kernelsize):
                tmpW += weight[j * channel + i]
            fp.write(str(bias[i] + tmpW * input_offset - self.int32_clip(bias[i] + tmpW * input_offset)) + ", ")
        fp.write("};\n")

        if bias_name is not None:
            string = f"{const_str}int32_t* {bias_name}=offsetRBias" + str(Lindex) + ";\n"
            fp.write(string)

    def _parseBias(self, Lindex, bias, bias_name=None, is_const=True):
        fp = self.header_handle
        const_str = "const " if is_const else ""
        string = f"{const_str}int32_t bias" + str(Lindex) + "[" + str(len(bias)) + "] = {"
        fp.write(string)
        for _, value in enumerate(bias):
            value = int(value)
            fp.write(str(value) + ", ")
        fp.write("};\n")

        if self.is_training:
            string = f"{const_str}float bias_fp" + str(Lindex) + "[" + str(len(bias)) + "] = {"
            fp.write(string)
            for _, b in enumerate(bias):
                fp.write(str(float(b)) + ", ")
            fp.write("};\n")

        if bias_name is not None:
            string = f"{const_str}int32_t* {bias_name}=bias" + str(Lindex) + ";\n"
            fp.write(string)

    def _parseRequantize(self, Lindex, shift, multiplier):
        fp = self.header_handle
        string = "const int32_t shift" + str(Lindex) + "[" + str(len(shift)) + "] = {"
        fp.write(string)
        for _, value in enumerate(shift):
            fp.write(str(value) + ", ")
        fp.write("};\n")

        string = "const int32_t multiplier" + str(Lindex) + "[" + str(len(multiplier)) + "] = {"
        fp.write(string)
        for _, value in enumerate(multiplier):
            fp.write(str(value) + ", ")
        fp.write("};\n")

    def int32_clip(self, a):
        if a < -(2**31):
            return -(2**31)
        elif a > 2**31 - 1:
            return 2**31 - 1
        return a.astype(int)

    def _closefp(self):
        self.header_handle.close()
        self.source_handle.close()


def _findtheinferenceOutput(layers):
    for cnt, op in enumerate(layers):
        if op.params["output_dtype"] != "int8":
            return layers[cnt - 1].params["output_buf_add_offset"]
    return layers[-1].params["output_buf_add_offset"]


class tensorRecorder:
    def __init__(self, name, len, dtype):
        self.name = name
        self.len = len
        self.dtype = dtype
