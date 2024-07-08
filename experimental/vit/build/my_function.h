#include <cstring>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// 양자화된 텐서의 덧셈을 수행하는 함수
void add_fpreq(int length, int8_t* input1, float input1_scale, int input1_zero_point,
               int8_t* input2, float input2_scale, int input2_zero_point,
               float output_scale, int output_zero_point, int8_t* output) {
    for (int i = 0; i < length; ++i) {
        // 입력 텐서를 실수값으로 변환
        float real_input1 = (input1[i] - input1_zero_point) * input1_scale;
        float real_input2 = (input2[i] - input2_zero_point) * input2_scale;

        // 두 입력 텐서를 더함
        float real_output = real_input1 + real_input2;

        // 결과를 양자화된 값으로 변환
        int32_t quantized_output = static_cast<int32_t>(std::round(real_output / output_scale)) + output_zero_point;

        // 출력 범위를 -128에서 127 사이로 제한
        output[i] = static_cast<int8_t>(std::max(-128, std::min(127, quantized_output)));
    }
}

// avg_pooling 함수의 정의
void avg_pooling(int8_t* input, int input_h, int input_w, int input_c,
                 int filter_h, int filter_w, int stride_h, int stride_w,
                 int pad_value, int min_value, int max_value,
                 int8_t* output) {
    // 출력 크기 계산
    int output_h = (input_h - filter_h) / stride_h + 1;
    int output_w = (input_w - filter_w) / stride_w + 1;

    // 입력 텐서를 순회하면서 평균 풀링 연산 수행
    for (int h = 0; h < output_h; ++h) {
        for (int w = 0; w < output_w; ++w) {
            for (int c = 0; c < input_c; ++c) {
                int32_t sum = 0;
                for (int fh = 0; fh < filter_h; ++fh) {
                    for (int fw = 0; fw < filter_w; ++fw) {
                        int ih = h * stride_h + fh;
                        int iw = w * stride_w + fw;
                        if (ih < 0 || ih >= input_h || iw < 0 || iw >= input_w) {
                            // 패딩 값 사용
                            sum += pad_value;
                        } else {
                            sum += input[(ih * input_w + iw) * input_c + c];
                        }
                    }
                }
                // 평균 값 계산
                int filter_area = filter_h * filter_w;
                int32_t avg = sum / filter_area;
                // 값 제한 (클램핑)
                avg = std::max(min_value, std::min(max_value, avg));
                // 출력에 저장
                output[(h * output_w + w) * input_c + c] = static_cast<int8_t>(avg);
            }
        }
    }
}

// batch_matmul 함수의 정의
void batch_matmul(float* input, float* input2, float* output, 
                  int batch_size, int M, int K, int N, 
                  bool adj_x, bool adj_y) {
    for (int b = 0; b < batch_size; ++b) {
        float* A = input + b * M * K;
        float* B = input2 + b * K * N;
        float* C = output + b * M * N;

        // Transpose A if adj_x is true
        float* tempA = nullptr;
        if (adj_x) {
            tempA = new float[M * K];
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < K; ++j) {
                    tempA[j * M + i] = A[i * K + j];
                }
            }
            A = tempA;
            std::swap(M, K);
        }

        // Transpose B if adj_y is true
        float* tempB = nullptr;
        if (adj_y) {
            tempB = new float[K * N];
            for (int i = 0; i < K; ++i) {
                for (int j = 0; j < N; ++j) {
                    tempB[j * K + i] = B[i * N + j];
                }
            }
            B = tempB;
            std::swap(K, N);
        }

        // Initialize C to zero
        std::memset(C, 0, sizeof(float) * M * N);

        // Perform matrix multiplication
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < K; ++k) {
                    C[i * N + j] += A[i * K + k] * B[k * N + j];
                }
            }
        }

        // Clean up temporary arrays
        if (adj_x) {
            delete[] A;
        }
        if (adj_y) {
            delete[] B;
        }
    }
}

// concatenate 함수의 정의
void concatenate(float* input1, float* input2, float* output, int axis, 
                 int* input1_shape, int* input2_shape, int* output_shape, int dims) {
    int input1_size = 1;
    int input2_size = 1;
    int output_size = 1;
    
    // Calculate the size of each input and output tensor
    for (int i = 0; i < dims; ++i) {
        input1_size *= input1_shape[i];
        input2_size *= input2_shape[i];
        output_size *= output_shape[i];
    }

    if (axis == 0) {
        std::memcpy(output, input1, sizeof(float) * input1_size);
        std::memcpy(output + input1_size, input2, sizeof(float) * input2_size);
    } else if (axis == 1) {
        for (int i = 0; i < input1_shape[0]; ++i) {
            std::memcpy(output + i * output_shape[1], 
                        input1 + i * input1_shape[1], 
                        sizeof(float) * input1_shape[1]);
            std::memcpy(output + i * output_shape[1] + input1_shape[1], 
                        input2 + i * input2_shape[1], 
                        sizeof(float) * input2_shape[1]);
        }
    }
    // Handle other axes if necessary
}

