#include "common.h"
#include "operators.h"
#include <utility>

struct Int8OPTAttention_output{
    Matrix3D<int8_t> attn_output;
    Matrix3D<int8_t> attn_probs_reshaped;
    std::pair<Matrix3D<int8_t>, Matrix3D<int8_t>> past_key_value;
};
struct Int8OPTAttention_input{
    Matrix3D<int8_t> hidden_states;
};

void load_BMM_S8T_S8N_F32T(struct BMM_S8T_S8N_F32T_params &param, std::string prefix);
void load_BMM_S8T_S8N_S8T(struct BMM_S8T_S8N_S8T_params &param, std::string prefix);
void load_W8A8B8O8Linear_params(struct W8A8B8O8Linear_params &param, std::string prefix);
void load_W8A8BFP32OFP32Linear_params(struct W8A8BFP32OFP32Linear_params &param, std::string prefix);

class Int8OPTAttention{
public:
    Int8OPTAttention(
        int embed_dim, int num_heads, struct BMM_S8T_S8N_F32T_params &qk_bmm,
        struct BMM_S8T_S8N_S8T_params &pv_bmm, struct W8A8B8O8Linear_params &k_proj,
        struct W8A8B8O8Linear_params &v_proj, struct W8A8B8O8Linear_params &q_proj,
        struct W8A8BFP32OFP32Linear_params &out_proj
    );
    struct Int8OPTAttention_output forward(const struct Int8OPTAttention_input &input);
private:
    int embed_dim, num_heads, head_dim;
    struct BMM_S8T_S8N_F32T_params *qk_bmm;
    struct BMM_S8T_S8N_S8T_params *pv_bmm;
    struct W8A8B8O8Linear_params *k_proj, *v_proj, *q_proj;
    struct W8A8BFP32OFP32Linear_params *out_proj;
};