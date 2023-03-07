/* Automatically generated source file */

#include <float.h>
#include <tinyengine_function.h>
#include <tinyengine_function_fp.h>

#include "genInclude.h"
#include "genModel.h"
#include "genNN.h"

/* Variables used by all ops */
ADD_params add_params;
int i;
int8_t *int8ptr, *int8ptr2;
int32_t* int32ptr;
float *fptr, *fptr2, *fptr3;

signed char* getInput() { return &buffer0[25600]; }
signed char* getOutput() { return NNoutput; }
void end2endinference(q7_t* img) { invoke(NULL); }
void invoke(float* labels) {
    /* layer 0:CONV_2D */
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[25600], 80, 80, 3, (const q7_t*)weight0, bias0, scales0,
                                                    -128, 1, -128, 127, &buffer0[0], 40, 40, 16, sbuf, kbuf, -1);
    /* layer 1:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 40, 40, 16, (const q7_t*)CHWweight1, offsetBias1,
                                                  offsetRBias1, scales1, -128, 128, -128, 127, &buffer0[0], 40, 40, 16,
                                                  sbuf, -128);
    /* layer 2:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[0], 40, 40, 16, (const q7_t*)weight2, bias2, scales2, -4, 128, -128, 127,
                               &buffer0[76800], 40, 40, 8, sbuf);
    /* layer 3:CONV_2D */
    convolve_1x1_s8_ch8_fpreq(&buffer0[76800], 40, 40, 8, (const q7_t*)weight3, bias3, scales3, -128, 4, -128, 127,
                              &buffer0[0], 40, 40, 48, sbuf);
    /* layer 4:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 40, 40, 48, (const q7_t*)CHWweight4, offsetBias4,
                                                  offsetRBias4, scales4, -128, 128, -128, 127, &buffer0[0], 20, 20, 48,
                                                  sbuf, -128);
    /* layer 5:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight5, bias5, scales5, -22, 128, -128, 127,
                               &buffer0[19200], 20, 20, 16, sbuf);
    /* layer 6:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[19200], 20, 20, 16, (const q7_t*)weight6, bias6, scales6, -128, 22, -128, 127,
                               &buffer0[0], 20, 20, 48, sbuf);
    /* layer 7:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight7, offsetBias7,
                                                  offsetRBias7, scales7, -128, 128, -128, 127, &buffer0[0], 20, 20, 48,
                                                  sbuf, -128);
    /* layer 8:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight8, bias8, scales8, 8, 128, -128, 127,
                               &buffer0[25600], 20, 20, 16, sbuf);
    /* layer 9:ADD */
    add_fpreq(6400, &buffer0[25600], 0.07699620723724365, 8, &buffer0[19200], 0.08598089963197708, -22,
              0.10197763890028, -20, &buffer0[32000]);
    /* layer 10:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[32000], 20, 20, 16, (const q7_t*)weight9, bias9, scales9, -128, 20, -128, 127,
                               &buffer0[0], 20, 20, 48, sbuf);
    /* layer 11:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight10, offsetBias10,
                                                  offsetRBias10, scales10, -128, 128, -128, 127, &buffer0[0], 20, 20,
                                                  48, sbuf, -128);
    /* layer 12:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight11, bias11, scales11, -4, 128, -128, 127,
                               &buffer0[19200], 20, 20, 16, sbuf);
    /* layer 13:ADD */
    add_fpreq(6400, &buffer0[19200], 0.024848325178027153, -4, &buffer0[32000], 0.10197763890028, -20,
              0.10464993864297867, -15, &buffer0[25600]);
    /* layer 14:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[25600], 20, 20, 16, (const q7_t*)weight12, bias12, scales12, -128, 15, -128,
                               127, &buffer0[0], 20, 20, 48, sbuf);
    /* layer 15:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight13, offsetBias13,
                                                  offsetRBias13, scales13, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  48, sbuf, -128);
    /* layer 16:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 10, 10, 48, (const q7_t*)weight14, bias14, scales14, -16, 128, -128, 127,
                               &buffer0[14400], 10, 10, 24, sbuf);
    /* layer 17:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[14400], 10, 10, 24, (const q7_t*)weight15, bias15, scales15, -128, 16, -128,
                               127, &buffer0[0], 10, 10, 144, sbuf);
    /* layer 18:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)CHWweight16, offsetBias16,
                                                  offsetRBias16, scales16, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  144, sbuf, -128);
    /* layer 19:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)weight17, bias17, scales17, -14, 128, -128, 127,
                          &buffer0[16800], 10, 10, 24, sbuf);
    /* layer 20:ADD */
    add_fpreq(2400, &buffer0[16800], 0.06462342292070389, -14, &buffer0[14400], 0.06311018764972687, -16,
              0.07023955136537552, -3, &buffer0[12000]);
    /* layer 21:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[12000], 10, 10, 24, (const q7_t*)weight18, bias18, scales18, -128, 3, -128, 127,
                               &buffer0[0], 10, 10, 120, sbuf);
    /* layer 22:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 10, 10, 120, (const q7_t*)CHWweight19, offsetBias19,
                                                  offsetRBias19, scales19, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  120, sbuf, -128);
    /* layer 23:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 10, 10, 120, (const q7_t*)weight20, bias20, scales20, -3, 128, -128, 127,
                          &buffer0[14400], 10, 10, 24, sbuf);
    /* layer 24:ADD */
    add_fpreq(2400, &buffer0[14400], 0.018893597647547722, -3, &buffer0[12000], 0.07023955136537552, -3,
              0.07366174459457397, -1, &buffer0[16800]);
    /* layer 25:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[16800], 10, 10, 24, (const q7_t*)weight21, bias21, scales21, -128, 1, -128, 127,
                               &buffer0[0], 10, 10, 144, sbuf);
    /* layer 26:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)CHWweight22, offsetBias22,
                                                  offsetRBias22, scales22, -128, 128, -128, 127, &buffer0[0], 5, 5, 144,
                                                  sbuf, -128);
    /* layer 27:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 144, (const q7_t*)weight23, bias23, scales23, -11, 128, -128, 127,
                          &buffer0[6000], 5, 5, 40, sbuf);
    /* layer 28:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[6000], 5, 5, 40, (const q7_t*)weight24, bias24, scales24, -128, 11, -128, 127,
                          &buffer0[0], 5, 5, 240, sbuf);
    /* layer 29:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight25, offsetBias25,
                                                  offsetRBias25, scales25, -128, 128, -128, 127, &buffer0[0], 5, 5, 240,
                                                  sbuf, -128);
    /* layer 30:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)weight26, bias26, scales26, 1, 128, -128, 127,
                          &buffer0[7000], 5, 5, 40, sbuf);
    /* layer 31:ADD */
    add_fpreq(1000, &buffer0[7000], 0.04340619966387749, 1, &buffer0[6000], 0.0489354208111763, -11,
              0.04974109306931496, -3, &buffer0[8000]);
    /* layer 32:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[8000], 5, 5, 40, (const q7_t*)weight27, bias27, scales27, -128, 3, -128, 127,
                          &buffer0[0], 5, 5, 240, sbuf);
    /* layer 33:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight28, offsetBias28,
                                                  offsetRBias28, scales28, -128, 128, -128, 127, &buffer0[0], 5, 5, 240,
                                                  sbuf, -128);
    /* layer 34:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)weight29, bias29, scales29, 15, 128, -128, 127,
                          &buffer0[6000], 5, 5, 48, sbuf);
    /* layer 35:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[6000], 5, 5, 48, (const q7_t*)weight30, bias30, scales30, -128, -15, -128, 127,
                               &buffer0[0], 5, 5, 192, sbuf);
    /* layer 36:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)CHWweight31, offsetBias31,
                                                  offsetRBias31, scales31, -128, 128, -128, 127, &buffer0[0], 5, 5, 192,
                                                  sbuf, -128);
    /* layer 37:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)weight32, bias32, scales32, -11, 128, -128, 127,
                          &buffer0[4800], 5, 5, 48, sbuf);
    /* layer 38:ADD */
    add_fpreq(1200, &buffer0[4800], 0.04923376813530922, -11, &buffer0[6000], 0.04425579309463501, 15,
              0.05336926504969597, 5, &buffer0[7200]);
    /* layer 39:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[7200], 5, 5, 48, (const q7_t*)weight33, bias33, scales33, -128, -5, -128, 127,
                               &buffer0[0], 5, 5, 240, sbuf);
    /* layer 40:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride2_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight34, offsetBias34,
                                                  offsetRBias34, scales34, -128, 128, -128, 127, &buffer0[0], 3, 3, 240,
                                                  sbuf, -128);
    /* layer 41:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 240, (const q7_t*)weight35, bias35, scales35, -7, 128, -128, 127,
                          &buffer0[4320], 3, 3, 96, sbuf);
    /* layer 42:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[4320], 3, 3, 96, (const q7_t*)weight36, bias36, scales36, -128, 7, -128, 127,
                          &buffer0[0], 3, 3, 480, sbuf);
    /* layer 43:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 480, (const q7_t*)CHWweight37, offsetBias37,
                                                  offsetRBias37, scales37, -128, 128, -128, 127, &buffer0[0], 3, 3, 480,
                                                  sbuf, -128);
    /* layer 44:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 480, (const q7_t*)weight38, bias38, scales38, -2, 128, -128, 127,
                          &buffer0[5184], 3, 3, 96, sbuf);
    /* layer 45:ADD */
    add_fpreq(864, &buffer0[5184], 0.04011229798197746, -2, &buffer0[4320], 0.042191825807094574, -7,
              0.03923744335770607, -9, &buffer0[3456]);
    /* layer 46:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[3456], 3, 3, 96, (const q7_t*)weight39, bias39, scales39, -128, 9, -128, 127,
                          &buffer0[0], 3, 3, 384, sbuf);
    /* layer 47:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 384, (const q7_t*)CHWweight40, offsetBias40,
                                                  offsetRBias40, scales40, -128, 128, -128, 127, &buffer0[0], 3, 3, 384,
                                                  sbuf, -128);
    /* layer 48:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 384, (const q7_t*)weight41, bias41, scales41, -6, 128, -128, 127,
                          &buffer0[4320], 3, 3, 96, sbuf);
    /* layer 49:ADD */
    add_fpreq(864, &buffer0[4320], 0.019318707287311554, -6, &buffer0[3456], 0.03923744335770607, -9,
              0.0419764444231987, -12, &buffer0[2592]);
    /* layer 50:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[2592], 3, 3, 96, (const q7_t*)weight42, bias42, scales42, -128, 12, -128, 127,
                          &buffer0[0], 3, 3, 288, sbuf);
    /* layer 51:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 288, (const q7_t*)CHWweight43, offsetBias43,
                                                  offsetRBias43, scales43, -128, 128, -128, 127, &buffer0[0], 3, 3, 288,
                                                  sbuf, -128);
    /* layer 52:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 288, (const q7_t*)weight44, bias44, scales44, 9, 128, -128, 127,
                          &buffer0[2592], 3, 3, 160, sbuf);
    /* layer 53:AVERAGE_POOL_2D */
    avg_pooling(&buffer0[2592], 3, 3, 160, 3, 3, 1, 1, -128, 127, &buffer0[0]);
    /* layer 54:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 1, 1, 160, (const q7_t*)weight45, bias45, scales45, -1, -9, -128, 127,
                          &buffer0[160], 1, 1, 2, sbuf);
}
void invoke_inf() {
    /* layer 0:CONV_2D */
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[25600], 80, 80, 3, (const q7_t*)weight0, bias0, scales0,
                                                    -128, 1, -128, 127, &buffer0[0], 40, 40, 16, sbuf, kbuf, -1);
    /* layer 1:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 40, 40, 16, (const q7_t*)CHWweight1, offsetBias1,
                                                  offsetRBias1, scales1, -128, 128, -128, 127, &buffer0[0], 40, 40, 16,
                                                  sbuf, -128);
    /* layer 2:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[0], 40, 40, 16, (const q7_t*)weight2, bias2, scales2, -4, 128, -128, 127,
                               &buffer0[76800], 40, 40, 8, sbuf);
    /* layer 3:CONV_2D */
    convolve_1x1_s8_ch8_fpreq(&buffer0[76800], 40, 40, 8, (const q7_t*)weight3, bias3, scales3, -128, 4, -128, 127,
                              &buffer0[0], 40, 40, 48, sbuf);
    /* layer 4:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 40, 40, 48, (const q7_t*)CHWweight4, offsetBias4,
                                                  offsetRBias4, scales4, -128, 128, -128, 127, &buffer0[0], 20, 20, 48,
                                                  sbuf, -128);
    /* layer 5:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight5, bias5, scales5, -22, 128, -128, 127,
                               &buffer0[19200], 20, 20, 16, sbuf);
    /* layer 6:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[19200], 20, 20, 16, (const q7_t*)weight6, bias6, scales6, -128, 22, -128, 127,
                               &buffer0[0], 20, 20, 48, sbuf);
    /* layer 7:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight7, offsetBias7,
                                                  offsetRBias7, scales7, -128, 128, -128, 127, &buffer0[0], 20, 20, 48,
                                                  sbuf, -128);
    /* layer 8:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight8, bias8, scales8, 8, 128, -128, 127,
                               &buffer0[25600], 20, 20, 16, sbuf);
    /* layer 9:ADD */
    add_fpreq(6400, &buffer0[25600], 0.07699620723724365, 8, &buffer0[19200], 0.08598089963197708, -22,
              0.10197763890028, -20, &buffer0[32000]);
    /* layer 10:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[32000], 20, 20, 16, (const q7_t*)weight9, bias9, scales9, -128, 20, -128, 127,
                               &buffer0[0], 20, 20, 48, sbuf);
    /* layer 11:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight10, offsetBias10,
                                                  offsetRBias10, scales10, -128, 128, -128, 127, &buffer0[0], 20, 20,
                                                  48, sbuf, -128);
    /* layer 12:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)weight11, bias11, scales11, -4, 128, -128, 127,
                               &buffer0[19200], 20, 20, 16, sbuf);
    /* layer 13:ADD */
    add_fpreq(6400, &buffer0[19200], 0.024848325178027153, -4, &buffer0[32000], 0.10197763890028, -20,
              0.10464993864297867, -15, &buffer0[25600]);
    /* layer 14:CONV_2D */
    convolve_1x1_s8_ch16_fpreq(&buffer0[25600], 20, 20, 16, (const q7_t*)weight12, bias12, scales12, -128, 15, -128,
                               127, &buffer0[0], 20, 20, 48, sbuf);
    /* layer 15:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 20, 20, 48, (const q7_t*)CHWweight13, offsetBias13,
                                                  offsetRBias13, scales13, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  48, sbuf, -128);
    /* layer 16:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[0], 10, 10, 48, (const q7_t*)weight14, bias14, scales14, -16, 128, -128, 127,
                               &buffer0[14400], 10, 10, 24, sbuf);
    /* layer 17:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[14400], 10, 10, 24, (const q7_t*)weight15, bias15, scales15, -128, 16, -128,
                               127, &buffer0[0], 10, 10, 144, sbuf);
    /* layer 18:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)CHWweight16, offsetBias16,
                                                  offsetRBias16, scales16, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  144, sbuf, -128);
    /* layer 19:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)weight17, bias17, scales17, -14, 128, -128, 127,
                          &buffer0[16800], 10, 10, 24, sbuf);
    /* layer 20:ADD */
    add_fpreq(2400, &buffer0[16800], 0.06462342292070389, -14, &buffer0[14400], 0.06311018764972687, -16,
              0.07023955136537552, -3, &buffer0[12000]);
    /* layer 21:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[12000], 10, 10, 24, (const q7_t*)weight18, bias18, scales18, -128, 3, -128, 127,
                               &buffer0[0], 10, 10, 120, sbuf);
    /* layer 22:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 10, 10, 120, (const q7_t*)CHWweight19, offsetBias19,
                                                  offsetRBias19, scales19, -128, 128, -128, 127, &buffer0[0], 10, 10,
                                                  120, sbuf, -128);
    /* layer 23:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 10, 10, 120, (const q7_t*)weight20, bias20, scales20, -3, 128, -128, 127,
                          &buffer0[14400], 10, 10, 24, sbuf);
    /* layer 24:ADD */
    add_fpreq(2400, &buffer0[14400], 0.018893597647547722, -3, &buffer0[12000], 0.07023955136537552, -3,
              0.07366174459457397, -1, &buffer0[16800]);
    /* layer 25:CONV_2D */
    convolve_1x1_s8_ch24_fpreq(&buffer0[16800], 10, 10, 24, (const q7_t*)weight21, bias21, scales21, -128, 1, -128, 127,
                               &buffer0[0], 10, 10, 144, sbuf);
    /* layer 26:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 10, 10, 144, (const q7_t*)CHWweight22, offsetBias22,
                                                  offsetRBias22, scales22, -128, 128, -128, 127, &buffer0[0], 5, 5, 144,
                                                  sbuf, -128);
    /* layer 27:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 144, (const q7_t*)weight23, bias23, scales23, -11, 128, -128, 127,
                          &buffer0[6000], 5, 5, 40, sbuf);
    /* layer 28:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[6000], 5, 5, 40, (const q7_t*)weight24, bias24, scales24, -128, 11, -128, 127,
                          &buffer0[0], 5, 5, 240, sbuf);
    /* layer 29:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight25, offsetBias25,
                                                  offsetRBias25, scales25, -128, 128, -128, 127, &buffer0[0], 5, 5, 240,
                                                  sbuf, -128);
    /* layer 30:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)weight26, bias26, scales26, 1, 128, -128, 127,
                          &buffer0[7000], 5, 5, 40, sbuf);
    /* layer 31:ADD */
    add_fpreq(1000, &buffer0[7000], 0.04340619966387749, 1, &buffer0[6000], 0.0489354208111763, -11,
              0.04974109306931496, -3, &buffer0[8000]);
    /* layer 32:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[8000], 5, 5, 40, (const q7_t*)weight27, bias27, scales27, -128, 3, -128, 127,
                          &buffer0[0], 5, 5, 240, sbuf);
    /* layer 33:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight28, offsetBias28,
                                                  offsetRBias28, scales28, -128, 128, -128, 127, &buffer0[0], 5, 5, 240,
                                                  sbuf, -128);
    /* layer 34:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)weight29, bias29, scales29, 15, 128, -128, 127,
                          &buffer0[6000], 5, 5, 48, sbuf);
    /* layer 35:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[6000], 5, 5, 48, (const q7_t*)weight30, bias30, scales30, -128, -15, -128, 127,
                               &buffer0[0], 5, 5, 192, sbuf);
    /* layer 36:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)CHWweight31, offsetBias31,
                                                  offsetRBias31, scales31, -128, 128, -128, 127, &buffer0[0], 5, 5, 192,
                                                  sbuf, -128);
    /* layer 37:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)weight32, bias32, scales32, -11, 128, -128, 127,
                          &buffer0[4800], 5, 5, 48, sbuf);
    /* layer 38:ADD */
    add_fpreq(1200, &buffer0[4800], 0.04923376813530922, -11, &buffer0[6000], 0.04425579309463501, 15,
              0.05336926504969597, 5, &buffer0[7200]);
    /* layer 39:CONV_2D */
    convolve_1x1_s8_ch48_fpreq(&buffer0[7200], 5, 5, 48, (const q7_t*)weight33, bias33, scales33, -128, -5, -128, 127,
                               &buffer0[0], 5, 5, 240, sbuf);
    /* layer 40:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride2_inplace_CHW_fpreq(&buffer0[0], 5, 5, 240, (const q7_t*)CHWweight34, offsetBias34,
                                                  offsetRBias34, scales34, -128, 128, -128, 127, &buffer0[0], 3, 3, 240,
                                                  sbuf, -128);
    /* layer 41:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 240, (const q7_t*)weight35, bias35, scales35, -7, 128, -128, 127,
                          &buffer0[4320], 3, 3, 96, sbuf);
    /* layer 42:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[4320], 3, 3, 96, (const q7_t*)weight36, bias36, scales36, -128, 7, -128, 127,
                          &buffer0[0], 3, 3, 480, sbuf);
    /* layer 43:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 480, (const q7_t*)CHWweight37, offsetBias37,
                                                  offsetRBias37, scales37, -128, 128, -128, 127, &buffer0[0], 3, 3, 480,
                                                  sbuf, -128);
    /* layer 44:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 480, (const q7_t*)weight38, bias38, scales38, -2, 128, -128, 127,
                          &buffer0[5184], 3, 3, 96, sbuf);
    /* layer 45:ADD */
    add_fpreq(864, &buffer0[5184], 0.04011229798197746, -2, &buffer0[4320], 0.042191825807094574, -7,
              0.03923744335770607, -9, &buffer0[3456]);
    /* layer 46:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[3456], 3, 3, 96, (const q7_t*)weight39, bias39, scales39, -128, 9, -128, 127,
                          &buffer0[0], 3, 3, 384, sbuf);
    /* layer 47:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 384, (const q7_t*)CHWweight40, offsetBias40,
                                                  offsetRBias40, scales40, -128, 128, -128, 127, &buffer0[0], 3, 3, 384,
                                                  sbuf, -128);
    /* layer 48:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 384, (const q7_t*)weight41, bias41, scales41, -6, 128, -128, 127,
                          &buffer0[4320], 3, 3, 96, sbuf);
    /* layer 49:ADD */
    add_fpreq(864, &buffer0[4320], 0.019318707287311554, -6, &buffer0[3456], 0.03923744335770607, -9,
              0.0419764444231987, -12, &buffer0[2592]);
    /* layer 50:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[2592], 3, 3, 96, (const q7_t*)weight42, bias42, scales42, -128, 12, -128, 127,
                          &buffer0[0], 3, 3, 288, sbuf);
    /* layer 51:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 3, 3, 288, (const q7_t*)CHWweight43, offsetBias43,
                                                  offsetRBias43, scales43, -128, 128, -128, 127, &buffer0[0], 3, 3, 288,
                                                  sbuf, -128);
    /* layer 52:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 3, 3, 288, (const q7_t*)weight44, bias44, scales44, 9, 128, -128, 127,
                          &buffer0[2592], 3, 3, 160, sbuf);
    /* layer 53:AVERAGE_POOL_2D */
    avg_pooling(&buffer0[2592], 3, 3, 160, 3, 3, 1, 1, -128, 127, &buffer0[0]);
    /* layer 54:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 1, 1, 160, (const q7_t*)weight45, bias45, scales45, -1, -9, -128, 127,
                          &buffer0[160], 1, 1, 2, sbuf);
}