// conv2d_16x16_fpreq 함수의 정의
void conv2d_16x16_fpreq(const int8_t* input, int input_w, int input_h, int input_c,
                        const int8_t* weights, const int32_t* bias, const float* scales,
                        int output_w, int output_h, int output_c,
                        int8_t* output, int stride, int pad, int input_zero_point, int output_zero_point, int min_val, int max_val) {
    for (int oc = 0; oc < output_c; ++oc) {
        for (int oh = 0; oh < output_h; ++oh) {
            for (int ow = 0; ow < output_w; ++ow) {
                // 초기화
                int32_t acc = bias[oc];
                
                // 커널 순회
                for (int kh = 0; kh < 16; ++kh) {
                    for (int kw = 0; kw < 16; ++kw) {
                        int ih = oh * stride + kh - pad;
                        int iw = ow * stride + kw - pad;
                        if (ih >= 0 && ih < input_h && iw >= 0 && iw < input_w) {
                            for (int ic = 0; ic < input_c; ++ic) {
                                int32_t input_val = input[(ih * input_w + iw) * input_c + ic];
                                int32_t weight_val = weights[((((oc * 16 + kh) * 16 + kw) * input_c) + ic)];
                                acc += (input_val - input_zero_point) * weight_val;
                            }
                        }
                    }
                }
                
                // Quantization and activation
                float scaled_acc = acc * scales[oc];
                int32_t output_val = static_cast<int32_t>(round(scaled_acc)) + output_zero_point;
                output_val = std::min(std::max(output_val, min_val), max_val);
                output[(oh * output_w + ow) * output_c + oc] = static_cast<int8_t>(output_val);
            }
        }
    }
}

// fully_connected 함수의 정의
void fully_connected(const int8_t* input, const int8_t* weights, const int8_t* bias, int8_t* output,
                     int input_zero_point, int weight_zero_point, int output_zero_point,
                     float input_scale, float weight_scale, float output_scale,
                     int input_multiplier, int weight_multiplier, int output_multiplier,
                     int input_shift, int weight_shift, int output_shift,
                     int activation, int input_size, int output_size) {
    // 출력을 0으로 초기화
    std::memset(output, 0, sizeof(int8_t) * output_size);

    for (int out_idx = 0; out_idx < output_size; ++out_idx) {
        int32_t acc = bias[out_idx];

        for (int in_idx = 0; in_idx < input_size; ++in_idx) {
            int32_t input_val = input[in_idx] - input_zero_point;
            int32_t weight_val = weights[out_idx * input_size + in_idx] - weight_zero_point;
            acc += input_val * weight_val;
        }

        // 양자화된 값으로 변환
        float scaled_acc = acc * input_scale * weight_scale / output_scale;
        int32_t quantized_output = static_cast<int32_t>(round(scaled_acc)) + output_zero_point;

        // 값 제한 (클램핑)
        if (activation == 1) { // Assuming 1 represents ReLU
            quantized_output = std::max(0, quantized_output);
        }

        quantized_output = std::min(127, std::max(-128, quantized_output));
        output[out_idx] = static_cast<int8_t>(quantized_output);
    }
}

// gather 함수의 정의
void gather(const int32_t* input, const int32_t* indices, int32_t* output, int num_indices, int input_size) {
    for (int i = 0; i < num_indices; ++i) {
        int idx = indices[i];
        if (idx < 0 || idx >= input_size) {
            // 인덱스가 유효하지 않은 경우 0을 반환하거나 다른 오류 처리를 할 수 있음
            output[i] = 0; // 또는 적절한 오류 처리를 추가
        } else {
            output[i] = input[idx];
        }
    }
}

