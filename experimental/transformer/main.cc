#include "OPTForCausalLM.h"
#include "input_input_ids.h"
#include "last_token_logits.h"
#include "utils.h"
#include <vector>
#include <cstdlib>

#define LM_HEAD_SIZE 50272 * 768
float lm_head_weight[LM_HEAD_SIZE];

int main() {
    Matrix3D<int> matrix_input_ids(input_ids, 1, 1, 512);
    Matrix3D<float> matrix_last_token_logits(last_token_logits, 1, 1, 512);

    // Read lm_head
    read_to_array("lm_head.bin", lm_head_weight, LM_HEAD_SIZE);

    // Print the float array
    for (int i = 0; i < 10; ++i) {
        std::cout << lm_head_weight[i] << " ";
    }
    std::cout << std::endl;

    struct OPTForCausalLM_input input = {matrix_input_ids};
    Matrix3D<float> lm_head(lm_head_weight, 1, 50272, 768);
    Int8OPTDecoder int8_decoder;
    OPTForCausalLM model(int8_decoder, lm_head);
    // TODO: initialize the decoder model

    auto output = model.forward(input);


    // TODO: add some tests here
}