#include "common.h"

struct Int8OPTDecoder_output{
    void* last_hidden_state;
    void* past_key_values;
    void* hidden_states;
    void* attentions;
};
struct Int8OPTDecoder_input{
     Matrix3D<int> input_ids;
};

class Int8OPTDecoder{
public:
    Int8OPTDecoder() {}
    struct Int8OPTDecoder_output forward(const struct Int8OPTDecoder_input &input);
};