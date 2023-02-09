/* Automatically generated source file */
#include "arm_nnfunctions.h"

#include "genNN.h"
#include "genModel.h"

#include "tinyengine_function.h"


/* Variables used by all ops */
ADD_params add_params;

signed char* getInput() {
	return buffer0;
}
signed char* getOutput() {
	return NNoutput;
}
const int anchors[3][3][2] = {
{{116,90},{156,198},{373,326},},
{{30,61},{62,45},{59,119},},
{{10,13},{16,30},{33,23},},
};
void det_post_procesing(int* box_cnt, det_box** ret_box, float threshold){
    int8_t *input_data[3]={bufferdagop4_output0,bufferdagop3_output0,bufferdagop2_output0,};
    int8_t y_zero[3]={14,44,110,};
    float y_scale[3]={0.12965305149555206,0.13986867666244507,0.557377278804779,};
 	postprocessing(input_data, y_zero, y_scale, front, 160, 128, 21, 2, anchors, 3, 0.45f, threshold, box_cnt, ret_box);
}
void end2endinference(q7_t* img){
}
void invoke(float* labels){
/* layer 0:CONV_2D */
convolve_s8_kernel3_inputch3_stride2_pad1(&buffer0[0],160,128,3,(const q7_t*) weight0,bias0,shift0,multiplier0,-128,0,-128,127,&buffer0[163840],80,64,16,sbuf,kbuf,0);
/* layer 1:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[163840],80,64,16,(const q7_t*) CHWweight1,offsetBias1,offsetRBias1,shift1,multiplier1,-128,128,-128,127,&buffer0[0],80,64,16,sbuf,-128);
/* layer 2:CONV_2D */
convolve_1x1_s8_ch16(&buffer0[163840],80,64,16,(const q7_t*) weight2,bias2,shift2,multiplier2,-10,128,-128,127,&buffer0[0],80,64,16,sbuf);
/* layer 3:CONV_2D */
convolve_1x1_s8_ch16(&buffer0[0],80,64,16,(const q7_t*) weight3,bias3,shift3,multiplier3,-128,10,-128,127,&buffer0[81920],80,64,32,sbuf);
/* layer 4:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride2_inplace_CHW(&buffer0[81920],80,64,32,(const q7_t*) CHWweight4,offsetBias4,offsetRBias4,shift4,multiplier4,-128,128,-128,127,&buffer0[0],40,32,32,sbuf,-128);
/* layer 5:CONV_2D */
convolve_1x1_s8(&buffer0[81920],40,32,32,(const q7_t*) weight5,bias5,shift5,multiplier5,12,128,-128,127,&buffer0[0],40,32,16,sbuf);
/* layer 6:CONV_2D */
convolve_1x1_s8_ch16(&buffer0[0],40,32,16,(const q7_t*) weight6,bias6,shift6,multiplier6,-128,-12,-128,127,&buffer0[163840],40,32,64,sbuf);
/* layer 7:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride2_inplace_CHW(&buffer0[163840],40,32,64,(const q7_t*) CHWweight7,offsetBias7,offsetRBias7,shift7,multiplier7,-128,128,-128,127,&buffer0[0],20,16,64,sbuf,-128);
/* layer 8:CONV_2D */
convolve_1x1_s8(&buffer0[163840],20,16,64,(const q7_t*) weight8,bias8,shift8,multiplier8,7,128,-128,127,&buffer1[0],20,16,16,sbuf);
/* layer 9:CONV_2D */
convolve_1x1_s8_ch16(&buffer1[0],20,16,16,(const q7_t*) weight9,bias9,shift9,multiplier9,-128,-7,-128,127,&buffer0[230400],20,16,48,sbuf);
/* layer 10:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[230400],20,16,48,(const q7_t*) CHWweight10,offsetBias10,offsetRBias10,shift10,multiplier10,-128,128,-128,127,&buffer0[0],20,16,48,sbuf,-128);
/* layer 11:CONV_2D */
convolve_1x1_s8_ch48(&buffer0[230400],20,16,48,(const q7_t*) weight11,bias11,shift11,multiplier11,11,128,-128,127,&buffer0[0],20,16,16,sbuf);
/* layer 12:ADD */
add_params.input_h = 16;add_params.input_w = 20;add_params.input_c = 16;
add_params.left_shift = 20;add_params.input1_offset = -11;add_params.input1_multiplier = 1906117632;add_params.input1_shift = -1;add_params.input2_offset = -7;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = 14;add_params.output_multiplier = 1622812805;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferstage2[0]);
/* layer 13:CONV_2D */
convolve_1x1_s8_ch16(&bufferstage2[0],20,16,16,(const q7_t*) weight12,bias12,shift12,multiplier12,-128,-14,-128,127,&buffer0[230400],20,16,48,sbuf);
/* layer 14:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride2_inplace_CHW(&buffer0[230400],20,16,48,(const q7_t*) CHWweight13,offsetBias13,offsetRBias13,shift13,multiplier13,-128,128,-128,127,&buffer0[0],10,8,48,sbuf,-128);
/* layer 15:CONV_2D */
convolve_1x1_s8_ch48(&buffer0[230400],10,8,48,(const q7_t*) weight14,bias14,shift14,multiplier14,6,128,-128,127,&buffer0[0],10,8,32,sbuf);
/* layer 16:CONV_2D */
convolve_1x1_s8(&buffer0[0],10,8,32,(const q7_t*) weight15,bias15,shift15,multiplier15,-128,-6,-128,127,&buffer0[230400],10,8,192,sbuf);
/* layer 17:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[230400],10,8,192,(const q7_t*) CHWweight16,offsetBias16,offsetRBias16,shift16,multiplier16,-128,128,-128,127,&buffer0[0],10,8,192,sbuf,-128);
/* layer 18:CONV_2D */
convolve_1x1_s8(&buffer0[230400],10,8,192,(const q7_t*) weight17,bias17,shift17,multiplier17,0,128,-128,127,&bufferstage4[0],10,8,32,sbuf);
/* layer 19:CONV_2D */
convolve_1x1_s8(&bufferstage4[0],10,8,32,(const q7_t*) weight18,bias18,shift18,multiplier18,-128,0,-128,127,&buffer0[235520],10,8,128,sbuf);
/* layer 20:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride2_inplace_CHW(&buffer0[235520],10,8,128,(const q7_t*) CHWweight19,offsetBias19,offsetRBias19,shift19,multiplier19,-128,128,-128,127,&buffer0[0],5,4,128,sbuf,-128);
/* layer 21:CONV_2D */
convolve_1x1_s8(&buffer0[235520],5,4,128,(const q7_t*) weight20,bias20,shift20,multiplier20,-4,128,-128,127,&buffer1[0],5,4,48,sbuf);
/* layer 22:CONV_2D */
convolve_1x1_s8_ch48(&buffer1[0],5,4,48,(const q7_t*) weight21,bias21,shift21,multiplier21,-128,4,-128,127,&buffer0[241920],5,4,192,sbuf);
/* layer 23:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[241920],5,4,192,(const q7_t*) CHWweight22,offsetBias22,offsetRBias22,shift22,multiplier22,-128,128,-128,127,&buffer0[0],5,4,192,sbuf,-128);
/* layer 24:CONV_2D */
convolve_1x1_s8(&buffer0[241920],5,4,192,(const q7_t*) weight23,bias23,shift23,multiplier23,2,128,-128,127,&buffer0[0],5,4,48,sbuf);
/* layer 25:ADD */
add_params.input_h = 4;add_params.input_w = 5;add_params.input_c = 48;
add_params.left_shift = 20;add_params.input1_offset = -2;add_params.input1_multiplier = 1781146880;add_params.input1_shift = -1;add_params.input2_offset = 4;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = -1;add_params.output_multiplier = 1911241504;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferstage5[0]);
/* layer 26:CONV_2D */
convolve_1x1_s8_ch48(&bufferstage5[0],5,4,48,(const q7_t*) weight24,bias24,shift24,multiplier24,-128,1,-128,127,&buffer0[241920],5,4,192,sbuf);
/* layer 27:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[241920],5,4,192,(const q7_t*) CHWweight25,offsetBias25,offsetRBias25,shift25,multiplier25,-128,128,-128,127,&buffer0[0],5,4,192,sbuf,-128);
/* layer 28:CONV_2D */
convolve_1x1_s8(&buffer0[241920],5,4,192,(const q7_t*) weight26,bias26,shift26,multiplier26,8,128,-128,127,&buffer0[0],5,4,48,sbuf);
/* layer 29:ADD */
add_params.input_h = 4;add_params.input_w = 5;add_params.input_c = 48;
add_params.left_shift = 20;add_params.input1_offset = -8;add_params.input1_multiplier = 1073741824;add_params.input1_shift = 0;add_params.input2_offset = 1;add_params.input2_multiplier = 2126750848;add_params.input2_shift = -2;add_params.output_offset = -4;add_params.output_multiplier = 1847336105;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &bufferstage5[0], &bufferdagop0_middle[0]);
/* layer 30:CONV_2D */
convolve_1x1_s8_ch48(&bufferdagop0_middle[0],5,4,48,(const q7_t*) weight27,bias27,shift27,multiplier27,-128,4,-128,127,&buffer0[245120],5,4,32,sbuf);
/* layer 31:UPSAMPLE */
upsample_byte(&buffer0[245120], 4, 5, 32, &bufferdagop0_output1[0], 2);
/* layer 32:ADD */
add_params.input_h = 8;add_params.input_w = 10;add_params.input_c = 32;
add_params.left_shift = 20;add_params.input1_offset = 128;add_params.input1_multiplier = 1469191040;add_params.input1_shift = -2;add_params.input2_offset = 0;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = -31;add_params.output_multiplier = 1853269638;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &bufferdagop0_output1[0], &bufferstage4[0], &buffer1[0]);
/* layer 33:CONV_2D */
convolve_1x1_s8(&buffer1[0],10,8,32,(const q7_t*) weight28,bias28,shift28,multiplier28,-128,31,-128,127,&buffer0[238080],10,8,96,sbuf);
/* layer 34:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[238080],10,8,96,(const q7_t*) CHWweight29,offsetBias29,offsetRBias29,shift29,multiplier29,-128,128,-128,127,&buffer0[0],10,8,96,sbuf,-128);
/* layer 35:CONV_2D */
convolve_1x1_s8(&buffer0[238080],10,8,96,(const q7_t*) weight30,bias30,shift30,multiplier30,-6,128,-128,127,&buffer0[0],10,8,32,sbuf);
/* layer 36:ADD */
add_params.input_h = 8;add_params.input_w = 10;add_params.input_c = 32;
add_params.left_shift = 20;add_params.input1_offset = 6;add_params.input1_multiplier = 1073741824;add_params.input1_shift = 0;add_params.input2_offset = 31;add_params.input2_multiplier = 1919632000;add_params.input2_shift = -2;add_params.output_offset = -20;add_params.output_multiplier = 1817618111;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferdagop1_middle[0]);
/* layer 37:CONV_2D */
convolve_1x1_s8(&bufferdagop1_middle[0],10,8,32,(const q7_t*) weight31,bias31,shift31,multiplier31,-128,20,-128,127,&buffer0[244480],10,8,16,sbuf);
/* layer 38:UPSAMPLE */
upsample_byte(&buffer0[244480], 8, 10, 16, &bufferdagop1_output1[0], 2);
/* layer 39:ADD */
add_params.input_h = 16;add_params.input_w = 20;add_params.input_c = 16;
add_params.left_shift = 20;add_params.input1_offset = 128;add_params.input1_multiplier = 1344574080;add_params.input1_shift = -2;add_params.input2_offset = -14;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = -25;add_params.output_multiplier = 1779888773;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &bufferdagop1_output1[0], &bufferstage2[0], &buffer1[0]);
/* layer 40:CONV_2D */
convolve_1x1_s8_ch16(&buffer1[0],20,16,16,(const q7_t*) weight32,bias32,shift32,multiplier32,-128,25,-128,127,&buffer0[235520],20,16,32,sbuf);
/* layer 41:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[235520],20,16,32,(const q7_t*) CHWweight33,offsetBias33,offsetRBias33,shift33,multiplier33,-128,128,-128,127,&buffer0[0],20,16,32,sbuf,-128);
/* layer 42:CONV_2D */
convolve_1x1_s8(&buffer0[235520],20,16,32,(const q7_t*) weight34,bias34,shift34,multiplier34,5,128,-128,127,&buffer0[0],20,16,16,sbuf);
/* layer 43:ADD */
add_params.input_h = 16;add_params.input_w = 20;add_params.input_c = 16;
add_params.left_shift = 20;add_params.input1_offset = -5;add_params.input1_multiplier = 1073741824;add_params.input1_shift = 0;add_params.input2_offset = 25;add_params.input2_multiplier = 1725031040;add_params.input2_shift = -2;add_params.output_offset = -5;add_params.output_multiplier = 2002381934;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferdagop2_middle[0]);
/* layer 44:CONV_2D */
convolve_1x1_s8_ch16(&bufferdagop2_middle[0],20,16,16,(const q7_t*) weight35,bias35,shift35,multiplier35,-128,5,-128,127,&buffer0[225280],20,16,64,sbuf);
/* layer 45:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[225280],20,16,64,(const q7_t*) CHWweight36,offsetBias36,offsetRBias36,shift36,multiplier36,-128,128,-128,127,&buffer0[0],20,16,64,sbuf,-128);
/* layer 46:CONV_2D */
convolve_1x1_s8(&buffer0[225280],20,16,64,(const q7_t*) weight37,bias37,shift37,multiplier37,-128,128,-128,127,&buffer0[0],20,16,48,sbuf);
/* layer 47:CONV_2D */
convolve_1x1_s8_oddch(&buffer0[0],20,16,48,(const q7_t*) weight38,bias38,shift38,multiplier38,110,128,-128,127,&bufferdagop2_output0[0],20,16,21,sbuf);
/* layer 48:MAX_POOL_2D */
max_pooling(&bufferdagop2_middle[0],16,20,16,2,2,8,10,-128,127,&buffer0[244480]);
/* layer 49:CONV_2D */
convolve_1x1_s8_ch16(&buffer0[244480],10,8,16,(const q7_t*) weight39,bias39,shift39,multiplier39,-128,5,-128,127,&bufferdagop2_output1[0],10,8,32,sbuf);
/* layer 50:ADD */
add_params.input_h = 8;add_params.input_w = 10;add_params.input_c = 32;
add_params.left_shift = 20;add_params.input1_offset = 128;add_params.input1_multiplier = 1918571008;add_params.input1_shift = -4;add_params.input2_offset = 20;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = -27;add_params.output_multiplier = 2065624548;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &bufferdagop2_output1[0], &bufferdagop1_middle[0], &buffer1[0]);
/* layer 51:CONV_2D */
convolve_1x1_s8(&buffer1[0],10,8,32,(const q7_t*) weight40,bias40,shift40,multiplier40,-128,27,-128,127,&buffer0[240640],10,8,64,sbuf);
/* layer 52:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[240640],10,8,64,(const q7_t*) CHWweight41,offsetBias41,offsetRBias41,shift41,multiplier41,-128,128,-128,127,&buffer0[0],10,8,64,sbuf,-128);
/* layer 53:CONV_2D */
convolve_1x1_s8(&buffer0[240640],10,8,64,(const q7_t*) weight42,bias42,shift42,multiplier42,-10,128,-128,127,&buffer0[0],10,8,32,sbuf);
/* layer 54:ADD */
add_params.input_h = 8;add_params.input_w = 10;add_params.input_c = 32;
add_params.left_shift = 20;add_params.input1_offset = 10;add_params.input1_multiplier = 1073741824;add_params.input1_shift = 0;add_params.input2_offset = 27;add_params.input2_multiplier = 1409551744;add_params.input2_shift = -1;add_params.output_offset = -24;add_params.output_multiplier = 1712299498;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferdagop3_middle[0]);
/* layer 55:CONV_2D */
convolve_1x1_s8(&bufferdagop3_middle[0],10,8,32,(const q7_t*) weight43,bias43,shift43,multiplier43,-128,24,-128,127,&buffer0[240640],10,8,64,sbuf);
/* layer 56:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[240640],10,8,64,(const q7_t*) CHWweight44,offsetBias44,offsetRBias44,shift44,multiplier44,-128,128,-128,127,&buffer0[0],10,8,64,sbuf,-128);
/* layer 57:CONV_2D */
convolve_1x1_s8(&buffer0[240640],10,8,64,(const q7_t*) weight45,bias45,shift45,multiplier45,-128,128,-128,127,&buffer0[0],10,8,96,sbuf);
/* layer 58:CONV_2D */
convolve_1x1_s8_oddch(&buffer0[0],10,8,96,(const q7_t*) weight46,bias46,shift46,multiplier46,44,128,-128,127,&bufferdagop3_output0[0],10,8,21,sbuf);
/* layer 59:MAX_POOL_2D */
max_pooling(&bufferdagop3_middle[0],8,10,32,2,2,4,5,-128,127,&buffer0[245120]);
/* layer 60:CONV_2D */
convolve_1x1_s8(&buffer0[245120],5,4,32,(const q7_t*) weight47,bias47,shift47,multiplier47,-128,24,-128,127,&bufferdagop3_output1[0],5,4,48,sbuf);
/* layer 61:ADD */
add_params.input_h = 4;add_params.input_w = 5;add_params.input_c = 48;
add_params.left_shift = 20;add_params.input1_offset = 128;add_params.input1_multiplier = 1219417856;add_params.input1_shift = -3;add_params.input2_offset = 4;add_params.input2_multiplier = 1073741824;add_params.input2_shift = 0;add_params.output_offset = -11;add_params.output_multiplier = 1088162234;add_params.output_shift = -18;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &bufferdagop3_output1[0], &bufferdagop0_middle[0], &buffer1[0]);
/* layer 62:CONV_2D */
convolve_1x1_s8_ch48(&buffer1[0],5,4,48,(const q7_t*) weight48,bias48,shift48,multiplier48,-128,11,-128,127,&buffer0[243840],5,4,96,sbuf);
/* layer 63:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[243840],5,4,96,(const q7_t*) CHWweight49,offsetBias49,offsetRBias49,shift49,multiplier49,-128,128,-128,127,&buffer0[0],5,4,96,sbuf,-128);
/* layer 64:CONV_2D */
convolve_1x1_s8(&buffer0[243840],5,4,96,(const q7_t*) weight50,bias50,shift50,multiplier50,0,128,-128,127,&buffer0[0],5,4,48,sbuf);
/* layer 65:ADD */
add_params.input_h = 4;add_params.input_w = 5;add_params.input_c = 48;
add_params.left_shift = 20;add_params.input1_offset = 0;add_params.input1_multiplier = 1073741824;add_params.input1_shift = 0;add_params.input2_offset = 11;add_params.input2_multiplier = 1960950912;add_params.input2_shift = -1;add_params.output_offset = -9;add_params.output_multiplier = 1638149896;add_params.output_shift = -19;add_params.quantized_activation_max = 127;add_params.quantized_activation_min = -128;
add(add_params.input_c * add_params.input_h * add_params.input_w, &add_params, &buffer0[0], &buffer1[0], &bufferdagop4_middle[0]);
/* layer 66:CONV_2D */
convolve_1x1_s8_ch48(&bufferdagop4_middle[0],5,4,48,(const q7_t*) weight51,bias51,shift51,multiplier51,-128,9,-128,127,&buffer0[241920],5,4,192,sbuf);
/* layer 67:DEPTHWISE_CONV_2D */
depthwise_kernel3x3_stride1_inplace_CHW(&buffer0[241920],5,4,192,(const q7_t*) CHWweight52,offsetBias52,offsetRBias52,shift52,multiplier52,-128,128,-128,127,&buffer0[0],5,4,192,sbuf,-128);
/* layer 68:CONV_2D */
convolve_1x1_s8(&buffer0[241920],5,4,192,(const q7_t*) weight53,bias53,shift53,multiplier53,-128,128,-128,127,&buffer0[0],5,4,144,sbuf);
/* layer 69:CONV_2D */
convolve_1x1_s8_oddch(&buffer0[0],5,4,144,(const q7_t*) weight54,bias54,shift54,multiplier54,14,128,-128,127,&bufferdagop4_output0[0],5,4,21,sbuf);
}
