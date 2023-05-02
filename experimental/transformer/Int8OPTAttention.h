#include <utility>

#include "common.h"
#include "operators.h"

struct Int8OPTAttention_output {
    Matrix3D<float> attn_output;
    Matrix3D<int8_t> attn_probs_reshaped;
    std::pair<Matrix3D<int8_t>, Matrix3D<int8_t>> past_key_value;
};
struct Int8OPTAttention_input {
    Matrix3D<int8_t> hidden_states;
    Matrix3D<float> attention_mask;
    Matrix3D<int8_t> *past_key = NULL, *past_value = NULL;

    Int8OPTAttention_input(Matrix3D<int8_t> hidden_states_, Matrix3D<float> attention_mask_)
        : hidden_states(hidden_states_), attention_mask(attention_mask_) {}
};

class Int8OPTAttention {
   public:
    Int8OPTAttention(int embed_dim, int num_heads, struct BMM_S8T_S8N_F32T_params &qk_bmm,
                     struct BMM_S8T_S8N_S8T_params &pv_bmm, W8A8B8O8Linear &k_proj, W8A8B8O8Linear &v_proj,
                     W8A8B8O8Linear &q_proj, W8A8BFP32OFP32Linear &out_proj);
    Int8OPTAttention(std::string param_path, int embed_dim, int num_heads, struct BMM_S8T_S8N_F32T_params &qk_bmm,
                     struct BMM_S8T_S8N_S8T_params &pv_bmm, W8A8B8O8Linear &k_proj, W8A8B8O8Linear &v_proj,
                     W8A8B8O8Linear &q_proj, W8A8BFP32OFP32Linear &out_proj);
    Int8OPTAttention() {}
    struct Int8OPTAttention_output forward(const struct Int8OPTAttention_input &input);

   private:
    void unshape(Matrix3D<int8_t> shaped, Matrix3D<int8_t> unshape, int sqlen);
    void shpae(Matrix3D<int8_t> unshape, Matrix3D<int8_t> shaped, int sqlen);
    int embed_dim, num_heads, head_dim;
    struct BMM_S8T_S8N_F32T_params qk_bmm;
    struct BMM_S8T_S8N_S8T_params pv_bmm;
    W8A8B8O8Linear k_proj, v_proj, q_proj;
    W8A8BFP32OFP32Linear out_proj;
};