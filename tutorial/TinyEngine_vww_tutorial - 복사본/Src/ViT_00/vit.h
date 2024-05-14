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
#include "arm_math.h"
#include "arm_nnfunctions.h"
//#include <tinyengine_function.h>
//#include <tinyengine_function_fp.h>

//#include "genNN.h"
//#include "genModel.h"
//#include "genInclude.h"

/* sram:93992, flash:469432 */
#define PEAK_MEM 93992
#define MODEL_SIZE 469432

static signed char buffer[93992];
static signed char *buffer0 = &buffer[0];

signed char* getInput();
signed char* getOutput();

signed char* getInput() {
    return &buffer0[25600];
}

//static signed char buffer[93992];
//static signed char *buffer0 = &buffer[0];
const int SBuffer_size = 3528;
const int KBuffer_size = 864;


float float_image[6400]; //80 x 80 image
const int _size = 80;
const int _num_heads = 3;
const float _weights[6400] = {0, };
const float _bias[6400] = {0, };

int vision_transformer(float* input);

int main_vision_transformer(uint16_t* input){

	for(int i = 0; i < 6400; i++) {
		float_image[i] = (float)input[i];
	}

	return vision_transformer(float_image);
};



#endif /* VIT_00_VIT_H_ */
