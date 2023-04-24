#include "common.h"
#include <utility>

struct Int8OPTAttention_output{
    Matrix3D<int8_t> attn_output;
    Matrix3D<int8_t> attn_probs_reshaped;
    std::pair<Matrix3D<int8_t>, Matrix3D<int8_t>> past_key_value;
};
struct Int8OPTAttention_input{
    Matrix3D<int8_t> hidden_states;
};

class Int8OPTAttention{
public:
    Int8OPTAttention(); // TODO: take a decoder
    struct Int8OPTAttention_output forward(const struct Int8OPTAttention_input &input);
private:
    int embed_dim;
    Matrix3D<int8_t>
}