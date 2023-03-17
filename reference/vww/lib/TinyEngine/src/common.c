#include "tinyengine_function.h"

#ifndef OPT
q7_t get_pixel(int h, int w, int c, int input_h, int input_w, int input_c, const q7_t *input) {
    if (h < 0 || h >= input_h) return 0;
    if (w < 0 || w >= input_w) return 0;

    return input[(h * input_w + w) * input_c + c];
}
#endif
