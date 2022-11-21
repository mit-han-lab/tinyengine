/* Automatically generated source file */
#include "genModel.h"

#include "arm_nnfunctions.h"
#include "genNN.h"
#include "tinyengine_function.h"

int32_t Add(int32_t a, int32_t b) { return a + b; }
int32_t ShiftRight(int32_t a, int offset) { return a >> offset; }
int32_t BitAnd(int32_t a, int32_t b) { return a & b; }
int32_t BitNot(int32_t a) { return ~a; }
int32_t MaskIfNonZero(int32_t a) {
    static const int32_t zero = 0;
    return a ? BitNot(zero) : zero;
}
int32_t MaskIfGreaterThan(int32_t a, int32_t b) { return MaskIfNonZero(a > b); }
int32_t MaskIfLessThan(int32_t a, int32_t b) { return MaskIfNonZero(a < b); }

static inline int32_t SaturatingRoundingDoublingHighMul(int32_t a, int32_t b) {
    int64_t a_64 = a;
    int64_t b_64 = b;
    int64_t ab_64 = a_64 * b_64;
    int32_t nudge = ab_64 >= 0 ? (1 << 30) : (1 - (1 << 30));
    int32_t ab_x2_high32 = (int32_t)((ab_64 + nudge) / (1ll << 31));
    return a == b && a == -2147483648 ? 2147483647 : ab_x2_high32;
}

static inline int32_t RoundingDivideByPOT(int32_t x, int exponent) {
    const int32_t mask = ((1ll << exponent) - 1);
    const int32_t zero = (0);
    const int32_t one = (1);
    const int32_t remainder = BitAnd(x, mask);
    const int32_t threshold = Add(ShiftRight(mask, 1), BitAnd(MaskIfLessThan(x, zero), one));
    return Add(ShiftRight(x, exponent), BitAnd(MaskIfGreaterThan(remainder, threshold), one));
}

static inline int32_t MultiplyByQuantizedMultiplierSmallerThanOneExp(int32_t x, int32_t quantized_multiplier,
                                                                     int left_shift) {
    return RoundingDivideByPOT(SaturatingRoundingDoublingHighMul(x, quantized_multiplier), -left_shift);
}

tinyengine_status add(int size, ADD_params* params, const int8_t* input1_data, const int8_t* input2_data,
                      int8_t* output_data) {
    for (int i = 0; i < size; ++i) {
        const int32_t input1_val = params->input1_offset + input1_data[i];
        const int32_t input2_val = params->input2_offset + input2_data[i];
        const int32_t shifted_input1_val = input1_val * (1 << params->left_shift);
        const int32_t shifted_input2_val = input2_val * (1 << params->left_shift);
        const int32_t scaled_input1_val = MultiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input1_val, params->input1_multiplier, params->input1_shift);
        const int32_t scaled_input2_val = MultiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input2_val, params->input2_multiplier, params->input2_shift);
        const int32_t raw_sum = scaled_input1_val + scaled_input2_val;
        const int32_t raw_output =
            MultiplyByQuantizedMultiplierSmallerThanOneExp(raw_sum, params->output_multiplier, params->output_shift) +
            params->output_offset;
        const int32_t clamped_output =
            TN_MIN(params->quantized_activation_max, TN_MAX(params->quantized_activation_min, raw_output));
        output_data[i] = (int8_t)(clamped_output);
    }
}

/* Variables used by all ops */
ADD_params add_params;

