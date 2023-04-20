#include "Int8OPTAttention.h"


struct Int8OPTAttention_output{
    void* residual;
    void* self_attn_weights;
    void* present_key_value;
};
struct Int8OPTAttention_input{
    void* hidden_states;
    void* attention_mask;
    void* layer_head_mask;
    void* output_attentions;
    void* use_cache;
    void* past_key_value;
};

struct Int8OPTAttention_output OPTForCausalLM::forward(const struct Int8OPTAttention_input &input){
    struct Int8OPTAttention_output output;


    return output;
}
