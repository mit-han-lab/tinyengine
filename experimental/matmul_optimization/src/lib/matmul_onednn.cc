#ifdef ONEDNN_ENABLE
#include "matmul.h"
#include "oneapi/dnnl/dnnl.hpp"

namespace matmul {
// void assign_data()

inline void write_to_dnnl_memory(void *handle, dnnl::memory &mem) {
    dnnl::engine eng = mem.get_engine();
    size_t size = mem.get_desc().get_size();

    if (!handle) throw std::runtime_error("handle is nullptr.");

    if (eng.get_kind() == dnnl::engine::kind::cpu) {
        uint8_t *dst = static_cast<uint8_t *>(mem.get_data_handle());
        if (!dst) throw std::runtime_error("get_data_handle returned nullptr.");
        for (size_t i = 0; i < size; ++i) dst[i] = ((uint8_t *)handle)[i];
        return;
    }

    assert(!"not expected");
}

inline void read_from_dnnl_memory(void *handle, dnnl::memory &mem) {
    dnnl::engine eng = mem.get_engine();
    size_t size = mem.get_desc().get_size();

    if (eng.get_kind() == dnnl::engine::kind::cpu) {
        uint8_t *src = static_cast<uint8_t *>(mem.get_data_handle());
        if (!src) throw std::runtime_error("get_data_handle returned nullptr.");
        for (size_t i = 0; i < size; ++i) ((uint8_t *)handle)[i] = src[i];
        return;
    }
}

void MatmulOperator::mat_mul_onednn(const struct matmul_params *params) {
    const struct matrix *A = &params->A, *B = &params->B, *C = &params->C;
    float *data_A = A->data_ptr, *data_B = B->data_ptr, *data_C = C->data_ptr;
    CHECK_MATRICES(A, B, C);

    int M = A->row, N = B->column, K = A->column;

    // Initialize description for matmul
    dnnl::memory::desc a_md({M, K}, dnnl::memory::data_type::f32, {K, 1});  // M x K layout
    dnnl::memory::desc b_md({K, N}, dnnl::memory::data_type::f32, {N, 1});  // K X N layout
    dnnl::memory::desc c_md({M, N}, dnnl::memory::data_type::f32, {N, 1});  // M x N layout

    dnnl::engine eng(dnnl::engine::kind::cpu, 0);
    auto matmul_desc = dnnl::matmul::primitive_desc(eng, a_md, b_md, c_md);

    // Initialize memory
    dnnl::memory A_fp_mem(matmul_desc.src_desc(), eng, (void *)data_A);
    dnnl::memory B_fp_mem(matmul_desc.weights_desc(), eng, (void *)data_B);
    dnnl::memory C_fp_mem(matmul_desc.dst_desc(), eng, (void *)data_C);

    // Operator
    dnnl::matmul matmul_p(matmul_desc);

    dnnl::stream s(eng);
    matmul_p.execute(s, {{DNNL_ARG_SRC, A_fp_mem}, {DNNL_ARG_WEIGHTS, B_fp_mem}, {DNNL_ARG_DST, C_fp_mem}});

    s.wait();
}
}  // namespace matmul
#endif
