#include "Int8OPTDecoder.h"
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

void test_Decoder_layers() {
    const int num_heads = 12, embed_dim = 768, sqlen = 108, b = 1, hidden_dim = 3072, voc_size = 50272, padding_idx = 1, num_layers = 12;
    MemoryAllocator mem_buf;

    Int8OPTDecoder decoder = Int8OPTDecoder("assets/decoder", voc_size, embed_dim, hidden_dim, num_heads,
                   padding_idx, num_layers);

    float* input_buf = mem_buf.get_fpbuffer(b * sqlen * embed_dim);
    Matrix3D<float> hidden_states(input_buf, b, sqlen, embed_dim);
    read_to_array("assets/tests/decoder/hidden_states.bin", hidden_states.m_data, b * sqlen * embed_dim);
    Matrix3D<float> attention_mask(mem_buf.get_fpbuffer(b * sqlen * sqlen), b, sqlen, sqlen);
    read_to_array("assets/tests/decoder/causal_attention_mask.bin", attention_mask.m_data, b * sqlen * sqlen);
    // print_first_k_elelment("hidden_states.m_data", hidden_states.m_data, 10);
    // print_first_k_elelment("attention_mask.m_data", attention_mask.m_data, 10);
    
    for (int i = 0; i < num_layers; i++){
        struct Int8OPTDecoderLayer_input input = {hidden_states, attention_mask};
        struct Int8OPTDecoderLayer_output output = decoder.layers[i].forward(input);
        // struct Int8OPTDecoderLayer_output output = layer.forward(input);
        hidden_states = output.hidden_states;
    }

    Matrix3D<float> residualGT(mem_buf.get_fpbuffer(b * sqlen * embed_dim), b, sqlen, embed_dim);
    read_to_array("assets/tests/decoder/output_hidden_states.bin", residualGT.m_data, b * sqlen * embed_dim);
    // print_first_k_elelment("O", hidden_states.m_data, 20);
    // print_first_k_elelment("G", residualGT.m_data, 20);
    bool sucess = check_two_equal(residualGT.m_data, hidden_states.m_data, b * sqlen * embed_dim, 0.0025);
    if (!sucess)
        std::cout << "Test of " << __func__ << ": Fail!" << std::endl;
    else
        std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}


void test_prepare_decoder_attention_mask() {
    const int num_heads = 12, embed_dim = 768, sqlen = 108, b = 1, hidden_dim = 3072, voc_size = 50272, padding_idx = 1, num_layers = 12;
    MemoryAllocator mem_buf;

    Int8OPTDecoder decoder = Int8OPTDecoder("assets/decoder", voc_size, embed_dim, hidden_dim, num_heads,
                   padding_idx, num_layers);

    // reasoning phase
    Matrix3D<float> causal_attention_mask = decoder.prepare_decoder_attention_mask(sqlen, 0);
    Matrix3D<float> causal_attention_maskGT(mem_buf.get_fpbuffer(b * sqlen * sqlen), b, sqlen, sqlen);
    read_to_array("assets/tests/decoder/generate_causal_attention_reason.bin", causal_attention_maskGT.m_data, b * sqlen * sqlen);
    bool sucess = check_two_equal(causal_attention_maskGT.m_data, causal_attention_mask.m_data, b * sqlen * sqlen);

    // generating phase
    Matrix3D<float> causal_attention_mask_g = decoder.prepare_decoder_attention_mask(sqlen+1, sqlen);
    Matrix3D<float> causal_attention_mask_gGT(mem_buf.get_fpbuffer(b * 1 * (sqlen + 1)), b, 1, sqlen + 1);
    read_to_array("assets/tests/decoder/generate_causal_attention_generate.bin", causal_attention_mask_gGT.m_data, b * 1 * (sqlen+1));
    sucess &= check_two_equal(causal_attention_mask_g.m_data, causal_attention_mask_gGT.m_data, b * 1 * (sqlen+1));

    if (!sucess)
        std::cout << "Test of " << __func__ << ": Fail!" << std::endl;
    else
        std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

void test_get_position_embed() {
    const int num_heads = 12, embed_dim = 768, sqlen = 108, b = 1, hidden_dim = 3072, voc_size = 50272, padding_idx = 1, num_layers = 12;
    MemoryAllocator mem_buf;

    Int8OPTDecoder decoder = Int8OPTDecoder("assets/decoder", voc_size, embed_dim, hidden_dim, num_heads,
                   padding_idx, num_layers);

    // reasoning phase
    Matrix3D<float> pos_embed = decoder.get_position_embed(sqlen, 0);
    assert(pos_embed.m_dim_y == sqlen);
    assert(pos_embed.m_dim_z == embed_dim);
    Matrix3D<float> pos_embedGT(mem_buf.get_fpbuffer(b * sqlen * embed_dim), b, sqlen, embed_dim);
    read_to_array("assets/tests/decoder/pos_embed_reasoning.bin", pos_embedGT.m_data, b * sqlen * embed_dim);
    bool sucess = check_two_equal(pos_embed.m_data, pos_embedGT.m_data, b * sqlen * embed_dim);

    // generating phase
    Matrix3D<float> pos_embed_g = decoder.get_position_embed(1, sqlen);
    Matrix3D<float> pos_embed_gGT(mem_buf.get_fpbuffer(b * 1 * embed_dim), b, 1, embed_dim);
    read_to_array("assets/tests/decoder/pos_embed_generate.bin", pos_embed_gGT.m_data, b * 1 * embed_dim);
    sucess &= check_two_equal(pos_embed_g.m_data, pos_embed_gGT.m_data, b * 1 * embed_dim);

    if (!sucess)
        std::cout << "Test of " << __func__ << ": Fail!" << std::endl;
    else
        std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}


int main() { test_Decoder_layers(); test_prepare_decoder_attention_mask(); test_get_position_embed();}