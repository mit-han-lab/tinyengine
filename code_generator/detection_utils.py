# ----------------------------------------------------------------------
# Project: TinyEngine
# Title:   detection_utils.py
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

import json


class detectionUtils:
    def __init__(self, layers, det_config_path) -> None:
        with open(det_config_path, "r") as f:
            self.det_conf = json.loads(f.read())
        self.layers = layers
        pass

    def genPostProcessing(self):
        output_string = ""
        output_ch = None
        num_class = None
        # generate anchors
        assert len(self.det_conf) == 3, "Only support 3 detection outputs."
        output_string += """const int anchors[3][3][2] = {\n"""
        for key in self.det_conf:
            output_config = self.det_conf[key]
            num_class = output_config["num_class"]
            assert output_config["name"] == "Yolo3Output", "Only support Yolo3Output now."
            output_string += "{"
            for i in range(len(output_config["anchors"]) // 2):
                output_string += "{" + f"{output_config['anchors'][i * 2]},{output_config['anchors'][i * 2 + 1]}" + "},"
            output_string += "},\n"
        output_string += "};\n"
        # generate the post processing part
        output_string += """void det_post_procesing(int* box_cnt, det_box** ret_box, float threshold){\n"""
        tensorstring = "    int8_t *input_data" + f"[{str(len(self.det_conf))}]" + "={"
        zerostring = "    int8_t y_zero" + f"[{str(len(self.det_conf))}]" + "={"
        scalestring = "    float y_scale" + f"[{str(len(self.det_conf))}]" + "={"
        top_avaialble_addrees = 0
        for key in self.det_conf:
            output_config = self.det_conf[key]
            # Find out the op that gerneate this output
            target_id = output_config["input_id"]
            for layer in self.layers:
                if str(layer.params["output_idx"]) == str(target_id):
                    print(f"Processing the output {layer.params['output_idx']} as a detection feature map.")
                    tensorstring += (
                        f"{layer._getBufferstr(layer.params['output_buf_add'], layer.params['output_buf_add_offset'])},"
                    )
                    zerostring += f'{layer.params["output_zero_point"]},'
                    scalestring += f'{layer.params["output_scale"]},'
                    output_ch = layer.params["output_c"]
                    top_avaialble_addrees = max(
                        top_avaialble_addrees, layer.params["output_buf_add_offset"] + layer.output_tensors[0].len()
                    )

        import math

        # Align by 4
        top_avaialble_addrees = math.ceil((float(top_avaialble_addrees) / 4) * 4)

        assert output_ch is not None
        tensorstring += "};\n"
        zerostring += "};\n"
        scalestring += "};\n"
        output_string += tensorstring + zerostring + scalestring
        output_string += (
            f" 	postprocessing(input_data, y_zero, y_scale, &buffer[{top_avaialble_addrees}],"
            + f" {self.layers[0].params['input_w']}, "
            + f'{self.layers[0].params["input_h"]}, {output_ch}, {num_class}, '
            + f"anchors, {len(self.det_conf)}, "
            + "0.45f, threshold, box_cnt, ret_box);\n}\n"
        )

        return output_string
