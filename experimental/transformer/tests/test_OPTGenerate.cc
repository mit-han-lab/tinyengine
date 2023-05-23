#include <iostream>
#include "OPTGenerate.h"

void test_OPTGenerate() {
    // std::cout << "Test End!" << std::endl;
}

int main() {
    // test_OPTGenerate();
    std::vector<int> input_ids = {37500, 10,  998, 64, 28, 626, 11,   158, 2007, 2402, 4,  152,  1579,  16,
                                       13,    937, 82,  6,  98, 52,  6876, 51,  218,  75,   33, 3280, 14198, 4};
    const struct opt_params generation_config;
    std::vector<int> generated_ids = OPTGenerate(input_ids, generation_config);
    //std::vector<int> generated_ids_answer = {}  // TODO: add answer
    
    /*
    std::cout << "Generated: ";
    for (int i = 0; i < generated_ids.size(); i++) {
        std::cout << generated_ids[i] << " ";
    }
    std::cout << std::endl;
    */
    
    /*
    // TODO: add comparison after adding answer
    bool is_equal = true;
    for (int i = 0; i < generated_ids.size(); i++) {
        if (generated_ids[i] != generated_ids_answer[i]) {
            is_equal = false;
            break;
        }
    }
    if (is_equal)
        std::cout << "-------- Test of OPTGenerate: Passed! -------- " << std::endl;
    else
        std::cout << "-------- Test of OPTGenerate: Failed! -------- " << std::endl;
    */
    std::cout << "-------- End of test_OPTGenerate --------" << std::endl;
};
