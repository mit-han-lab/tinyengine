#include <stdint.h>
#include <stdlib.h>

#include "weights.h"

signed char* getInput();
signed char* getOutput();
void inference(int imp_choice);

enum fc_imp {
    naive = 0,
    unroll = 1,
    unroll_simd = 2,
};

#define NNoutput &buffer[100];

/* sram:1084, flash:269480 */
#define PEAK_MEM 1084
#define MODEL_SIZE 269480

static signed char buffer[1084];