// mul_int8 함수의 정의
void mul_int8(int size, const int8_t* input1, const int8_t* input2, int8_t* output,
              int input1_zero_point, int input2_zero_point, int output_zero_point,
              float input1_scale, float input2_scale, float output_scale) {
    for (int i = 0; i < size; ++i) {
        // 입력 값을 실수 값으로 변환
        float real_input1 = (input1[i] - input1_zero_point) * input1_scale;
        float real_input2 = (input2[i] - input2_zero_point) * input2_scale;

        // 두 입력 값을 곱함
        float real_output = real_input1 * real_input2;

        // 결과 값을 양자화
        int32_t quantized_output = static_cast<int32_t>(real_output / output_scale) + output_zero_point;

        // 값 제한 (클램핑)
        quantized_output = std::min(127, std::max(-128, quantized_output));
        output[i] = static_cast<int8_t>(quantized_output);
    }
}


// pack 함수의 정의
void pack(const float** inputs, float* output, int num_inputs, int axis, const int* input_shape, const int* output_shape, int dims) {
    int input_size = 1;
    for (int i = 0; i < dims; ++i) {
        input_size *= input_shape[i];
    }

    int output_stride = 1;
    for (int i = axis + 1; i < dims; ++i) {
        output_stride *= output_shape[i];
    }

    int output_offset = 0;
    for (int i = 0; i < num_inputs; ++i) {
        const float* input = inputs[i];
        for (int j = 0; j < input_size; ++j) {
            int out_idx = output_offset + (j / output_stride) * output_stride * num_inputs + (j % output_stride);
            output[out_idx] = input[j];
        }
        output_offset += output_stride;
    }
}

// placeholder_for_greater_op_codes 함수의 정의
void placeholder_for_greater_op_codes(const float* input, float* output, int input_dim1, int input_dim2) {
    // 입력 텐서를 출력 텐서로 단순히 복사하는 기본 구현
    for (int i = 0; i < input_dim1; ++i) {
        for (int j = 0; j < input_dim2; ++j) {
            output[i * input_dim2 + j] = input[i * input_dim2 + j];
        }
    }
}


// reduce_prod_int32 함수의 정의
void reduce_prod_int32(const int32_t* input, int32_t* output, int input_size, int output_size, const int* reduction_axes, int num_reduction_axes) {
    // 초기화: 출력 배열의 모든 요소를 1로 설정
    for (int i = 0; i < output_size; ++i) {
        output[i] = 1;
    }

    // 각 입력 요소를 순회하며 곱셈을 수행
    for (int i = 0; i < input_size; ++i) {
        int output_index = 0;

        // 입력 인덱스를 출력 인덱스로 매핑
        for (int j = 0; j < num_reduction_axes; ++j) {
            output_index += (i / reduction_axes[j]) % output_size;
        }

        // 입력 값을 출력 배열의 해당 요소에 곱함
        output[output_index] *= input[i];
    }
}

// rsqrt 함수의 정의
void rsqrt(const float* input, float* output, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            output[index] = 1.0f / std::sqrt(input[index]);
        }
    }
}

// shape 함수의 정의
void shape(const int8_t* input, int32_t* output, int num_dims, const int32_t* input_shape) {
    std::memcpy(output, input_shape, num_dims * sizeof(int32_t));
}

// softmax 함수의 정의
void softmax(const float* input, float* output, int batch_size, int num_classes, float beta) {
    for (int b = 0; b < batch_size; ++b) {
        const float* input_batch = input + b * num_classes;
        float* output_batch = output + b * num_classes;

        // 입력 텐서의 최대값 찾기
        float max_val = *std::max_element(input_batch, input_batch + num_classes);

        // 지수 함수의 합 계산
        float sum = 0.0f;
        for (int i = 0; i < num_classes; ++i) {
            output_batch[i] = std::exp((input_batch[i] - max_val) * beta);
            sum += output_batch[i];
        }

        // 출력 텐서에 소프트맥스 값 저장
        for (int i = 0; i < num_classes; ++i) {
            output_batch[i] /= sum;
        }
    }
}

// squared_difference 함수의 정의
void squared_difference(const float* input1, const float* input2, float* output, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            float diff = input1[index] - input2[index];
            output[index] = diff * diff;
        }
    }
}

