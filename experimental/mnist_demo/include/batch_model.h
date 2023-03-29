#include <stdint.h>
#include <stdlib.h>

#include "weights.h"

#define MAX_BATCH_SIZE 100
signed char* getInput();
signed char* getOutput();
void batch_inference(int batch_size, int imp_choice);

enum fc_imp {
    naive = 0,
    unroll = 1,
    unroll_simd = 2,
    unroll_simd_tiling = 3,
};

#define NNoutput &buffer[MAX_BATCH_SIZE * 100];

/* sram:1084, flash:269480 */
#define PEAK_MEM 1084
#define MODEL_SIZE 269480

static signed char buffer[MAX_BATCH_SIZE * 1084];
