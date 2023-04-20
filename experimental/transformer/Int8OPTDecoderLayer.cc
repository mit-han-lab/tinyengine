#include "Int8OPTDecoderLayer.h"


struct Int8OPTDecoderLayer_output{
    void* residual;
    void* self_attn_weights;
    void* present_key_value;
};
struct Int8OPTDecoderLayer_input{
    void* hidden_states;
    void* attention_mask;
    void* layer_head_mask;
    void* output_attentions;
    void* use_cache;
    void* past_key_value;
};

struct Int8OPTDecoderLayer_output OPTForCausalLM::forward(const struct Int8OPTDecoderLayer_input &input){
    struct Int8OPTDecoderLayer_output output;


    return output;
}
