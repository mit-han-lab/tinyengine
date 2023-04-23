#include "common.h"

struct Int8OPTDecoderLayer_output{
    void* last_hidden_state;
    void* past_key_values;
    void* hidden_states;
    void* attentions;
};
struct Int8OPTDecoderLayer_input{
    Matrix3D<int> input_ids;
};

class Int8OPTDecoderLayer{
public:
    Int8OPTDecoderLayer(int embed_dim, int num_attention_heads); // TODO: take a decoder
    struct Int8OPTDecoderLayer_output forward(const struct Int8OPTDecoderLayer_input &input);

    int embed_dim, num_attention_heads;
    LayerNormQ self_attn_layer_norm; // from torch_int.nn
    W8A8B8O8LinearReLU fc1;
    W8A8BFP32OFP32Linear fc2;
    LayerNormQ final_layer_norm;
    Int8OPTAttention sefl_attn;
}