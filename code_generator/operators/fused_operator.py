# code_generator/operators/fused_operator.py

from .basic_utils import basicOperator, deep_copy_dicts, overwrite_dicts
import numpy as np

default_params = {
    "op": "FUSED_RESHAPE_ADD_TRANSPOSE_FC",
    "input_idx": None,
    "weight_idx": None,
    "bias_idx": None,
    "output_idx": None,
    "input_dtype": "float32",
    "weight_dtype": "float32",
    "output_dtype": "float32",
    "input_shape": None,
    "output_shape": None
}

class FusedOperator(basicOperator):
    def __init__(self, params: dict) -> None:
        self.params = deep_copy_dicts(default_params)
        overwrite_dicts(self.params, params)
        super().__init__()
        self._add_input(self.params["input_idx"], self.params["input_dtype"], *self.params["input_shape"])
        self._add_output(self.params["output_idx"], self.params["output_dtype"], *self.params["output_shape"])

    def generate_inference_str(self):
        return self._generate_fused_operation_str()

    def _generate_fused_operation_str(self):
        input_shape = self.params["input_shape"]
        output_shape = self.params["output_shape"]
        
        # Generate the necessary strings to handle reshaping, addition, transposition, and FC
        return f"""
        void fused_operation(float* input, float* weight, float* bias, float* output) {{
            // Reshape
            float reshaped_input[{np.prod(output_shape)}];
            reshape(input, reshaped_input, {input_shape}, {output_shape});
            
            // Add
            float added_output[{np.prod(output_shape)}];
            add(reshaped_input, weight, added_output);
            
            // Transpose
            float transposed_output[{np.prod(output_shape)}];
            transpose(added_output, transposed_output);
            
            // Fully Connected
            fully_connected(transposed_output, weight, bias, output);
        }}
        """

    def get_macs(self):
        # Implement MACs calculation for the fused operator
        return 0

    def get_weights_size(self):
        # Implement weight size calculation for the fused operator
        return 0

    def get_bias_size(self):
        # Implement bias size calculation for the fused operator
        return 0

    def get_scale_size(self):
        # Implement scale size calculation for the fused operator
        return 0