// strided_slice_4Dto4D_int32 함수 정의
void strided_slice_4Dto4D_int32(const int32_t* input, int d1, int d2, int d3, int d4,
                                const uint16_t* begin, const uint16_t* end, const uint16_t* strides,
                                int32_t* output, int o_d1, int o_d2, int o_d3, int o_d4) {
    int input_shape[] = {d1, d2, d3, d4};
    int output_shape[] = {o_d1, o_d2, o_d3, o_d4};
    int input_strides[4] = {d2 * d3 * d4, d3 * d4, d4, 1};
    int output_strides[4] = {o_d2 * o_d3 * o_d4, o_d3 * o_d4, o_d4, 1};

    for (int i = 0; i < o_d1; ++i) {
        for (int j = 0; j < o_d2; ++j) {
            for (int k = 0; k < o_d3; ++k) {
                for (int l = 0; l < o_d4; ++l) {
                    int input_idx = (begin[0] + i * strides[0]) * input_strides[0] +
                                    (begin[1] + j * strides[1]) * input_strides[1] +
                                    (begin[2] + k * strides[2]) * input_strides[2] +
                                    (begin[3] + l * strides[3]) * input_strides[3];
                    int output_idx = i * output_strides[0] + j * output_strides[1] + k * output_strides[2] + l * output_strides[3];
                    output[output_idx] = input[input_idx];
                }
            }
        }
    }
}

// strided_slice_4Dto4D_int8 함수 정의
void strided_slice_4Dto4D_int8(const int8_t* input, int d1, int d2, int d3, int d4,
                               const uint16_t* begin, const uint16_t* end, const uint16_t* strides,
                               int8_t* output, int o_d1, int o_d2, int o_d3, int o_d4) {
    int input_shape[] = {d1, d2, d3, d4};
    int output_shape[] = {o_d1, o_d2, o_d3, o_d4};
    int input_strides[4] = {d2 * d3 * d4, d3 * d4, d4, 1};
    int output_strides[4] = {o_d2 * o_d3 * o_d4, o_d3 * o_d4, o_d4, 1};

    for (int i = 0; i < o_d1; ++i) {
        for (int j = 0; j < o_d2; ++j) {
            for (int k = 0; k < o_d3; ++k) {
                for (int l = 0; l < o_d4; ++l) {
                    int input_idx = (begin[0] + i * strides[0]) * input_strides[0] +
                                    (begin[1] + j * strides[1]) * input_strides[1] +
                                    (begin[2] + k * strides[2]) * input_strides[2] +
                                    (begin[3] + l * strides[3]) * input_strides[3];
                    int output_idx = i * output_strides[0] + j * output_strides[1] + k * output_strides[2] + l * output_strides[3];
                    output[output_idx] = input[input_idx];
                }
            }
        }
    }
}

// sub_int8 함수의 정의
void sub_int8(int size, const int8_t* input1, int input1_zero_point, int input1_multiplier, int input1_shift,
              const int8_t* input2, int input2_zero_point, int input2_multiplier, int input2_shift,
              int8_t* output, int output_zero_point, int output_multiplier, int output_shift, int left_shift) {
    for (int i = 0; i < size; ++i) {
        // 입력값을 실수 범위로 변환
        int32_t input1_val = (input1[i] - input1_zero_point) << left_shift;
        int32_t input2_val = (input2[i] - input2_zero_point) << left_shift;

        // 입력값에 승수 및 시프트 적용
        int32_t input1_scaled = input1_val * input1_multiplier >> input1_shift;
        int32_t input2_scaled = input2_val * input2_multiplier >> input2_shift;

        // 뺄셈 수행
        int32_t raw_output = input1_scaled - input2_scaled;

        // 출력 승수 및 시프트 적용
        int32_t scaled_output = raw_output * output_multiplier >> output_shift;

        // 제로 포인트 추가 및 클램핑
        int32_t final_output = scaled_output + output_zero_point;
        final_output = std::min(127, std::max(-128, final_output));

        // 결과를 출력 배열에 저장
        output[i] = static_cast<int8_t>(final_output);
    }
}

// tile_3D_int8 함수 정의
void tile_3D_int8(const int8_t* input, int input_h, int input_w, int input_c,
                  int8_t* output, int output_h, int output_w, int output_c) {
    int input_size = input_h * input_w * input_c;
    int output_size = output_h * output_w * output_c;

    // 반복 횟수 계산
    int rep_h = output_h / input_h;
    int rep_w = output_w / input_w;
    int rep_c = output_c / input_c;

    for (int oh = 0; oh < output_h; ++oh) {
        for (int ow = 0; ow < output_w; ++ow) {
            for (int oc = 0; oc < output_c; ++oc) {
                // 입력 인덱스 계산
                int ih = oh % input_h;
                int iw = ow % input_w;
                int ic = oc % input_c;

                // 입력 및 출력 인덱스 계산
                int input_index = (ih * input_w + iw) * input_c + ic;
                int output_index = (oh * output_w + ow) * output_c + oc;

                // 값 복사
                output[output_index] = input[input_index];
            }
        }
    }
}