signed char* getInput() { return buffer0; }
signed char* getOutput() { return NNoutput; }
void end2endinference(q7_t* img) { invoke(NULL); }
void invoke(float* label) {
    /* layer 0:CONV_2D */
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[0], 120, 120, 3, (const q7_t*)weight0, bias0, scales0,
                                                    -128, 1, -128, 127, &buffer0[144000], 60, 60, 16, sbuf, kbuf, -1);
    /* layer 1:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[144000], 60, 60, 16, (const q7_t*)CHWweight1, offsetBias1,
                                                  offsetRBias1, scales1, -128, 128, -128, 127, &buffer0[0], 60, 60, 16,
                                                  sbuf, -128);
    /* layer 2:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[144000], 60, 60, 16, (const q7_t*)weight2, bias2, scales2, -2, 128, -128, 127,
                               &buffer0[0], 60, 60, 8, sbuf);
    /* layer 3:CONV_2D */
    convolve_1x1_s8_ch8_fpreq(&buffer0[0], 60, 60, 8, (const q7_t*)weight3, bias3, scales3, -128, 2, -128, 127,
                              &buffer0[28800], 60, 60, 48, sbuf);
    /* layer 4:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[28800], 60, 60, 48, (const q7_t*)CHWweight4, offsetBias4,
                                                  offsetRBias4, scales4, -128, 128, -128, 127, &buffer0[0], 30, 30, 48,
                                                  sbuf, -128);
    /* layer 5:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[28800], 30, 30, 48, (const q7_t*)weight5, bias5, scales5, -19, 128, -128, 127,
                               &buffer1[0], 30, 30, 16, sbuf);
    /* layer 6:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer1[0], 30, 30, 16, (const q7_t*)weight6, bias6, scales6, -128, 19, -128, 127,
                               &buffer0[158400], 30, 30, 48, sbuf);
    /* layer 7:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[158400], 30, 30, 48, (const q7_t*)CHWweight7, offsetBias7,
                                                  offsetRBias7, scales7, -128, 128, -128, 127, &buffer0[0], 30, 30, 48,
                                                  sbuf, -128);
    /* layer 8:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[158400], 30, 30, 48, (const q7_t*)weight8, bias8, scales8, 12, 128, -128, 127,
                               &buffer0[0], 30, 30, 16, sbuf);
    /* layer 9:ADD */
    add_params.input_h = 30;
    add_params.input_w = 30;
    add_params.input_c = 16;
    add_params.left_shift = 20;
    add_params.input1_offset = -12;
    add_params.input1_multiplier = 1853017153;
    add_params.input1_shift = -1;
    add_params.input2_offset = 19;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = -22;
    add_params.output_multiplier = 1738872347;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer1[0]);
    /* layer 10:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer1[0], 30, 30, 16, (const q7_t*)weight9, bias9, scales9, -128, 22, -128, 127,
                               &buffer0[158400], 30, 30, 48, sbuf);
    /* layer 11:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[158400], 30, 30, 48, (const q7_t*)CHWweight10, offsetBias10,
                                                  offsetRBias10, scales10, -128, 128, -128, 127, &buffer0[0], 30, 30,
                                                  48, sbuf, -128);
    /* layer 12:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[158400], 30, 30, 48, (const q7_t*)weight11, bias11, scales11, -10, 128, -128,
                               127, &buffer0[0], 30, 30, 16, sbuf);
    /* layer 13:ADD */
    add_params.input_h = 30;
    add_params.input_w = 30;
    add_params.input_c = 16;
    add_params.left_shift = 20;
    add_params.input1_offset = 10;
    add_params.input1_multiplier = 1983546124;
    add_params.input1_shift = -3;
    add_params.input2_offset = 22;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = -14;
    add_params.output_multiplier = 2104562937;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer0[187200]);
    /* layer 14:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[187200], 30, 30, 16, (const q7_t*)weight12, bias12, scales12, -128, 14, -128,
                               127, &buffer0[0], 30, 30, 48, sbuf);
    /* layer 15:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 30, 30, 48, (const q7_t*)CHWweight13, offsetBias13,
                                                  offsetRBias13, scales13, -128, 128, -128, 127, &buffer0[190800], 15,
                                                  15, 48, sbuf, -128);
    /* layer 16:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 15, 15, 48, (const q7_t*)weight14, bias14, scales14, 2, 128, -128, 127,
                               &buffer1[0], 15, 15, 24, sbuf);
    /* layer 17:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer1[0], 15, 15, 24, (const q7_t*)weight15, bias15, scales15, -128, -2, -128, 127,
                               &buffer0[169200], 15, 15, 144, sbuf);
    /* layer 18:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[169200], 15, 15, 144, (const q7_t*)CHWweight16, offsetBias16,
                                                  offsetRBias16, scales16, -128, 128, -128, 127, &buffer0[0], 15, 15,
                                                  144, sbuf, -128);
    /* layer 19:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[169200], 15, 15, 144, (const q7_t*)weight17, bias17, scales17, 2, 128, -128, 127,
                          &buffer0[0], 15, 15, 24, sbuf);
    /* layer 20:ADD */
    add_params.input_h = 15;
    add_params.input_w = 15;
    add_params.input_c = 24;
    add_params.left_shift = 20;
    add_params.input1_offset = -2;
    add_params.input1_multiplier = 1073741824;
    add_params.input1_shift = 0;
    add_params.input2_offset = -2;
    add_params.input2_multiplier = 2067874274;
    add_params.input2_shift = -1;
    add_params.output_offset = 12;
    add_params.output_multiplier = 1844006744;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer1[0]);
    /* layer 21:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer1[0], 15, 15, 24, (const q7_t*)weight18, bias18, scales18, -128, -12, -128, 127,
                               &buffer0[174600], 15, 15, 120, sbuf);
    /* layer 22:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[174600], 15, 15, 120, (const q7_t*)CHWweight19, offsetBias19,
                                                  offsetRBias19, scales19, -128, 128, -128, 127, &buffer0[0], 15, 15,
                                                  120, sbuf, -128);
    /* layer 23:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[174600], 15, 15, 120, (const q7_t*)weight20, bias20, scales20, 0, 128, -128, 127,
                          &buffer0[0], 15, 15, 24, sbuf);
    /* layer 24:ADD */
    add_params.input_h = 15;
    add_params.input_w = 15;
    add_params.input_c = 24;
    add_params.left_shift = 20;
    add_params.input1_offset = 0;
    add_params.input1_multiplier = 2142432871;
    add_params.input1_shift = -3;
    add_params.input2_offset = -12;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = 18;
    add_params.output_multiplier = 1986712119;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer0[196200]);
    /* layer 25:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[196200], 15, 15, 24, (const q7_t*)weight21, bias21, scales21, -128, -18, -128,
                               127, &buffer0[0], 15, 15, 144, sbuf);
    /* layer 26:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 15, 15, 144, (const q7_t*)CHWweight22, offsetBias22,
                                                  offsetRBias22, scales22, -128, 128, -128, 127, &buffer0[192384], 8, 8,
                                                  144, sbuf, -128);
    /* layer 27:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 8, 8, 144, (const q7_t*)weight23, bias23, scales23, -11, 128, -128, 127,
                          &buffer1[0], 8, 8, 40, sbuf);
    /* layer 28:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer1[0], 8, 8, 40, (const q7_t*)weight24, bias24, scales24, -128, 11, -128, 127,
                          &buffer0[186240], 8, 8, 240, sbuf);
    /* layer 29:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[186240], 8, 8, 240, (const q7_t*)CHWweight25, offsetBias25,
                                                  offsetRBias25, scales25, -128, 128, -128, 127, &buffer0[0], 8, 8, 240,
                                                  sbuf, -128);
    /* layer 30:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[186240], 8, 8, 240, (const q7_t*)weight26, bias26, scales26, 7, 128, -128, 127,
                          &buffer0[0], 8, 8, 40, sbuf);
    /* layer 31:ADD */
    add_params.input_h = 8;
    add_params.input_w = 8;
    add_params.input_c = 40;
    add_params.left_shift = 20;
    add_params.input1_offset = -7;
    add_params.input1_multiplier = 1626712137;
    add_params.input1_shift = -1;
    add_params.input2_offset = 11;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = -3;
    add_params.output_multiplier = 1074737776;
    add_params.output_shift = -18;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer0[199040]);
    /* layer 32:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[199040], 8, 8, 40, (const q7_t*)weight27, bias27, scales27, -128, 3, -128, 127,
                          &buffer0[0], 8, 8, 240, sbuf);
    /* layer 33:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 8, 8, 240, (const q7_t*)CHWweight28, offsetBias28,
                                                  offsetRBias28, scales28, -128, 128, -128, 127, &buffer0[186240], 8, 8,
                                                  240, sbuf, -128);
    /* layer 34:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 8, 8, 240, (const q7_t*)weight29, bias29, scales29, -5, 128, -128, 127,
                          &buffer1[0], 8, 8, 48, sbuf);
    /* layer 35:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer1[0], 8, 8, 48, (const q7_t*)weight30, bias30, scales30, -128, 5, -128, 127,
                               &buffer0[189312], 8, 8, 192, sbuf);
    /* layer 36:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[189312], 8, 8, 192, (const q7_t*)CHWweight31, offsetBias31,
                                                  offsetRBias31, scales31, -128, 128, -128, 127, &buffer0[0], 8, 8, 192,
                                                  sbuf, -128);
    /* layer 37:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[189312], 8, 8, 192, (const q7_t*)weight32, bias32, scales32, -10, 128, -128, 127,
                          &buffer0[0], 8, 8, 48, sbuf);
    /* layer 38:ADD */
    add_params.input_h = 8;
    add_params.input_w = 8;
    add_params.input_c = 48;
    add_params.left_shift = 20;
    add_params.input1_offset = 10;
    add_params.input1_multiplier = 1073741824;
    add_params.input1_shift = 0;
    add_params.input2_offset = 5;
    add_params.input2_multiplier = 1728811214;
    add_params.input2_shift = -1;
    add_params.output_offset = -2;
    add_params.output_multiplier = 1092740395;
    add_params.output_shift = -18;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer0[198528]);
    /* layer 39:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[198528], 8, 8, 48, (const q7_t*)weight33, bias33, scales33, -128, 2, -128, 127,
                               &buffer0[0], 8, 8, 240, sbuf);
    /* layer 40:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride2_inplace_CHW_fpreq(&buffer0[0], 8, 8, 240, (const q7_t*)CHWweight34, offsetBias34,
                                                  offsetRBias34, scales34, -128, 128, -128, 127, &buffer0[197760], 4, 4,
                                                  240, sbuf, -128);
    /* layer 41:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 4, 4, 240, (const q7_t*)weight35, bias35, scales35, -8, 128, -128, 127,
                          &buffer1[0], 4, 4, 96, sbuf);
    /* layer 42:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer1[0], 4, 4, 96, (const q7_t*)weight36, bias36, scales36, -128, 8, -128, 127,
                          &buffer0[193920], 4, 4, 480, sbuf);
    /* layer 43:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[193920], 4, 4, 480, (const q7_t*)CHWweight37, offsetBias37,
                                                  offsetRBias37, scales37, -128, 128, -128, 127, &buffer0[0], 4, 4, 480,
                                                  sbuf, -128);
    /* layer 44:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[193920], 4, 4, 480, (const q7_t*)weight38, bias38, scales38, 14, 128, -128, 127,
                          &buffer0[0], 4, 4, 96, sbuf);
    /* layer 45:ADD */
    add_params.input_h = 4;
    add_params.input_w = 4;
    add_params.input_c = 96;
    add_params.left_shift = 20;
    add_params.input1_offset = -14;
    add_params.input1_multiplier = 1931955708;
    add_params.input1_shift = -1;
    add_params.input2_offset = 8;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = 9;
    add_params.output_multiplier = 2125452290;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer1[0]);
    /* layer 46:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer1[0], 4, 4, 96, (const q7_t*)weight39, bias39, scales39, -128, -9, -128, 127,
                          &buffer0[195456], 4, 4, 384, sbuf);
    /* layer 47:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[195456], 4, 4, 384, (const q7_t*)CHWweight40, offsetBias40,
                                                  offsetRBias40, scales40, -128, 128, -128, 127, &buffer0[0], 4, 4, 384,
                                                  sbuf, -128);
    /* layer 48:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[195456], 4, 4, 384, (const q7_t*)weight41, bias41, scales41, 1, 128, -128, 127,
                          &buffer0[0], 4, 4, 96, sbuf);
    /* layer 49:ADD */
    add_params.input_h = 4;
    add_params.input_w = 4;
    add_params.input_c = 96;
    add_params.left_shift = 20;
    add_params.input1_offset = -1;
    add_params.input1_multiplier = 1890466689;
    add_params.input1_shift = -2;
    add_params.input2_offset = -9;
    add_params.input2_multiplier = 1073741824;
    add_params.input2_shift = 0;
    add_params.output_offset = 7;
    add_params.output_multiplier = 1887616697;
    add_params.output_shift = -19;
    add_params.quantized_activation_max = 127;
    add_params.quantized_activation_min = -128;
    add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0],
        &buffer0[200064]);
    /* layer 50:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[200064], 4, 4, 96, (const q7_t*)weight42, bias42, scales42, -128, -7, -128, 127,
                          &buffer0[0], 4, 4, 288, sbuf);
    /* layer 51:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 4, 4, 288, (const q7_t*)CHWweight43, offsetBias43,
                                                  offsetRBias43, scales43, -128, 128, -128, 127, &buffer0[196992], 4, 4,
                                                  288, sbuf, -128);
    /* layer 52:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 4, 4, 288, (const q7_t*)weight44, bias44, scales44, 6, 128, -128, 127,
                          &buffer0[199040], 4, 4, 160, sbuf);
    /* layer 53:AVERAGE_POOL_2D */
    avg_pooling(&buffer0[199040], 4, 4, 160, 4, 4, 1, 1, -128, 127, &buffer0[0]);
    ///* layer 54:CONV_2D */
    // convolve_1x1_s8_fpreq(&buffer0[0],1,1,160,(const q7_t*)
    // weight45,bias45,scales45,0,-6,-128,127,&buffer0[201598],1,1,2,sbuf);
}
