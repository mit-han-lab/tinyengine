struct Int8OPTAttention_output{
    void* last_hidden_state;
    void* past_key_values;
    void* hidden_states;
    void* attentions;
};
struct Int8OPTAttention_input{
    void* input_ids;
};

class Int8OPTAttention{
public:
    Int8OPTAttention(); // TODO: take a decoder
    struct Int8OPTAttention_output forward(const struct Int8OPTAttention_input &input);
    
}