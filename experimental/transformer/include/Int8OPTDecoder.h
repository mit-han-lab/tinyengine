#include <cstdlib>
#include <string>
#include <vector>

#include "Int8OPTDecoderLayer.h"
#include "common.h"
#include "operators.h"

struct Int8OPTDecoder_output {
    Matrix3D<float> last_hidden_state;
};
struct Int8OPTDecoder_input {
    Matrix3D<int> input_ids;
};

class Int8OPTDecoder {
   public:
    Int8OPTDecoder(std::string param_path, int voc_size_, int embed_dim_, int hidden_dim_, int num_heads_,
                   int padding_idx_, int num_layers);
    Matrix3D<float> prepare_decoder_attention_mask(int length, int past_length);
    struct Int8OPTDecoder_output forward(const struct Int8OPTDecoder_input &input);
    Embedding embed_tokens;
    int voc_size, embed_dim, padding_idx, hidden_dim, num_heads;
    std::vector<Int8OPTDecoderLayer> layers;
};