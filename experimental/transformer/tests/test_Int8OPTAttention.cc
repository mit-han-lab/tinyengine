#include "../operators.h"
#include "../utils.h"
#include "../Int8OPTAttention.h"

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

void test_Int8OPTAttention() {
    const int num_heads = 12, embed_dim = 768;
    MemoryAllocator mem_buf;

    struct BMM_S8T_S8N_F32T_params qk_bmm;
    load_BMM_S8T_S8N_F32T(qk_bmm, "assets/decoder_layer0_attn_qk_bmm_BMM_S8T_S8N_F32T");
    struct BMM_S8T_S8N_S8T_params pv_bmm;
    load_BMM_S8T_S8N_S8T(pv_bmm, "assets/decoder_layer0_attn_pv_bmm_BMM_S8T_S8N_S8T");
    struct W8A8B8O8Linear_params k_proj, v_proj, q_proj;
    Matrix3D<int8_t> k_proj_weight(mem_buf.get_int8buffer(embed_dim * embed_dim), 1, embed_dim, embed_dim);
    Matrix3D<int32_t> k_proj_bias(mem_buf.get_intbuffer(embed_dim), 1, 1, embed_dim);
    k_proj.weight = k_proj_weight; k_proj.bias = k_proj_bias;
    load_W8A8B8O8Linear_params(k_proj, "assets/decoder_layer0_attn_k_proj_W8A8B8O8Linear");
    Matrix3D<int8_t> v_proj_weight(mem_buf.get_int8buffer(embed_dim * embed_dim), 1, embed_dim, embed_dim);
    Matrix3D<int32_t> v_proj_bias(mem_buf.get_intbuffer(embed_dim), 1, 1, embed_dim);
    v_proj.weight = v_proj_weight; v_proj.bias = v_proj_bias;
    load_W8A8B8O8Linear_params(k_proj, "assets/decoder_layer0_attn_v_proj_W8A8B8O8Linear");
    Matrix3D<int8_t> q_proj_weight(mem_buf.get_int8buffer(embed_dim * embed_dim), 1, embed_dim, embed_dim);
    Matrix3D<int32_t> q_proj_bias(mem_buf.get_intbuffer(embed_dim), 1, 1, embed_dim);
    q_proj.weight = q_proj_weight; q_proj.bias = q_proj_bias;
    struct W8A8BFP32OFP32Linear_params out_proj;
    Matrix3D<int8_t> out_proj_weight(mem_buf.get_int8buffer(embed_dim * embed_dim), 1, embed_dim, embed_dim);
    Matrix3D<float> out_proj_bias(mem_buf.get_fpbuffer(embed_dim), 1, 1, embed_dim);
    out_proj.weight = out_proj_weight; out_proj.bias = out_proj_bias;
    load_W8A8BFP32OFP32Linear_params(out_proj, "assets/decoder_layer0_attn_out_proj_W8A8BFP32OFP32Linear");

    std::cout << "Test of " << __func__ << ": Passed!" << std::endl;
}

int main() {
    test_Int8OPTAttention();
}