#include <cmath>

#include "operators.h"
#include "utils.h"

#define MAX_TEST_MEMORY_BUF 1024 * 1024 * 1024  // 1 GB
static char buffer[MAX_TEST_MEMORY_BUF];

class MemoryAllocator {
   public:
    MemoryAllocator() { this->counter = 0; }
    float* get_fpbuffer(int size) {
        int byte_size = size * sizeof(float);
        if (this->counter + byte_size > MAX_TEST_MEMORY_BUF) {
            throw("Memory allocation fails! Test case uses too much memory.");
        }
        int cur_counter = counter;
        this->counter += ((byte_size + 3) / 4) * 4;
        return (float*)&buffer[cur_counter];
    }
    int8_t* get_int8buffer(int size) {
        int byte_size = size * sizeof(int8_t);
        if (this->counter + byte_size > MAX_TEST_MEMORY_BUF) {
            throw("Memory allocation fails! Test case uses too much memory.");
        }
        int cur_counter = counter;
        this->counter += ((byte_size + 3) / 4) * 4;
        return (int8_t*)&buffer[cur_counter];
    }
    int* get_intbuffer(int size) {
        int byte_size = size * sizeof(int);
        if (this->counter + byte_size > MAX_TEST_MEMORY_BUF) {
            throw("Memory allocation fails! Test case uses too much memory.");
        }
        int cur_counter = counter;
        this->counter += ((byte_size + 3) / 4) * 4;
        return (int*)&buffer[cur_counter];
    }

   private:
    int counter;
};

