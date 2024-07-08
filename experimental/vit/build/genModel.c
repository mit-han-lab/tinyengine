/* Automatically generated source file */
#include <float.h>
#include <tinyengine_function.h>
#include <tinyengine_function_fp.h>

#include "genNN.h"
#include "genModel.h"
#include "genInclude.h"
#include "my_function.h"

#include <stdint.h>

/* Variables used by all ops */
ADD_params add_params;
int i;
int8_t *int8ptr,*int8ptr2;
int32_t *int32ptr;
//float *fptr,*fptr2,*fptr3;
signed char *fptr,*fptr2,*fptr3;

signed char* getInput() {
    return &buffer0[226376];
}
signed char* getOutput() {
    return NNoutput;
}
void end2endinference(q7_t* img){
    invoke_inf(NULL);
}
void invoke_inf(){
/* layer 0:SHAPE */
shape(&buffer0[0], &buffer0[0], 4);/* layer 1:STRIDED_SLICE */
const uint16_t begin0[] = {0, 0, 0, 0};
const uint16_t end0[] = {1, 0, 0, 0};
const uint16_t strides0[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[0], 4, 1, 1, 1, begin0, end0, strides0, (int32_t*)&buffer0[16], 1, 1, 1, 1);
/* layer 2:PACK */
pack((int32_t*)&buffer0[16], (int32_t*)&buffer0[4975496], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4975496], 3, 0);/* layer 3:PACK */
pack((int32_t*)&buffer0[4975500], (int32_t*)&buffer0[16], (int32_t*)&buffer0[4975500], (int32_t*)&buffer0[4975500], 4, 0);/* layer 4:CONCATENATION */
concatenate((int32_t*)&buffer0[4975504], (int32_t*)&buffer0[4975504], (int32_t*)&buffer0[4975504], 0);
/* layer 5:CONV_2D */
conv2d_16x16_fpreq(&buffer0[0],224,224,3,(const q7_t*) weight0,bias0,scales0,-68,-127,-128,127,&buffer0[0],14,14,192,sbuf,127);
/* layer 6:TILE */
tile_3D_int8((signed char*)&buffer0[4975508],1,1,1,(signed char*)&buffer0[4975508],1,1,1);
/* layer 7:CONCATENATION */
concatenate((signed char*)&buffer0[4975512], (signed char*)&buffer0[4975512], (signed char*)&buffer0[188352], 1);
/* layer 8:ADD */
add_fpreq(1, &buffer0[188352],0.03561903163790703,-68,&buffer0[4975516],0.044805221259593964,-27,0.07606906443834305,-55,&buffer0[150528]);
/* layer 9:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[150528], (signed char*)&buffer0[188352], (signed char*)&buffer0[188352], 1, 197);
/* layer 10:AVERAGE_POOL_2D */
avg_pooling(&buffer0[226376],192,1,197,192,1,1,1,-128,127,&buffer0[226176]);
/* layer 11:ADD */
add_fpreq(197, &buffer0[226176],0.006562494672834873,-128,&buffer0[4802880],3.921568403342235e-09,0,0.006562494672834873,-128,&buffer0[226376]);
/* layer 12:RSQRT */
rsqrt((signed char*)&buffer0[226376], (signed char*)&buffer0[4954176], 1, 197);
/* layer 13:MUL */
fptr = (float*)(signed char*)&buffer0[226176];
fptr3 = (float*)(signed char*)&buffer0[264000];
fptr2 = (signed char*)&buffer0[226176];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 14:MUL */
mul_int8(37824,(signed char*)&buffer0[301824],(signed char*)&buffer0[264000],(signed char*)&buffer0[301824]);
/* layer 15:MUL */
fptr = (signed char*)&buffer0[264000];
fptr3 = (float*)(signed char*)&buffer0[339648];fptr2 = (signed char*)&buffer0[339648];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 16:SUB */
sub_int8(192,(signed char*)&buffer0[4959176], 0, 1, 0,(signed char*)&buffer0[4959176], 0, 1, 0,(signed char*)&buffer0[264000], 0, 1, 0, 0);
/* layer 17:ADD */
add_fpreq(37824, &buffer0[377472],0.053666651248931885,-65,&buffer0[264000],0.007968767546117306,7,0.05217108875513077,-73,&buffer0[415296]);
/* layer 18:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 19:STRIDED_SLICE */
const uint16_t begin1[] = {0, 0, 0, 0};
const uint16_t end1[] = {1, 0, 0, 0};
const uint16_t strides1[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[264000], 3, 1, 1, 1, begin1, end1, strides1, (int32_t*)&buffer0[264012], 1, 1, 1, 1);
/* layer 20:PACK */
pack((int32_t*)&buffer0[264012], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975520], 4, 0);/* layer 21:PACK */
pack((int32_t*)&buffer0[264012], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975524], 3, 0);/* layer 22:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 23:REDUCE_PROD */
int reduction_axes_0[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[264012], (int32_t*)(int32_t*)&buffer0[264020], 2, 1, reduction_axes_0, 4);
/* layer 24:CONCATENATION */
concatenate((int32_t*)&buffer0[264012], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975528], 0);
/* layer 25:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 26:REDUCE_PROD */
int reduction_axes_1[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[264012], (int32_t*)(int32_t*)&buffer0[264000], 1, 1, reduction_axes_1, 4);
/* layer 27:PACK */
pack((int32_t*)&buffer0[264020], (int32_t*)&buffer0[264000], (int32_t*)&buffer0[4975532], 2, 0);/* layer 28:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[264000], (signed char*)&buffer0[4975536], NULL, (signed char*)&buffer0[4975536], -73, 0, 14, 0.05217108875513077, 0.0033674186561256647, 0.036277152597904205, 0, 0, 0, 0, 0, 0, 0);/* layer 29:ADD */
add_fpreq(1, &buffer0[4959368],0.036277152597904205,14,&buffer0[4959368],0.00592309283092618,10,0.037194766104221344,15,&buffer0[4959368]);
/* layer 30:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[264000], (signed char*)&buffer0[4975540], NULL, (signed char*)&buffer0[4975540], -73, 0, 8, 0.05217108875513077, 0.0037488278467208147, 0.02718372270464897, 0, 0, 0, 0, 0, 0, 0);/* layer 31:ADD */
add_fpreq(1, &buffer0[4959560],0.02718372270464897,8,&buffer0[4959560],0.02005819045007229,-4,0.03273185342550278,-5,&buffer0[4959560]);
/* layer 32:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4959752], (signed char*)&buffer0[4959752], (signed char*)&buffer0[4959752], 1, 3, 1, 3, false, false);
/* layer 33:MUL */
fptr = (float*)(signed char*)&buffer0[4975544];
fptr3 = (float*)(signed char*)&buffer0[4975544];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 34:ADD */
add_fpreq(3, &buffer0[4975548],0.04994654655456543,25,&buffer0[4613776],3.921568403342235e-09,0,0.04994654655456543,25,&buffer0[264004]);
/* layer 35:SOFTMAX */
softmax((signed char*)&buffer0[264004], (signed char*)&buffer0[264008], 1, 3, 1.0);
/* layer 36:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[264000], (signed char*)&buffer0[4975552], NULL, (signed char*)&buffer0[4975552], -73, 0, -4, 0.05217108875513077, 0.004369327798485756, 0.057599183171987534, 0, 0, 0, 0, 0, 0, 0);/* layer 37:ADD */
add_fpreq(1, &buffer0[4959944],0.057599183171987534,-4,&buffer0[4959944],0.003290692111477256,26,0.0587209016084671,-6,&buffer0[4959944]);
/* layer 38:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[264008], (signed char*)&buffer0[4960136], (signed char*)&buffer0[642240], 1, 3, 1, 3, false, false);
/* layer 39:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 40:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 41:REDUCE_PROD */
int reduction_axes_2[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[264012], (int32_t*)(int32_t*)&buffer0[264020], 2, 1, reduction_axes_2, 4);
/* layer 42:CONCATENATION */
concatenate((int32_t*)&buffer0[264012], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975560], 0);
/* layer 43:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 44:REDUCE_PROD */
int reduction_axes_3[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[264012], (int32_t*)(int32_t*)&buffer0[264000], 1, 1, reduction_axes_3, 4);
/* layer 45:PACK */
pack((int32_t*)&buffer0[264020], (int32_t*)&buffer0[264000], (int32_t*)&buffer0[4975564], 2, 0);/* layer 46:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975568], (signed char*)&buffer0[4975568], NULL, (signed char*)&buffer0[4975568], 4, 0, 8, 0.0339677520096302, 0.0030441817361861467, 0.024411432445049286, 0, 0, 0, 0, 0, 0, 0);/* layer 47:ADD */
add_fpreq(1, &buffer0[4975572],0.024411432445049286,8,&buffer0[4975572],0.01588466577231884,-44,0.027592187747359276,8,&buffer0[415296]);
/* layer 48:ADD */
add_fpreq(192, &buffer0[415296],0.027592187747359276,8,&buffer0[150528],0.07606906443834305,-55,0.07331028580665588,-72,&buffer0[264000]);
/* layer 49:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[264000], (signed char*)&buffer0[150528], (signed char*)&buffer0[150528], 1, 197);
/* layer 50:AVERAGE_POOL_2D */
avg_pooling(&buffer0[642240],192,1,197,192,1,1,1,-128,127,&buffer0[415296]);
/* layer 51:ADD */
add_fpreq(197, &buffer0[415296],0.0058204177767038345,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0058204177767038345,-128,&buffer0[415496]);
/* layer 52:RSQRT */
rsqrt((signed char*)&buffer0[415496], (signed char*)&buffer0[4954376], 1, 197);
/* layer 53:MUL */
fptr = (float*)(signed char*)&buffer0[415296];
fptr3 = (float*)(signed char*)&buffer0[453120];
fptr2 = (signed char*)&buffer0[415296];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 54:MUL */
mul_int8(37824,(signed char*)&buffer0[490944],(signed char*)&buffer0[453120],(signed char*)&buffer0[490944]);
/* layer 55:MUL */
fptr = (signed char*)&buffer0[453120];
fptr3 = (float*)(signed char*)&buffer0[528768];fptr2 = (signed char*)&buffer0[528768];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 56:SUB */
sub_int8(192,(signed char*)&buffer0[4960328], 0, 1, 0,(signed char*)&buffer0[4960328], 0, 1, 0,(signed char*)&buffer0[453120], 0, 1, 0, 0);
/* layer 57:ADD */
add_fpreq(37824, &buffer0[566592],0.06944842636585236,-52,&buffer0[453120],0.012530267238616943,6,0.06402282416820526,-48,&buffer0[604416]);
/* layer 58:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 59:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 60:REDUCE_PROD */
int reduction_axes_4[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[453132], (int32_t*)(int32_t*)&buffer0[453140], 2, 1, reduction_axes_4, 4);
/* layer 61:CONCATENATION */
concatenate((int32_t*)&buffer0[453132], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4975576], 0);
/* layer 62:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 63:REDUCE_PROD */
int reduction_axes_5[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[453132], (int32_t*)(int32_t*)&buffer0[453120], 1, 1, reduction_axes_5, 4);
/* layer 64:PACK */
pack((int32_t*)&buffer0[453140], (int32_t*)&buffer0[453120], (int32_t*)&buffer0[4975580], 2, 0);/* layer 65:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975584], (signed char*)&buffer0[4975584], NULL, (signed char*)&buffer0[4975584], -48, 0, 17, 0.06402282416820526, 0.004406847059726715, 0.02999480813741684, 0, 0, 0, 0, 0, 0, 0);/* layer 66:ADD */
add_fpreq(1, &buffer0[4975588],0.02999480813741684,17,&buffer0[4975588],0.01081228256225586,66,0.030785299837589264,54,&buffer0[453120]);
/* layer 67:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[453120], (signed char*)&buffer0[453888], 1, 1);
/* layer 68:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 69:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 70:REDUCE_PROD */
int reduction_axes_6[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[453132], (int32_t*)(int32_t*)&buffer0[453140], 2, 1, reduction_axes_6, 4);
/* layer 71:CONCATENATION */
concatenate((int32_t*)&buffer0[453132], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975592], 0);
/* layer 72:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 73:REDUCE_PROD */
int reduction_axes_7[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[453132], (int32_t*)(int32_t*)&buffer0[453120], 1, 1, reduction_axes_7, 4);
/* layer 74:PACK */
pack((int32_t*)&buffer0[453140], (int32_t*)&buffer0[453120], (int32_t*)&buffer0[4975596], 2, 0);/* layer 75:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975600], (signed char*)&buffer0[4975600], NULL, (signed char*)&buffer0[4975600], -110, 0, 31, 0.009326877072453499, 0.003403405426070094, 0.016177864745259285, 0, 0, 0, 0, 0, 0, 0);/* layer 76:ADD */
add_fpreq(1, &buffer0[4975604],0.016177864745259285,31,&buffer0[4975604],0.019764402881264687,-29,0.018786253407597542,15,&buffer0[604416]);
/* layer 77:ADD */
add_fpreq(192, &buffer0[604416],0.018786253407597542,15,&buffer0[264000],0.07331028580665588,-72,0.07122217863798141,-85,&buffer0[453120]);
/* layer 78:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[453120], (signed char*)&buffer0[264000], (signed char*)&buffer0[264000], 1, 197);
/* layer 79:AVERAGE_POOL_2D */
avg_pooling(&buffer0[642240],192,1,197,192,1,1,1,-128,127,&buffer0[604416]);
/* layer 80:ADD */
add_fpreq(197, &buffer0[604416],0.005648959428071976,-128,&buffer0[4802880],3.921568403342235e-09,0,0.005648959428071976,-128,&buffer0[604616]);
/* layer 81:RSQRT */
rsqrt((signed char*)&buffer0[604616], (signed char*)&buffer0[4954576], 1, 197);
/* layer 82:MUL */
fptr = (float*)(signed char*)&buffer0[604416];
fptr3 = (float*)(signed char*)&buffer0[642240];
fptr2 = (signed char*)&buffer0[604416];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 83:MUL */
mul_int8(37824,(signed char*)&buffer0[680064],(signed char*)&buffer0[642240],(signed char*)&buffer0[680064]);
/* layer 84:MUL */
fptr = (signed char*)&buffer0[642240];
fptr3 = (float*)(signed char*)&buffer0[717888];fptr2 = (signed char*)&buffer0[717888];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 85:SUB */
sub_int8(192,(signed char*)&buffer0[4960520], 0, 1, 0,(signed char*)&buffer0[4960520], 0, 1, 0,(signed char*)&buffer0[642240], 0, 1, 0, 0);
/* layer 86:ADD */
add_fpreq(37824, &buffer0[755712],0.05855756253004074,-24,&buffer0[642240],0.01574414223432541,39,0.04724297672510147,-7,&buffer0[793536]);
/* layer 87:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 88:STRIDED_SLICE */
const uint16_t begin2[] = {0, 0, 0, 0};
const uint16_t end2[] = {1, 0, 0, 0};
const uint16_t strides2[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[642240], 3, 1, 1, 1, begin2, end2, strides2, (int32_t*)&buffer0[642252], 1, 1, 1, 1);
/* layer 89:PACK */
pack((int32_t*)&buffer0[642252], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975608], 4, 0);/* layer 90:PACK */
pack((int32_t*)&buffer0[642252], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975612], 3, 0);/* layer 91:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 92:REDUCE_PROD */
int reduction_axes_8[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[642252], (int32_t*)(int32_t*)&buffer0[642260], 2, 1, reduction_axes_8, 4);
/* layer 93:CONCATENATION */
concatenate((int32_t*)&buffer0[642252], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975616], 0);
/* layer 94:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 95:REDUCE_PROD */
int reduction_axes_9[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[642252], (int32_t*)(int32_t*)&buffer0[642240], 1, 1, reduction_axes_9, 4);
/* layer 96:PACK */
pack((int32_t*)&buffer0[642260], (int32_t*)&buffer0[642240], (int32_t*)&buffer0[4975620], 2, 0);/* layer 97:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[642240], (signed char*)&buffer0[4975624], NULL, (signed char*)&buffer0[4975624], -7, 0, 6, 0.04724297672510147, 0.005337569862604141, 0.04215116426348686, 0, 0, 0, 0, 0, 0, 0);/* layer 98:ADD */
add_fpreq(1, &buffer0[4960712],0.04215116426348686,6,&buffer0[4960712],0.006209865212440491,-8,0.042530905455350876,7,&buffer0[4960712]);
/* layer 99:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[642240], (signed char*)&buffer0[4975628], NULL, (signed char*)&buffer0[4975628], -7, 0, 11, 0.04724297672510147, 0.00397686380892992, 0.04255728796124458, 0, 0, 0, 0, 0, 0, 0);/* layer 100:ADD */
add_fpreq(1, &buffer0[4960904],0.04255728796124458,11,&buffer0[4960904],0.018467668443918228,1,0.04548297077417374,6,&buffer0[4960904]);
/* layer 101:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4961096], (signed char*)&buffer0[4961096], (signed char*)&buffer0[4961096], 1, 3, 1, 3, false, false);
/* layer 102:MUL */
fptr = (float*)(signed char*)&buffer0[4975632];
fptr3 = (float*)(signed char*)&buffer0[4975632];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 103:ADD */
add_fpreq(3, &buffer0[4975636],0.21282459795475006,16,&buffer0[4613776],3.921568403342235e-09,0,0.21282459795475006,16,&buffer0[642244]);
/* layer 104:SOFTMAX */
softmax((signed char*)&buffer0[642244], (signed char*)&buffer0[642248], 1, 3, 1.0);
/* layer 105:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[642240], (signed char*)&buffer0[4975640], NULL, (signed char*)&buffer0[4975640], -7, 0, 23, 0.04724297672510147, 0.00231668958440423, 0.023010825738310814, 0, 0, 0, 0, 0, 0, 0);/* layer 106:ADD */
add_fpreq(1, &buffer0[4961288],0.023010825738310814,23,&buffer0[4961288],0.002069819485768676,-14,0.024170396849513054,19,&buffer0[4961288]);
/* layer 107:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[642248], (signed char*)&buffer0[4961480], (signed char*)&buffer0[1020480], 1, 3, 1, 3, false, false);
/* layer 108:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 109:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 110:REDUCE_PROD */
int reduction_axes_10[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[642252], (int32_t*)(int32_t*)&buffer0[642260], 2, 1, reduction_axes_10, 4);
/* layer 111:CONCATENATION */
concatenate((int32_t*)&buffer0[642252], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975648], 0);
/* layer 112:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 113:REDUCE_PROD */
int reduction_axes_11[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[642252], (int32_t*)(int32_t*)&buffer0[642240], 1, 1, reduction_axes_11, 4);
/* layer 114:PACK */
pack((int32_t*)&buffer0[642260], (int32_t*)&buffer0[642240], (int32_t*)&buffer0[4975652], 2, 0);/* layer 115:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975656], (signed char*)&buffer0[4975656], NULL, (signed char*)&buffer0[4975656], 19, 0, 1, 0.02158612571656704, 0.0024947619531303644, 0.012468787841498852, 0, 0, 0, 0, 0, 0, 0);/* layer 116:ADD */
add_fpreq(1, &buffer0[4975660],0.012468787841498852,1,&buffer0[4975660],0.008915960788726807,4,0.013032671995460987,4,&buffer0[793536]);
/* layer 117:ADD */
add_fpreq(192, &buffer0[793536],0.013032671995460987,4,&buffer0[453120],0.07122217863798141,-85,0.06616988033056259,-84,&buffer0[642240]);
/* layer 118:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[642240], (signed char*)&buffer0[453120], (signed char*)&buffer0[453120], 1, 197);
/* layer 119:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1020480],192,1,197,192,1,1,1,-128,127,&buffer0[793536]);
/* layer 120:ADD */
add_fpreq(197, &buffer0[793536],0.004706353880465031,-128,&buffer0[4802880],3.921568403342235e-09,0,0.004706353880465031,-128,&buffer0[793736]);
/* layer 121:RSQRT */
rsqrt((signed char*)&buffer0[793736], (signed char*)&buffer0[4954776], 1, 197);
/* layer 122:MUL */
fptr = (float*)(signed char*)&buffer0[793536];
fptr3 = (float*)(signed char*)&buffer0[831360];
fptr2 = (signed char*)&buffer0[793536];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 123:MUL */
mul_int8(37824,(signed char*)&buffer0[869184],(signed char*)&buffer0[831360],(signed char*)&buffer0[869184]);
/* layer 124:MUL */
fptr = (signed char*)&buffer0[831360];
fptr3 = (float*)(signed char*)&buffer0[907008];fptr2 = (signed char*)&buffer0[907008];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 125:SUB */
sub_int8(192,(signed char*)&buffer0[4961672], 0, 1, 0,(signed char*)&buffer0[4961672], 0, 1, 0,(signed char*)&buffer0[831360], 0, 1, 0, 0);
/* layer 126:ADD */
add_fpreq(37824, &buffer0[944832],0.04719182476401329,-29,&buffer0[831360],0.010350772179663181,16,0.04292942211031914,-12,&buffer0[982656]);
/* layer 127:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 128:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 129:REDUCE_PROD */
int reduction_axes_12[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[831372], (int32_t*)(int32_t*)&buffer0[831380], 2, 1, reduction_axes_12, 4);
/* layer 130:CONCATENATION */
concatenate((int32_t*)&buffer0[831372], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4975664], 0);
/* layer 131:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 132:REDUCE_PROD */
int reduction_axes_13[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[831372], (int32_t*)(int32_t*)&buffer0[831360], 1, 1, reduction_axes_13, 4);
/* layer 133:PACK */
pack((int32_t*)&buffer0[831380], (int32_t*)&buffer0[831360], (int32_t*)&buffer0[4975668], 2, 0);/* layer 134:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975672], (signed char*)&buffer0[4975672], NULL, (signed char*)&buffer0[4975672], -12, 0, 28, 0.04292942211031914, 0.003758038394153118, 0.027618441730737686, 0, 0, 0, 0, 0, 0, 0);/* layer 135:ADD */
add_fpreq(1, &buffer0[4975676],0.027618441730737686,28,&buffer0[4975676],0.011366539634764194,99,0.024819299578666687,73,&buffer0[831360]);
/* layer 136:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[831360], (signed char*)&buffer0[832128], 1, 1);
/* layer 137:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 138:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 139:REDUCE_PROD */
int reduction_axes_14[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[831372], (int32_t*)(int32_t*)&buffer0[831380], 2, 1, reduction_axes_14, 4);
/* layer 140:CONCATENATION */
concatenate((int32_t*)&buffer0[831372], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975680], 0);
/* layer 141:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 142:REDUCE_PROD */
int reduction_axes_15[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[831372], (int32_t*)(int32_t*)&buffer0[831360], 1, 1, reduction_axes_15, 4);
/* layer 143:PACK */
pack((int32_t*)&buffer0[831380], (int32_t*)&buffer0[831360], (int32_t*)&buffer0[4975684], 2, 0);/* layer 144:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975688], (signed char*)&buffer0[4975688], NULL, (signed char*)&buffer0[4975688], -97, 0, 28, 0.005439549684524536, 0.003336171619594097, 0.012186631560325623, 0, 0, 0, 0, 0, 0, 0);/* layer 145:ADD */
add_fpreq(1, &buffer0[4975692],0.012186631560325623,28,&buffer0[4975692],0.005987726151943207,20,0.012142813764512539,14,&buffer0[982656]);
/* layer 146:ADD */
add_fpreq(192, &buffer0[982656],0.012142813764512539,14,&buffer0[642240],0.06616988033056259,-84,0.06143566220998764,-79,&buffer0[831360]);
/* layer 147:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[831360], (signed char*)&buffer0[642240], (signed char*)&buffer0[642240], 1, 197);
/* layer 148:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1020480],192,1,197,192,1,1,1,-128,127,&buffer0[982656]);
/* layer 149:ADD */
add_fpreq(197, &buffer0[982656],0.0037605157122015953,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0037605157122015953,-128,&buffer0[982856]);
/* layer 150:RSQRT */
rsqrt((signed char*)&buffer0[982856], (signed char*)&buffer0[4954976], 1, 197);
/* layer 151:MUL */
fptr = (float*)(signed char*)&buffer0[982656];
fptr3 = (float*)(signed char*)&buffer0[1020480];
fptr2 = (signed char*)&buffer0[982656];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 152:MUL */
mul_int8(37824,(signed char*)&buffer0[1058304],(signed char*)&buffer0[1020480],(signed char*)&buffer0[1058304]);
/* layer 153:MUL */
fptr = (signed char*)&buffer0[1020480];
fptr3 = (float*)(signed char*)&buffer0[1096128];fptr2 = (signed char*)&buffer0[1096128];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 154:SUB */
sub_int8(192,(signed char*)&buffer0[4961864], 0, 1, 0,(signed char*)&buffer0[4961864], 0, 1, 0,(signed char*)&buffer0[1020480], 0, 1, 0, 0);
/* layer 155:ADD */
add_fpreq(37824, &buffer0[1133952],0.0655011460185051,-14,&buffer0[1020480],0.015844622626900673,49,0.06442344188690186,-9,&buffer0[1171776]);
/* layer 156:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 157:STRIDED_SLICE */
const uint16_t begin3[] = {0, 0, 0, 0};
const uint16_t end3[] = {1, 0, 0, 0};
const uint16_t strides3[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[1020480], 3, 1, 1, 1, begin3, end3, strides3, (int32_t*)&buffer0[1020492], 1, 1, 1, 1);
/* layer 158:PACK */
pack((int32_t*)&buffer0[1020492], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975696], 4, 0);/* layer 159:PACK */
pack((int32_t*)&buffer0[1020492], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975700], 3, 0);/* layer 160:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 161:REDUCE_PROD */
int reduction_axes_16[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1020492], (int32_t*)(int32_t*)&buffer0[1020500], 2, 1, reduction_axes_16, 4);
/* layer 162:CONCATENATION */
concatenate((int32_t*)&buffer0[1020492], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975704], 0);
/* layer 163:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 164:REDUCE_PROD */
int reduction_axes_17[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1020492], (int32_t*)(int32_t*)&buffer0[1020480], 1, 1, reduction_axes_17, 4);
/* layer 165:PACK */
pack((int32_t*)&buffer0[1020500], (int32_t*)&buffer0[1020480], (int32_t*)&buffer0[4975708], 2, 0);/* layer 166:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1020480], (signed char*)&buffer0[4975712], NULL, (signed char*)&buffer0[4975712], -9, 0, 6, 0.06442344188690186, 0.0035121419932693243, 0.047510720789432526, 0, 0, 0, 0, 0, 0, 0);/* layer 167:ADD */
add_fpreq(1, &buffer0[4962056],0.047510720789432526,6,&buffer0[4962056],0.007155312690883875,26,0.050903066992759705,11,&buffer0[4962056]);
/* layer 168:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1020480], (signed char*)&buffer0[4975716], NULL, (signed char*)&buffer0[4975716], -9, 0, 1, 0.06442344188690186, 0.0027636459562927485, 0.04301666095852852, 0, 0, 0, 0, 0, 0, 0);/* layer 169:ADD */
add_fpreq(1, &buffer0[4962248],0.04301666095852852,1,&buffer0[4962248],0.01638728193938732,-26,0.04851795360445976,6,&buffer0[4962248]);
/* layer 170:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4962440], (signed char*)&buffer0[4962440], (signed char*)&buffer0[4962440], 1, 3, 1, 3, false, false);
/* layer 171:MUL */
fptr = (float*)(signed char*)&buffer0[4975720];
fptr3 = (float*)(signed char*)&buffer0[4975720];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 172:ADD */
add_fpreq(3, &buffer0[4975724],0.21013617515563965,27,&buffer0[4613776],3.921568403342235e-09,0,0.21013617515563965,27,&buffer0[1020484]);
/* layer 173:SOFTMAX */
softmax((signed char*)&buffer0[1020484], (signed char*)&buffer0[1020488], 1, 3, 1.0);
/* layer 174:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1020480], (signed char*)&buffer0[4975728], NULL, (signed char*)&buffer0[4975728], -9, 0, -10, 0.06442344188690186, 0.0020338473841547966, 0.017086708918213844, 0, 0, 0, 0, 0, 0, 0);/* layer 175:ADD */
add_fpreq(1, &buffer0[4962632],0.017086708918213844,-10,&buffer0[4962632],0.0009652384323999286,21,0.016477186232805252,-6,&buffer0[4962632]);
/* layer 176:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[1020488], (signed char*)&buffer0[4962824], (signed char*)&buffer0[1398720], 1, 3, 1, 3, false, false);
/* layer 177:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 178:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 179:REDUCE_PROD */
int reduction_axes_18[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1020492], (int32_t*)(int32_t*)&buffer0[1020500], 2, 1, reduction_axes_18, 4);
/* layer 180:CONCATENATION */
concatenate((int32_t*)&buffer0[1020492], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975736], 0);
/* layer 181:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 182:REDUCE_PROD */
int reduction_axes_19[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1020492], (int32_t*)(int32_t*)&buffer0[1020480], 1, 1, reduction_axes_19, 4);
/* layer 183:PACK */
pack((int32_t*)&buffer0[1020500], (int32_t*)&buffer0[1020480], (int32_t*)&buffer0[4975740], 2, 0);/* layer 184:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975744], (signed char*)&buffer0[4975744], NULL, (signed char*)&buffer0[4975744], -24, 0, -9, 0.013385390862822533, 0.0017181998118758202, 0.009926880709826946, 0, 0, 0, 0, 0, 0, 0);/* layer 185:ADD */
add_fpreq(1, &buffer0[4975748],0.009926880709826946,-9,&buffer0[4975748],0.005744992755353451,-5,0.010658321902155876,14,&buffer0[1171776]);
/* layer 186:ADD */
add_fpreq(192, &buffer0[1171776],0.010658321902155876,14,&buffer0[831360],0.06143566220998764,-79,0.06001211702823639,-92,&buffer0[1020480]);
/* layer 187:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1020480], (signed char*)&buffer0[831360], (signed char*)&buffer0[831360], 1, 197);
/* layer 188:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1398720],192,1,197,192,1,1,1,-128,127,&buffer0[1171776]);
/* layer 189:ADD */
add_fpreq(197, &buffer0[1171776],0.004008620046079159,-128,&buffer0[4802880],3.921568403342235e-09,0,0.004008620046079159,-128,&buffer0[1171976]);
/* layer 190:RSQRT */
rsqrt((signed char*)&buffer0[1171976], (signed char*)&buffer0[4955176], 1, 197);
/* layer 191:MUL */
fptr = (float*)(signed char*)&buffer0[1171776];
fptr3 = (float*)(signed char*)&buffer0[1209600];
fptr2 = (signed char*)&buffer0[1171776];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 192:MUL */
mul_int8(37824,(signed char*)&buffer0[1247424],(signed char*)&buffer0[1209600],(signed char*)&buffer0[1247424]);
/* layer 193:MUL */
fptr = (signed char*)&buffer0[1209600];
fptr3 = (float*)(signed char*)&buffer0[1285248];fptr2 = (signed char*)&buffer0[1285248];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 194:SUB */
sub_int8(192,(signed char*)&buffer0[4963016], 0, 1, 0,(signed char*)&buffer0[4963016], 0, 1, 0,(signed char*)&buffer0[1209600], 0, 1, 0, 0);
/* layer 195:ADD */
add_fpreq(37824, &buffer0[1323072],0.04849676787853241,-13,&buffer0[1209600],0.009378289803862572,42,0.041165854781866074,-1,&buffer0[1360896]);
/* layer 196:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 197:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 198:REDUCE_PROD */
int reduction_axes_20[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1209612], (int32_t*)(int32_t*)&buffer0[1209620], 2, 1, reduction_axes_20, 4);
/* layer 199:CONCATENATION */
concatenate((int32_t*)&buffer0[1209612], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4975752], 0);
/* layer 200:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 201:REDUCE_PROD */
int reduction_axes_21[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1209612], (int32_t*)(int32_t*)&buffer0[1209600], 1, 1, reduction_axes_21, 4);
/* layer 202:PACK */
pack((int32_t*)&buffer0[1209620], (int32_t*)&buffer0[1209600], (int32_t*)&buffer0[4975756], 2, 0);/* layer 203:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975760], (signed char*)&buffer0[4975760], NULL, (signed char*)&buffer0[4975760], -1, 0, 3, 0.041165854781866074, 0.0036591808311641216, 0.027139704674482346, 0, 0, 0, 0, 0, 0, 0);/* layer 204:ADD */
add_fpreq(1, &buffer0[4975764],0.027139704674482346,3,&buffer0[4975764],0.01051859837025404,86,0.026722365990281105,54,&buffer0[1209600]);
/* layer 205:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[1209600], (signed char*)&buffer0[1210368], 1, 1);
/* layer 206:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 207:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 208:REDUCE_PROD */
int reduction_axes_22[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1209612], (int32_t*)(int32_t*)&buffer0[1209620], 2, 1, reduction_axes_22, 4);
/* layer 209:CONCATENATION */
concatenate((int32_t*)&buffer0[1209612], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975768], 0);
/* layer 210:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 211:REDUCE_PROD */
int reduction_axes_23[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1209612], (int32_t*)(int32_t*)&buffer0[1209600], 1, 1, reduction_axes_23, 4);
/* layer 212:PACK */
pack((int32_t*)&buffer0[1209620], (int32_t*)&buffer0[1209600], (int32_t*)&buffer0[4975772], 2, 0);/* layer 213:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975776], (signed char*)&buffer0[4975776], NULL, (signed char*)&buffer0[4975776], -107, 0, 2, 0.008177894167602062, 0.004228953737765551, 0.008324963971972466, 0, 0, 0, 0, 0, 0, 0);/* layer 214:ADD */
add_fpreq(1, &buffer0[4975780],0.008324963971972466,2,&buffer0[4975780],0.004164565820246935,27,0.010033843107521534,23,&buffer0[1360896]);
/* layer 215:ADD */
add_fpreq(192, &buffer0[1360896],0.010033843107521534,23,&buffer0[1020480],0.06001211702823639,-92,0.060634762048721313,-71,&buffer0[1209600]);
/* layer 216:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1209600], (signed char*)&buffer0[1020480], (signed char*)&buffer0[1020480], 1, 197);
/* layer 217:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1398720],192,1,197,192,1,1,1,-128,127,&buffer0[1360896]);
/* layer 218:ADD */
add_fpreq(197, &buffer0[1360896],0.0032857402693480253,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0032857402693480253,-128,&buffer0[1361096]);
/* layer 219:RSQRT */
rsqrt((signed char*)&buffer0[1361096], (signed char*)&buffer0[4955376], 1, 197);
/* layer 220:MUL */
fptr = (float*)(signed char*)&buffer0[1360896];
fptr3 = (float*)(signed char*)&buffer0[1398720];
fptr2 = (signed char*)&buffer0[1360896];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 221:MUL */
mul_int8(37824,(signed char*)&buffer0[1436544],(signed char*)&buffer0[1398720],(signed char*)&buffer0[1436544]);
/* layer 222:MUL */
fptr = (signed char*)&buffer0[1398720];
fptr3 = (float*)(signed char*)&buffer0[1474368];fptr2 = (signed char*)&buffer0[1474368];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 223:SUB */
sub_int8(192,(signed char*)&buffer0[4963208], 0, 1, 0,(signed char*)&buffer0[4963208], 0, 1, 0,(signed char*)&buffer0[1398720], 0, 1, 0, 0);
/* layer 224:ADD */
add_fpreq(37824, &buffer0[1512192],0.05836442485451698,-38,&buffer0[1398720],0.014707772992551327,60,0.04716828465461731,-21,&buffer0[1550016]);
/* layer 225:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 226:STRIDED_SLICE */
const uint16_t begin4[] = {0, 0, 0, 0};
const uint16_t end4[] = {1, 0, 0, 0};
const uint16_t strides4[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[1398720], 3, 1, 1, 1, begin4, end4, strides4, (int32_t*)&buffer0[1398732], 1, 1, 1, 1);
/* layer 227:PACK */
pack((int32_t*)&buffer0[1398732], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975784], 4, 0);/* layer 228:PACK */
pack((int32_t*)&buffer0[1398732], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975788], 3, 0);/* layer 229:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 230:REDUCE_PROD */
int reduction_axes_24[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1398732], (int32_t*)(int32_t*)&buffer0[1398740], 2, 1, reduction_axes_24, 4);
/* layer 231:CONCATENATION */
concatenate((int32_t*)&buffer0[1398732], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975792], 0);
/* layer 232:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 233:REDUCE_PROD */
int reduction_axes_25[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1398732], (int32_t*)(int32_t*)&buffer0[1398720], 1, 1, reduction_axes_25, 4);
/* layer 234:PACK */
pack((int32_t*)&buffer0[1398740], (int32_t*)&buffer0[1398720], (int32_t*)&buffer0[4975796], 2, 0);/* layer 235:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1398720], (signed char*)&buffer0[4975800], NULL, (signed char*)&buffer0[4975800], -21, 0, -16, 0.04716828465461731, 0.0032991173211485147, 0.03479106351733208, 0, 0, 0, 0, 0, 0, 0);/* layer 236:ADD */
add_fpreq(1, &buffer0[4963400],0.03479106351733208,-16,&buffer0[4963400],0.006676297634840012,-6,0.03459205478429794,-11,&buffer0[4963400]);
/* layer 237:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1398720], (signed char*)&buffer0[4975804], NULL, (signed char*)&buffer0[4975804], -21, 0, 6, 0.04716828465461731, 0.0025572304148226976, 0.029045458883047104, 0, 0, 0, 0, 0, 0, 0);/* layer 238:ADD */
add_fpreq(1, &buffer0[4963592],0.029045458883047104,6,&buffer0[4963592],0.015046791173517704,12,0.03224066644906998,12,&buffer0[4963592]);
/* layer 239:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4963784], (signed char*)&buffer0[4963784], (signed char*)&buffer0[4963784], 1, 3, 1, 3, false, false);
/* layer 240:MUL */
fptr = (float*)(signed char*)&buffer0[4975808];
fptr3 = (float*)(signed char*)&buffer0[4975808];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 241:ADD */
add_fpreq(3, &buffer0[4975812],0.0752592384815216,-27,&buffer0[4613776],3.921568403342235e-09,0,0.0752592384815216,-27,&buffer0[1398724]);
/* layer 242:SOFTMAX */
softmax((signed char*)&buffer0[1398724], (signed char*)&buffer0[1398728], 1, 3, 1.0);
/* layer 243:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1398720], (signed char*)&buffer0[4975816], NULL, (signed char*)&buffer0[4975816], -21, 0, -18, 0.04716828465461731, 0.0020360106136649847, 0.022917844355106354, 0, 0, 0, 0, 0, 0, 0);/* layer 244:ADD */
add_fpreq(1, &buffer0[4963976],0.022917844355106354,-18,&buffer0[4963976],0.0008649601368233562,-8,0.02262859418988228,-19,&buffer0[4963976]);
/* layer 245:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[1398728], (signed char*)&buffer0[4964168], (signed char*)&buffer0[1776960], 1, 3, 1, 3, false, false);
/* layer 246:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 247:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 248:REDUCE_PROD */
int reduction_axes_26[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1398732], (int32_t*)(int32_t*)&buffer0[1398740], 2, 1, reduction_axes_26, 4);
/* layer 249:CONCATENATION */
concatenate((int32_t*)&buffer0[1398732], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975824], 0);
/* layer 250:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 251:REDUCE_PROD */
int reduction_axes_27[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1398732], (int32_t*)(int32_t*)&buffer0[1398720], 1, 1, reduction_axes_27, 4);
/* layer 252:PACK */
pack((int32_t*)&buffer0[1398740], (int32_t*)&buffer0[1398720], (int32_t*)&buffer0[4975828], 2, 0);/* layer 253:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975832], (signed char*)&buffer0[4975832], NULL, (signed char*)&buffer0[4975832], -12, 0, -3, 0.011639120057225227, 0.0030611292459070683, 0.007845945656299591, 0, 0, 0, 0, 0, 0, 0);/* layer 254:ADD */
add_fpreq(1, &buffer0[4975836],0.007845945656299591,-3,&buffer0[4975836],0.00441111670807004,3,0.007795114070177078,-1,&buffer0[1550016]);
/* layer 255:ADD */
add_fpreq(192, &buffer0[1550016],0.007795114070177078,-1,&buffer0[1209600],0.060634762048721313,-71,0.06240391358733177,-75,&buffer0[1398720]);
/* layer 256:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1398720], (signed char*)&buffer0[1209600], (signed char*)&buffer0[1209600], 1, 197);
/* layer 257:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1776960],192,1,197,192,1,1,1,-128,127,&buffer0[1550016]);
/* layer 258:ADD */
add_fpreq(197, &buffer0[1550016],0.0035723792389035225,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0035723792389035225,-128,&buffer0[1550216]);
/* layer 259:RSQRT */
rsqrt((signed char*)&buffer0[1550216], (signed char*)&buffer0[4955576], 1, 197);
/* layer 260:MUL */
fptr = (float*)(signed char*)&buffer0[1550016];
fptr3 = (float*)(signed char*)&buffer0[1587840];
fptr2 = (signed char*)&buffer0[1550016];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 261:MUL */
mul_int8(37824,(signed char*)&buffer0[1625664],(signed char*)&buffer0[1587840],(signed char*)&buffer0[1625664]);
/* layer 262:MUL */
fptr = (signed char*)&buffer0[1587840];
fptr3 = (float*)(signed char*)&buffer0[1663488];fptr2 = (signed char*)&buffer0[1663488];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 263:SUB */
sub_int8(192,(signed char*)&buffer0[4964360], 0, 1, 0,(signed char*)&buffer0[4964360], 0, 1, 0,(signed char*)&buffer0[1587840], 0, 1, 0, 0);
/* layer 264:ADD */
add_fpreq(37824, &buffer0[1701312],0.07072785496711731,-1,&buffer0[1587840],0.009670726023614407,51,0.06360391527414322,9,&buffer0[1739136]);
/* layer 265:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 266:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 267:REDUCE_PROD */
int reduction_axes_28[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1587852], (int32_t*)(int32_t*)&buffer0[1587860], 2, 1, reduction_axes_28, 4);
/* layer 268:CONCATENATION */
concatenate((int32_t*)&buffer0[1587852], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4975840], 0);
/* layer 269:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 270:REDUCE_PROD */
int reduction_axes_29[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1587852], (int32_t*)(int32_t*)&buffer0[1587840], 1, 1, reduction_axes_29, 4);
/* layer 271:PACK */
pack((int32_t*)&buffer0[1587860], (int32_t*)&buffer0[1587840], (int32_t*)&buffer0[4975844], 2, 0);/* layer 272:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975848], (signed char*)&buffer0[4975848], NULL, (signed char*)&buffer0[4975848], 9, 0, 3, 0.06360391527414322, 0.002772318432107568, 0.028547095134854317, 0, 0, 0, 0, 0, 0, 0);/* layer 273:ADD */
add_fpreq(1, &buffer0[4975852],0.028547095134854317,3,&buffer0[4975852],0.008221670985221863,89,0.030376331880688667,50,&buffer0[1587840]);
/* layer 274:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[1587840], (signed char*)&buffer0[1588608], 1, 1);
/* layer 275:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 276:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 277:REDUCE_PROD */
int reduction_axes_30[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1587852], (int32_t*)(int32_t*)&buffer0[1587860], 2, 1, reduction_axes_30, 4);
/* layer 278:CONCATENATION */
concatenate((int32_t*)&buffer0[1587852], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975856], 0);
/* layer 279:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 280:REDUCE_PROD */
int reduction_axes_31[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1587852], (int32_t*)(int32_t*)&buffer0[1587840], 1, 1, reduction_axes_31, 4);
/* layer 281:PACK */
pack((int32_t*)&buffer0[1587860], (int32_t*)&buffer0[1587840], (int32_t*)&buffer0[4975860], 2, 0);/* layer 282:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975864], (signed char*)&buffer0[4975864], NULL, (signed char*)&buffer0[4975864], -111, 0, -8, 0.009737360291182995, 0.004548172000795603, 0.00953757669776678, 0, 0, 0, 0, 0, 0, 0);/* layer 283:ADD */
add_fpreq(1, &buffer0[4975868],0.00953757669776678,-8,&buffer0[4975868],0.003389317775145173,7,0.010049563832581043,-14,&buffer0[1739136]);
/* layer 284:ADD */
add_fpreq(192, &buffer0[1739136],0.010049563832581043,-14,&buffer0[1398720],0.06240391358733177,-75,0.06117568537592888,-63,&buffer0[1587840]);
/* layer 285:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1587840], (signed char*)&buffer0[1398720], (signed char*)&buffer0[1398720], 1, 197);
/* layer 286:AVERAGE_POOL_2D */
avg_pooling(&buffer0[1776960],192,1,197,192,1,1,1,-128,127,&buffer0[1739136]);
/* layer 287:ADD */
add_fpreq(197, &buffer0[1739136],0.002990166423842311,-128,&buffer0[4802880],3.921568403342235e-09,0,0.002990166423842311,-128,&buffer0[1739336]);
/* layer 288:RSQRT */
rsqrt((signed char*)&buffer0[1739336], (signed char*)&buffer0[4955776], 1, 197);
/* layer 289:MUL */
fptr = (float*)(signed char*)&buffer0[1739136];
fptr3 = (float*)(signed char*)&buffer0[1776960];
fptr2 = (signed char*)&buffer0[1739136];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 290:MUL */
mul_int8(37824,(signed char*)&buffer0[1814784],(signed char*)&buffer0[1776960],(signed char*)&buffer0[1814784]);
/* layer 291:MUL */
fptr = (signed char*)&buffer0[1776960];
fptr3 = (float*)(signed char*)&buffer0[1852608];fptr2 = (signed char*)&buffer0[1852608];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 292:SUB */
sub_int8(192,(signed char*)&buffer0[4964552], 0, 1, 0,(signed char*)&buffer0[4964552], 0, 1, 0,(signed char*)&buffer0[1776960], 0, 1, 0, 0);
/* layer 293:ADD */
add_fpreq(37824, &buffer0[1890432],0.06891237199306488,-27,&buffer0[1776960],0.016123061999678612,54,0.053612641990184784,-19,&buffer0[1928256]);
/* layer 294:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 295:STRIDED_SLICE */
const uint16_t begin5[] = {0, 0, 0, 0};
const uint16_t end5[] = {1, 0, 0, 0};
const uint16_t strides5[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[1776960], 3, 1, 1, 1, begin5, end5, strides5, (int32_t*)&buffer0[1776972], 1, 1, 1, 1);
/* layer 296:PACK */
pack((int32_t*)&buffer0[1776972], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975872], 4, 0);/* layer 297:PACK */
pack((int32_t*)&buffer0[1776972], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975876], 3, 0);/* layer 298:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 299:REDUCE_PROD */
int reduction_axes_32[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1776972], (int32_t*)(int32_t*)&buffer0[1776980], 2, 1, reduction_axes_32, 4);
/* layer 300:CONCATENATION */
concatenate((int32_t*)&buffer0[1776972], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975880], 0);
/* layer 301:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 302:REDUCE_PROD */
int reduction_axes_33[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1776972], (int32_t*)(int32_t*)&buffer0[1776960], 1, 1, reduction_axes_33, 4);
/* layer 303:PACK */
pack((int32_t*)&buffer0[1776980], (int32_t*)&buffer0[1776960], (int32_t*)&buffer0[4975884], 2, 0);/* layer 304:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1776960], (signed char*)&buffer0[4975888], NULL, (signed char*)&buffer0[4975888], -19, 0, -8, 0.053612641990184784, 0.002474472625181079, 0.031279556453228, 0, 0, 0, 0, 0, 0, 0);/* layer 305:ADD */
add_fpreq(1, &buffer0[4964744],0.031279556453228,-8,&buffer0[4964744],0.006217514630407095,4,0.0326475165784359,-11,&buffer0[4964744]);
/* layer 306:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1776960], (signed char*)&buffer0[4975892], NULL, (signed char*)&buffer0[4975892], -19, 0, -7, 0.053612641990184784, 0.002017154823988676, 0.027241624891757965, 0, 0, 0, 0, 0, 0, 0);/* layer 307:ADD */
add_fpreq(1, &buffer0[4964936],0.027241624891757965,-7,&buffer0[4964936],0.018548492342233658,9,0.027326561510562897,-4,&buffer0[4964936]);
/* layer 308:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4965128], (signed char*)&buffer0[4965128], (signed char*)&buffer0[4965128], 1, 3, 1, 3, false, false);
/* layer 309:MUL */
fptr = (float*)(signed char*)&buffer0[4975896];
fptr3 = (float*)(signed char*)&buffer0[4975896];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 310:ADD */
add_fpreq(3, &buffer0[4975900],0.050548531115055084,-32,&buffer0[4613776],3.921568403342235e-09,0,0.050548531115055084,-32,&buffer0[1776964]);
/* layer 311:SOFTMAX */
softmax((signed char*)&buffer0[1776964], (signed char*)&buffer0[1776968], 1, 3, 1.0);
/* layer 312:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[1776960], (signed char*)&buffer0[4975904], NULL, (signed char*)&buffer0[4975904], -19, 0, 23, 0.053612641990184784, 0.0017810716526582837, 0.023612037301063538, 0, 0, 0, 0, 0, 0, 0);/* layer 313:ADD */
add_fpreq(1, &buffer0[4965320],0.023612037301063538,23,&buffer0[4965320],0.0024455171078443527,-42,0.021702878177165985,17,&buffer0[4965320]);
/* layer 314:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[1776968], (signed char*)&buffer0[4965512], (signed char*)&buffer0[2155200], 1, 3, 1, 3, false, false);
/* layer 315:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 316:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 317:REDUCE_PROD */
int reduction_axes_34[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1776972], (int32_t*)(int32_t*)&buffer0[1776980], 2, 1, reduction_axes_34, 4);
/* layer 318:CONCATENATION */
concatenate((int32_t*)&buffer0[1776972], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975912], 0);
/* layer 319:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 320:REDUCE_PROD */
int reduction_axes_35[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1776972], (int32_t*)(int32_t*)&buffer0[1776960], 1, 1, reduction_axes_35, 4);
/* layer 321:PACK */
pack((int32_t*)&buffer0[1776980], (int32_t*)&buffer0[1776960], (int32_t*)&buffer0[4975916], 2, 0);/* layer 322:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975920], (signed char*)&buffer0[4975920], NULL, (signed char*)&buffer0[4975920], 28, 0, 10, 0.012209981679916382, 0.001827572239562869, 0.0072349379770457745, 0, 0, 0, 0, 0, 0, 0);/* layer 323:ADD */
add_fpreq(1, &buffer0[4975924],0.0072349379770457745,10,&buffer0[4975924],0.004267646931111813,-3,0.007866538129746914,-31,&buffer0[1928256]);
/* layer 324:ADD */
add_fpreq(192, &buffer0[1928256],0.007866538129746914,-31,&buffer0[1587840],0.06117568537592888,-63,0.06505315750837326,-67,&buffer0[1776960]);
/* layer 325:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1776960], (signed char*)&buffer0[1587840], (signed char*)&buffer0[1587840], 1, 197);
/* layer 326:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2155200],192,1,197,192,1,1,1,-128,127,&buffer0[1928256]);
/* layer 327:ADD */
add_fpreq(197, &buffer0[1928256],0.0037287971936166286,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0037287971936166286,-128,&buffer0[1928456]);
/* layer 328:RSQRT */
rsqrt((signed char*)&buffer0[1928456], (signed char*)&buffer0[4955976], 1, 197);
/* layer 329:MUL */
fptr = (float*)(signed char*)&buffer0[1928256];
fptr3 = (float*)(signed char*)&buffer0[1966080];
fptr2 = (signed char*)&buffer0[1928256];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 330:MUL */
mul_int8(37824,(signed char*)&buffer0[2003904],(signed char*)&buffer0[1966080],(signed char*)&buffer0[2003904]);
/* layer 331:MUL */
fptr = (signed char*)&buffer0[1966080];
fptr3 = (float*)(signed char*)&buffer0[2041728];fptr2 = (signed char*)&buffer0[2041728];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 332:SUB */
sub_int8(192,(signed char*)&buffer0[4965704], 0, 1, 0,(signed char*)&buffer0[4965704], 0, 1, 0,(signed char*)&buffer0[1966080], 0, 1, 0, 0);
/* layer 333:ADD */
add_fpreq(37824, &buffer0[2079552],0.07331616431474686,12,&buffer0[1966080],0.0081015694886446,16,0.06994669139385223,16,&buffer0[2117376]);
/* layer 334:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 335:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 336:REDUCE_PROD */
int reduction_axes_36[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1966092], (int32_t*)(int32_t*)&buffer0[1966100], 2, 1, reduction_axes_36, 4);
/* layer 337:CONCATENATION */
concatenate((int32_t*)&buffer0[1966092], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4975928], 0);
/* layer 338:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 339:REDUCE_PROD */
int reduction_axes_37[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1966092], (int32_t*)(int32_t*)&buffer0[1966080], 1, 1, reduction_axes_37, 4);
/* layer 340:PACK */
pack((int32_t*)&buffer0[1966100], (int32_t*)&buffer0[1966080], (int32_t*)&buffer0[4975932], 2, 0);/* layer 341:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975936], (signed char*)&buffer0[4975936], NULL, (signed char*)&buffer0[4975936], 16, 0, -22, 0.06994669139385223, 0.002570262411609292, 0.03363244608044624, 0, 0, 0, 0, 0, 0, 0);/* layer 342:ADD */
add_fpreq(1, &buffer0[4975940],0.03363244608044624,-22,&buffer0[4975940],0.00960757676512003,102,0.03231131285429001,15,&buffer0[1966080]);
/* layer 343:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[1966080], (signed char*)&buffer0[1966848], 1, 1);
/* layer 344:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 345:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 346:REDUCE_PROD */
int reduction_axes_38[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1966092], (int32_t*)(int32_t*)&buffer0[1966100], 2, 1, reduction_axes_38, 4);
/* layer 347:CONCATENATION */
concatenate((int32_t*)&buffer0[1966092], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975944], 0);
/* layer 348:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 349:REDUCE_PROD */
int reduction_axes_39[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[1966092], (int32_t*)(int32_t*)&buffer0[1966080], 1, 1, reduction_axes_39, 4);
/* layer 350:PACK */
pack((int32_t*)&buffer0[1966100], (int32_t*)&buffer0[1966080], (int32_t*)&buffer0[4975948], 2, 0);/* layer 351:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4975952], (signed char*)&buffer0[4975952], NULL, (signed char*)&buffer0[4975952], -117, 0, 20, 0.01483044121414423, 0.0027862698771059513, 0.010490582324564457, 0, 0, 0, 0, 0, 0, 0);/* layer 352:ADD */
add_fpreq(1, &buffer0[4975956],0.010490582324564457,20,&buffer0[4975956],0.004802290815860033,40,0.01082419790327549,20,&buffer0[2117376]);
/* layer 353:ADD */
add_fpreq(192, &buffer0[2117376],0.01082419790327549,20,&buffer0[1776960],0.06505315750837326,-67,0.06090417876839638,-61,&buffer0[1966080]);
/* layer 354:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[1966080], (signed char*)&buffer0[1776960], (signed char*)&buffer0[1776960], 1, 197);
/* layer 355:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2155200],192,1,197,192,1,1,1,-128,127,&buffer0[2117376]);
/* layer 356:ADD */
add_fpreq(197, &buffer0[2117376],0.0029494320042431355,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0029494320042431355,-128,&buffer0[2117576]);
/* layer 357:RSQRT */
rsqrt((signed char*)&buffer0[2117576], (signed char*)&buffer0[4956176], 1, 197);
/* layer 358:MUL */
fptr = (float*)(signed char*)&buffer0[2117376];
fptr3 = (float*)(signed char*)&buffer0[2155200];
fptr2 = (signed char*)&buffer0[2117376];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 359:MUL */
mul_int8(37824,(signed char*)&buffer0[2193024],(signed char*)&buffer0[2155200],(signed char*)&buffer0[2193024]);
/* layer 360:MUL */
fptr = (signed char*)&buffer0[2155200];
fptr3 = (float*)(signed char*)&buffer0[2230848];fptr2 = (signed char*)&buffer0[2230848];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 361:SUB */
sub_int8(192,(signed char*)&buffer0[4965896], 0, 1, 0,(signed char*)&buffer0[4965896], 0, 1, 0,(signed char*)&buffer0[2155200], 0, 1, 0, 0);
/* layer 362:ADD */
add_fpreq(37824, &buffer0[2268672],0.06014043092727661,-26,&buffer0[2155200],0.014532736502587795,57,0.04601742699742317,-17,&buffer0[2306496]);
/* layer 363:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 364:STRIDED_SLICE */
const uint16_t begin6[] = {0, 0, 0, 0};
const uint16_t end6[] = {1, 0, 0, 0};
const uint16_t strides6[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[2155200], 3, 1, 1, 1, begin6, end6, strides6, (int32_t*)&buffer0[2155212], 1, 1, 1, 1);
/* layer 365:PACK */
pack((int32_t*)&buffer0[2155212], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4975960], 4, 0);/* layer 366:PACK */
pack((int32_t*)&buffer0[2155212], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4975964], 3, 0);/* layer 367:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 368:REDUCE_PROD */
int reduction_axes_40[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2155212], (int32_t*)(int32_t*)&buffer0[2155220], 2, 1, reduction_axes_40, 4);
/* layer 369:CONCATENATION */
concatenate((int32_t*)&buffer0[2155212], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4975968], 0);
/* layer 370:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 371:REDUCE_PROD */
int reduction_axes_41[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2155212], (int32_t*)(int32_t*)&buffer0[2155200], 1, 1, reduction_axes_41, 4);
/* layer 372:PACK */
pack((int32_t*)&buffer0[2155220], (int32_t*)&buffer0[2155200], (int32_t*)&buffer0[4975972], 2, 0);/* layer 373:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2155200], (signed char*)&buffer0[4975976], NULL, (signed char*)&buffer0[4975976], -17, 0, -13, 0.04601742699742317, 0.002689782530069351, 0.029837585985660553, 0, 0, 0, 0, 0, 0, 0);/* layer 374:ADD */
add_fpreq(1, &buffer0[4966088],0.029837585985660553,-13,&buffer0[4966088],0.005697095766663551,14,0.0314527191221714,-17,&buffer0[4966088]);
/* layer 375:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2155200], (signed char*)&buffer0[4975980], NULL, (signed char*)&buffer0[4975980], -17, 0, 4, 0.04601742699742317, 0.0022834381088614464, 0.024994561448693275, 0, 0, 0, 0, 0, 0, 0);/* layer 376:ADD */
add_fpreq(1, &buffer0[4966280],0.024994561448693275,4,&buffer0[4966280],0.017813127487897873,5,0.028756333515048027,7,&buffer0[4966280]);
/* layer 377:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4966472], (signed char*)&buffer0[4966472], (signed char*)&buffer0[4966472], 1, 3, 1, 3, false, false);
/* layer 378:MUL */
fptr = (float*)(signed char*)&buffer0[4975984];
fptr3 = (float*)(signed char*)&buffer0[4975984];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 379:ADD */
add_fpreq(3, &buffer0[4975988],0.06356193870306015,-27,&buffer0[4613776],3.921568403342235e-09,0,0.06356193870306015,-27,&buffer0[2155204]);
/* layer 380:SOFTMAX */
softmax((signed char*)&buffer0[2155204], (signed char*)&buffer0[2155208], 1, 3, 1.0);
/* layer 381:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2155200], (signed char*)&buffer0[4975992], NULL, (signed char*)&buffer0[4975992], -17, 0, 7, 0.04601742699742317, 0.0017664502374827862, 0.02178666740655899, 0, 0, 0, 0, 0, 0, 0);/* layer 382:ADD */
add_fpreq(1, &buffer0[4966664],0.02178666740655899,7,&buffer0[4966664],0.001861301832832396,23,0.02241349406540394,8,&buffer0[4966664]);
/* layer 383:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[2155208], (signed char*)&buffer0[4966856], (signed char*)&buffer0[2533440], 1, 3, 1, 3, false, false);
/* layer 384:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 385:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 386:REDUCE_PROD */
int reduction_axes_42[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2155212], (int32_t*)(int32_t*)&buffer0[2155220], 2, 1, reduction_axes_42, 4);
/* layer 387:CONCATENATION */
concatenate((int32_t*)&buffer0[2155212], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976000], 0);
/* layer 388:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 389:REDUCE_PROD */
int reduction_axes_43[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2155212], (int32_t*)(int32_t*)&buffer0[2155200], 1, 1, reduction_axes_43, 4);
/* layer 390:PACK */
pack((int32_t*)&buffer0[2155220], (int32_t*)&buffer0[2155200], (int32_t*)&buffer0[4976004], 2, 0);/* layer 391:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976008], (signed char*)&buffer0[4976008], NULL, (signed char*)&buffer0[4976008], 31, 0, -10, 0.011430738493800163, 0.001918644062243402, 0.008043862879276276, 0, 0, 0, 0, 0, 0, 0);/* layer 392:ADD */
add_fpreq(1, &buffer0[4976012],0.008043862879276276,-10,&buffer0[4976012],0.005784030072391033,-35,0.011371105909347534,-40,&buffer0[2306496]);
/* layer 393:ADD */
add_fpreq(192, &buffer0[2306496],0.011371105909347534,-40,&buffer0[1966080],0.06090417876839638,-61,0.06618373095989227,-63,&buffer0[2155200]);
/* layer 394:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[2155200], (signed char*)&buffer0[1966080], (signed char*)&buffer0[1966080], 1, 197);
/* layer 395:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2533440],192,1,197,192,1,1,1,-128,127,&buffer0[2306496]);
/* layer 396:ADD */
add_fpreq(197, &buffer0[2306496],0.0035886066034436226,-128,&buffer0[4802880],3.921568403342235e-09,0,0.0035886066034436226,-128,&buffer0[2306696]);
/* layer 397:RSQRT */
rsqrt((signed char*)&buffer0[2306696], (signed char*)&buffer0[4956376], 1, 197);
/* layer 398:MUL */
fptr = (float*)(signed char*)&buffer0[2306496];
fptr3 = (float*)(signed char*)&buffer0[2344320];
fptr2 = (signed char*)&buffer0[2306496];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 399:MUL */
mul_int8(37824,(signed char*)&buffer0[2382144],(signed char*)&buffer0[2344320],(signed char*)&buffer0[2382144]);
/* layer 400:MUL */
fptr = (signed char*)&buffer0[2344320];
fptr3 = (float*)(signed char*)&buffer0[2419968];fptr2 = (signed char*)&buffer0[2419968];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 401:SUB */
sub_int8(192,(signed char*)&buffer0[4967048], 0, 1, 0,(signed char*)&buffer0[4967048], 0, 1, 0,(signed char*)&buffer0[2344320], 0, 1, 0, 0);
/* layer 402:ADD */
add_fpreq(37824, &buffer0[2457792],0.07897642254829407,14,&buffer0[2344320],0.00740535743534565,1,0.07689365744590759,19,&buffer0[2495616]);
/* layer 403:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 404:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 405:REDUCE_PROD */
int reduction_axes_44[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2344332], (int32_t*)(int32_t*)&buffer0[2344340], 2, 1, reduction_axes_44, 4);
/* layer 406:CONCATENATION */
concatenate((int32_t*)&buffer0[2344332], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976016], 0);
/* layer 407:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 408:REDUCE_PROD */
int reduction_axes_45[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2344332], (int32_t*)(int32_t*)&buffer0[2344320], 1, 1, reduction_axes_45, 4);
/* layer 409:PACK */
pack((int32_t*)&buffer0[2344340], (int32_t*)&buffer0[2344320], (int32_t*)&buffer0[4976020], 2, 0);/* layer 410:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976024], (signed char*)&buffer0[4976024], NULL, (signed char*)&buffer0[4976024], 19, 0, 23, 0.07689365744590759, 0.003014134243130684, 0.03650485351681709, 0, 0, 0, 0, 0, 0, 0);/* layer 411:ADD */
add_fpreq(1, &buffer0[4976028],0.03650485351681709,23,&buffer0[4976028],0.00765087129548192,68,0.03905240818858147,43,&buffer0[2344320]);
/* layer 412:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[2344320], (signed char*)&buffer0[2345088], 1, 1);
/* layer 413:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 414:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 415:REDUCE_PROD */
int reduction_axes_46[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2344332], (int32_t*)(int32_t*)&buffer0[2344340], 2, 1, reduction_axes_46, 4);
/* layer 416:CONCATENATION */
concatenate((int32_t*)&buffer0[2344332], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976032], 0);
/* layer 417:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 418:REDUCE_PROD */
int reduction_axes_47[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2344332], (int32_t*)(int32_t*)&buffer0[2344320], 1, 1, reduction_axes_47, 4);
/* layer 419:PACK */
pack((int32_t*)&buffer0[2344340], (int32_t*)&buffer0[2344320], (int32_t*)&buffer0[4976036], 2, 0);/* layer 420:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976040], (signed char*)&buffer0[4976040], NULL, (signed char*)&buffer0[4976040], -116, 0, 15, 0.013600464910268784, 0.004038409795612097, 0.00976597424596548, 0, 0, 0, 0, 0, 0, 0);/* layer 421:ADD */
add_fpreq(1, &buffer0[4976044],0.00976597424596548,15,&buffer0[4976044],0.00520235113799572,37,0.01045856811106205,24,&buffer0[2495616]);
/* layer 422:ADD */
add_fpreq(192, &buffer0[2495616],0.01045856811106205,24,&buffer0[2155200],0.06618373095989227,-63,0.062228087335824966,-65,&buffer0[2344320]);
/* layer 423:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[2344320], (signed char*)&buffer0[2155200], (signed char*)&buffer0[2155200], 1, 197);
/* layer 424:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2533440],192,1,197,192,1,1,1,-128,127,&buffer0[2495616]);
/* layer 425:ADD */
add_fpreq(197, &buffer0[2495616],0.003146624192595482,-128,&buffer0[4802880],3.921568403342235e-09,0,0.003146624192595482,-128,&buffer0[2495816]);
/* layer 426:RSQRT */
rsqrt((signed char*)&buffer0[2495816], (signed char*)&buffer0[4956576], 1, 197);
/* layer 427:MUL */
fptr = (float*)(signed char*)&buffer0[2495616];
fptr3 = (float*)(signed char*)&buffer0[2533440];
fptr2 = (signed char*)&buffer0[2495616];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 428:MUL */
mul_int8(37824,(signed char*)&buffer0[2571264],(signed char*)&buffer0[2533440],(signed char*)&buffer0[2571264]);
/* layer 429:MUL */
fptr = (signed char*)&buffer0[2533440];
fptr3 = (float*)(signed char*)&buffer0[2609088];fptr2 = (signed char*)&buffer0[2609088];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 430:SUB */
sub_int8(192,(signed char*)&buffer0[4967240], 0, 1, 0,(signed char*)&buffer0[4967240], 0, 1, 0,(signed char*)&buffer0[2533440], 0, 1, 0, 0);
/* layer 431:ADD */
add_fpreq(37824, &buffer0[2646912],0.06772414594888687,-25,&buffer0[2533440],0.015506287105381489,45,0.053712207823991776,-16,&buffer0[2684736]);
/* layer 432:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 433:STRIDED_SLICE */
const uint16_t begin7[] = {0, 0, 0, 0};
const uint16_t end7[] = {1, 0, 0, 0};
const uint16_t strides7[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[2533440], 3, 1, 1, 1, begin7, end7, strides7, (int32_t*)&buffer0[2533452], 1, 1, 1, 1);
/* layer 434:PACK */
pack((int32_t*)&buffer0[2533452], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976048], 4, 0);/* layer 435:PACK */
pack((int32_t*)&buffer0[2533452], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976052], 3, 0);/* layer 436:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 437:REDUCE_PROD */
int reduction_axes_48[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2533452], (int32_t*)(int32_t*)&buffer0[2533460], 2, 1, reduction_axes_48, 4);
/* layer 438:CONCATENATION */
concatenate((int32_t*)&buffer0[2533452], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976056], 0);
/* layer 439:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 440:REDUCE_PROD */
int reduction_axes_49[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2533452], (int32_t*)(int32_t*)&buffer0[2533440], 1, 1, reduction_axes_49, 4);
/* layer 441:PACK */
pack((int32_t*)&buffer0[2533460], (int32_t*)&buffer0[2533440], (int32_t*)&buffer0[4976060], 2, 0);/* layer 442:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2533440], (signed char*)&buffer0[4976064], NULL, (signed char*)&buffer0[4976064], -16, 0, -1, 0.053712207823991776, 0.0025056826416403055, 0.03143930807709694, 0, 0, 0, 0, 0, 0, 0);/* layer 443:ADD */
add_fpreq(1, &buffer0[4967432],0.03143930807709694,-1,&buffer0[4967432],0.005972251761704683,6,0.032479237765073776,-10,&buffer0[4967432]);
/* layer 444:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2533440], (signed char*)&buffer0[4976068], NULL, (signed char*)&buffer0[4976068], -16, 0, 5, 0.053712207823991776, 0.0019442900083959103, 0.023363782092928886, 0, 0, 0, 0, 0, 0, 0);/* layer 445:ADD */
add_fpreq(1, &buffer0[4967624],0.023363782092928886,5,&buffer0[4967624],0.018284562975168228,18,0.029168227687478065,0,&buffer0[4967624]);
/* layer 446:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4967816], (signed char*)&buffer0[4967816], (signed char*)&buffer0[4967816], 1, 3, 1, 3, false, false);
/* layer 447:MUL */
fptr = (float*)(signed char*)&buffer0[4976072];
fptr3 = (float*)(signed char*)&buffer0[4976072];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 448:ADD */
add_fpreq(3, &buffer0[4976076],0.06000262126326561,-13,&buffer0[4613776],3.921568403342235e-09,0,0.06000262126326561,-13,&buffer0[2533444]);
/* layer 449:SOFTMAX */
softmax((signed char*)&buffer0[2533444], (signed char*)&buffer0[2533448], 1, 3, 1.0);
/* layer 450:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2533440], (signed char*)&buffer0[4976080], NULL, (signed char*)&buffer0[4976080], -16, 0, 8, 0.053712207823991776, 0.0020506095606833696, 0.023085469380021095, 0, 0, 0, 0, 0, 0, 0);/* layer 451:ADD */
add_fpreq(1, &buffer0[4968008],0.023085469380021095,8,&buffer0[4968008],0.0015541684115305543,2,0.02328779175877571,10,&buffer0[4968008]);
/* layer 452:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[2533448], (signed char*)&buffer0[4968200], (signed char*)&buffer0[2911680], 1, 3, 1, 3, false, false);
/* layer 453:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 454:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 455:REDUCE_PROD */
int reduction_axes_50[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2533452], (int32_t*)(int32_t*)&buffer0[2533460], 2, 1, reduction_axes_50, 4);
/* layer 456:CONCATENATION */
concatenate((int32_t*)&buffer0[2533452], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976088], 0);
/* layer 457:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 458:REDUCE_PROD */
int reduction_axes_51[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2533452], (int32_t*)(int32_t*)&buffer0[2533440], 1, 1, reduction_axes_51, 4);
/* layer 459:PACK */
pack((int32_t*)&buffer0[2533460], (int32_t*)&buffer0[2533440], (int32_t*)&buffer0[4976092], 2, 0);/* layer 460:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976096], (signed char*)&buffer0[4976096], NULL, (signed char*)&buffer0[4976096], 20, 0, -17, 0.008714239113032818, 0.0020095694344490767, 0.007510835770517588, 0, 0, 0, 0, 0, 0, 0);/* layer 461:ADD */
add_fpreq(1, &buffer0[4976100],0.007510835770517588,-17,&buffer0[4976100],0.005465126130729914,-51,0.011394873261451721,-46,&buffer0[2684736]);
/* layer 462:ADD */
add_fpreq(192, &buffer0[2684736],0.011394873261451721,-46,&buffer0[2344320],0.062228087335824966,-65,0.06840337067842484,-70,&buffer0[2533440]);
/* layer 463:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[2533440], (signed char*)&buffer0[2344320], (signed char*)&buffer0[2344320], 1, 197);
/* layer 464:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2911680],192,1,197,192,1,1,1,-128,127,&buffer0[2684736]);
/* layer 465:ADD */
add_fpreq(197, &buffer0[2684736],0.004122321028262377,-128,&buffer0[4802880],3.921568403342235e-09,0,0.004122321028262377,-128,&buffer0[2684936]);
/* layer 466:RSQRT */
rsqrt((signed char*)&buffer0[2684936], (signed char*)&buffer0[4956776], 1, 197);
/* layer 467:MUL */
fptr = (float*)(signed char*)&buffer0[2684736];
fptr3 = (float*)(signed char*)&buffer0[2722560];
fptr2 = (signed char*)&buffer0[2684736];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 468:MUL */
mul_int8(37824,(signed char*)&buffer0[2760384],(signed char*)&buffer0[2722560],(signed char*)&buffer0[2760384]);
/* layer 469:MUL */
fptr = (signed char*)&buffer0[2722560];
fptr3 = (float*)(signed char*)&buffer0[2798208];fptr2 = (signed char*)&buffer0[2798208];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 470:SUB */
sub_int8(192,(signed char*)&buffer0[4968392], 0, 1, 0,(signed char*)&buffer0[4968392], 0, 1, 0,(signed char*)&buffer0[2722560], 0, 1, 0, 0);
/* layer 471:ADD */
add_fpreq(37824, &buffer0[2836032],0.07823023945093155,6,&buffer0[2722560],0.007821690291166306,-18,0.07872951775789261,6,&buffer0[2873856]);
/* layer 472:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 473:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 474:REDUCE_PROD */
int reduction_axes_52[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2722572], (int32_t*)(int32_t*)&buffer0[2722580], 2, 1, reduction_axes_52, 4);
/* layer 475:CONCATENATION */
concatenate((int32_t*)&buffer0[2722572], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976104], 0);
/* layer 476:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 477:REDUCE_PROD */
int reduction_axes_53[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2722572], (int32_t*)(int32_t*)&buffer0[2722560], 1, 1, reduction_axes_53, 4);
/* layer 478:PACK */
pack((int32_t*)&buffer0[2722580], (int32_t*)&buffer0[2722560], (int32_t*)&buffer0[4976108], 2, 0);/* layer 479:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976112], (signed char*)&buffer0[4976112], NULL, (signed char*)&buffer0[4976112], 6, 0, -5, 0.07872951775789261, 0.002893657423555851, 0.037175290286540985, 0, 0, 0, 0, 0, 0, 0);/* layer 480:ADD */
add_fpreq(1, &buffer0[4976116],0.037175290286540985,-5,&buffer0[4976116],0.006446956191211939,81,0.038889214396476746,24,&buffer0[2722560]);
/* layer 481:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[2722560], (signed char*)&buffer0[2723328], 1, 1);
/* layer 482:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 483:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 484:REDUCE_PROD */
int reduction_axes_54[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2722572], (int32_t*)(int32_t*)&buffer0[2722580], 2, 1, reduction_axes_54, 4);
/* layer 485:CONCATENATION */
concatenate((int32_t*)&buffer0[2722572], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976120], 0);
/* layer 486:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 487:REDUCE_PROD */
int reduction_axes_55[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2722572], (int32_t*)(int32_t*)&buffer0[2722560], 1, 1, reduction_axes_55, 4);
/* layer 488:PACK */
pack((int32_t*)&buffer0[2722580], (int32_t*)&buffer0[2722560], (int32_t*)&buffer0[4976124], 2, 0);/* layer 489:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976128], (signed char*)&buffer0[4976128], NULL, (signed char*)&buffer0[4976128], -118, 0, -34, 0.01641068048775196, 0.004074613098055124, 0.022182146087288857, 0, 0, 0, 0, 0, 0, 0);/* layer 490:ADD */
add_fpreq(1, &buffer0[4976132],0.022182146087288857,-34,&buffer0[4976132],0.004101485945284367,59,0.018755698576569557,-23,&buffer0[2873856]);
/* layer 491:ADD */
add_fpreq(192, &buffer0[2873856],0.018755698576569557,-23,&buffer0[2533440],0.06840337067842484,-70,0.07114069163799286,-60,&buffer0[2722560]);
/* layer 492:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[2722560], (signed char*)&buffer0[2533440], (signed char*)&buffer0[2533440], 1, 197);
/* layer 493:AVERAGE_POOL_2D */
avg_pooling(&buffer0[2911680],192,1,197,192,1,1,1,-128,127,&buffer0[2873856]);
/* layer 494:ADD */
add_fpreq(197, &buffer0[2873856],0.003908632323145866,-128,&buffer0[4802880],3.921568403342235e-09,0,0.003908632323145866,-128,&buffer0[2874056]);
/* layer 495:RSQRT */
rsqrt((signed char*)&buffer0[2874056], (signed char*)&buffer0[4956976], 1, 197);
/* layer 496:MUL */
fptr = (float*)(signed char*)&buffer0[2873856];
fptr3 = (float*)(signed char*)&buffer0[2911680];
fptr2 = (signed char*)&buffer0[2873856];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 497:MUL */
mul_int8(37824,(signed char*)&buffer0[2949504],(signed char*)&buffer0[2911680],(signed char*)&buffer0[2949504]);
/* layer 498:MUL */
fptr = (signed char*)&buffer0[2911680];
fptr3 = (float*)(signed char*)&buffer0[2987328];fptr2 = (signed char*)&buffer0[2987328];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 499:SUB */
sub_int8(192,(signed char*)&buffer0[4968584], 0, 1, 0,(signed char*)&buffer0[4968584], 0, 1, 0,(signed char*)&buffer0[2911680], 0, 1, 0, 0);
/* layer 500:ADD */
add_fpreq(37824, &buffer0[3025152],0.06378039717674255,-26,&buffer0[2911680],0.01585412584245205,43,0.051959775388240814,-11,&buffer0[3062976]);
/* layer 501:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 502:STRIDED_SLICE */
const uint16_t begin8[] = {0, 0, 0, 0};
const uint16_t end8[] = {1, 0, 0, 0};
const uint16_t strides8[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[2911680], 3, 1, 1, 1, begin8, end8, strides8, (int32_t*)&buffer0[2911692], 1, 1, 1, 1);
/* layer 503:PACK */
pack((int32_t*)&buffer0[2911692], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976136], 4, 0);/* layer 504:PACK */
pack((int32_t*)&buffer0[2911692], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976140], 3, 0);/* layer 505:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 506:REDUCE_PROD */
int reduction_axes_56[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2911692], (int32_t*)(int32_t*)&buffer0[2911700], 2, 1, reduction_axes_56, 4);
/* layer 507:CONCATENATION */
concatenate((int32_t*)&buffer0[2911692], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976144], 0);
/* layer 508:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 509:REDUCE_PROD */
int reduction_axes_57[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2911692], (int32_t*)(int32_t*)&buffer0[2911680], 1, 1, reduction_axes_57, 4);
/* layer 510:PACK */
pack((int32_t*)&buffer0[2911700], (int32_t*)&buffer0[2911680], (int32_t*)&buffer0[4976148], 2, 0);/* layer 511:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2911680], (signed char*)&buffer0[4976152], NULL, (signed char*)&buffer0[4976152], -11, 0, -9, 0.051959775388240814, 0.002784992102533579, 0.030675971880555153, 0, 0, 0, 0, 0, 0, 0);/* layer 512:ADD */
add_fpreq(1, &buffer0[4968776],0.030675971880555153,-9,&buffer0[4968776],0.006766873877495527,7,0.032197412103414536,-4,&buffer0[4968776]);
/* layer 513:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2911680], (signed char*)&buffer0[4976156], NULL, (signed char*)&buffer0[4976156], -11, 0, 9, 0.051959775388240814, 0.0024430863559246063, 0.025940030813217163, 0, 0, 0, 0, 0, 0, 0);/* layer 514:ADD */
add_fpreq(1, &buffer0[4968968],0.025940030813217163,9,&buffer0[4968968],0.01693965308368206,-25,0.030307436361908913,1,&buffer0[4968968]);
/* layer 515:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4969160], (signed char*)&buffer0[4969160], (signed char*)&buffer0[4969160], 1, 3, 1, 3, false, false);
/* layer 516:MUL */
fptr = (float*)(signed char*)&buffer0[4976160];
fptr3 = (float*)(signed char*)&buffer0[4976160];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 517:ADD */
add_fpreq(3, &buffer0[4976164],0.04817187786102295,-36,&buffer0[4613776],3.921568403342235e-09,0,0.04817187786102295,-36,&buffer0[2911684]);
/* layer 518:SOFTMAX */
softmax((signed char*)&buffer0[2911684], (signed char*)&buffer0[2911688], 1, 3, 1.0);
/* layer 519:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[2911680], (signed char*)&buffer0[4976168], NULL, (signed char*)&buffer0[4976168], -11, 0, -4, 0.051959775388240814, 0.001793020754121244, 0.014211583882570267, 0, 0, 0, 0, 0, 0, 0);/* layer 520:ADD */
add_fpreq(1, &buffer0[4969352],0.014211583882570267,-4,&buffer0[4969352],0.001889632549136877,11,0.015129088424146175,-6,&buffer0[4969352]);
/* layer 521:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[2911688], (signed char*)&buffer0[4969544], (signed char*)&buffer0[3289920], 1, 3, 1, 3, false, false);
/* layer 522:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 523:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 524:REDUCE_PROD */
int reduction_axes_58[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2911692], (int32_t*)(int32_t*)&buffer0[2911700], 2, 1, reduction_axes_58, 4);
/* layer 525:CONCATENATION */
concatenate((int32_t*)&buffer0[2911692], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976176], 0);
/* layer 526:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 527:REDUCE_PROD */
int reduction_axes_59[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[2911692], (int32_t*)(int32_t*)&buffer0[2911680], 1, 1, reduction_axes_59, 4);
/* layer 528:PACK */
pack((int32_t*)&buffer0[2911700], (int32_t*)&buffer0[2911680], (int32_t*)&buffer0[4976180], 2, 0);/* layer 529:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976184], (signed char*)&buffer0[4976184], NULL, (signed char*)&buffer0[4976184], -3, 0, -30, 0.007671399507671595, 0.0016675881342962384, 0.005407122895121574, 0, 0, 0, 0, 0, 0, 0);/* layer 530:ADD */
add_fpreq(1, &buffer0[4976188],0.005407122895121574,-30,&buffer0[4976188],0.004040050320327282,-23,0.00798548199236393,-55,&buffer0[3062976]);
/* layer 531:ADD */
add_fpreq(192, &buffer0[3062976],0.00798548199236393,-55,&buffer0[2722560],0.07114069163799286,-60,0.07819028198719025,-61,&buffer0[2911680]);
/* layer 532:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[2911680], (signed char*)&buffer0[2722560], (signed char*)&buffer0[2722560], 1, 197);
/* layer 533:AVERAGE_POOL_2D */
avg_pooling(&buffer0[3289920],192,1,197,192,1,1,1,-128,127,&buffer0[3062976]);
/* layer 534:ADD */
add_fpreq(197, &buffer0[3062976],0.004870566073805094,-128,&buffer0[4802880],3.921568403342235e-09,0,0.004870566073805094,-128,&buffer0[3063176]);
/* layer 535:RSQRT */
rsqrt((signed char*)&buffer0[3063176], (signed char*)&buffer0[4957176], 1, 197);
/* layer 536:MUL */
fptr = (float*)(signed char*)&buffer0[3062976];
fptr3 = (float*)(signed char*)&buffer0[3100800];
fptr2 = (signed char*)&buffer0[3062976];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 537:MUL */
mul_int8(37824,(signed char*)&buffer0[3138624],(signed char*)&buffer0[3100800],(signed char*)&buffer0[3138624]);
/* layer 538:MUL */
fptr = (signed char*)&buffer0[3100800];
fptr3 = (float*)(signed char*)&buffer0[3176448];fptr2 = (signed char*)&buffer0[3176448];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 539:SUB */
sub_int8(192,(signed char*)&buffer0[4969736], 0, 1, 0,(signed char*)&buffer0[4969736], 0, 1, 0,(signed char*)&buffer0[3100800], 0, 1, 0, 0);
/* layer 540:ADD */
add_fpreq(37824, &buffer0[3214272],0.08300342410802841,-9,&buffer0[3100800],0.008258466608822346,-4,0.08691918104887009,-11,&buffer0[3252096]);
/* layer 541:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 542:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 543:REDUCE_PROD */
int reduction_axes_60[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3100812], (int32_t*)(int32_t*)&buffer0[3100820], 2, 1, reduction_axes_60, 4);
/* layer 544:CONCATENATION */
concatenate((int32_t*)&buffer0[3100812], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976192], 0);
/* layer 545:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 546:REDUCE_PROD */
int reduction_axes_61[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3100812], (int32_t*)(int32_t*)&buffer0[3100800], 1, 1, reduction_axes_61, 4);
/* layer 547:PACK */
pack((int32_t*)&buffer0[3100820], (int32_t*)&buffer0[3100800], (int32_t*)&buffer0[4976196], 2, 0);/* layer 548:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976200], (signed char*)&buffer0[4976200], NULL, (signed char*)&buffer0[4976200], -11, 0, 4, 0.08691918104887009, 0.0019645828288048506, 0.03062625601887703, 0, 0, 0, 0, 0, 0, 0);/* layer 549:ADD */
add_fpreq(1, &buffer0[4976204],0.03062625601887703,4,&buffer0[4976204],0.0066042873077094555,61,0.031743910163640976,23,&buffer0[3100800]);
/* layer 550:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[3100800], (signed char*)&buffer0[3101568], 1, 1);
/* layer 551:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 552:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 553:REDUCE_PROD */
int reduction_axes_62[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3100812], (int32_t*)(int32_t*)&buffer0[3100820], 2, 1, reduction_axes_62, 4);
/* layer 554:CONCATENATION */
concatenate((int32_t*)&buffer0[3100812], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976208], 0);
/* layer 555:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 556:REDUCE_PROD */
int reduction_axes_63[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3100812], (int32_t*)(int32_t*)&buffer0[3100800], 1, 1, reduction_axes_63, 4);
/* layer 557:PACK */
pack((int32_t*)&buffer0[3100820], (int32_t*)&buffer0[3100800], (int32_t*)&buffer0[4976212], 2, 0);/* layer 558:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976216], (signed char*)&buffer0[4976216], NULL, (signed char*)&buffer0[4976216], -116, 0, -61, 0.013599695637822151, 0.004301212728023529, 0.018395932391285896, 0, 0, 0, 0, 0, 0, 0);/* layer 559:ADD */
add_fpreq(1, &buffer0[4976220],0.018395932391285896,-61,&buffer0[4976220],0.004678177181631327,58,0.016028854995965958,-34,&buffer0[3252096]);
/* layer 560:ADD */
add_fpreq(192, &buffer0[3252096],0.016028854995965958,-34,&buffer0[2911680],0.07819028198719025,-61,0.08034245669841766,-46,&buffer0[3100800]);
/* layer 561:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[3100800], (signed char*)&buffer0[2911680], (signed char*)&buffer0[2911680], 1, 197);
/* layer 562:AVERAGE_POOL_2D */
avg_pooling(&buffer0[3289920],192,1,197,192,1,1,1,-128,127,&buffer0[3252096]);
/* layer 563:ADD */
add_fpreq(197, &buffer0[3252096],0.00553574413061142,-128,&buffer0[4802880],3.921568403342235e-09,0,0.00553574413061142,-128,&buffer0[3252296]);
/* layer 564:RSQRT */
rsqrt((signed char*)&buffer0[3252296], (signed char*)&buffer0[4957376], 1, 197);
/* layer 565:MUL */
fptr = (float*)(signed char*)&buffer0[3252096];
fptr3 = (float*)(signed char*)&buffer0[3289920];
fptr2 = (signed char*)&buffer0[3252096];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 566:MUL */
mul_int8(37824,(signed char*)&buffer0[3327744],(signed char*)&buffer0[3289920],(signed char*)&buffer0[3327744]);
/* layer 567:MUL */
fptr = (signed char*)&buffer0[3289920];
fptr3 = (float*)(signed char*)&buffer0[3365568];fptr2 = (signed char*)&buffer0[3365568];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 568:SUB */
sub_int8(192,(signed char*)&buffer0[4969928], 0, 1, 0,(signed char*)&buffer0[4969928], 0, 1, 0,(signed char*)&buffer0[3289920], 0, 1, 0, 0);
/* layer 569:ADD */
add_fpreq(37824, &buffer0[3403392],0.05794074758887291,-13,&buffer0[3289920],0.0147576704621315,36,0.04635380581021309,3,&buffer0[3441216]);
/* layer 570:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 571:STRIDED_SLICE */
const uint16_t begin9[] = {0, 0, 0, 0};
const uint16_t end9[] = {1, 0, 0, 0};
const uint16_t strides9[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[3289920], 3, 1, 1, 1, begin9, end9, strides9, (int32_t*)&buffer0[3289932], 1, 1, 1, 1);
/* layer 572:PACK */
pack((int32_t*)&buffer0[3289932], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976224], 4, 0);/* layer 573:PACK */
pack((int32_t*)&buffer0[3289932], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976228], 3, 0);/* layer 574:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 575:REDUCE_PROD */
int reduction_axes_64[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3289932], (int32_t*)(int32_t*)&buffer0[3289940], 2, 1, reduction_axes_64, 4);
/* layer 576:CONCATENATION */
concatenate((int32_t*)&buffer0[3289932], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976232], 0);
/* layer 577:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 578:REDUCE_PROD */
int reduction_axes_65[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3289932], (int32_t*)(int32_t*)&buffer0[3289920], 1, 1, reduction_axes_65, 4);
/* layer 579:PACK */
pack((int32_t*)&buffer0[3289940], (int32_t*)&buffer0[3289920], (int32_t*)&buffer0[4976236], 2, 0);/* layer 580:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3289920], (signed char*)&buffer0[4976240], NULL, (signed char*)&buffer0[4976240], 3, 0, -4, 0.04635380581021309, 0.0025980353821069, 0.030473504215478897, 0, 0, 0, 0, 0, 0, 0);/* layer 581:ADD */
add_fpreq(1, &buffer0[4970120],0.030473504215478897,-4,&buffer0[4970120],0.005511438939720392,-1,0.030733386054635048,0,&buffer0[4970120]);
/* layer 582:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3289920], (signed char*)&buffer0[4976244], NULL, (signed char*)&buffer0[4976244], 3, 0, -21, 0.04635380581021309, 0.002475669840350747, 0.022740501910448074, 0, 0, 0, 0, 0, 0, 0);/* layer 583:ADD */
add_fpreq(1, &buffer0[4970312],0.022740501910448074,-21,&buffer0[4970312],0.018983876332640648,25,0.027987685054540634,-10,&buffer0[4970312]);
/* layer 584:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4970504], (signed char*)&buffer0[4970504], (signed char*)&buffer0[4970504], 1, 3, 1, 3, false, false);
/* layer 585:MUL */
fptr = (float*)(signed char*)&buffer0[4976248];
fptr3 = (float*)(signed char*)&buffer0[4976248];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 586:ADD */
add_fpreq(3, &buffer0[4976252],0.037875689566135406,-25,&buffer0[4613776],3.921568403342235e-09,0,0.037875689566135406,-25,&buffer0[3289924]);
/* layer 587:SOFTMAX */
softmax((signed char*)&buffer0[3289924], (signed char*)&buffer0[3289928], 1, 3, 1.0);
/* layer 588:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3289920], (signed char*)&buffer0[4976256], NULL, (signed char*)&buffer0[4976256], 3, 0, 2, 0.04635380581021309, 0.001844488549977541, 0.01449628546833992, 0, 0, 0, 0, 0, 0, 0);/* layer 589:ADD */
add_fpreq(1, &buffer0[4970696],0.01449628546833992,2,&buffer0[4970696],0.0011566269677132368,-4,0.014623548835515976,5,&buffer0[4970696]);
/* layer 590:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[3289928], (signed char*)&buffer0[4970888], (signed char*)&buffer0[3668160], 1, 3, 1, 3, false, false);
/* layer 591:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 592:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 593:REDUCE_PROD */
int reduction_axes_66[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3289932], (int32_t*)(int32_t*)&buffer0[3289940], 2, 1, reduction_axes_66, 4);
/* layer 594:CONCATENATION */
concatenate((int32_t*)&buffer0[3289932], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976264], 0);
/* layer 595:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 596:REDUCE_PROD */
int reduction_axes_67[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3289932], (int32_t*)(int32_t*)&buffer0[3289920], 1, 1, reduction_axes_67, 4);
/* layer 597:PACK */
pack((int32_t*)&buffer0[3289940], (int32_t*)&buffer0[3289920], (int32_t*)&buffer0[4976268], 2, 0);/* layer 598:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976272], (signed char*)&buffer0[4976272], NULL, (signed char*)&buffer0[4976272], -3, 0, -49, 0.00931659433990717, 0.0017964420840144157, 0.00701017864048481, 0, 0, 0, 0, 0, 0, 0);/* layer 599:ADD */
add_fpreq(1, &buffer0[4976276],0.00701017864048481,-49,&buffer0[4976276],0.0025843943003565073,-4,0.00760662741959095,-55,&buffer0[3441216]);
/* layer 600:ADD */
add_fpreq(192, &buffer0[3441216],0.00760662741959095,-55,&buffer0[3100800],0.08034245669841766,-46,0.08217331767082214,-53,&buffer0[3289920]);
/* layer 601:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[3289920], (signed char*)&buffer0[3100800], (signed char*)&buffer0[3100800], 1, 197);
/* layer 602:AVERAGE_POOL_2D */
avg_pooling(&buffer0[3668160],192,1,197,192,1,1,1,-128,127,&buffer0[3441216]);
/* layer 603:ADD */
add_fpreq(197, &buffer0[3441216],0.005802507512271404,-128,&buffer0[4802880],3.921568403342235e-09,0,0.005802507512271404,-128,&buffer0[3441416]);
/* layer 604:RSQRT */
rsqrt((signed char*)&buffer0[3441416], (signed char*)&buffer0[4957576], 1, 197);
/* layer 605:MUL */
fptr = (float*)(signed char*)&buffer0[3441216];
fptr3 = (float*)(signed char*)&buffer0[3479040];
fptr2 = (signed char*)&buffer0[3441216];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 606:MUL */
mul_int8(37824,(signed char*)&buffer0[3516864],(signed char*)&buffer0[3479040],(signed char*)&buffer0[3516864]);
/* layer 607:MUL */
fptr = (signed char*)&buffer0[3479040];
fptr3 = (float*)(signed char*)&buffer0[3554688];fptr2 = (signed char*)&buffer0[3554688];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 608:SUB */
sub_int8(192,(signed char*)&buffer0[4971080], 0, 1, 0,(signed char*)&buffer0[4971080], 0, 1, 0,(signed char*)&buffer0[3479040], 0, 1, 0, 0);
/* layer 609:ADD */
add_fpreq(37824, &buffer0[3592512],0.09017661958932877,19,&buffer0[3479040],0.01224416121840477,-19,0.0934523195028305,24,&buffer0[3630336]);
/* layer 610:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 611:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 612:REDUCE_PROD */
int reduction_axes_68[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3479052], (int32_t*)(int32_t*)&buffer0[3479060], 2, 1, reduction_axes_68, 4);
/* layer 613:CONCATENATION */
concatenate((int32_t*)&buffer0[3479052], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976280], 0);
/* layer 614:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 615:REDUCE_PROD */
int reduction_axes_69[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3479052], (int32_t*)(int32_t*)&buffer0[3479040], 1, 1, reduction_axes_69, 4);
/* layer 616:PACK */
pack((int32_t*)&buffer0[3479060], (int32_t*)&buffer0[3479040], (int32_t*)&buffer0[4976284], 2, 0);/* layer 617:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976288], (signed char*)&buffer0[4976288], NULL, (signed char*)&buffer0[4976288], 24, 0, 4, 0.0934523195028305, 0.0021465353202074766, 0.0446586087346077, 0, 0, 0, 0, 0, 0, 0);/* layer 618:ADD */
add_fpreq(1, &buffer0[4976292],0.0446586087346077,4,&buffer0[4976292],0.006532740313559771,61,0.04535302892327309,18,&buffer0[3479040]);
/* layer 619:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[3479040], (signed char*)&buffer0[3479808], 1, 1);
/* layer 620:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 621:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 622:REDUCE_PROD */
int reduction_axes_70[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3479052], (int32_t*)(int32_t*)&buffer0[3479060], 2, 1, reduction_axes_70, 4);
/* layer 623:CONCATENATION */
concatenate((int32_t*)&buffer0[3479052], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976296], 0);
/* layer 624:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 625:REDUCE_PROD */
int reduction_axes_71[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3479052], (int32_t*)(int32_t*)&buffer0[3479040], 1, 1, reduction_axes_71, 4);
/* layer 626:PACK */
pack((int32_t*)&buffer0[3479060], (int32_t*)&buffer0[3479040], (int32_t*)&buffer0[4976300], 2, 0);/* layer 627:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976304], (signed char*)&buffer0[4976304], NULL, (signed char*)&buffer0[4976304], -120, 0, -70, 0.02005135454237461, 0.005169431678950787, 0.021317608654499054, 0, 0, 0, 0, 0, 0, 0);/* layer 628:ADD */
add_fpreq(1, &buffer0[4976308],0.021317608654499054,-70,&buffer0[4976308],0.005631342995911837,60,0.018668988719582558,-41,&buffer0[3630336]);
/* layer 629:ADD */
add_fpreq(192, &buffer0[3630336],0.018668988719582558,-41,&buffer0[3289920],0.08217331767082214,-53,0.0802844762802124,-40,&buffer0[3479040]);
/* layer 630:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[3479040], (signed char*)&buffer0[3289920], (signed char*)&buffer0[3289920], 1, 197);
/* layer 631:AVERAGE_POOL_2D */
avg_pooling(&buffer0[3668160],192,1,197,192,1,1,1,-128,127,&buffer0[3630336]);
/* layer 632:ADD */
add_fpreq(197, &buffer0[3630336],0.007949251681566238,-128,&buffer0[4802880],3.921568403342235e-09,0,0.007949251681566238,-128,&buffer0[3630536]);
/* layer 633:RSQRT */
rsqrt((signed char*)&buffer0[3630536], (signed char*)&buffer0[4957776], 1, 197);
/* layer 634:MUL */
fptr = (float*)(signed char*)&buffer0[3630336];
fptr3 = (float*)(signed char*)&buffer0[3668160];
fptr2 = (signed char*)&buffer0[3630336];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 635:MUL */
mul_int8(37824,(signed char*)&buffer0[3705984],(signed char*)&buffer0[3668160],(signed char*)&buffer0[3705984]);
/* layer 636:MUL */
fptr = (signed char*)&buffer0[3668160];
fptr3 = (float*)(signed char*)&buffer0[3743808];fptr2 = (signed char*)&buffer0[3743808];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 637:SUB */
sub_int8(192,(signed char*)&buffer0[4971272], 0, 1, 0,(signed char*)&buffer0[4971272], 0, 1, 0,(signed char*)&buffer0[3668160], 0, 1, 0, 0);
/* layer 638:ADD */
add_fpreq(37824, &buffer0[3781632],0.055035751312971115,-9,&buffer0[3668160],0.013281001709401608,24,0.043726373463869095,1,&buffer0[3819456]);
/* layer 639:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 640:STRIDED_SLICE */
const uint16_t begin10[] = {0, 0, 0, 0};
const uint16_t end10[] = {1, 0, 0, 0};
const uint16_t strides10[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[3668160], 3, 1, 1, 1, begin10, end10, strides10, (int32_t*)&buffer0[3668172], 1, 1, 1, 1);
/* layer 641:PACK */
pack((int32_t*)&buffer0[3668172], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976312], 4, 0);/* layer 642:PACK */
pack((int32_t*)&buffer0[3668172], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976316], 3, 0);/* layer 643:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 644:REDUCE_PROD */
int reduction_axes_72[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3668172], (int32_t*)(int32_t*)&buffer0[3668180], 2, 1, reduction_axes_72, 4);
/* layer 645:CONCATENATION */
concatenate((int32_t*)&buffer0[3668172], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976320], 0);
/* layer 646:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 647:REDUCE_PROD */
int reduction_axes_73[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3668172], (int32_t*)(int32_t*)&buffer0[3668160], 1, 1, reduction_axes_73, 4);
/* layer 648:PACK */
pack((int32_t*)&buffer0[3668180], (int32_t*)&buffer0[3668160], (int32_t*)&buffer0[4976324], 2, 0);/* layer 649:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3668160], (signed char*)&buffer0[4976328], NULL, (signed char*)&buffer0[4976328], 1, 0, 0, 0.043726373463869095, 0.0024063747841864824, 0.027060789987444878, 0, 0, 0, 0, 0, 0, 0);/* layer 650:ADD */
add_fpreq(1, &buffer0[4971464],0.027060789987444878,0,&buffer0[4971464],0.006525822915136814,19,0.027709191665053368,6,&buffer0[4971464]);
/* layer 651:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3668160], (signed char*)&buffer0[4976332], NULL, (signed char*)&buffer0[4976332], 1, 0, 5, 0.043726373463869095, 0.0018595580477267504, 0.020421769469976425, 0, 0, 0, 0, 0, 0, 0);/* layer 652:ADD */
add_fpreq(1, &buffer0[4971656],0.020421769469976425,5,&buffer0[4971656],0.015568424947559834,3,0.028072498738765717,-4,&buffer0[4971656]);
/* layer 653:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4971848], (signed char*)&buffer0[4971848], (signed char*)&buffer0[4971848], 1, 3, 1, 3, false, false);
/* layer 654:MUL */
fptr = (float*)(signed char*)&buffer0[4976336];
fptr3 = (float*)(signed char*)&buffer0[4976336];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 655:ADD */
add_fpreq(3, &buffer0[4976340],0.03742483630776405,-11,&buffer0[4613776],3.921568403342235e-09,0,0.03742483630776405,-11,&buffer0[3668164]);
/* layer 656:SOFTMAX */
softmax((signed char*)&buffer0[3668164], (signed char*)&buffer0[3668168], 1, 3, 1.0);
/* layer 657:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[3668160], (signed char*)&buffer0[4976344], NULL, (signed char*)&buffer0[4976344], 1, 0, 5, 0.043726373463869095, 0.001736168866045773, 0.01254748273640871, 0, 0, 0, 0, 0, 0, 0);/* layer 658:ADD */
add_fpreq(1, &buffer0[4972040],0.01254748273640871,5,&buffer0[4972040],0.0014096004888415337,-5,0.011867360211908817,2,&buffer0[4972040]);
/* layer 659:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[3668168], (signed char*)&buffer0[4972232], (signed char*)&buffer0[4046400], 1, 3, 1, 3, false, false);
/* layer 660:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 661:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 662:REDUCE_PROD */
int reduction_axes_74[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3668172], (int32_t*)(int32_t*)&buffer0[3668180], 2, 1, reduction_axes_74, 4);
/* layer 663:CONCATENATION */
concatenate((int32_t*)&buffer0[3668172], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976352], 0);
/* layer 664:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 665:REDUCE_PROD */
int reduction_axes_75[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3668172], (int32_t*)(int32_t*)&buffer0[3668160], 1, 1, reduction_axes_75, 4);
/* layer 666:PACK */
pack((int32_t*)&buffer0[3668180], (int32_t*)&buffer0[3668160], (int32_t*)&buffer0[4976356], 2, 0);/* layer 667:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976360], (signed char*)&buffer0[4976360], NULL, (signed char*)&buffer0[4976360], 11, 0, -71, 0.006546683609485626, 0.0021273791790008545, 0.008550671860575676, 0, 0, 0, 0, 0, 0, 0);/* layer 668:ADD */
add_fpreq(1, &buffer0[4976364],0.008550671860575676,-71,&buffer0[4976364],0.00416896166279912,48,0.006655711680650711,-17,&buffer0[3819456]);
/* layer 669:ADD */
add_fpreq(192, &buffer0[3819456],0.006655711680650711,-17,&buffer0[3479040],0.0802844762802124,-40,0.08184413611888885,-46,&buffer0[3668160]);
/* layer 670:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[3668160], (signed char*)&buffer0[3479040], (signed char*)&buffer0[3479040], 1, 197);
/* layer 671:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4046400],192,1,197,192,1,1,1,-128,127,&buffer0[3819456]);
/* layer 672:ADD */
add_fpreq(197, &buffer0[3819456],0.007894350215792656,-128,&buffer0[4802880],3.921568403342235e-09,0,0.007894350215792656,-128,&buffer0[3819656]);
/* layer 673:RSQRT */
rsqrt((signed char*)&buffer0[3819656], (signed char*)&buffer0[4957976], 1, 197);
/* layer 674:MUL */
fptr = (float*)(signed char*)&buffer0[3819456];
fptr3 = (float*)(signed char*)&buffer0[3857280];
fptr2 = (signed char*)&buffer0[3819456];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 675:MUL */
mul_int8(37824,(signed char*)&buffer0[3895104],(signed char*)&buffer0[3857280],(signed char*)&buffer0[3895104]);
/* layer 676:MUL */
fptr = (signed char*)&buffer0[3857280];
fptr3 = (float*)(signed char*)&buffer0[3932928];fptr2 = (signed char*)&buffer0[3932928];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 677:SUB */
sub_int8(192,(signed char*)&buffer0[4972424], 0, 1, 0,(signed char*)&buffer0[4972424], 0, 1, 0,(signed char*)&buffer0[3857280], 0, 1, 0, 0);
/* layer 678:ADD */
add_fpreq(37824, &buffer0[3970752],0.0652521476149559,12,&buffer0[3857280],0.009663521312177181,2,0.06507173925638199,13,&buffer0[4008576]);
/* layer 679:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 680:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 681:REDUCE_PROD */
int reduction_axes_76[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3857292], (int32_t*)(int32_t*)&buffer0[3857300], 2, 1, reduction_axes_76, 4);
/* layer 682:CONCATENATION */
concatenate((int32_t*)&buffer0[3857292], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976368], 0);
/* layer 683:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 684:REDUCE_PROD */
int reduction_axes_77[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3857292], (int32_t*)(int32_t*)&buffer0[3857280], 1, 1, reduction_axes_77, 4);
/* layer 685:PACK */
pack((int32_t*)&buffer0[3857300], (int32_t*)&buffer0[3857280], (int32_t*)&buffer0[4976372], 2, 0);/* layer 686:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976376], (signed char*)&buffer0[4976376], NULL, (signed char*)&buffer0[4976376], 13, 0, 6, 0.06507173925638199, 0.002205091994255781, 0.034765250980854034, 0, 0, 0, 0, 0, 0, 0);/* layer 687:ADD */
add_fpreq(1, &buffer0[4976380],0.034765250980854034,6,&buffer0[4976380],0.007060060277581215,60,0.034320540726184845,21,&buffer0[3857280]);
/* layer 688:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[3857280], (signed char*)&buffer0[3858048], 1, 1);
/* layer 689:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 690:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 691:REDUCE_PROD */
int reduction_axes_78[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3857292], (int32_t*)(int32_t*)&buffer0[3857300], 2, 1, reduction_axes_78, 4);
/* layer 692:CONCATENATION */
concatenate((int32_t*)&buffer0[3857292], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976384], 0);
/* layer 693:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 694:REDUCE_PROD */
int reduction_axes_79[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[3857292], (int32_t*)(int32_t*)&buffer0[3857280], 1, 1, reduction_axes_79, 4);
/* layer 695:PACK */
pack((int32_t*)&buffer0[3857300], (int32_t*)&buffer0[3857280], (int32_t*)&buffer0[4976388], 2, 0);/* layer 696:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976392], (signed char*)&buffer0[4976392], NULL, (signed char*)&buffer0[4976392], -117, 0, -35, 0.014935165643692017, 0.004727357532829046, 0.020794924348592758, 0, 0, 0, 0, 0, 0, 0);/* layer 697:ADD */
add_fpreq(1, &buffer0[4976396],0.020794924348592758,-35,&buffer0[4976396],0.006802030839025974,71,0.020794924348592758,30,&buffer0[4008576]);
/* layer 698:ADD */
add_fpreq(192, &buffer0[4008576],0.020794924348592758,30,&buffer0[3668160],0.08184413611888885,-46,0.09201667457818985,-39,&buffer0[3857280]);
/* layer 699:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[3857280], (signed char*)&buffer0[3668160], (signed char*)&buffer0[3668160], 1, 197);
/* layer 700:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4046400],192,1,197,192,1,1,1,-128,127,&buffer0[4008576]);
/* layer 701:ADD */
add_fpreq(197, &buffer0[4008576],0.009699857793748379,-128,&buffer0[4802880],3.921568403342235e-09,0,0.009699857793748379,-128,&buffer0[4008776]);
/* layer 702:RSQRT */
rsqrt((signed char*)&buffer0[4008776], (signed char*)&buffer0[4958176], 1, 197);
/* layer 703:MUL */
fptr = (float*)(signed char*)&buffer0[4008576];
fptr3 = (float*)(signed char*)&buffer0[4046400];
fptr2 = (signed char*)&buffer0[4008576];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 704:MUL */
mul_int8(37824,(signed char*)&buffer0[4084224],(signed char*)&buffer0[4046400],(signed char*)&buffer0[4084224]);
/* layer 705:MUL */
fptr = (signed char*)&buffer0[4046400];
fptr3 = (float*)(signed char*)&buffer0[4122048];fptr2 = (signed char*)&buffer0[4122048];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 706:SUB */
sub_int8(192,(signed char*)&buffer0[4972616], 0, 1, 0,(signed char*)&buffer0[4972616], 0, 1, 0,(signed char*)&buffer0[4046400], 0, 1, 0, 0);
/* layer 707:ADD */
add_fpreq(37824, &buffer0[4159872],0.05647273734211922,-19,&buffer0[4046400],0.011918888427317142,10,0.04663272947072983,-16,&buffer0[4197696]);
/* layer 708:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 709:STRIDED_SLICE */
const uint16_t begin11[] = {0, 0, 0, 0};
const uint16_t end11[] = {1, 0, 0, 0};
const uint16_t strides11[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[4046400], 3, 1, 1, 1, begin11, end11, strides11, (int32_t*)&buffer0[4046412], 1, 1, 1, 1);
/* layer 710:PACK */
pack((int32_t*)&buffer0[4046412], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976400], 4, 0);/* layer 711:PACK */
pack((int32_t*)&buffer0[4046412], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976404], 3, 0);/* layer 712:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 713:REDUCE_PROD */
int reduction_axes_80[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4046412], (int32_t*)(int32_t*)&buffer0[4046420], 2, 1, reduction_axes_80, 4);
/* layer 714:CONCATENATION */
concatenate((int32_t*)&buffer0[4046412], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976408], 0);
/* layer 715:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 716:REDUCE_PROD */
int reduction_axes_81[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4046412], (int32_t*)(int32_t*)&buffer0[4046400], 1, 1, reduction_axes_81, 4);
/* layer 717:PACK */
pack((int32_t*)&buffer0[4046420], (int32_t*)&buffer0[4046400], (int32_t*)&buffer0[4976412], 2, 0);/* layer 718:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4046400], (signed char*)&buffer0[4976416], NULL, (signed char*)&buffer0[4976416], -16, 0, -9, 0.04663272947072983, 0.002038859063759446, 0.02681802585721016, 0, 0, 0, 0, 0, 0, 0);/* layer 719:ADD */
add_fpreq(1, &buffer0[4972808],0.02681802585721016,-9,&buffer0[4972808],0.0058515965938568115,-12,0.02576550841331482,-17,&buffer0[4972808]);
/* layer 720:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4046400], (signed char*)&buffer0[4976420], NULL, (signed char*)&buffer0[4976420], -16, 0, 1, 0.04663272947072983, 0.0014340195339173079, 0.012241535820066929, 0, 0, 0, 0, 0, 0, 0);/* layer 721:ADD */
add_fpreq(1, &buffer0[4973000],0.012241535820066929,1,&buffer0[4973000],0.014969825744628906,8,0.025973781943321228,9,&buffer0[4973000]);
/* layer 722:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4973192], (signed char*)&buffer0[4973192], (signed char*)&buffer0[4973192], 1, 3, 1, 3, false, false);
/* layer 723:MUL */
fptr = (float*)(signed char*)&buffer0[4976424];
fptr3 = (float*)(signed char*)&buffer0[4976424];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 724:ADD */
add_fpreq(3, &buffer0[4976428],0.027005920186638832,21,&buffer0[4613776],3.921568403342235e-09,0,0.027005920186638832,21,&buffer0[4046404]);
/* layer 725:SOFTMAX */
softmax((signed char*)&buffer0[4046404], (signed char*)&buffer0[4046408], 1, 3, 1.0);
/* layer 726:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4046400], (signed char*)&buffer0[4976432], NULL, (signed char*)&buffer0[4976432], -16, 0, -14, 0.04663272947072983, 0.001862609526142478, 0.011697896756231785, 0, 0, 0, 0, 0, 0, 0);/* layer 727:ADD */
add_fpreq(1, &buffer0[4973384],0.011697896756231785,-14,&buffer0[4973384],0.0014541356358677149,-12,0.01143968291580677,-10,&buffer0[4973384]);
/* layer 728:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4046408], (signed char*)&buffer0[4973576], (signed char*)&buffer0[4424640], 1, 3, 1, 3, false, false);
/* layer 729:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 730:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 731:REDUCE_PROD */
int reduction_axes_82[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4046412], (int32_t*)(int32_t*)&buffer0[4046420], 2, 1, reduction_axes_82, 4);
/* layer 732:CONCATENATION */
concatenate((int32_t*)&buffer0[4046412], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976440], 0);
/* layer 733:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 734:REDUCE_PROD */
int reduction_axes_83[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4046412], (int32_t*)(int32_t*)&buffer0[4046400], 1, 1, reduction_axes_83, 4);
/* layer 735:PACK */
pack((int32_t*)&buffer0[4046420], (int32_t*)&buffer0[4046400], (int32_t*)&buffer0[4976444], 2, 0);/* layer 736:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976448], (signed char*)&buffer0[4976448], NULL, (signed char*)&buffer0[4976448], -3, 0, -65, 0.005933795124292374, 0.002281743101775646, 0.007975099608302116, 0, 0, 0, 0, 0, 0, 0);/* layer 737:ADD */
add_fpreq(1, &buffer0[4976452],0.007975099608302116,-65,&buffer0[4976452],0.006844973191618919,60,0.004075899254530668,-8,&buffer0[4197696]);
/* layer 738:ADD */
add_fpreq(192, &buffer0[4197696],0.004075899254530668,-8,&buffer0[3857280],0.09201667457818985,-39,0.09341123700141907,-39,&buffer0[4046400]);
/* layer 739:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[4046400], (signed char*)&buffer0[3857280], (signed char*)&buffer0[3857280], 1, 197);
/* layer 740:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4424640],192,1,197,192,1,1,1,-128,127,&buffer0[4197696]);
/* layer 741:ADD */
add_fpreq(197, &buffer0[4197696],0.009823985397815704,-128,&buffer0[4802880],3.921568403342235e-09,0,0.009823985397815704,-128,&buffer0[4197896]);
/* layer 742:RSQRT */
rsqrt((signed char*)&buffer0[4197896], (signed char*)&buffer0[4958376], 1, 197);
/* layer 743:MUL */
fptr = (float*)(signed char*)&buffer0[4197696];
fptr3 = (float*)(signed char*)&buffer0[4235520];
fptr2 = (signed char*)&buffer0[4197696];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 744:MUL */
mul_int8(37824,(signed char*)&buffer0[4273344],(signed char*)&buffer0[4235520],(signed char*)&buffer0[4273344]);
/* layer 745:MUL */
fptr = (signed char*)&buffer0[4235520];
fptr3 = (float*)(signed char*)&buffer0[4311168];fptr2 = (signed char*)&buffer0[4311168];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 746:SUB */
sub_int8(192,(signed char*)&buffer0[4973768], 0, 1, 0,(signed char*)&buffer0[4973768], 0, 1, 0,(signed char*)&buffer0[4235520], 0, 1, 0, 0);
/* layer 747:ADD */
add_fpreq(37824, &buffer0[4348992],0.05619353428483009,26,&buffer0[4235520],0.008994141593575478,23,0.05419570580124855,24,&buffer0[4386816]);
/* layer 748:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 749:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 750:REDUCE_PROD */
int reduction_axes_84[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4235532], (int32_t*)(int32_t*)&buffer0[4235540], 2, 1, reduction_axes_84, 4);
/* layer 751:CONCATENATION */
concatenate((int32_t*)&buffer0[4235532], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976456], 0);
/* layer 752:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 753:REDUCE_PROD */
int reduction_axes_85[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4235532], (int32_t*)(int32_t*)&buffer0[4235520], 1, 1, reduction_axes_85, 4);
/* layer 754:PACK */
pack((int32_t*)&buffer0[4235540], (int32_t*)&buffer0[4235520], (int32_t*)&buffer0[4976460], 2, 0);/* layer 755:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976464], (signed char*)&buffer0[4976464], NULL, (signed char*)&buffer0[4976464], 24, 0, 30, 0.05419570580124855, 0.0020548542961478233, 0.02786640264093876, 0, 0, 0, 0, 0, 0, 0);/* layer 756:ADD */
add_fpreq(1, &buffer0[4976468],0.02786640264093876,30,&buffer0[4976468],0.007671330124139786,64,0.03100898675620556,44,&buffer0[4235520]);
/* layer 757:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[4235520], (signed char*)&buffer0[4236288], 1, 1);
/* layer 758:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 759:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 760:REDUCE_PROD */
int reduction_axes_86[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4235532], (int32_t*)(int32_t*)&buffer0[4235540], 2, 1, reduction_axes_86, 4);
/* layer 761:CONCATENATION */
concatenate((int32_t*)&buffer0[4235532], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976472], 0);
/* layer 762:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 763:REDUCE_PROD */
int reduction_axes_87[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4235532], (int32_t*)(int32_t*)&buffer0[4235520], 1, 1, reduction_axes_87, 4);
/* layer 764:PACK */
pack((int32_t*)&buffer0[4235540], (int32_t*)&buffer0[4235520], (int32_t*)&buffer0[4976476], 2, 0);/* layer 765:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976480], (signed char*)&buffer0[4976480], NULL, (signed char*)&buffer0[4976480], -112, 0, -56, 0.010713178664445877, 0.005930224433541298, 0.013472194783389568, 0, 0, 0, 0, 0, 0, 0);/* layer 766:ADD */
add_fpreq(1, &buffer0[4976484],0.013472194783389568,-56,&buffer0[4976484],0.004097616765648127,-38,0.014029902406036854,-97,&buffer0[4386816]);
/* layer 767:ADD */
add_fpreq(192, &buffer0[4386816],0.014029902406036854,-97,&buffer0[4046400],0.09341123700141907,-39,0.10116223245859146,-46,&buffer0[4235520]);
/* layer 768:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[4235520], (signed char*)&buffer0[4046400], (signed char*)&buffer0[4046400], 1, 197);
/* layer 769:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4424640],192,1,197,192,1,1,1,-128,127,&buffer0[4386816]);
/* layer 770:ADD */
add_fpreq(197, &buffer0[4386816],0.011350908316671848,-128,&buffer0[4802880],3.921568403342235e-09,0,0.011350908316671848,-128,&buffer0[4387016]);
/* layer 771:RSQRT */
rsqrt((signed char*)&buffer0[4387016], (signed char*)&buffer0[4958576], 1, 197);
/* layer 772:MUL */
fptr = (float*)(signed char*)&buffer0[4386816];
fptr3 = (float*)(signed char*)&buffer0[4424640];
fptr2 = (signed char*)&buffer0[4386816];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 773:MUL */
mul_int8(37824,(signed char*)&buffer0[4462464],(signed char*)&buffer0[4424640],(signed char*)&buffer0[4462464]);
/* layer 774:MUL */
fptr = (signed char*)&buffer0[4424640];
fptr3 = (float*)(signed char*)&buffer0[4500288];fptr2 = (signed char*)&buffer0[4500288];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 775:SUB */
sub_int8(192,(signed char*)&buffer0[4973960], 0, 1, 0,(signed char*)&buffer0[4973960], 0, 1, 0,(signed char*)&buffer0[4424640], 0, 1, 0, 0);
/* layer 776:ADD */
add_fpreq(37824, &buffer0[4538112],0.059777382761240005,-31,&buffer0[4424640],0.0123208062723279,27,0.04872538894414902,-29,&buffer0[4575936]);
/* layer 777:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 778:STRIDED_SLICE */
const uint16_t begin12[] = {0, 0, 0, 0};
const uint16_t end12[] = {1, 0, 0, 0};
const uint16_t strides12[] = {1, 0, 0, 0};
strided_slice_4Dto4D_int32((int32_t*)&buffer0[4424640], 3, 1, 1, 1, begin12, end12, strides12, (int32_t*)&buffer0[4424652], 1, 1, 1, 1);
/* layer 779:PACK */
pack((int32_t*)&buffer0[4424652], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613764], (int32_t*)&buffer0[4976488], 4, 0);/* layer 780:PACK */
pack((int32_t*)&buffer0[4424652], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4613768], (int32_t*)&buffer0[4976492], 3, 0);/* layer 781:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 782:REDUCE_PROD */
int reduction_axes_88[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4424652], (int32_t*)(int32_t*)&buffer0[4424660], 2, 1, reduction_axes_88, 4);
/* layer 783:CONCATENATION */
concatenate((int32_t*)&buffer0[4424652], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976496], 0);
/* layer 784:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 785:REDUCE_PROD */
int reduction_axes_89[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4424652], (int32_t*)(int32_t*)&buffer0[4424640], 1, 1, reduction_axes_89, 4);
/* layer 786:PACK */
pack((int32_t*)&buffer0[4424660], (int32_t*)&buffer0[4424640], (int32_t*)&buffer0[4976500], 2, 0);/* layer 787:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4424640], (signed char*)&buffer0[4976504], NULL, (signed char*)&buffer0[4976504], -29, 0, -13, 0.04872538894414902, 0.002158954506739974, 0.02564122900366783, 0, 0, 0, 0, 0, 0, 0);/* layer 788:ADD */
add_fpreq(1, &buffer0[4974152],0.02564122900366783,-13,&buffer0[4974152],0.0056891413405537605,27,0.025951800867915154,-5,&buffer0[4974152]);
/* layer 789:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4424640], (signed char*)&buffer0[4976508], NULL, (signed char*)&buffer0[4976508], -29, 0, 11, 0.04872538894414902, 0.0014787878608331084, 0.012024598196148872, 0, 0, 0, 0, 0, 0, 0);/* layer 790:ADD */
add_fpreq(1, &buffer0[4974344],0.012024598196148872,11,&buffer0[4974344],0.017286360263824463,10,0.02668600343167782,16,&buffer0[4974344]);
/* layer 791:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4974536], (signed char*)&buffer0[4974536], (signed char*)&buffer0[4974536], 1, 3, 1, 3, false, false);
/* layer 792:MUL */
fptr = (float*)(signed char*)&buffer0[4976512];
fptr3 = (float*)(signed char*)&buffer0[4976512];
fptr2 = (signed char*)&buffer0[4613772];
for(int i = 0; i < 3; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 793:ADD */
add_fpreq(3, &buffer0[4976516],0.02998032048344612,20,&buffer0[4613776],3.921568403342235e-09,0,0.02998032048344612,20,&buffer0[4424644]);
/* layer 794:SOFTMAX */
softmax((signed char*)&buffer0[4424644], (signed char*)&buffer0[4424648], 1, 3, 1.0);
/* layer 795:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4424640], (signed char*)&buffer0[4976520], NULL, (signed char*)&buffer0[4976520], -29, 0, 6, 0.04872538894414902, 0.002062201267108321, 0.011426953598856926, 0, 0, 0, 0, 0, 0, 0);/* layer 796:ADD */
add_fpreq(1, &buffer0[4974728],0.011426953598856926,6,&buffer0[4974728],0.002756946487352252,-15,0.011140444315969944,16,&buffer0[4974728]);
/* layer 797:BATCH_MATMUL */
batch_matmul((signed char*)&buffer0[4424648], (signed char*)&buffer0[4974920], (signed char*)&buffer0[4803080], 1, 3, 1, 3, false, false);
/* layer 798:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 799:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 800:REDUCE_PROD */
int reduction_axes_90[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4424652], (int32_t*)(int32_t*)&buffer0[4424660], 2, 1, reduction_axes_90, 4);
/* layer 801:CONCATENATION */
concatenate((int32_t*)&buffer0[4424652], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976528], 0);
/* layer 802:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 803:REDUCE_PROD */
int reduction_axes_91[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4424652], (int32_t*)(int32_t*)&buffer0[4424640], 1, 1, reduction_axes_91, 4);
/* layer 804:PACK */
pack((int32_t*)&buffer0[4424660], (int32_t*)&buffer0[4424640], (int32_t*)&buffer0[4976532], 2, 0);/* layer 805:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976536], (signed char*)&buffer0[4976536], NULL, (signed char*)&buffer0[4976536], 20, 0, -93, 0.0070658582262694836, 0.0040364922024309635, 0.018584299832582474, 0, 0, 0, 0, 0, 0, 0);/* layer 806:ADD */
add_fpreq(1, &buffer0[4976540],0.018584299832582474,-93,&buffer0[4976540],0.006090965121984482,8,0.014101816341280937,-104,&buffer0[4575936]);
/* layer 807:ADD */
add_fpreq(192, &buffer0[4575936],0.014101816341280937,-104,&buffer0[4235520],0.10116223245859146,-46,0.11278730630874634,-56,&buffer0[4424640]);
/* layer 808:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[4424640], (signed char*)&buffer0[4235520], (signed char*)&buffer0[4235520], 1, 197);
/* layer 809:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4803080],192,1,197,192,1,1,1,-128,127,&buffer0[4575936]);
/* layer 810:ADD */
add_fpreq(197, &buffer0[4575936],0.013707119040191174,-128,&buffer0[4802880],3.921568403342235e-09,0,0.013707119040191174,-128,&buffer0[4576136]);
/* layer 811:RSQRT */
rsqrt((signed char*)&buffer0[4576136], (signed char*)&buffer0[4958776], 1, 197);
/* layer 812:MUL */
fptr = (float*)(signed char*)&buffer0[4575936];
fptr3 = (float*)(signed char*)&buffer0[4613760];
fptr2 = (signed char*)&buffer0[4575936];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 813:MUL */
mul_int8(37824,(signed char*)&buffer0[4651584],(signed char*)&buffer0[4613760],(signed char*)&buffer0[4651584]);
/* layer 814:MUL */
fptr = (signed char*)&buffer0[4613760];
fptr3 = (float*)(signed char*)&buffer0[4689408];fptr2 = (signed char*)&buffer0[4689408];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 815:SUB */
sub_int8(192,(signed char*)&buffer0[4975112], 0, 1, 0,(signed char*)&buffer0[4975112], 0, 1, 0,(signed char*)&buffer0[4613760], 0, 1, 0, 0);
/* layer 816:ADD */
add_fpreq(37824, &buffer0[4727232],0.043600086122751236,31,&buffer0[4613760],0.007779769599437714,62,0.04174940660595894,36,&buffer0[4765056]);
/* layer 817:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 818:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 819:REDUCE_PROD */
int reduction_axes_92[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4613772], (int32_t*)(int32_t*)&buffer0[4613780], 2, 1, reduction_axes_92, 4);
/* layer 820:CONCATENATION */
concatenate((int32_t*)&buffer0[4613772], (int32_t*)&buffer0[4803280], (int32_t*)&buffer0[4976544], 0);
/* layer 821:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 822:REDUCE_PROD */
int reduction_axes_93[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4613772], (int32_t*)(int32_t*)&buffer0[4613760], 1, 1, reduction_axes_93, 4);
/* layer 823:PACK */
pack((int32_t*)&buffer0[4613780], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4976548], 2, 0);/* layer 824:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976552], (signed char*)&buffer0[4976552], NULL, (signed char*)&buffer0[4976552], 36, 0, 47, 0.04174940660595894, 0.0032202221918851137, 0.021232130005955696, 0, 0, 0, 0, 0, 0, 0);/* layer 825:ADD */
add_fpreq(1, &buffer0[4976556],0.021232130005955696,47,&buffer0[4976556],0.0111541748046875,-16,0.026827337220311165,32,&buffer0[4613760]);
/* layer 826:PLACEHOLDER_FOR_GREATER_OP_CODES */
placeholder_for_greater_op_codes((signed char*)&buffer0[4613760], (signed char*)&buffer0[4614528], 1, 1);
/* layer 827:SHAPE */
shape(&buffer0[0], &buffer0[0], 3);/* layer 828:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 829:REDUCE_PROD */
int reduction_axes_94[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4613772], (int32_t*)(int32_t*)&buffer0[4613780], 2, 1, reduction_axes_94, 4);
/* layer 830:CONCATENATION */
concatenate((int32_t*)&buffer0[4613772], (int32_t*)&buffer0[4803272], (int32_t*)&buffer0[4976560], 0);
/* layer 831:GATHER */
gather(&buffer0[0], &buffer0[0]);/* layer 832:REDUCE_PROD */
int reduction_axes_95[] = {0, 0, 0, 0};
reduce_prod_int32((int32_t*)(int32_t*)&buffer0[4613772], (int32_t*)(int32_t*)&buffer0[4613760], 1, 1, reduction_axes_95, 4);
/* layer 833:PACK */
pack((int32_t*)&buffer0[4613780], (int32_t*)&buffer0[4613760], (int32_t*)&buffer0[4976564], 2, 0);/* layer 834:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4976568], (signed char*)&buffer0[4976568], NULL, (signed char*)&buffer0[4976568], -112, 0, 58, 0.010636428371071815, 0.00647391565144062, 0.01380007341504097, 0, 0, 0, 0, 0, 0, 0);/* layer 835:ADD */
add_fpreq(1, &buffer0[4976572],0.01380007341504097,58,&buffer0[4976572],0.006615052465349436,-71,0.007185020018368959,47,&buffer0[4765056]);
/* layer 836:ADD */
add_fpreq(192, &buffer0[4765056],0.007185020018368959,47,&buffer0[4424640],0.11278730630874634,-56,0.1058778464794159,-57,&buffer0[4613760]);
/* layer 837:SQUARED_DIFFERENCE */
squared_difference((signed char*)&buffer0[4613760], (signed char*)&buffer0[4424640], (signed char*)&buffer0[4424640], 1, 197);
/* layer 838:AVERAGE_POOL_2D */
avg_pooling(&buffer0[4803080],192,1,197,192,1,1,1,-128,127,&buffer0[4613760]);
/* layer 839:ADD */
add_fpreq(197, &buffer0[4613760],0.01261476892977953,-128,&buffer0[4802880],3.921568403342235e-09,0,0.01261476892977953,-128,&buffer0[4613960]);
/* layer 840:RSQRT */
rsqrt((signed char*)&buffer0[4613960], (signed char*)&buffer0[4958976], 1, 197);
/* layer 841:MUL */
fptr = (float*)(signed char*)&buffer0[4613760];
fptr3 = (float*)(signed char*)&buffer0[4765056];
fptr2 = (signed char*)&buffer0[4613760];
for(int i = 0; i < 37824; i++) fptr3[i] = *fptr2 * fptr[i];
/* layer 842:MUL */
mul_int8(37824,(signed char*)&buffer0[4802880],(signed char*)&buffer0[4765056],(signed char*)&buffer0[4802880]);
/* layer 843:MUL */
fptr = (signed char*)&buffer0[4765056];
fptr3 = (float*)(signed char*)&buffer0[4840704];fptr2 = (signed char*)&buffer0[4840704];
for(int hw = 0; hw < 192; hw++){
for(int i = 0; i < 197; i++) *fptr3++ = fptr2[i] * *fptr++;
}
/* layer 844:SUB */
sub_int8(192,(signed char*)&buffer0[4975304], 0, 1, 0,(signed char*)&buffer0[4975304], 0, 1, 0,(signed char*)&buffer0[4765056], 0, 1, 0, 0);
/* layer 845:ADD */
add_fpreq(37824, &buffer0[4878528],0.0574597604572773,13,&buffer0[4765056],0.01044915709644556,27,0.057887349277734756,17,&buffer0[4916352]);
/* layer 846:STRIDED_SLICE */
const uint16_t begin13[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint16_t end13[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
const uint16_t strides13[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
strided_slice_4Dto4D_int8((signed char*)&buffer0[4916352], 1, 197, 192, 1, begin13, end13, strides13, (signed char*)&buffer0[4765056], 1, 192, 1, 1);
/* layer 847:FULLY_CONNECTED */
fully_connected((signed char*)&buffer0[4765056], (signed char*)&buffer0[4765248], (signed char*)&buffer0[4765248], (signed char*)&buffer0[4765248], 17, 0, -22, 0.057887349277734756, 0.0016220927936956286, 0.0074904668144881725, 0, 0, 0, 0, 0, 0, 0);}
void genModel(signed char* input, signed char* output){
    invoke_inf();
    }