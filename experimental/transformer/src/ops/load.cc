#include "operators.h"
#include "utils.h"

void load_LayerNormQ(LayerNormQ &op, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), op.params.weight.m_data, op.params.weight.length());
    read_to_array((prefix + "/bias.bin").c_str(), op.params.bias.m_data, op.params.bias.length());
}
void load_BMM_S8T_S8N_F32T(BMM_S8T_S8N_F32T &op, std::string prefix) {
    read_to_array((prefix + "/alpha.bin").c_str(), &op.alpha, 1);
}
void load_BMM_S8T_S8N_S8T(BMM_S8T_S8N_S8T &op, std::string prefix) {
    read_to_array((prefix + "/alpha.bin").c_str(), &op.alpha, 1);
}
void load_W8A8B8O8Linear_params(W8A8B8O8Linear &op, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), op.params.B.int8_data_ptr, op.params.B.length());
    read_to_array((prefix + "/bias.bin").c_str(), op.params.bias.int32_data_ptr, op.params.bias.length());
    read_to_array((prefix + "/alpha.bin").c_str(), &op.alpha, 1);
    read_to_array((prefix + "/beta.bin").c_str(), &op.beta, 1);
}
void load_W8A8B8O8LinearReLU_params(W8A8B8O8LinearReLU &op, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), op.params.B.int8_data_ptr, op.params.B.length());
    read_to_array((prefix + "/bias.bin").c_str(), op.params.bias.int32_data_ptr, op.params.bias.length());
    read_to_array((prefix + "/alpha.bin").c_str(), &op.alpha, 1);
    read_to_array((prefix + "/beta.bin").c_str(), &op.beta, 1);
}
void load_W8A8BFP32OFP32Linear_params(W8A8BFP32OFP32Linear &op, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), op.params.B.int8_data_ptr, op.params.B.length());
    read_to_array((prefix + "/bias.bin").c_str(), op.params.bias.data_ptr, op.params.bias.length());
    read_to_array((prefix + "/alpha.bin").c_str(), &op.alpha, 1);
}