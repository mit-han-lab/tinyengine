/* Automatically generated source file */
#include <float.h>
#include <tinyengine_function.h>
#include <tinyengine_function_fp.h>

#include "genInclude.h"
#include "genNN.h"
#include "genModel.h"

/* Variables used by all ops */
ADD_params add_params;
int i;
int8_t *int8ptr, *int8ptr2;
int32_t* int32ptr;
float *fptr, *fptr2, *fptr3;

signed char* getInput() { return &buffer0[82944]; }
signed char* getOutput() { return NNoutput; }
void end2endinference(q7_t* img) { invoke(NULL); }
void invoke(float* labels) {
    /* layer 0:CONV_2D */
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[82944], 144, 144, 3, (const q7_t*)weight0, bias0, scales0,
                                                    -128, 1, -128, 127, &buffer0[0], 72, 72, 16, sbuf, kbuf, -1);
    /* layer 1:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 72, 72, 16, (const q7_t*)CHWweight1, offsetBias1,
                                                  offsetRBias1, scales1, -128, 128, -128, 127, &buffer0[0], 72, 72, 16,
                                                  sbuf, -128);
    /* layer 2:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 72, 72, 16, (const q7_t*)weight2, bias2, scales2, 6, 128, -128, 127,
                          &buffer0[124416], 72, 72, 8, sbuf);
    /* layer 3:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[124416], 72, 72, 8, (const q7_t*)weight3, bias3, scales3, -128, -6, -128, 127,
                          &buffer0[0], 72, 72, 24, sbuf);
    /* layer 4:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 72, 72, 24, (const q7_t*)CHWweight4, offsetBias4,
                                                  offsetRBias4, scales4, -128, 128, -128, 127, &buffer0[0], 36, 36, 24,
                                                  sbuf, -128);
    /* layer 5:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 24, (const q7_t*)weight5, bias5, scales5, 8, 128, -128, 127,
                          &buffer0[103680], 36, 36, 16, sbuf);
    /* layer 6:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[103680], 36, 36, 16, (const q7_t*)weight6, bias6, scales6, -128, -8, -128, 127,
                          &buffer0[0], 36, 36, 80, sbuf);
    /* layer 7:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)CHWweight7, offsetBias7,
                                                  offsetRBias7, scales7, -128, 128, -128, 127, &buffer0[0], 36, 36, 80,
                                                  sbuf, -128);
    /* layer 8:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)weight8, bias8, scales8, -3, 128, -128, 127,
                          &buffer0[124416], 36, 36, 16, sbuf);
    /* layer 9:ADD */
    add_fpreq(20736, &buffer0[124416], 0.11007079482078552, -3, &buffer0[103680], 0.08261138945817947, 8,
              0.11468882113695145, 5, &buffer0[145152]);
    /* layer 10:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[145152], 36, 36, 16, (const q7_t*)weight9, bias9, scales9, -128, -5, -128, 127,
                          &buffer0[0], 36, 36, 96, sbuf);
    /* layer 11:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 96, (const q7_t*)CHWweight10, offsetBias10,
                                                  offsetRBias10, scales10, -128, 128, -128, 127, &buffer0[0], 36, 36,
                                                  96, sbuf, -128);
    /* layer 12:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 96, (const q7_t*)weight11, bias11, scales11, -4, 128, -128, 127,
                          &buffer0[124416], 36, 36, 16, sbuf);
    /* layer 13:ADD */
    add_fpreq(20736, &buffer0[124416], 0.030577559024095535, -4, &buffer0[145152], 0.11468882113695145, 5,
              0.12079815566539764, -1, &buffer0[62208]);
    /* layer 14:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[62208], 36, 36, 16, (const q7_t*)weight12, bias12, scales12, -128, 1, -128, 127,
                          &buffer0[0], 36, 36, 48, sbuf);
    /* layer 15:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 48, (const q7_t*)CHWweight13, offsetBias13,
                                                  offsetRBias13, scales13, -128, 128, -128, 127, &buffer0[0], 36, 36,
                                                  48, sbuf, -128);
    /* layer 16:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 48, (const q7_t*)weight14, bias14, scales14, -11, 128, -128, 127,
                          &buffer0[82944], 36, 36, 16, sbuf);
    /* layer 17:ADD */
    add_fpreq(20736, &buffer0[82944], 0.014525755308568478, -11, &buffer0[62208], 0.12079815566539764, -1,
              0.12125824391841888, 4, &buffer0[103680]);
    /* layer 18:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[103680], 36, 36, 16, (const q7_t*)weight15, bias15, scales15, -128, -4, -128, 127,
                          &buffer0[0], 36, 36, 80, sbuf);
    /* layer 19:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)CHWweight16, offsetBias16,
                                                  offsetRBias16, scales16, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  80, sbuf, -128);
    /* layer 20:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 80, (const q7_t*)weight17, bias17, scales17, 10, 128, -128, 127,
                          &buffer0[31104], 18, 18, 24, sbuf);
    /* layer 21:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[31104], 18, 18, 24, (const q7_t*)weight18, bias18, scales18, -128, -10, -128, 127,
                          &buffer0[0], 18, 18, 96, sbuf);
    /* layer 22:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)CHWweight19, offsetBias19,
                                                  offsetRBias19, scales19, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  96, sbuf, -128);
    /* layer 23:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)weight20, bias20, scales20, 0, 128, -128, 127,
                          &buffer0[38880], 18, 18, 24, sbuf);
    /* layer 24:ADD */
    add_fpreq(7776, &buffer0[38880], 0.07240664958953857, 0, &buffer0[31104], 0.06941551715135574, 10,
              0.10135574638843536, 16, &buffer0[46656]);
    /* layer 25:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[46656], 18, 18, 24, (const q7_t*)weight21, bias21, scales21, -128, -16, -128, 127,
                          &buffer0[0], 18, 18, 96, sbuf);
    /* layer 26:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)CHWweight22, offsetBias22,
                                                  offsetRBias22, scales22, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  96, sbuf, -128);
    /* layer 27:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)weight23, bias23, scales23, 9, 128, -128, 127,
                          &buffer0[31104], 18, 18, 24, sbuf);
    /* layer 28:ADD */
    add_fpreq(7776, &buffer0[31104], 0.021588750183582306, 9, &buffer0[46656], 0.10135574638843536, 16,
              0.10526679456233978, 18, &buffer0[54432]);
    /* layer 29:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[54432], 18, 18, 24, (const q7_t*)weight24, bias24, scales24, -128, -18, -128, 127,
                          &buffer0[0], 18, 18, 144, sbuf);
    /* layer 30:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)CHWweight25, offsetBias25,
                                                  offsetRBias25, scales25, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  144, sbuf, -128);
    /* layer 31:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)weight26, bias26, scales26, -3, 128, -128, 127,
                          &buffer0[46656], 18, 18, 24, sbuf);
    /* layer 32:ADD */
    add_fpreq(7776, &buffer0[46656], 0.008338475599884987, -3, &buffer0[54432], 0.10526679456233978, 18,
              0.10625039041042328, 17, &buffer0[62208]);
    /* layer 33:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[62208], 18, 18, 24, (const q7_t*)weight27, bias27, scales27, -128, -17, -128, 127,
                          &buffer0[0], 18, 18, 144, sbuf);
    /* layer 34:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)CHWweight28, offsetBias28,
                                                  offsetRBias28, scales28, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 35:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight29, bias29, scales29, 3, 128, -128, 127,
                          &buffer0[19440], 9, 9, 40, sbuf);
    /* layer 36:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 40, (const q7_t*)weight30, bias30, scales30, -128, -3, -128, 127,
                          &buffer0[0], 9, 9, 240, sbuf);
    /* layer 37:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 240, (const q7_t*)CHWweight31, offsetBias31,
                                                  offsetRBias31, scales31, -128, 128, -128, 127, &buffer0[0], 9, 9, 240,
                                                  sbuf, -128);
    /* layer 38:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 240, (const q7_t*)weight32, bias32, scales32, 17, 128, -128, 127,
                          &buffer0[22680], 9, 9, 40, sbuf);
    /* layer 39:ADD */
    add_fpreq(3240, &buffer0[22680], 0.047017451375722885, 17, &buffer0[19440], 0.06724316626787186, 3,
              0.07311003655195236, 2, &buffer0[12960]);
    /* layer 40:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12960], 9, 9, 40, (const q7_t*)weight33, bias33, scales33, -128, -2, -128, 127,
                          &buffer0[0], 9, 9, 160, sbuf);
    /* layer 41:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 160, (const q7_t*)CHWweight34, offsetBias34,
                                                  offsetRBias34, scales34, -128, 128, -128, 127, &buffer0[0], 9, 9, 160,
                                                  sbuf, -128);
    /* layer 42:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 160, (const q7_t*)weight35, bias35, scales35, -22, 128, -128, 127,
                          &buffer0[16200], 9, 9, 40, sbuf);
    /* layer 43:ADD */
    add_fpreq(3240, &buffer0[16200], 0.026774989441037178, -22, &buffer0[12960], 0.07311003655195236, 2,
              0.07379238307476044, 3, &buffer0[19440]);
    /* layer 44:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 40, (const q7_t*)weight36, bias36, scales36, -128, -3, -128, 127,
                          &buffer0[0], 9, 9, 200, sbuf);
    /* layer 45:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)CHWweight37, offsetBias37,
                                                  offsetRBias37, scales37, -128, 128, -128, 127, &buffer0[0], 9, 9, 200,
                                                  sbuf, -128);
    /* layer 46:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)weight38, bias38, scales38, 8, 128, -128, 127,
                          &buffer0[16200], 9, 9, 40, sbuf);
    /* layer 47:ADD */
    add_fpreq(3240, &buffer0[16200], 0.01692686229944229, 8, &buffer0[19440], 0.07379238307476044, 3,
              0.07368244230747223, 2, &buffer0[22680]);
    /* layer 48:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[22680], 9, 9, 40, (const q7_t*)weight39, bias39, scales39, -128, -2, -128, 127,
                          &buffer0[0], 9, 9, 200, sbuf);
    /* layer 49:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)CHWweight40, offsetBias40,
                                                  offsetRBias40, scales40, -128, 128, -128, 127, &buffer0[0], 9, 9, 200,
                                                  sbuf, -128);
    /* layer 50:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)weight41, bias41, scales41, -12, 128, -128, 127,
                          &buffer0[16200], 9, 9, 48, sbuf);
    /* layer 51:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[16200], 9, 9, 48, (const q7_t*)weight42, bias42, scales42, -128, 12, -128, 127,
                          &buffer0[0], 9, 9, 144, sbuf);
    /* layer 52:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)CHWweight43, offsetBias43,
                                                  offsetRBias43, scales43, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 53:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight44, bias44, scales44, -10, 128, -128, 127,
                          &buffer0[11664], 9, 9, 48, sbuf);
    /* layer 54:ADD */
    add_fpreq(3888, &buffer0[11664], 0.05333873629570007, -10, &buffer0[16200], 0.057206179946660995, -12,
              0.07569750398397446, -8, &buffer0[20088]);
    /* layer 55:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[20088], 9, 9, 48, (const q7_t*)weight45, bias45, scales45, -128, 8, -128, 127,
                          &buffer0[0], 9, 9, 192, sbuf);
    /* layer 56:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)CHWweight46, offsetBias46,
                                                  offsetRBias46, scales46, -128, 128, -128, 127, &buffer0[0], 9, 9, 192,
                                                  sbuf, -128);
    /* layer 57:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)weight47, bias47, scales47, -5, 128, -128, 127,
                          &buffer0[15552], 9, 9, 48, sbuf);
    /* layer 58:ADD */
    add_fpreq(3888, &buffer0[15552], 0.01734577864408493, -5, &buffer0[20088], 0.07569750398397446, -8,
              0.08066040277481079, 5, &buffer0[11664]);
    /* layer 59:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[11664], 9, 9, 48, (const q7_t*)weight48, bias48, scales48, -128, -5, -128, 127,
                          &buffer0[0], 9, 9, 144, sbuf);
    /* layer 60:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)CHWweight49, offsetBias49,
                                                  offsetRBias49, scales49, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 61:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight50, bias50, scales50, 12, 128, -128, 127,
                          &buffer0[15552], 9, 9, 48, sbuf);
    /* layer 62:ADD */
    add_fpreq(3888, &buffer0[15552], 0.007162634748965502, 12, &buffer0[11664], 0.08066040277481079, 5,
              0.08187197148799896, 6, &buffer0[19440]);
    /* layer 63:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 48, (const q7_t*)weight51, bias51, scales51, -128, -6, -128, 127,
                          &buffer0[0], 9, 9, 192, sbuf);
    /* layer 64:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)CHWweight52, offsetBias52,
                                                  offsetRBias52, scales52, -128, 128, -128, 127, &buffer0[0], 5, 5, 192,
                                                  sbuf, -128);
    /* layer 65:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)weight53, bias53, scales53, 12, 128, -128, 127,
                          &buffer0[12000], 5, 5, 96, sbuf);
    /* layer 66:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12000], 5, 5, 96, (const q7_t*)weight54, bias54, scales54, -128, -12, -128, 127,
                          &buffer0[0], 5, 5, 480, sbuf);
    /* layer 67:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)CHWweight55, offsetBias55,
                                                  offsetRBias55, scales55, -128, 128, -128, 127, &buffer0[0], 5, 5, 480,
                                                  sbuf, -128);
    /* layer 68:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)weight56, bias56, scales56, -8, 128, -128, 127,
                          &buffer0[14400], 5, 5, 96, sbuf);
    /* layer 69:ADD */
    add_fpreq(2400, &buffer0[14400], 0.0438091941177845, -8, &buffer0[12000], 0.04971432313323021, 12,
              0.04909515380859375, 1, &buffer0[9600]);
    /* layer 70:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[9600], 5, 5, 96, (const q7_t*)weight57, bias57, scales57, -128, -1, -128, 127,
                          &buffer0[0], 5, 5, 384, sbuf);
    /* layer 71:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)CHWweight58, offsetBias58,
                                                  offsetRBias58, scales58, -128, 128, -128, 127, &buffer0[0], 5, 5, 384,
                                                  sbuf, -128);
    /* layer 72:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)weight59, bias59, scales59, -2, 128, -128, 127,
                          &buffer0[12000], 5, 5, 96, sbuf);
    /* layer 73:ADD */
    add_fpreq(2400, &buffer0[12000], 0.021911632269620895, -2, &buffer0[9600], 0.04909515380859375, 1,
              0.05280669406056404, 0, &buffer0[14400]);
    /* layer 74:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[14400], 5, 5, 96, (const q7_t*)weight60, bias60, scales60, -128, 0, -128, 127,
                          &buffer0[0], 5, 5, 384, sbuf);
    /* layer 75:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)CHWweight61, offsetBias61,
                                                  offsetRBias61, scales61, -128, 128, -128, 127, &buffer0[0], 5, 5, 384,
                                                  sbuf, -128);
    /* layer 76:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)weight62, bias62, scales62, -26, 128, -128, 127,
                          &buffer0[9600], 5, 5, 96, sbuf);
    /* layer 77:ADD */
    add_fpreq(2400, &buffer0[9600], 0.013492698781192303, -26, &buffer0[14400], 0.05280669406056404, 0,
              0.05457576736807823, -2, &buffer0[12000]);
    /* layer 78:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12000], 5, 5, 96, (const q7_t*)weight63, bias63, scales63, -128, 2, -128, 127,
                          &buffer0[0], 5, 5, 480, sbuf);
    /* layer 79:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)CHWweight64, offsetBias64,
                                                  offsetRBias64, scales64, -128, 128, -128, 127, &buffer0[0], 5, 5, 480,
                                                  sbuf, -128);
    /* layer 80:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)weight65, bias65, scales65, -7, 128, -128, 127,
                          &buffer0[12000], 5, 5, 160, sbuf);
    /* layer 81:AVERAGE_POOL_2D */
    avg_pooling(&buffer0[12000], 5, 5, 160, 5, 5, 1, 1, -128, 127, &buffer0[0]);
    /* layer 82:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 1, 1, 160, (const q7_t*)weight66, bias66, scales66, 0, 7, -128, 127,
                          &buffer0[160], 1, 1, 2, sbuf);
}
void invoke_inf() {
    /* layer 0:CONV_2D */
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[82944], 144, 144, 3, (const q7_t*)weight0, bias0, scales0,
                                                    -128, 1, -128, 127, &buffer0[0], 72, 72, 16, sbuf, kbuf, -1);
    /* layer 1:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 72, 72, 16, (const q7_t*)CHWweight1, offsetBias1,
                                                  offsetRBias1, scales1, -128, 128, -128, 127, &buffer0[0], 72, 72, 16,
                                                  sbuf, -128);
    /* layer 2:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 72, 72, 16, (const q7_t*)weight2, bias2, scales2, 6, 128, -128, 127,
                          &buffer0[124416], 72, 72, 8, sbuf);
    /* layer 3:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[124416], 72, 72, 8, (const q7_t*)weight3, bias3, scales3, -128, -6, -128, 127,
                          &buffer0[0], 72, 72, 24, sbuf);
    /* layer 4:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 72, 72, 24, (const q7_t*)CHWweight4, offsetBias4,
                                                  offsetRBias4, scales4, -128, 128, -128, 127, &buffer0[0], 36, 36, 24,
                                                  sbuf, -128);
    /* layer 5:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 24, (const q7_t*)weight5, bias5, scales5, 8, 128, -128, 127,
                          &buffer0[103680], 36, 36, 16, sbuf);
    /* layer 6:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[103680], 36, 36, 16, (const q7_t*)weight6, bias6, scales6, -128, -8, -128, 127,
                          &buffer0[0], 36, 36, 80, sbuf);
    /* layer 7:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)CHWweight7, offsetBias7,
                                                  offsetRBias7, scales7, -128, 128, -128, 127, &buffer0[0], 36, 36, 80,
                                                  sbuf, -128);
    /* layer 8:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)weight8, bias8, scales8, -3, 128, -128, 127,
                          &buffer0[124416], 36, 36, 16, sbuf);
    /* layer 9:ADD */
    add_fpreq(20736, &buffer0[124416], 0.11007079482078552, -3, &buffer0[103680], 0.08261138945817947, 8,
              0.11468882113695145, 5, &buffer0[145152]);
    /* layer 10:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[145152], 36, 36, 16, (const q7_t*)weight9, bias9, scales9, -128, -5, -128, 127,
                          &buffer0[0], 36, 36, 96, sbuf);
    /* layer 11:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 96, (const q7_t*)CHWweight10, offsetBias10,
                                                  offsetRBias10, scales10, -128, 128, -128, 127, &buffer0[0], 36, 36,
                                                  96, sbuf, -128);
    /* layer 12:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 96, (const q7_t*)weight11, bias11, scales11, -4, 128, -128, 127,
                          &buffer0[124416], 36, 36, 16, sbuf);
    /* layer 13:ADD */
    add_fpreq(20736, &buffer0[124416], 0.030577559024095535, -4, &buffer0[145152], 0.11468882113695145, 5,
              0.12079815566539764, -1, &buffer0[62208]);
    /* layer 14:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[62208], 36, 36, 16, (const q7_t*)weight12, bias12, scales12, -128, 1, -128, 127,
                          &buffer0[0], 36, 36, 48, sbuf);
    /* layer 15:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 36, 36, 48, (const q7_t*)CHWweight13, offsetBias13,
                                                  offsetRBias13, scales13, -128, 128, -128, 127, &buffer0[0], 36, 36,
                                                  48, sbuf, -128);
    /* layer 16:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 36, 36, 48, (const q7_t*)weight14, bias14, scales14, -11, 128, -128, 127,
                          &buffer0[82944], 36, 36, 16, sbuf);
    /* layer 17:ADD */
    add_fpreq(20736, &buffer0[82944], 0.014525755308568478, -11, &buffer0[62208], 0.12079815566539764, -1,
              0.12125824391841888, 4, &buffer0[103680]);
    /* layer 18:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[103680], 36, 36, 16, (const q7_t*)weight15, bias15, scales15, -128, -4, -128, 127,
                          &buffer0[0], 36, 36, 80, sbuf);
    /* layer 19:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride2_inplace_CHW_fpreq(&buffer0[0], 36, 36, 80, (const q7_t*)CHWweight16, offsetBias16,
                                                  offsetRBias16, scales16, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  80, sbuf, -128);
    /* layer 20:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 80, (const q7_t*)weight17, bias17, scales17, 10, 128, -128, 127,
                          &buffer0[31104], 18, 18, 24, sbuf);
    /* layer 21:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[31104], 18, 18, 24, (const q7_t*)weight18, bias18, scales18, -128, -10, -128, 127,
                          &buffer0[0], 18, 18, 96, sbuf);
    /* layer 22:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)CHWweight19, offsetBias19,
                                                  offsetRBias19, scales19, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  96, sbuf, -128);
    /* layer 23:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)weight20, bias20, scales20, 0, 128, -128, 127,
                          &buffer0[38880], 18, 18, 24, sbuf);
    /* layer 24:ADD */
    add_fpreq(7776, &buffer0[38880], 0.07240664958953857, 0, &buffer0[31104], 0.06941551715135574, 10,
              0.10135574638843536, 16, &buffer0[46656]);
    /* layer 25:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[46656], 18, 18, 24, (const q7_t*)weight21, bias21, scales21, -128, -16, -128, 127,
                          &buffer0[0], 18, 18, 96, sbuf);
    /* layer 26:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)CHWweight22, offsetBias22,
                                                  offsetRBias22, scales22, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  96, sbuf, -128);
    /* layer 27:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 96, (const q7_t*)weight23, bias23, scales23, 9, 128, -128, 127,
                          &buffer0[31104], 18, 18, 24, sbuf);
    /* layer 28:ADD */
    add_fpreq(7776, &buffer0[31104], 0.021588750183582306, 9, &buffer0[46656], 0.10135574638843536, 16,
              0.10526679456233978, 18, &buffer0[54432]);
    /* layer 29:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[54432], 18, 18, 24, (const q7_t*)weight24, bias24, scales24, -128, -18, -128, 127,
                          &buffer0[0], 18, 18, 144, sbuf);
    /* layer 30:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)CHWweight25, offsetBias25,
                                                  offsetRBias25, scales25, -128, 128, -128, 127, &buffer0[0], 18, 18,
                                                  144, sbuf, -128);
    /* layer 31:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)weight26, bias26, scales26, -3, 128, -128, 127,
                          &buffer0[46656], 18, 18, 24, sbuf);
    /* layer 32:ADD */
    add_fpreq(7776, &buffer0[46656], 0.008338475599884987, -3, &buffer0[54432], 0.10526679456233978, 18,
              0.10625039041042328, 17, &buffer0[62208]);
    /* layer 33:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[62208], 18, 18, 24, (const q7_t*)weight27, bias27, scales27, -128, -17, -128, 127,
                          &buffer0[0], 18, 18, 144, sbuf);
    /* layer 34:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 18, 18, 144, (const q7_t*)CHWweight28, offsetBias28,
                                                  offsetRBias28, scales28, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 35:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight29, bias29, scales29, 3, 128, -128, 127,
                          &buffer0[19440], 9, 9, 40, sbuf);
    /* layer 36:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 40, (const q7_t*)weight30, bias30, scales30, -128, -3, -128, 127,
                          &buffer0[0], 9, 9, 240, sbuf);
    /* layer 37:DEPTHWISE_CONV_2D */
    depthwise_kernel7x7_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 240, (const q7_t*)CHWweight31, offsetBias31,
                                                  offsetRBias31, scales31, -128, 128, -128, 127, &buffer0[0], 9, 9, 240,
                                                  sbuf, -128);
    /* layer 38:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 240, (const q7_t*)weight32, bias32, scales32, 17, 128, -128, 127,
                          &buffer0[22680], 9, 9, 40, sbuf);
    /* layer 39:ADD */
    add_fpreq(3240, &buffer0[22680], 0.047017451375722885, 17, &buffer0[19440], 0.06724316626787186, 3,
              0.07311003655195236, 2, &buffer0[12960]);
    /* layer 40:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12960], 9, 9, 40, (const q7_t*)weight33, bias33, scales33, -128, -2, -128, 127,
                          &buffer0[0], 9, 9, 160, sbuf);
    /* layer 41:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 160, (const q7_t*)CHWweight34, offsetBias34,
                                                  offsetRBias34, scales34, -128, 128, -128, 127, &buffer0[0], 9, 9, 160,
                                                  sbuf, -128);
    /* layer 42:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 160, (const q7_t*)weight35, bias35, scales35, -22, 128, -128, 127,
                          &buffer0[16200], 9, 9, 40, sbuf);
    /* layer 43:ADD */
    add_fpreq(3240, &buffer0[16200], 0.026774989441037178, -22, &buffer0[12960], 0.07311003655195236, 2,
              0.07379238307476044, 3, &buffer0[19440]);
    /* layer 44:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 40, (const q7_t*)weight36, bias36, scales36, -128, -3, -128, 127,
                          &buffer0[0], 9, 9, 200, sbuf);
    /* layer 45:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)CHWweight37, offsetBias37,
                                                  offsetRBias37, scales37, -128, 128, -128, 127, &buffer0[0], 9, 9, 200,
                                                  sbuf, -128);
    /* layer 46:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)weight38, bias38, scales38, 8, 128, -128, 127,
                          &buffer0[16200], 9, 9, 40, sbuf);
    /* layer 47:ADD */
    add_fpreq(3240, &buffer0[16200], 0.01692686229944229, 8, &buffer0[19440], 0.07379238307476044, 3,
              0.07368244230747223, 2, &buffer0[22680]);
    /* layer 48:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[22680], 9, 9, 40, (const q7_t*)weight39, bias39, scales39, -128, -2, -128, 127,
                          &buffer0[0], 9, 9, 200, sbuf);
    /* layer 49:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)CHWweight40, offsetBias40,
                                                  offsetRBias40, scales40, -128, 128, -128, 127, &buffer0[0], 9, 9, 200,
                                                  sbuf, -128);
    /* layer 50:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 200, (const q7_t*)weight41, bias41, scales41, -12, 128, -128, 127,
                          &buffer0[16200], 9, 9, 48, sbuf);
    /* layer 51:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[16200], 9, 9, 48, (const q7_t*)weight42, bias42, scales42, -128, 12, -128, 127,
                          &buffer0[0], 9, 9, 144, sbuf);
    /* layer 52:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)CHWweight43, offsetBias43,
                                                  offsetRBias43, scales43, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 53:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight44, bias44, scales44, -10, 128, -128, 127,
                          &buffer0[11664], 9, 9, 48, sbuf);
    /* layer 54:ADD */
    add_fpreq(3888, &buffer0[11664], 0.05333873629570007, -10, &buffer0[16200], 0.057206179946660995, -12,
              0.07569750398397446, -8, &buffer0[20088]);
    /* layer 55:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[20088], 9, 9, 48, (const q7_t*)weight45, bias45, scales45, -128, 8, -128, 127,
                          &buffer0[0], 9, 9, 192, sbuf);
    /* layer 56:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)CHWweight46, offsetBias46,
                                                  offsetRBias46, scales46, -128, 128, -128, 127, &buffer0[0], 9, 9, 192,
                                                  sbuf, -128);
    /* layer 57:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)weight47, bias47, scales47, -5, 128, -128, 127,
                          &buffer0[15552], 9, 9, 48, sbuf);
    /* layer 58:ADD */
    add_fpreq(3888, &buffer0[15552], 0.01734577864408493, -5, &buffer0[20088], 0.07569750398397446, -8,
              0.08066040277481079, 5, &buffer0[11664]);
    /* layer 59:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[11664], 9, 9, 48, (const q7_t*)weight48, bias48, scales48, -128, -5, -128, 127,
                          &buffer0[0], 9, 9, 144, sbuf);
    /* layer 60:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)CHWweight49, offsetBias49,
                                                  offsetRBias49, scales49, -128, 128, -128, 127, &buffer0[0], 9, 9, 144,
                                                  sbuf, -128);
    /* layer 61:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 9, 9, 144, (const q7_t*)weight50, bias50, scales50, 12, 128, -128, 127,
                          &buffer0[15552], 9, 9, 48, sbuf);
    /* layer 62:ADD */
    add_fpreq(3888, &buffer0[15552], 0.007162634748965502, 12, &buffer0[11664], 0.08066040277481079, 5,
              0.08187197148799896, 6, &buffer0[19440]);
    /* layer 63:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[19440], 9, 9, 48, (const q7_t*)weight51, bias51, scales51, -128, -6, -128, 127,
                          &buffer0[0], 9, 9, 192, sbuf);
    /* layer 64:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride2_inplace_CHW_fpreq(&buffer0[0], 9, 9, 192, (const q7_t*)CHWweight52, offsetBias52,
                                                  offsetRBias52, scales52, -128, 128, -128, 127, &buffer0[0], 5, 5, 192,
                                                  sbuf, -128);
    /* layer 65:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 192, (const q7_t*)weight53, bias53, scales53, 12, 128, -128, 127,
                          &buffer0[12000], 5, 5, 96, sbuf);
    /* layer 66:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12000], 5, 5, 96, (const q7_t*)weight54, bias54, scales54, -128, -12, -128, 127,
                          &buffer0[0], 5, 5, 480, sbuf);
    /* layer 67:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)CHWweight55, offsetBias55,
                                                  offsetRBias55, scales55, -128, 128, -128, 127, &buffer0[0], 5, 5, 480,
                                                  sbuf, -128);
    /* layer 68:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)weight56, bias56, scales56, -8, 128, -128, 127,
                          &buffer0[14400], 5, 5, 96, sbuf);
    /* layer 69:ADD */
    add_fpreq(2400, &buffer0[14400], 0.0438091941177845, -8, &buffer0[12000], 0.04971432313323021, 12,
              0.04909515380859375, 1, &buffer0[9600]);
    /* layer 70:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[9600], 5, 5, 96, (const q7_t*)weight57, bias57, scales57, -128, -1, -128, 127,
                          &buffer0[0], 5, 5, 384, sbuf);
    /* layer 71:DEPTHWISE_CONV_2D */
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)CHWweight58, offsetBias58,
                                                  offsetRBias58, scales58, -128, 128, -128, 127, &buffer0[0], 5, 5, 384,
                                                  sbuf, -128);
    /* layer 72:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)weight59, bias59, scales59, -2, 128, -128, 127,
                          &buffer0[12000], 5, 5, 96, sbuf);
    /* layer 73:ADD */
    add_fpreq(2400, &buffer0[12000], 0.021911632269620895, -2, &buffer0[9600], 0.04909515380859375, 1,
              0.05280669406056404, 0, &buffer0[14400]);
    /* layer 74:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[14400], 5, 5, 96, (const q7_t*)weight60, bias60, scales60, -128, 0, -128, 127,
                          &buffer0[0], 5, 5, 384, sbuf);
    /* layer 75:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)CHWweight61, offsetBias61,
                                                  offsetRBias61, scales61, -128, 128, -128, 127, &buffer0[0], 5, 5, 384,
                                                  sbuf, -128);
    /* layer 76:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 384, (const q7_t*)weight62, bias62, scales62, -26, 128, -128, 127,
                          &buffer0[9600], 5, 5, 96, sbuf);
    /* layer 77:ADD */
    add_fpreq(2400, &buffer0[9600], 0.013492698781192303, -26, &buffer0[14400], 0.05280669406056404, 0,
              0.05457576736807823, -2, &buffer0[12000]);
    /* layer 78:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[12000], 5, 5, 96, (const q7_t*)weight63, bias63, scales63, -128, 2, -128, 127,
                          &buffer0[0], 5, 5, 480, sbuf);
    /* layer 79:DEPTHWISE_CONV_2D */
    depthwise_kernel3x3_stride1_inplace_CHW_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)CHWweight64, offsetBias64,
                                                  offsetRBias64, scales64, -128, 128, -128, 127, &buffer0[0], 5, 5, 480,
                                                  sbuf, -128);
    /* layer 80:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 5, 5, 480, (const q7_t*)weight65, bias65, scales65, -7, 128, -128, 127,
                          &buffer0[12000], 5, 5, 160, sbuf);
    /* layer 81:AVERAGE_POOL_2D */
    avg_pooling(&buffer0[12000], 5, 5, 160, 5, 5, 1, 1, -128, 127, &buffer0[0]);
    /* layer 82:CONV_2D */
    convolve_1x1_s8_fpreq(&buffer0[0], 1, 1, 160, (const q7_t*)weight66, bias66, scales66, 0, 7, -128, 127,
                          &buffer0[160], 1, 1, 2, sbuf);
}