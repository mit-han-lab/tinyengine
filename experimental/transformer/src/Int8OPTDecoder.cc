#include <iostream>
#include <cstring>
#include "Int8OPTDecoder.h"

#include "OPTDecoder.h"

float attention_mask_buf[MAXSQLEN * MAXSQLEN];
float pos_embeds_buf[MAXSQLEN * MAXSQLEN];

Matrix3D<float> Int8OPTDecoder::prepare_decoder_attention_mask(int length, int past_length){
    assert(length - past_length > 0);
    Matrix3D<float> causal_attention_mask(attention_mask_buf, 1, length - past_length, length);
    for (int i = 0; i < length - past_length; i++){
        for (int j = 0; j < length; j++){
            if (i + past_length < j){
                causal_attention_mask(0, i, j) = -65504;
            }
            else{
                causal_attention_mask(0, i, j) = 0;
            }
        }
    }

    return causal_attention_mask;
}

Matrix3D<float> Int8OPTDecoder::get_position_embed(int sql_length, int past_length){
    const int offset = 2; // This is specific for OPT model
    Matrix3D<float> pos_embeds(attention_mask_buf, 1, sql_length, this->embed_dim);

    int start_idx = past_length + offset, end_idx = past_length + sql_length + offset;
    assert(end_idx < this->embed_positions.lookup.m_dim_y);

    memcpy(pos_embeds.m_data, &this->embed_positions.lookup.m_data[start_idx*this->embed_dim], sql_length * this->embed_dim * sizeof(float));

    return pos_embeds;
}

Int8OPTDecoder::Int8OPTDecoder(std::string param_path, int voc_size_, int embed_dim_, int hidden_dim_, int num_heads_,
                               int padding_idx_, int num_layers) {
    this->voc_size = voc_size_;
    this->embed_dim = embed_dim_;
    this->hidden_dim = hidden_dim_;
    this->num_heads = num_heads_;
    this->padding_idx = padding_idx_;
    
    // Embedding
    Matrix3D<float> embweight(new float[voc_size*embed_dim], 1, voc_size, embed_dim);
    this->embed_tokens = Embedding(embed_dim, voc_size, padding_idx, embweight);
    load_Embedding_params(this->embed_tokens, param_path + "/embed_tokens");
    Matrix3D<float> posweight(new float[2048*embed_dim], 1, 2048, embed_dim);
    this->embed_positions = Embedding(embed_dim, 2048, padding_idx, posweight);
    load_Embedding_params(this->embed_positions, param_path + "/embed_positions");

    // Load all the decoder layers
    for (int layer_idx = 0; layer_idx < num_layers; layer_idx++) {
        DEBUG_INS(std::cout << "Start loading layer:" << layer_idx << "..." << std::endl;)
        struct BMM_S8T_S8N_F32T_params qk_bmm;
        struct BMM_S8T_S8N_S8T_params pv_bmm;
        struct W8A8B8O8Linear_params k_proj, v_proj, q_proj;
        Matrix3D<int8_t> k_proj_weight(new int8_t[embed_dim * embed_dim], 1, embed_dim, embed_dim);
        Matrix3D<int32_t> k_proj_bias(new int32_t[embed_dim], 1, 1, embed_dim);
        k_proj.weight = k_proj_weight;
        k_proj.bias = k_proj_bias;
        auto k_proj_op = W8A8B8O8Linear(k_proj);
        Matrix3D<int8_t> v_proj_weight(new int8_t[embed_dim * embed_dim], 1, embed_dim, embed_dim);
        Matrix3D<int32_t> v_proj_bias(new int32_t[embed_dim], 1, 1, embed_dim);
        v_proj.weight = v_proj_weight;
        v_proj.bias = v_proj_bias;
        auto v_proj_op = W8A8B8O8Linear(v_proj);
        Matrix3D<int8_t> q_proj_weight(new int8_t[embed_dim * embed_dim], 1, embed_dim, embed_dim);
        Matrix3D<int32_t> q_proj_bias(new int32_t[embed_dim], 1, 1, embed_dim);
        q_proj.weight = q_proj_weight;
        q_proj.bias = q_proj_bias;
        auto q_proj_op = W8A8B8O8Linear(q_proj);

        struct W8A8BFP32OFP32Linear_params out_proj;
        Matrix3D<int8_t> out_proj_weight(new int8_t[embed_dim * embed_dim], 1, embed_dim, embed_dim);
        Matrix3D<float> out_proj_bias(new float[embed_dim], 1, 1, embed_dim);
        out_proj.weight = out_proj_weight;
        out_proj.bias = out_proj_bias;
        auto out_proj_op = W8A8BFP32OFP32Linear(out_proj);

        struct LayerNormQ_params self_attn_layer_norm, final_layer_norm;
        Matrix3D<float> self_attn_layer_norm_weight(new float[embed_dim], 1, 1, embed_dim);
        Matrix3D<float> self_attn_layer_norm_bias(new float[embed_dim], 1, 1, embed_dim);
        self_attn_layer_norm.weight = self_attn_layer_norm_weight;
        self_attn_layer_norm.bias = self_attn_layer_norm_bias;

        Matrix3D<float> final_layer_norm_weight(new float[embed_dim], 1, 1, embed_dim);
        Matrix3D<float> final_layer_norm_bias(new float[embed_dim], 1, 1, embed_dim);
        final_layer_norm.weight = final_layer_norm_weight;
        final_layer_norm.bias = final_layer_norm_bias;
        LayerNormQ self_attn_layer_norm_op = LayerNormQ(self_attn_layer_norm);
        LayerNormQ final_layer_norm_op = LayerNormQ(final_layer_norm);

        struct W8A8B8O8Linear_params fc1;
        Matrix3D<int8_t> fc1_weight(new int8_t[embed_dim * hidden_dim], 1, hidden_dim, embed_dim);
        Matrix3D<int32_t> fc1_bias(new int32_t[hidden_dim], 1, 1, hidden_dim);
        fc1.weight = fc1_weight;
        fc1.bias = fc1_bias;
        auto fc1_op = W8A8B8O8LinearReLU(fc1);

        struct W8A8BFP32OFP32Linear_params fc2;
        Matrix3D<int8_t> fc2_weight(new int8_t[embed_dim * hidden_dim], 1, embed_dim, hidden_dim);
        Matrix3D<float> fc2_bias(new float[embed_dim], 1, 1, embed_dim);
        fc2.weight = fc2_weight;
        fc2.bias = fc2_bias;
        auto fc2_op = W8A8BFP32OFP32Linear(fc2);

        auto qk_bmm_op = BMM_S8T_S8N_F32T(qk_bmm);
        auto pv_bmm_op = BMM_S8T_S8N_S8T(pv_bmm);

        std::string path = param_path + "/layer" + std::to_string(layer_idx);

        Int8OPTDecoderLayer layer = Int8OPTDecoderLayer(
            path, embed_dim, num_heads, hidden_dim, layer_idx, self_attn_layer_norm_op,
            final_layer_norm_op, fc1_op, fc2_op, qk_bmm_op, pv_bmm_op, k_proj_op, v_proj_op, q_proj_op, out_proj_op);
        
        this->layers.push_back(layer);
    }
};

struct Int8OPTDecoder_output Int8OPTDecoder::forward(const struct Int8OPTDecoder_input &input) {
    // TODO P2: handle input_len%16 != 0
    // pad the input to the multiple of 16

    // OPTDecoder:

    // TODO P2: slice the output to the original length
    struct Int8OPTDecoder_output output;
    return output;
}
