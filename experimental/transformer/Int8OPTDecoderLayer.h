#include "Int8OPTAttention.h"
#include "common.h"
#include "operators.h"

struct Int8OPTDecoderLayer_output {
    Matrix3D<float> hidden_states;
    Matrix3D<int8_t> attentions;
    std::pair<Matrix3D<int8_t>, Matrix3D<int8_t>> past_key_value;

    Int8OPTDecoderLayer_output(Matrix3D<float> hidden_states_, Matrix3D<int8_t> attentions_,
                               std::pair<Matrix3D<int8_t>, Matrix3D<int8_t>> past_key_value_) {
        hidden_states = hidden_states_;
        attentions = attentions_;
        past_key_value = past_key_value_;
    };
};
struct Int8OPTDecoderLayer_input {
    Matrix3D<float> hidden_states;
    Matrix3D<float> attention_mask;
};

class Int8OPTDecoderLayer {
   public:
    Int8OPTDecoderLayer(std::string param_path, int embed_dim, int num_heads, int hidden_dim,
                        LayerNormQ &self_attn_layer_norm, LayerNormQ &final_layer_norm,
                        W8A8B8O8LinearReLU &fc1, struct W8A8BFP32OFP32Linear_params &fc2,
                        struct BMM_S8T_S8N_F32T_params &qk_bmm, struct BMM_S8T_S8N_S8T_params &pv_bmm,
                        W8A8B8O8Linear &k_proj, W8A8B8O8Linear &v_proj,
                        W8A8B8O8Linear &q_proj, struct W8A8BFP32OFP32Linear_params &out_proj);
    struct Int8OPTDecoderLayer_output forward(const struct Int8OPTDecoderLayer_input &input);

    int embed_dim, num_attention_heads, hidden_dim;
    LayerNormQ self_attn_layer_norm, final_layer_norm;  // from torch_int.nn
    W8A8B8O8LinearReLU fc1;
    struct W8A8BFP32OFP32Linear_params fc2;
    Int8OPTAttention attn;
};