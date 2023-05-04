#include "Int8OPTDecoder.h"

#include "OPTDecoder.h"

Int8OPTDecoder::Int8OPTDecoder(std::string param_path, int voc_size_, int embed_dim_, int padding_idx_,
                               Embedding &embed_tokens_){
    this->voc_size = voc_size_;
    this->embed_dim = embed_dim_;
    this->padding_idx = padding_idx_;
    this->embed_tokens = embed_tokens_;
    load_Embedding_params(this->embed_tokens, param_path + "/embed_tokens");
};

struct Int8OPTDecoder_output Int8OPTDecoder::forward(const struct Int8OPTDecoder_input &input) {
    // TODO P2: handle input_len%16 != 0
    // pad the input to the multiple of 16

    // OPTDecoder:

    // TODO P2: slice the output to the original length
    struct Int8OPTDecoder_output output;
    return output;
}
