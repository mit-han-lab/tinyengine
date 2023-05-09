#include "Int8OPTDecoderLayer.h"

#include "utils.h"

template <typename T>
void add(Matrix3D<T> a, Matrix3D<T> b, Matrix3D<T> c) {
    assert(c.length() == a.length() && a.length() == b.length());

    for (int i = 0; i < a.length(); i++) {
        c.m_data[i] = a.m_data[i] + b.m_data[i];
    }
}

// TODO: allocate memory for these!
float hidden_states_float_arr[MAXSQLEN * EMBED_DIM];
float hidden_states_floatGT_arr[MAXSQLEN * EMBED_DIM];
int8_t final_layer_norm_arr[MAXSQLEN * EMBED_DIM];
int8_t final_layer_normGT_arr[MAXSQLEN * EMBED_DIM];
int8_t fc_1_x_arr[MAXSQLEN * EMBED_DIM];
int8_t fc_1_arr[MAXSQLEN * HIDDEN_DIM];
int8_t fc_1_weight_arr[HIDDEN_DIM * EMBED_DIM];
int32_t fc_1_bias_arr[HIDDEN_DIM];
int8_t fc_1_arrGT[MAXSQLEN * HIDDEN_DIM];
float fc_2_arr[MAXSQLEN * EMBED_DIM];
float temp[MAXSQLEN * EMBED_DIM];
int8_t hidden_states_int8_arr[MAXSQLEN * EMBED_DIM];
struct Int8OPTDecoderLayer_output Int8OPTDecoderLayer::forward(const struct Int8OPTDecoderLayer_input &input) {
    PROFILE_START(profile_name);
    Matrix3D<int8_t> hidden_states_int8(hidden_states_int8_arr, input.hidden_states.m_dim_x,
                                        input.hidden_states.m_dim_y, input.hidden_states.m_dim_z);
    this->self_attn_layer_norm.forward(input.hidden_states, hidden_states_int8);

    struct Int8OPTAttention_input attn_param(hidden_states_int8, input.attention_mask, input.past_key, input.past_value,
                                             input.has_past_key_value, this->layer_idx);
    struct Int8OPTAttention_output attn_output = this->attn.forward(attn_param);

    // opt.py: residual.add_(hidden_states.to(residual.dtype))
    Matrix3D<float> residual_add(hidden_states_float_arr, input.hidden_states.m_dim_x, input.hidden_states.m_dim_y,
                                 input.hidden_states.m_dim_z);
    add(input.hidden_states, attn_output.attn_output, residual_add);

    // opt.py: hidden_states = self.final_layer_norm(residual)
    Matrix3D<int8_t> final_layer_norm(final_layer_norm_arr, input.hidden_states.m_dim_x, input.hidden_states.m_dim_y,
                                      input.hidden_states.m_dim_z);
    this->final_layer_norm.forward(residual_add, final_layer_norm);

    // opt.py: hidden_states = self.fc1(hidden_states)
    Matrix3D<int8_t> fc1_out(fc_1_arr, input.hidden_states.m_dim_x, input.hidden_states.m_dim_y, this->hidden_dim);
    // this->fc1.x = final_layer_norm;
    // this->fc1.output = fc1_out;
    this->fc1.forward(final_layer_norm, fc1_out);
    // W8A8B8O8LinearReLU(this->fc1);

    // opt.py: hidden_states = self.fc2(hidden_states)
    Matrix3D<float> fc2_out(fc_2_arr, input.hidden_states.m_dim_x, input.hidden_states.m_dim_y,
                            input.hidden_states.m_dim_z);
    this->fc2.forward(fc1_out, fc2_out);

    // opt.py: residual.add_(hidden_states.to(residual.dtype))
    add(residual_add, fc2_out, residual_add);

    struct Int8OPTDecoderLayer_output output(residual_add, attn_output.attn_probs_reshaped, attn_output.past_key_value);
    PROFILE_END(profile_name);
    return output;
}

Int8OPTDecoderLayer::Int8OPTDecoderLayer(std::string param_path, int embed_dim, int num_heads, int hidden_dim, int layer_idx,
                                         LayerNormQ self_attn_layer_norm, LayerNormQ final_layer_norm,
                                         W8A8B8O8LinearReLU fc1, W8A8BFP32OFP32Linear fc2, BMM_S8T_S8N_F32T qk_bmm,
                                         BMM_S8T_S8N_S8T pv_bmm, W8A8B8O8Linear k_proj, W8A8B8O8Linear v_proj,
                                         W8A8B8O8Linear q_proj, W8A8BFP32OFP32Linear out_proj) {
    load_LayerNormQ(self_attn_layer_norm, param_path + "/self_attn_layer_norm");
    load_W8A8B8O8LinearReLU_params(fc1, param_path + "/fc1");
    load_W8A8BFP32OFP32Linear_params(fc2, param_path + "/fc2");
    load_LayerNormQ(final_layer_norm, param_path + "/final_layer_norm");

    this->embed_dim = embed_dim;
    this->num_attention_heads = num_heads;
    this->hidden_dim = hidden_dim;
    this->layer_idx = layer_idx;
    this->self_attn_layer_norm = self_attn_layer_norm;
    this->fc1 = fc1;
    this->fc2 = fc2;
    this->final_layer_norm = final_layer_norm;

    this->attn = Int8OPTAttention(param_path + "/self_attn", embed_dim, num_heads, qk_bmm, pv_bmm, k_proj, v_proj,
                                  q_proj, out_proj);
}