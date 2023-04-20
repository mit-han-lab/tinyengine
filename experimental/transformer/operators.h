#include <cassert>
#include "common.h"

template<typename T>
void linear(Matrix3D<T> &a, Matrix3D<T> &b, Matrix3D<T> &c) {
    // a: m x k   b: n x k   c: m x n
    assert(a.m_dim_x == b.m_dim_x);  // batch dim
    assert(a.m_dim_z == b.m_dim_z);  // k
    assert(a.m_dim_y == c.m_dim_y);  // m
    assert(b.m_dim_y == c.m_dim_z);  // n

    int m = a.m_dim_y, n = b.m_dim_y, k = a.m_dim_z, b_size = b.m_dim_x;

    for (int b_ = 0; b_ < b_size; b_++) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                T acc = 0;
                for(int kk = 0; kk < k; k++){
                    acc += a(b_, i, kk) * b(b_, j, kk);
                }

                c(b_, i, j) = acc;
            }
        }
    }
}