#include "vit.h"
#include "profile.h"
#include "./arm_cmsis/arm_math.h"
#include "./arm_cmsis/arm_nnfunctions.h"
#include <math.h>

// Layer Normalization Layer
//void layer_normalization(float* input, float* output, uint16_t size, float eps = 1e-5) {
void layer_normalization(float* input, float* output, float eps = 1e-5) {
	printLog("layer_normalization \n\r");
	float mean = 0.0f;
    float var = 0.0f;

    // Calculate mean
    for(int i = 0; i < _size; i++) {
        mean += input[i];
    }
    mean /= _size;

    // Calculate variance
    for(int i = 0; i < _size; i++) {
        var += (input[i] - mean) * (input[i] - mean);
    }
    var /= _size;

    // Normalize
    for(int i = 0; i < _size; i++) {
        output[i] = (input[i] - mean) / sqrtf(var + eps);
    }
}



void arm_softmax_f32(float* input, int length, float* output) {
	printLog("arm_softmax_f32 \n\r");
    float sum = 0.0f;

    // Compute exponent of each input element and sum
    for (int i = 0; i < length; i++) {
        output[i] = expf(input[i]);
        sum += output[i];
    }

    // Normalize
    for (int i = 0; i < length; i++) {
        output[i] /= sum;
    }
}
// Multi-head Self Attention Layer
//void multi_head_self_attention(q7_t* input, q7_t* output, uint16_t size, int num_heads) {
void multi_head_self_attention(float* input, float* output) {
	printLog("multi_head_self_attention \n\r");
    // Assume that the input is already transformed into Q, K, V matrices
	float* Q = input;
	float* K = input + _size;
	float* V = input + 2 * _size;

    // Calculate attention scores
    for(int i = 0; i < _size; i++) {
    	q7_t sum = 0.0f;
        for(int j = 0; j < _size; j++) {
            sum += Q[i] * K[j];
        }
        output[i] = sum / sqrtf(_size);
    }

    // Apply softmax 'void arm_softmax_q7(const q7_t*, uint16_t, q7_t*)'
//    arm_softmax_q7(output, size, output);
    arm_softmax_f32(output, _size, output);


    // Multiply with V
    for(int i = 0; i < _size; i++) {
        output[i] *= V[i];
    }
}

// Linear Layer
//void linear_layer(float* input, float* output, uint16_t size, float* weights, float* bias) {
void linear_layer(float* input, float* output) {
	printLog("linear_layer \n\r");
    for(int i = 0; i < _size; i++) {
        output[i] = 0.0f;
        for(int j = 0; j < _size; j++) {
            output[i] += input[j] * _weights[i * _size + j];
        }
        output[i] += _bias[i];
    }
}

// Classification Layer
int classification_layer(float* input) {
	printLog("classification_layer \n\r");
    int max_index = 0;
    float max_value = input[0];
    for(int i = 1; i < _size; i++) {
        if(input[i] > max_value) {
            max_value = input[i];
            max_index = i;
        }
    }
    return max_index;
}

// Vision Transformer
//int vision_transformer(float* input, uint16_t size, int num_heads, float* weights, float* bias) {

int vision_transformer(float* input) {
	printLog("vision_transformer \n\r");
	int area = _size*_size;
    float* output = new float[area];

    // Projection
    linear_layer(input, output);

    // Multi-head Self Attention
    multi_head_self_attention(output, output);

    // Classification
    int class_index = classification_layer(output);
    char str[200];
    sprintf(str, "classification_layer : %d \n\r", class_index);
    printLog(str);
    delete[] output;

    return class_index;
}

static float float_image_test[6400] = {0.0f}; //80 x 80 image
int main_vision_transformer(uint16_t* input){
	printLog("main_vision_transformer \n\r");

	for(int i = 0; i < 6400; i++) {
		float_image_test[i] = (float)input[i];
	}

	printLog("before vision_transformer \n\r");
	int ret = vision_transformer(float_image_test);

	return ret;
};
