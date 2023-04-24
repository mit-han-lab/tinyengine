#include "../OPTForCausalLM.h"
#include "../input_input_ids.h"
#include "../utils.h"
#include <vector>
#include <cstdlib>

#define LM_HEAD_SIZE 50272 * 768
float lm_head_weight[LM_HEAD_SIZE];
float last_token_logits_array[50272];
float gt_last_token_logits_array[50272];
#define pad_len 404

int main() {
    Matrix3D<int> matrix_input_ids(input_ids, 1, 1, 512);

    // Read lm_head
    read_to_array((char*)"lm_head.bin", lm_head_weight, LM_HEAD_SIZE);

    struct OPTForCausalLM_input input = {matrix_input_ids};
    Matrix3D<float> lm_head(lm_head_weight, 1, 50272, 768);
    Int8OPTDecoder int8_decoder;
    OPTForCausalLM model(int8_decoder, lm_head);

    // smoothquant: outputs = model(input_ids)
    auto output = model.forward(input);

    // smoothquant: last_token_logits = outputs.logits[:, -2-pad_len, :]
    Matrix3D<float> last_token_logits(last_token_logits_array, 1, 1, 50272);
    for (int i = 0; i < 50272; i++){
        last_token_logits(0, 0, i) = output.logits(0, (512 - (2 + pad_len)), i);
    }

    // End result tests here
    read_to_array((char*)"last_token_logits.bin", gt_last_token_logits_array, 50272);
    bool sucess = check_two_equal(gt_last_token_logits_array, last_token_logits_array, 50272);
    if (!sucess)
        printf("End result incorrect!\n");
    else
        printf("End result correct!\n");
}