#include <cstdlib>
#include <string>
#include <vector>
#include "common.h"
#include "operators.h"
#include "Int8OPTDecoderLayer.h"

struct Int8OPTDecoder_output {
    Matrix3D<float> last_hidden_state;
};
struct Int8OPTDecoder_input {
    Matrix3D<int> input_ids;
};

class Int8OPTDecoder {
   public:
    Int8OPTDecoder(std::string param_path, int voc_size_, int embed_dim_, int padding_idx_, Embedding &embed_tokens_);
    struct Int8OPTDecoder_output forward(const struct Int8OPTDecoder_input &input);
    Embedding embed_tokens;
    int voc_size, embed_dim, padding_idx;
    std::vector<Int8OPTDecoderLayer> layers;
};