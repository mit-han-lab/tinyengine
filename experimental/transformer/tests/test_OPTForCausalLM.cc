#include <chrono>
#include <cstring>

#include "OPTForCausalLM.h"
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

void test_OPTForCausalLM() {
    const int num_heads = 12, embed_dim = 768, sqlen = 108, b = 1, hidden_dim = 3072, voc_size = 50272, padding_idx = 1,
              num_layers = 12;
    MemoryAllocator mem_buf;

    // reasoning phase: 1st run
    Matrix3D<int> input_ids(mem_buf.get_intbuffer(sqlen), b, 1, sqlen);
    read_to_array("assets/tests/causallm/1st_input_ids.bin", input_ids.m_data, input_ids.length());
    struct OPTForCausalLM_input input_1st = {input_ids};

    OPTForCausalLM model =
        OPTForCausalLM("assets/", voc_size, embed_dim, hidden_dim, num_heads, padding_idx, num_layers);

    struct OPTForCausalLM_output output_1st = model.forward(input_1st);

    Matrix3D<float> logits(mem_buf.get_fpbuffer(b * sqlen * voc_size), b, sqlen, voc_size);
    read_to_array("assets/tests/causallm/1st_logits.bin", logits.m_data, logits.length());
    // print_first_k_elelment("O", output_1st.logits.m_data, 20);
    // print_first_k_elelment("G", logits.m_data, 20);
    bool sucess = check_two_equal(output_1st.logits.m_data, logits.m_data, logits.length(), 0.053);

    Matrix3D<int8_t> temp_key_value(mem_buf.get_int8buffer(b * sqlen * embed_dim), num_heads, sqlen,
                                    embed_dim / num_heads);
    for (int i = 0; i < num_layers; i++) {
        std::string path = "assets/tests/decoder/decoder_1st_past_key" + std::to_string(i) + ".bin";
        read_to_array(path.c_str(), temp_key_value.m_data, temp_key_value.length());
        // print_first_k_elelment("output_1st.past_keys[i].m_data", output_1st.past_keys[i].m_data, 20);
        // print_first_k_elelment("temp_key_value.m_data", temp_key_value.m_data, 20);
        sucess &= check_two_equal(output_1st.past_keys[i].m_data, temp_key_value.m_data, temp_key_value.length(), 4.5);

        path = "assets/tests/decoder/decoder_1st_past_value" + std::to_string(i) + ".bin";
        read_to_array(path.c_str(), temp_key_value.m_data, temp_key_value.length());
        // print_first_k_elelment("output_1st.past_values[i].m_data", output_1st.past_values[i].m_data, 20);
        // print_first_k_elelment("temp_key_value.m_data", temp_key_value.m_data, 20);
        sucess &=
            check_two_equal(output_1st.past_values[i].m_data, temp_key_value.m_data, temp_key_value.length(), 4.5);
    }

    Profiler::getInstance().report();
    Profiler::getInstance().reset();

    // generating phase: 2nd run
    Matrix3D<int> input_ids_2nd(mem_buf.get_intbuffer(sqlen), b, 1, 1);
    read_to_array("assets/tests/causallm/2nd_input_ids.bin", input_ids_2nd.m_data, input_ids_2nd.length());
    struct OPTForCausalLM_input input_2nd = {input_ids_2nd, output_1st.past_keys, output_1st.past_values};

    struct OPTForCausalLM_output output_2nd = model.forward(input_2nd);

    logits = Matrix3D<float>(mem_buf.get_fpbuffer(b * 1 * voc_size), b, 1, voc_size);
    read_to_array("assets/tests/causallm/2nd_logits.bin", logits.m_data, logits.length());
    // print_first_k_elelment("O", output_2nd.logits.m_data, 20);
    // print_first_k_elelment("G", logits.m_data, 20);
    sucess &= check_two_equal(output_2nd.logits.m_data, logits.m_data, logits.length(), 0.041);

    temp_key_value =
        Matrix3D<int8_t>(mem_buf.get_int8buffer(b * 1 * embed_dim), num_heads, (sqlen + 1), embed_dim / num_heads);
    for (int i = 0; i < num_layers; i++) {
        std::string path = "assets/tests/decoder/decoder_2nd_past_key" + std::to_string(i) + ".bin";
        read_to_array(path.c_str(), temp_key_value.m_data, temp_key_value.length());
        // print_first_k_elelment("output_2nd.past_keys[i].m_data", output_2nd.past_keys[i].m_data, 20);
        // print_first_k_elelment("temp_key_value.m_data", temp_key_value.m_data, 20);
        sucess &= check_two_equal(output_2nd.past_keys[i].m_data, temp_key_value.m_data, temp_key_value.length(), 2.8);

        path = "assets/tests/decoder/decoder_2nd_past_value" + std::to_string(i) + ".bin";
        read_to_array(path.c_str(), temp_key_value.m_data, temp_key_value.length());
        // print_first_k_elelment("output_2nd.past_values[i].m_data", output_2nd.past_values[i].m_data, 20);
        // print_first_k_elelment("temp_key_value.m_data", temp_key_value.m_data, 20);
        sucess &=
            check_two_equal(output_2nd.past_values[i].m_data, temp_key_value.m_data, temp_key_value.length(), 4.2);
    }

    Profiler::getInstance().report();
    if (!sucess)
        std::cout << "-------- Test of " << __func__ << ": Fail! -------- " << std::endl;
    else
        std::cout << "-------- Test of " << __func__ << ": Passed! -------- " << std::endl;
}

int main() { test_OPTForCausalLM(); }
