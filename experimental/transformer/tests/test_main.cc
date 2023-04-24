#include "../operators.h"
#include "../utils.h"

float test_LayerNormQ_input[512 * 768];
float test_LayerNormQ_weight[768];
float test_LayerNormQ_bias[768];
int8_t test_LayerNormQ_output[512 * 768];
int8_t test_LayerNormQ_GToutput[512 * 768];
void test_LayerNormQ() {
    Matrix3D<float> input(test_LayerNormQ_input, 1, 512, 768);
    Matrix3D<float> weight(test_LayerNormQ_weight, 1, 1, 768);
    Matrix3D<float> bias(test_LayerNormQ_bias, 1, 1, 768);
    Matrix3D<int8_t> output(test_LayerNormQ_output, 1, 512, 768);
    Matrix3D<int8_t> GToutput(test_LayerNormQ_GToutput, 1, 512, 768);

    read_to_array((char*)"assets/LayerNormQ_bias.bin", test_LayerNormQ_bias, 768);
    read_to_array((char*)"assets/LayerNormQ_x.bin", test_LayerNormQ_input, 512 * 768);
    read_to_array((char*)"assets/LayerNormQ_weight.bin", test_LayerNormQ_weight, 768);
    read_to_array((char*)"assets/LayerNormQ_out.bin", test_LayerNormQ_GToutput, 512 * 768);

    LayerNormQ(input, weight, bias, output);

    assert(check_two_equal(test_LayerNormQ_output, test_LayerNormQ_GToutput, 512 * 768));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int8_t test_W8A8B8O8LinearReLU_input[512 * 768];
int8_t test_W8A8B8O8LinearReLU_weight[3072 * 768];
int8_t test_W8A8B8O8LinearReLU_bias_int8[3072];
int32_t test_W8A8B8O8LinearReLU_bias[3072];
int8_t test_W8A8B8O8LinearReLU_output[512 * 3072];
int8_t test_W8A8B8O8LinearReLU_GToutput[512 * 3072];
void test_W8A8B8O8LinearReLU() {
    const int m = 512, k = 768, n = 3072;
    const float alpha = 0.0005035400390625, beta = 0.02130126953125;
    Matrix3D<int8_t> input(test_W8A8B8O8LinearReLU_input, 1, m, k);
    Matrix3D<int8_t> weight(test_W8A8B8O8LinearReLU_weight, 1, n, k);
    Matrix3D<int32_t> bias(test_W8A8B8O8LinearReLU_bias, 1, 1, n);
    Matrix3D<int8_t> output(test_W8A8B8O8LinearReLU_output, 1, m, n);
    Matrix3D<int8_t> GToutput(test_W8A8B8O8LinearReLU_GToutput, 1, m, n);

    read_to_array((char*)"assets/W8A8B8O8LinearReLU_x.bin", test_W8A8B8O8LinearReLU_input, m * k);
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_weight.bin", test_W8A8B8O8LinearReLU_weight, n * k);
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_bias.bin", test_W8A8B8O8LinearReLU_bias_int8, n);
    for (int i = 0; i < n; i++) {
        float int8_v = static_cast<float>(test_W8A8B8O8LinearReLU_bias_int8[i]);
        float bias_effective_scale = int8_v * (beta / alpha);
        test_W8A8B8O8LinearReLU_bias[i] = static_cast<int32_t>(std::round(bias_effective_scale));
    }
    read_to_array((char*)"assets/W8A8B8O8LinearReLU_y.bin", test_W8A8B8O8LinearReLU_GToutput, m * n);

    W8A8B8O8LinearReLU(input, weight, bias, output, alpha);

    assert(check_two_equal(test_W8A8B8O8LinearReLU_output, test_W8A8B8O8LinearReLU_GToutput, m * n));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int8_t test_W8A8BFP32OFP32Linear_input[512 * 768];
int8_t test_W8A8BFP32OFP32Linear_weight[768 * 768];
float test_W8A8BFP32OFP32Linear_bias[768];
float test_W8A8BFP32OFP32Linear_output[512 * 768];
float test_W8A8BFP32OFP32Linear_GToutput[512 * 768];
void test_W8A8BFP32OFP32Linear() {
    const int m = 512, k = 768, n = 768;
    const float alpha = 0.00004565715789794922;
    Matrix3D<int8_t> input(test_W8A8BFP32OFP32Linear_input, 1, m, k);
    Matrix3D<int8_t> weight(test_W8A8BFP32OFP32Linear_weight, 1, n, k);
    Matrix3D<float> bias(test_W8A8BFP32OFP32Linear_bias, 1, 1, n);
    Matrix3D<float> output(test_W8A8BFP32OFP32Linear_output, 1, m, n);
    Matrix3D<float> GToutput(test_W8A8BFP32OFP32Linear_GToutput, 1, m, n);

    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_x.bin", test_W8A8BFP32OFP32Linear_input, m * k);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_weight.bin", test_W8A8BFP32OFP32Linear_weight, n * k);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_bias.bin", test_W8A8BFP32OFP32Linear_bias, n);
    read_to_array((char*)"assets/W8A8BFP32OFP32Linear_y.bin", test_W8A8BFP32OFP32Linear_GToutput, m * n);

    W8A8BFP32OFP32Linear(input, weight, bias, output, alpha);

    assert(check_two_equal(test_W8A8BFP32OFP32Linear_output, test_W8A8BFP32OFP32Linear_GToutput, m * n));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int8_t test_W8A8B8O8Linear_input[512 * 768];
int8_t test_W8A8B8O8Linear_weight[768 * 768];
int8_t test_W8A8B8O8Linear_bias_int8[768];
int32_t test_W8A8B8O8Linear_bias[768];
int8_t test_W8A8B8O8Linear_output[512 * 768];
int8_t test_W8A8B8O8Linear_GToutput[512 * 768];
void test_W8A8B8O8Linear() {
    const int m = 512, k = 768, n = 768;
    const float alpha = 0.0020542144775390625, beta = 0.06591796875;
    Matrix3D<int8_t> input(test_W8A8B8O8Linear_input, 1, m, k);
    Matrix3D<int8_t> weight(test_W8A8B8O8Linear_weight, 1, n, k);
    Matrix3D<int32_t> bias(test_W8A8B8O8Linear_bias, 1, 1, n);
    Matrix3D<int8_t> output(test_W8A8B8O8Linear_output, 1, m, n);
    Matrix3D<int8_t> GToutput(test_W8A8B8O8Linear_GToutput, 1, m, n);

    read_to_array((char*)"assets/W8A8B8O8Linear_x.bin", test_W8A8B8O8Linear_input, m * k);
    read_to_array((char*)"assets/W8A8B8O8Linear_weight.bin", test_W8A8B8O8Linear_weight, n * k);
    read_to_array((char*)"assets/W8A8B8O8Linear_bias.bin", test_W8A8B8O8Linear_bias_int8, n);
    for (int i = 0; i < n; i++) {
        float int8_v = static_cast<float>(test_W8A8B8O8Linear_bias_int8[i]);
        float bias_effective_scale = int8_v * (beta / alpha);
        test_W8A8B8O8Linear_bias[i] = static_cast<int32_t>(std::round(bias_effective_scale));
    }
    read_to_array((char*)"assets/W8A8B8O8Linear_y.bin", test_W8A8B8O8Linear_GToutput, m * n);

    W8A8B8O8Linear(input, weight, bias, output, alpha);

    assert(check_two_equal(test_W8A8B8O8Linear_output, test_W8A8B8O8Linear_GToutput, m * n));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int8_t test_BMM_S8T_S8N_F32T_input[12 * 512 * 64];
int8_t test_BMM_S8T_S8N_F32T_weight[12 * 512 * 64];
float test_BMM_S8T_S8N_F32T_output[12 * 512 * 512];
float test_BMM_S8T_S8N_F32T_GToutput[12 * 512 * 512];
void test_BMM_S8T_S8N_F32T() {
    const int b = 12, m = 512, k = 64, n = 512;
    const float alpha = 0.0006456375122070312;
    Matrix3D<int8_t> input(test_BMM_S8T_S8N_F32T_input, b, m, k);
    Matrix3D<int8_t> weight(test_BMM_S8T_S8N_F32T_weight, b, n, k);
    Matrix3D<float> output(test_BMM_S8T_S8N_F32T_output, b, m, n);
    Matrix3D<float> GToutput(test_BMM_S8T_S8N_F32T_GToutput, b, m, n);

    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_x.bin", test_BMM_S8T_S8N_F32T_input, b * m * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_weight.bin", test_BMM_S8T_S8N_F32T_weight, b * n * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_y.bin", test_BMM_S8T_S8N_F32T_GToutput, b * m * n);

    BMM_S8T_S8N_F32T(input, weight, bias, output, alpha);

    assert(check_two_equal(test_BMM_S8T_S8N_F32T_output, test_BMM_S8T_S8N_F32T_GToutput, b * m * n));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int8_t test_BMM_S8T_S8N_F32T_input[12 * 512 * 64];
int8_t test_BMM_S8T_S8N_F32T_weight[12 * 512 * 64];
int8_t test_BMM_S8T_S8N_F32T_output[12 * 512 * 512];
int8_t test_BMM_S8T_S8N_F32T_GToutput[12 * 512 * 512];
void test_BMM_S8T_S8N_F32T() {
    const int b = 12, m = 512, k = 64, n = 512;
    const float alpha = 0.0006456375122070312;
    Matrix3D<int8_t> input(test_BMM_S8T_S8N_F32T_input, b, m, k);
    Matrix3D<int8_t> weight(test_BMM_S8T_S8N_F32T_weight, b, n, k);
    Matrix3D<float> output(test_BMM_S8T_S8N_F32T_output, b, m, n);
    Matrix3D<float> GToutput(test_BMM_S8T_S8N_F32T_GToutput, b, m, n);

    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_x.bin", test_BMM_S8T_S8N_F32T_input, b * m * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_weight.bin", test_BMM_S8T_S8N_F32T_weight, b * n * k);
    read_to_array((char*)"assets/BMM_S8T_S8N_F32T_y.bin", test_BMM_S8T_S8N_F32T_GToutput, b * m * n);

    BMM_S8T_S8N_F32T(input, weight, bias, output, alpha);

    assert(check_two_equal(test_BMM_S8T_S8N_F32T_output, test_BMM_S8T_S8N_F32T_GToutput, b * m * n));

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int main() {
    test_LayerNormQ();
    test_W8A8B8O8LinearReLU();
    test_W8A8BFP32OFP32Linear();
    test_W8A8B8O8Linear();
    test_BMM_S8T_S8N_F32T();
}