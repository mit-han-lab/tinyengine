/*
 * vit.h
 *
 *  Created on: Apr 20, 2024
 *      Author: sm5pyo
 */

#ifndef VIT_00_VIT_H_
#define VIT_00_VIT_H_


/* Automatically generated source file */
#include <float.h>
#include "./arm_cmsis/arm_math.h"
#include "./arm_cmsis/arm_nnfunctions.h"
//#include <tinyengine_function.h>
//#include <tinyengine_function_fp.h>

//#include "genNN.h"
//#include "genModel.h"
//#include "genInclude.h"

/* sram:93992, flash:469432 */
#define PEAK_MEM 93992
#define MODEL_SIZE 469432

//static signed char buffer[93992];
//static signed char *buffer0 = &buffer[0];
const int SBuffer_size = 3528;
const int KBuffer_size = 864;



const int _size = 80;
const int _num_heads = 3;
const float _weights[6400] = {0.0f};
const float _bias[6400] = {0.0f};

int main_vision_transformer(uint16_t* input);


#endif /* VIT_00_VIT_H_ */
