#include "genModel.h"
#include <iostream>

int main() {
    signed char input[3072] = { 0, };
    signed char output[3072];

    genModel(input, output);

    // 결과 출력 또는 추가 작업
    for (int i = 0; i < 3072; ++i) {
        std::cout << "Output[" << i << "]: " << static_cast<int>(output[i]) << std::endl;
    }

    return 0;
}
