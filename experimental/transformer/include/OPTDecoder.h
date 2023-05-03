#include "common.h"

struct OPTDecoder_output{
    void* last_hidden_state;
    void* past_key_values;
    void* hidden_states;
    void* attentions;
};
struct OPTDecoder_input{
     Matrix3D<int> input_ids;
};

class OPTDecoder{
public:
    OPTDecoder() {}
    struct OPTDecoder_output forward(const struct OPTDecoder_input &input);
};