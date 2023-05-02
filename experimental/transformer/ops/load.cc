#include "../operators.h"
#include "../utils.h"

void load_LayerNormQ(struct LayerNormQ_params &param, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), param.weight.m_data, param.weight.length());
    read_to_array((prefix + "/bias.bin").c_str(), param.bias.m_data, param.bias.length());
}
void load_BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &param, std::string prefix) {
    read_to_array((prefix + "/alpha.bin").c_str(), &param.alpha, 1);
}
void load_BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &param, std::string prefix) {
    read_to_array((prefix + "/alpha.bin").c_str(), &param.alpha, 1);
}
void load_W8A8B8O8Linear_params(struct W8A8B8O8Linear_params &param, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), param.weight.m_data, param.weight.length());
    read_to_array((prefix + "/bias.bin").c_str(), param.bias.m_data, param.bias.length());
    read_to_array((prefix + "/alpha.bin").c_str(), &param.alpha, 1);
    read_to_array((prefix + "/beta.bin").c_str(), &param.beta, 1);
}
void load_W8A8BFP32OFP32Linear_params(struct W8A8BFP32OFP32Linear_params &param, std::string prefix) {
    read_to_array((prefix + "/weight.bin").c_str(), param.weight.m_data, param.weight.length());
    read_to_array((prefix + "/bias.bin").c_str(), param.bias.m_data, param.bias.length());
    read_to_array((prefix + "/alpha.bin").c_str(), &param.alpha, 1);
}
