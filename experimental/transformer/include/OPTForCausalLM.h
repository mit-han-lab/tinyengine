#include "Int8OPTDecoder.h"

struct OPTForCausalLM_output{
    Matrix3D<float> logits;
};
struct OPTForCausalLM_input{
    Matrix3D<int> input_ids;
};

class OPTForCausalLM{
public:
    OPTForCausalLM(Int8OPTDecoder decoder, Matrix3D<float> lm_head): m_decoder(decoder), lm_head_weights(lm_head) {} // TODO: take a decoder
    struct OPTForCausalLM_output forward(const struct OPTForCausalLM_input &input);
private:
    Int8OPTDecoder m_decoder;
    Matrix3D<float> lm_head_weights;
};