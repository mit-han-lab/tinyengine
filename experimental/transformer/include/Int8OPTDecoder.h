#include "common.h"

struct Int8OPTDecoder_output{
    Matrix3D<float> last_hidden_state;
};
struct Int8OPTDecoder_input{
    Matrix3D<int> input_ids;
};

class Int8OPTDecoder{
public:
    Int8OPTDecoder();
    struct Int8OPTDecoder_output forward(const struct Int8OPTDecoder_input &input);
};