void test_LayerNormQ() {
    const int b = 1, m = 512, n = 768;
    MemoryAllocator mem_buf;

    float* intput_arr = mem_buf.get_fpbuffer(b * m * n);
    float* weight_arr = mem_buf.get_fpbuffer(b * n);
    float* bias_arr = mem_buf.get_fpbuffer(b * n);
    int8_t* output_arr = mem_buf.get_int8buffer(b * m * n);
    int8_t* GToutput_arr = mem_buf.get_int8buffer(b * m * n);

    Matrix3D<float> input(intput_arr, b, m, n);
    Matrix3D<float> weight(weight_arr, b, 1, n);
    Matrix3D<float> bias(bias_arr, b, 1, n);
    Matrix3D<int8_t> output(output_arr, b, m, n);
    Matrix3D<int8_t> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/LayerNormQ_bias.bin", bias_arr, b * n);
    read_to_array((char*)"assets/LayerNormQ_x.bin", intput_arr, b * m * n);
    read_to_array((char*)"assets/LayerNormQ_weight.bin", weight_arr, b * n);
    read_to_array((char*)"assets/LayerNormQ_out.bin", GToutput_arr, b * m * n);

    struct LayerNormQ_params op_params = {weight, bias};

    LayerNormQ test_op = LayerNormQ(op_params);

    test_op.forward(input, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_LayerNorm() {
    const int b = 1, m = 108, n = 768;
    MemoryAllocator mem_buf;

    float* intput_arr = mem_buf.get_fpbuffer(b * m * n);
    float* weight_arr = mem_buf.get_fpbuffer(b * n);
    float* bias_arr = mem_buf.get_fpbuffer(b * n);
    float* output_arr = mem_buf.get_fpbuffer(b * m * n);
    float* GToutput_arr = mem_buf.get_fpbuffer(b * m * n);

    Matrix3D<float> input(intput_arr, b, m, n);
    Matrix3D<float> weight(weight_arr, b, 1, n);
    Matrix3D<float> bias(bias_arr, b, 1, n);
    Matrix3D<float> output(output_arr, b, m, n);
    Matrix3D<float> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/decoder/final_layer_norm/bias.bin", bias_arr, b * n);
    read_to_array((char*)"assets/tests/decoder/final_layer_norm_hidden_states.bin", intput_arr, b * m * n);
    read_to_array((char*)"assets/decoder/final_layer_norm/weight.bin", weight_arr, b * n);
    read_to_array((char*)"assets/tests/decoder/final_layer_norm_output.bin", GToutput_arr, b * m * n);

    struct LayerNorm_params op_params = {weight, bias};

    LayerNorm test_op = LayerNorm(op_params);

    test_op.forward(input, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_W8A8B8O8LinearReLU() {
    const int b = 1, m = 512, k = 768, n = 3072;
    const float alpha = 0.0005035400390625, beta = 0.02130126953125;
    MemoryAllocator mem_buf;

    int8_t* intput_arr = mem_buf.get_int8buffer(b * m * k);
    int8_t* weight_arr = mem_buf.get_int8buffer(b * k * n);
    int8_t* biasint8_arr = mem_buf.get_int8buffer(b * n);
    int32_t* biasint32_arr = mem_buf.get_intbuffer(b * n);
    int8_t* output_arr = mem_buf.get_int8buffer(b * m * n);
    int8_t* GToutput_arr = mem_buf.get_int8buffer(b * m * n);

    Matrix3D<int8_t> input(intput_arr, b, m, k);
    Matrix3D<int8_t> weight(weight_arr, b, n, k);
    Matrix3D<int32_t> bias(biasint32_arr, b, 1, n);
    Matrix3D<int8_t> output(output_arr, b, m, n);
    Matrix3D<int8_t> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/W8A8B8O8LinearReLU_x.bin", intput_arr, b * m * k);
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_weight.bin", weight_arr, b * n * k);
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_bias.bin", biasint8_arr, b * n);
    for (int i = 0; i < b * n; i++) {
        float int8_v = static_cast<float>(biasint8_arr[i]);
        float bias_effective_scale = int8_v * (beta / alpha);
        biasint32_arr[i] = static_cast<int32_t>(std::round(bias_effective_scale));
    }
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_y.bin", GToutput_arr, m * n);

    struct W8A8B8O8Linear_params op_params = {weight, bias, alpha, beta};

    W8A8B8O8LinearReLU test_op = W8A8B8O8LinearReLU(op_params);

    test_op.forward(input, output);

    assert(check_two_equal(output_arr, GToutput_arr, m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_W8A8BFP32OFP32Linear() {
    const int b = 1, m = 512, k = 768, n = 768;
    const float alpha = 0.00004565715789794922;
    MemoryAllocator mem_buf;

    int8_t* intput_arr = mem_buf.get_int8buffer(b * m * k);
    int8_t* weight_arr = mem_buf.get_int8buffer(b * k * n);
    float* bias_arr = mem_buf.get_fpbuffer(b * n);
    float* output_arr = mem_buf.get_fpbuffer(b * m * n);
    float* GToutput_arr = mem_buf.get_fpbuffer(b * m * n);

    Matrix3D<int8_t> input(intput_arr, b, m, k);
    Matrix3D<int8_t> weight(weight_arr, b, n, k);
    Matrix3D<float> bias(bias_arr, b, 1, n);
    Matrix3D<float> output(output_arr, b, m, n);
    Matrix3D<float> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_x.bin", intput_arr, b * m * k);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_weight.bin", weight_arr, b * n * k);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_bias.bin", bias_arr, b * n);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_y.bin", GToutput_arr, b * m * n);

    struct W8A8BFP32OFP32Linear_params op_params = {weight, bias, alpha};

    auto test_op = W8A8BFP32OFP32Linear(op_params);
    test_op.forward(input, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_W8A8B8O8Linear() {
    const int b = 1, m = 512, k = 768, n = 768;
    const float alpha = 0.0020542144775390625, beta = 0.06591796875;
    MemoryAllocator mem_buf;

    int8_t* intput_arr = mem_buf.get_int8buffer(b * m * k);
    int8_t* weight_arr = mem_buf.get_int8buffer(b * k * n);
    int8_t* biasint8_arr = mem_buf.get_int8buffer(b * n);
    int32_t* biasint32_arr = mem_buf.get_intbuffer(b * n);
    int8_t* output_arr = mem_buf.get_int8buffer(b * m * n);
    int8_t* GToutput_arr = mem_buf.get_int8buffer(b * m * n);

    Matrix3D<int8_t> input(intput_arr, b, m, k);
    Matrix3D<int8_t> weight(weight_arr, b, n, k);
    Matrix3D<int32_t> bias(biasint32_arr, b, 1, n);
    Matrix3D<int8_t> output(output_arr, b, m, n);
    Matrix3D<int8_t> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/W8A8B8O8Linear_x.bin", intput_arr, b * m * k);
    read_to_array((char*)"assets/W8A8B8O8Linear_weight.bin", weight_arr, b * n * k);
    read_to_array((char*)"assets/W8A8B8O8Linear_bias.bin", biasint8_arr, b * n);
    for (int i = 0; i < b * n; i++) {
        float int8_v = static_cast<float>(biasint8_arr[i]);
        float bias_effective_scale = int8_v * (beta / alpha);
        biasint32_arr[i] = static_cast<int32_t>(std::round(bias_effective_scale));
    }
    read_to_array((char*)"assets/W8A8B8O8Linear_y.bin", GToutput_arr, b * m * n);

    struct W8A8B8O8Linear_params op_params = {weight, bias, alpha};

    W8A8B8O8Linear test_op = W8A8B8O8Linear(op_params);

    test_op.forward(input, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_BMM_S8T_S8N_F32T() {
    const int b = 12, m = 512, k = 64, n = 512;
    const float alpha = 0.0006456375122070312;
    MemoryAllocator mem_buf;

    int8_t* intput_arr = mem_buf.get_int8buffer(b * m * k);
    int8_t* weight_arr = mem_buf.get_int8buffer(b * k * n);
    float* output_arr = mem_buf.get_fpbuffer(b * m * n);
    float* GToutput_arr = mem_buf.get_fpbuffer(b * m * n);

    Matrix3D<int8_t> input(intput_arr, b, m, k);
    Matrix3D<int8_t> weight(weight_arr, b, n, k);
    Matrix3D<float> output(output_arr, b, m, n);
    Matrix3D<float> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_x.bin", intput_arr, b * m * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_weight.bin", weight_arr, b * n * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_y.bin", GToutput_arr, b * m * n);

    struct BMM_S8T_S8N_F32T_params op_params = {alpha};

    auto test_op = BMM_S8T_S8N_F32T(op_params);
    test_op.forward(input, weight, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_BMM_S8T_S8N_S8T() {
    const int b = 12, m = 512, k = 512, n = 64;
    const float alpha = 0.013031005859375;
    MemoryAllocator mem_buf;

    int8_t* intput_arr = mem_buf.get_int8buffer(b * m * k);
    int8_t* weight_arr = mem_buf.get_int8buffer(b * k * n);
    int8_t* output_arr = mem_buf.get_int8buffer(b * m * n);
    int8_t* GToutput_arr = mem_buf.get_int8buffer(b * m * n);

    Matrix3D<int8_t> input(intput_arr, b, m, k);
    Matrix3D<int8_t> weight(weight_arr, b, n, k);
    Matrix3D<int8_t> output(output_arr, b, m, n);
    Matrix3D<int8_t> GToutput(GToutput_arr, b, m, n);

    read_to_array((char*)"assets/BMM_S8T_S8N_S8T_x.bin", intput_arr, b * m * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_S8T_weight.bin", weight_arr, b * n * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_S8T_y.bin", GToutput_arr, b * m * n);

    struct BMM_S8T_S8N_S8T_params op_params = {alpha};

    auto test_op = BMM_S8T_S8N_S8T(op_params);
    test_op.forward(input, weight, output);

    assert(check_two_equal(output_arr, GToutput_arr, b * m * n));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

void test_Embedding() {
    const int voc_size = 50272, embed_dim = 768, sqlen = 512, padding_idx = 1;
    MemoryAllocator mem_buf;

    Matrix3D<int> input(mem_buf.get_intbuffer(sqlen), 1, 1, sqlen);
    Matrix3D<float> weight(mem_buf.get_fpbuffer(voc_size * embed_dim), 1, voc_size, embed_dim);
    Matrix3D<float> output(mem_buf.get_fpbuffer(sqlen * embed_dim), 1, sqlen, embed_dim);
    Matrix3D<float> outputGT(mem_buf.get_fpbuffer(sqlen * embed_dim), 1, sqlen, embed_dim);

    read_to_array((char*)"assets/input_ids.bin", input.m_data, sqlen);
    read_to_array((char*)"assets/inputs_embeds.bin", outputGT.m_data, sqlen * embed_dim);

    auto embed_tokens = Embedding(embed_dim, voc_size, padding_idx, weight);
    load_Embedding_params(embed_tokens, "assets/decoder/embed_tokens");

    embed_tokens.forward(input, output);
    assert(check_two_equal(output.m_data, outputGT.m_data, sqlen * embed_dim));

    std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

int main() {
    test_LayerNormQ();
    test_LayerNorm();
    test_W8A8B8O8LinearReLU();
    test_W8A8BFP32OFP32Linear();
    test_W8A8B8O8Linear();
    test_BMM_S8T_S8N_F32T();
    test_BMM_S8T_S8N_S8T();
    test_Embedding();
}
