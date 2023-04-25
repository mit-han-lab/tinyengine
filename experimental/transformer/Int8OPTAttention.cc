#include "Int8OPTAttention.h"
#include "utils.h"

struct Int8OPTAttention_output Int8OPTAttention::forward(const struct Int8OPTAttention_input &input) {
    struct Int8OPTAttention_output output;

    return output;
}

Int8OPTAttention::Int8OPTAttention(int embed_dim, int num_heads, struct BMM_S8T_S8N_F32T_params &qk_bmm,
                                   struct BMM_S8T_S8N_S8T_params &pv_bmm, struct W8A8B8O8Linear_params &k_proj,
                                   struct W8A8B8O8Linear_params &v_proj, struct W8A8B8O8Linear_params &q_proj,
                                   struct W8A8BFP32OFP32Linear_params &out_proj) {
    this->embed_dim = embed_dim;
    this->num_heads = num_heads;
    this->head_dim = embed_dim / num_heads;
    this->qk_bmm = &qk_bmm;
    this->pv_bmm = &pv_bmm;
    this->k_proj = &k_proj;
    this->v_proj = &v_proj;
    this->q_proj = &q_proj;
    this->out_proj = &out_proj;
}

void load_BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &param, std::string prefix){
    read_to_array((prefix + "_alpha.bin").c_str(), &param.alpha, 1);
}
void load_BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &param, std::string prefix){
    read_to_array((prefix + "_alpha.bin").c_str(), &param.alpha, 1);
}
void load_W8A8B8O8Linear_params(struct W8A8B8O8Linear_params &param, std::string prefix){
    read_to_array((prefix + "_weight.bin").c_str(), param.weight.m_data, param.weight.lenth());
    read_to_array((prefix + "_bias.bin").c_str(), param.bias.m_data, param.bias.lenth());
    read_to_array((prefix + "_alpha.bin").c_str(), &param.alpha, 1);
    read_to_array((prefix + "_beta.bin").c_str(), &param.beta, 1);
}
void load_W8A8BFP32OFP32Linear_params(struct W8A8BFP32OFP32Linear_params &param, std::string prefix){
    read_to_array((prefix + "_weight.bin").c_str(), param.weight.m_data, param.weight.lenth());
    read_to_array((prefix + "_bias.bin").c_str(), param.bias.m_data, param.bias.lenth());
    read_to_array((prefix + "_alpha.bin").c_str(), &param.alpha, 1);
}


