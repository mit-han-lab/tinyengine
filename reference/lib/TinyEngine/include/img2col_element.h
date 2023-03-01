/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   img2col_element.h
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#ifndef ARMNN_INCLUDE_IMG2COL_ELEMENT_H_
#define ARMNN_INCLUDE_IMG2COL_ELEMENT_H_

#include "arm_math.h"
#include "arm_nnsupportfunctions.h"

#define b2_q7_q15_offset_ele(src, dst)                                                \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    /*in_q7x4 = b2_nn_read_q7x4_ia((const q7_t **)&src);                              \
    in_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                         \
    in_q15x2_2 = __SXTB16(in_q7x4);		*/                                               \
    in_q15x2_1 = ((src[0] & 0x0C) >> 2) + ((src[0] & 0xC0) << 10);                    \
    in_q15x2_2 = (src[0] & 0x03) + ((src[0] & 0x30) << 12);                           \
    src += 1;                                                                         \
    out_q15x2_2 = __PKHTB(in_q15x2_1, in_q15x2_2, 16);                                \
    /* Maximum of 9 bits from the addition is expected */                             \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    out_q15x2_1 = __PKHBT(in_q15x2_2, in_q15x2_1, 16);                                \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_1);                                                \
    write_q15x2_ia(&dst, out_q15x2_2);

#define b4_q7_q15_offset_ele(src, dst)                                                \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    /*in_q7x4 = b4_nn_read_q7x4_ia((const q7_t **)&src);                              \
    in_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                         \
    in_q15x2_2 = __SXTB16(in_q7x4);		*/                                               \
    in_q15x2_1 = ((src[0] & 0xF0) >> 4) + ((src[1] & 0xF0) << 12);                    \
    in_q15x2_2 = (src[0] & 0x0F) + ((src[1] & 0x0F) << 16);                           \
    src += 2;                                                                         \
    out_q15x2_2 = __PKHTB(in_q15x2_1, in_q15x2_2, 16);                                \
    /* Maximum of 9 bits from the addition is expected */                             \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    out_q15x2_1 = __PKHBT(in_q15x2_2, in_q15x2_1, 16);                                \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_1);                                                \
    write_q15x2_ia(&dst, out_q15x2_2);

#define q7_q15_offset_ele(src, dst)                                                   \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    in_q7x4 = arm_nn_read_q7x4_ia((const q7_t **)&src);                               \
    /* Extract and sign extend each of the four q7 values to q15 */                   \
    in_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                         \
    in_q15x2_2 = __SXTB16(in_q7x4);                                                   \
                                                                                      \
    out_q15x2_2 = __PKHTB(in_q15x2_1, in_q15x2_2, 16);                                \
    /* Maximum of 9 bits from the addition is expected */                             \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    out_q15x2_1 = __PKHBT(in_q15x2_2, in_q15x2_1, 16);                                \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_1);                                                \
    write_q15x2_ia(&dst, out_q15x2_2);

#define q8_q15_offset_ele(src, dst)                                                   \
    /* convert from q8 to q15 and then store the results in the destination buffer */ \
    in_q7x4 = arm_nn_read_q7x4_ia((const q8_t **)&src);                               \
    /* Extend each of the four q8 values to q15 */                                    \
    in_q15x2_1 = __UXTB16(__ROR(in_q7x4, 8));                                         \
    in_q15x2_2 = __UXTB16(in_q7x4);                                                   \
                                                                                      \
    out_q15x2_2 = __PKHTB(in_q15x2_1, in_q15x2_2, 16);                                \
    /* Maximum of 9 bits from the addition is expected */                             \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    out_q15x2_1 = __PKHBT(in_q15x2_2, in_q15x2_1, 16);                                \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_1);                                                \
    write_q15x2_ia(&dst, out_q15x2_2);

#define b4_q15_offset_reordered_ele(src, dst)                                         \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    in_q7x4 = b4_nn_read_q7x4_ia((const q7_t **)&src);                                \
                                                                                      \
    /* Extract and sign extend each of the four q7 values to q15 */                   \
    out_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                        \
    out_q15x2_2 = __SXTB16(in_q7x4);                                                  \
                                                                                      \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_2);                                                \
    write_q15x2_ia(&dst, out_q15x2_1);

#define b2_q15_offset_reordered_ele(src, dst)                                         \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    in_q7x4 = b2_nn_read_q7x4_ia(&src);                                               \
                                                                                      \
    /* Extract and sign extend each of the four q7 values to q15 */                   \
    out_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                        \
    out_q15x2_2 = __SXTB16(in_q7x4);                                                  \
                                                                                      \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_2);                                                \
    write_q15x2_ia(&dst, out_q15x2_1);

#define q7_q15_offset_reordered_ele(src, dst)                                         \
    /* convert from q7 to q15 and then store the results in the destination buffer */ \
    in_q7x4 = arm_nn_read_q7x4_ia((const q7_t **)&src);                               \
                                                                                      \
    /* Extract and sign extend each of the four q7 values to q15 */                   \
    out_q15x2_1 = __SXTB16(__ROR(in_q7x4, 8));                                        \
    out_q15x2_2 = __SXTB16(in_q7x4);                                                  \
                                                                                      \
    out_q15x2_1 = __SADD16(out_q15x2_1, offset_q15x2);                                \
    out_q15x2_2 = __SADD16(out_q15x2_2, offset_q15x2);                                \
                                                                                      \
    write_q15x2_ia(&dst, out_q15x2_2);                                                \
    write_q15x2_ia(&dst, out_q15x2_1);

#define q31_assign2(src, dst) \
    *dst++ = *src++;          \
    *dst++ = *src++;

#define q31_assign4(src, dst) q31_assign2(src, dst) q31_assign2(src, dst)

#define q31_assign6(src, dst) q31_assign4(src, dst) q31_assign2(src, dst)

#define q31_assign8(src, dst) q31_assign4(src, dst) q31_assign4(src, dst)

#define q31_assign10(src, dst) q31_assign8(src, dst) q31_assign2(src, dst)

#define q31_assign12(src, dst) q31_assign10(src, dst) q31_assign2(src, dst)

#define q31_pad2(dst, padvalue) \
    *dst++ = padvalue;          \
    *dst++ = padvalue;

#define q31_pad4(dst, padvalue) q31_pad2(dst, padvalue) q31_pad2(dst, padvalue)

#define q31_pad6(dst, padvalue) q31_pad4(dst, padvalue) q31_pad2(dst, padvalue)

#define q31_pad10(dst, padvalue) q31_pad6(dst, padvalue) q31_pad4(dst, padvalue)

#define q31_pad14(dst, padvalue) q31_pad6(dst, padvalue) q31_pad6(dst, padvalue) q31_pad2(dst, padvalue)

#define assignq31toq15()     \
    dst = (q15_t *)dst_31;   \
    dst2 = (q15_t *)dst2_31; \
    dst3 = (q15_t *)dst3_31; \
    dst4 = (q15_t *)dst4_31; \
    dst5 = (q15_t *)dst5_31; \
    dst6 = (q15_t *)dst6_31; \
    dst7 = (q15_t *)dst7_31;

#define assignq15toq31()     \
    dst_31 = (q31_t *)dst;   \
    dst2_31 = (q31_t *)dst2; \
    dst3_31 = (q31_t *)dst3; \
    dst4_31 = (q31_t *)dst4; \
    dst5_31 = (q31_t *)dst5; \
    dst6_31 = (q31_t *)dst6; \
    dst7_31 = (q31_t *)dst7;

/* ---------------------------------- Pad ---------------------------------- */
#define basic_pad_1row(col, dst_31, pad_out_q15x2)   \
    block_cnt = channel_div4 * col;                  \
    while (block_cnt > 0) {                          \
        q31_pad2(dst_31, pad_out_q15x2) block_cnt--; \
    }

#define basic_pad_2row(col, dst_31, dst2_31, pad_out_q15x2)                           \
    block_cnt = channel_div4 * col;                                                   \
    while (block_cnt > 0) {                                                           \
        q31_pad2(dst_31, pad_out_q15x2) q31_pad2(dst2_31, pad_out_q15x2) block_cnt--; \
    }

#define basic_pad_3row(col, dst_31, dst2_31, dst3_31, pad_out_q15x2)                                                   \
    block_cnt = channel_div4 * col;                                                                                    \
    while (block_cnt > 0) {                                                                                            \
        q31_pad2(dst_31, pad_out_q15x2) q31_pad2(dst2_31, pad_out_q15x2) q31_pad2(dst3_31, pad_out_q15x2) block_cnt--; \
    }

#define basic_pad_4row(col, dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2)                             \
    block_cnt = channel_div4 * col;                                                                       \
    while (block_cnt > 0) {                                                                               \
        q31_pad2(dst_31, pad_out_q15x2) q31_pad2(dst2_31, pad_out_q15x2) q31_pad2(dst3_31, pad_out_q15x2) \
            q31_pad2(dst4_31, pad_out_q15x2) block_cnt--;                                                 \
    }

#define basic_pad_5row(col, dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2)                    \
    block_cnt = channel_div4 * col;                                                                       \
    while (block_cnt > 0) {                                                                               \
        q31_pad2(dst_31, pad_out_q15x2) q31_pad2(dst2_31, pad_out_q15x2) q31_pad2(dst3_31, pad_out_q15x2) \
            q31_pad2(dst4_31, pad_out_q15x2) q31_pad2(dst5_31, pad_out_q15x2) block_cnt--;                \
    }

#define pad_1row_1col(dst_31, pad_out_q15x2) basic_pad_1row(1, dst_31, pad_out_q15x2)
#define pad_1row_2col(dst_31, pad_out_q15x2) basic_pad_1row(2, dst_31, pad_out_q15x2)
#define pad_1row_3col(dst_31, pad_out_q15x2) basic_pad_1row(3, dst_31, pad_out_q15x2)
#define pad_2row_1col(dst_31, dst2_31, pad_out_q15x2) basic_pad_2row(1, dst_31, dst2_31, pad_out_q15x2)
#define pad_2row_2col(dst_31, dst2_31, pad_out_q15x2) basic_pad_2row(2, dst_31, dst2_31, pad_out_q15x2)
#define pad_2row_3col(dst_31, dst2_31, pad_out_q15x2) basic_pad_2row(3, dst_31, dst2_31, pad_out_q15x2)
#define pad_2row_4col(dst_31, dst2_31, pad_out_q15x2) basic_pad_2row(4, dst_31, dst2_31, pad_out_q15x2)
#define pad_2row_5col(dst_31, dst2_31, pad_out_q15x2) basic_pad_2row(5, dst_31, dst2_31, pad_out_q15x2)
#define pad_3row_1col(dst_31, dst2_31, dst3_31, pad_out_q15x2) \
    basic_pad_3row(1, dst_31, dst2_31, dst3_31, pad_out_q15x2)
#define pad_3row_2col(dst_31, dst2_31, dst3_31, pad_out_q15x2) \
    basic_pad_3row(2, dst_31, dst2_31, dst3_31, pad_out_q15x2)
#define pad_3row_3col(dst_31, dst2_31, dst3_31, pad_out_q15x2) \
    basic_pad_3row(3, dst_31, dst2_31, dst3_31, pad_out_q15x2)
#define pad_4row_1col(dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2) \
    basic_pad_4row(1, dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2)
#define pad_4row_2col(dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2) \
    basic_pad_4row(2, dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2)
#define pad_4row_3col(dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2) \
    basic_pad_4row(3, dst_31, dst2_31, dst3_31, dst4_31, pad_out_q15x2)
#define pad_5row_1col(dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2) \
    basic_pad_5row(1, dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2)
#define pad_5row_2col(dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2) \
    basic_pad_5row(2, dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2)
#define pad_5row_3col(dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2) \
    basic_pad_5row(3, dst_31, dst2_31, dst3_31, dst4_31, dst5_31, pad_out_q15x2)

/* ---------------------------------- Load ---------------------------------- */
#define basic_load_1row(col, src, dst)           \
    block_cnt = channel_div4 * col;              \
    while (block_cnt > 0) {                      \
        q7_q15_offset_ele(src, dst) block_cnt--; \
    }
#define basic_load_2row(col, src, src2, dst, dst2)                             \
    block_cnt = channel_div4 * col;                                            \
    while (block_cnt > 0) {                                                    \
        q7_q15_offset_ele(src, dst) q7_q15_offset_ele(src2, dst2) block_cnt--; \
    }
#define basic_load_3row(col, src, src2, src3, dst, dst2, dst3)                                               \
    block_cnt = channel_div4 * col;                                                                          \
    while (block_cnt > 0) {                                                                                  \
        q7_q15_offset_ele(src, dst) q7_q15_offset_ele(src2, dst2) q7_q15_offset_ele(src3, dst3) block_cnt--; \
    }
#define basic_load_4row(col, src, src2, src3, src4, dst, dst2, dst3, dst4)                      \
    block_cnt = channel_div4 * col;                                                             \
    while (block_cnt > 0) {                                                                     \
        q7_q15_offset_ele(src, dst) q7_q15_offset_ele(src2, dst2) q7_q15_offset_ele(src3, dst3) \
            q7_q15_offset_ele(src4, dst4) block_cnt--;                                          \
    }
#define basic_load_5row(col, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)          \
    block_cnt = channel_div4 * col;                                                             \
    while (block_cnt > 0) {                                                                     \
        q7_q15_offset_ele(src, dst) q7_q15_offset_ele(src2, dst2) q7_q15_offset_ele(src3, dst3) \
            q7_q15_offset_ele(src4, dst4) q7_q15_offset_ele(src5, dst5) block_cnt--;            \
    }

///////////////////////// 4bit //////////////////////////
#define b4_load_1row(col, src, dst)                 \
    block_cnt = channel_div4 * col;                 \
    while (block_cnt > 0) {                         \
        b4_q7_q15_offset_ele(src, dst) block_cnt--; \
    }
#define b4_load_2row(col, src, src2, dst, dst2)                                      \
    block_cnt = channel_div4 * col;                                                  \
    while (block_cnt > 0) {                                                          \
        b4_q7_q15_offset_ele(src, dst) b4_q7_q15_offset_ele(src2, dst2) block_cnt--; \
    }
#define b4_load_3row(col, src, src2, src3, dst, dst2, dst3)                                                           \
    block_cnt = channel_div4 * col;                                                                                   \
    while (block_cnt > 0) {                                                                                           \
        b4_q7_q15_offset_ele(src, dst) b4_q7_q15_offset_ele(src2, dst2) b4_q7_q15_offset_ele(src3, dst3) block_cnt--; \
    }
#define b4_load_4row(col, src, src2, src3, src4, dst, dst2, dst3, dst4)                                  \
    block_cnt = channel_div4 * col;                                                                      \
    while (block_cnt > 0) {                                                                              \
        b4_q7_q15_offset_ele(src, dst) b4_q7_q15_offset_ele(src2, dst2) b4_q7_q15_offset_ele(src3, dst3) \
            b4_q7_q15_offset_ele(src4, dst4) block_cnt--;                                                \
    }
#define b4_load_5row(col, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)                      \
    block_cnt = channel_div4 * col;                                                                      \
    while (block_cnt > 0) {                                                                              \
        b4_q7_q15_offset_ele(src, dst) b4_q7_q15_offset_ele(src2, dst2) b4_q7_q15_offset_ele(src3, dst3) \
            b4_q7_q15_offset_ele(src4, dst4) b4_q7_q15_offset_ele(src5, dst5) block_cnt--;               \
    }
///////////////////////// 2bit //////////////////////////
#define b2_load_1row(col, src, dst)                 \
    block_cnt = channel_div4 * col;                 \
    while (block_cnt > 0) {                         \
        b2_q7_q15_offset_ele(src, dst) block_cnt--; \
    }
#define b2_load_2row(col, src, src2, dst, dst2)                                      \
    block_cnt = channel_div4 * col;                                                  \
    while (block_cnt > 0) {                                                          \
        b2_q7_q15_offset_ele(src, dst) b2_q7_q15_offset_ele(src2, dst2) block_cnt--; \
    }
#define b2_load_3row(col, src, src2, src3, dst, dst2, dst3)                                                           \
    block_cnt = channel_div4 * col;                                                                                   \
    while (block_cnt > 0) {                                                                                           \
        b2_q7_q15_offset_ele(src, dst) b2_q7_q15_offset_ele(src2, dst2) b2_q7_q15_offset_ele(src3, dst3) block_cnt--; \
    }
#define b2_load_4row(col, src, src2, src3, src4, dst, dst2, dst3, dst4)                                  \
    block_cnt = channel_div4 * col;                                                                      \
    while (block_cnt > 0) {                                                                              \
        b2_q7_q15_offset_ele(src, dst) b2_q7_q15_offset_ele(src2, dst2) b2_q7_q15_offset_ele(src3, dst3) \
            b2_q7_q15_offset_ele(src4, dst4) block_cnt--;                                                \
    }
#define b2_load_5row(col, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)                      \
    block_cnt = channel_div4 * col;                                                                      \
    while (block_cnt > 0) {                                                                              \
        b2_q7_q15_offset_ele(src, dst) b2_q7_q15_offset_ele(src2, dst2) b2_q7_q15_offset_ele(src3, dst3) \
            b2_q7_q15_offset_ele(src4, dst4) b2_q7_q15_offset_ele(src5, dst5) block_cnt--;               \
    }

#define b4_load_1row_1col(src, dst) b4_load_1row(1, src, dst)
#define b4_load_1row_2col(src, dst) b4_load_1row(2, src, dst)
#define b4_load_1row_3col(src, dst) b4_load_1row(3, src, dst)
#define b4_load_1row_4col(src, dst) b4_load_1row(4, src, dst)
#define b4_load_2row_1col(src, src2, dst, dst2) b4_load_2row(1, src, src2, dst, dst2)
#define b4_load_2row_2col(src, src2, dst, dst2) b4_load_2row(2, src, src2, dst, dst2)
#define b4_load_2row_3col(src, src2, dst, dst2) b4_load_2row(3, src, src2, dst, dst2)
#define b4_load_2row_4col(src, src2, dst, dst2) b4_load_2row(4, src, src2, dst, dst2)
#define b4_load_3row_1col(src, src2, src3, dst, dst2, dst3) b4_load_3row(1, src, src2, src3, dst, dst2, dst3)
#define b4_load_3row_2col(src, src2, src3, dst, dst2, dst3) b4_load_3row(2, src, src2, src3, dst, dst2, dst3)
#define b4_load_3row_3col(src, src2, src3, dst, dst2, dst3) b4_load_3row(3, src, src2, src3, dst, dst2, dst3)
#define b4_load_3row_4col(src, src2, src3, dst, dst2, dst3) b4_load_3row(4, src, src2, src3, dst, dst2, dst3)
#define b4_load_4row_1col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b4_load_4row(1, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b4_load_4row_2col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b4_load_4row(2, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b4_load_4row_3col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b4_load_4row(3, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b4_load_4row_4col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b4_load_4row(4, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b4_load_5row_1col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b4_load_5row(1, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b4_load_5row_2col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b4_load_5row(2, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b4_load_5row_3col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b4_load_5row(3, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b4_load_5row_4col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b4_load_5row(4, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)

#define b2_load_1row_1col(src, dst) b2_load_1row(1, src, dst)
#define b2_load_1row_2col(src, dst) b2_load_1row(2, src, dst)
#define b2_load_1row_3col(src, dst) b2_load_1row(3, src, dst)
#define b2_load_1row_4col(src, dst) b2_load_1row(4, src, dst)
#define b2_load_2row_1col(src, src2, dst, dst2) b2_load_2row(1, src, src2, dst, dst2)
#define b2_load_2row_2col(src, src2, dst, dst2) b2_load_2row(2, src, src2, dst, dst2)
#define b2_load_2row_3col(src, src2, dst, dst2) b2_load_2row(3, src, src2, dst, dst2)
#define b2_load_2row_4col(src, src2, dst, dst2) b2_load_2row(4, src, src2, dst, dst2)
#define b2_load_3row_1col(src, src2, src3, dst, dst2, dst3) b2_load_3row(1, src, src2, src3, dst, dst2, dst3)
#define b2_load_3row_2col(src, src2, src3, dst, dst2, dst3) b2_load_3row(2, src, src2, src3, dst, dst2, dst3)
#define b2_load_3row_3col(src, src2, src3, dst, dst2, dst3) b2_load_3row(3, src, src2, src3, dst, dst2, dst3)
#define b2_load_3row_4col(src, src2, src3, dst, dst2, dst3) b2_load_3row(4, src, src2, src3, dst, dst2, dst3)
#define b2_load_4row_1col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b2_load_4row(1, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b2_load_4row_2col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b2_load_4row(2, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b2_load_4row_3col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b2_load_4row(3, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b2_load_4row_4col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    b2_load_4row(4, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define b2_load_5row_1col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b2_load_5row(1, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b2_load_5row_2col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b2_load_5row(2, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b2_load_5row_3col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b2_load_5row(3, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define b2_load_5row_4col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    b2_load_5row(4, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)

#define load_1row_1col(src, dst) basic_load_1row(1, src, dst)
#define load_1row_2col(src, dst) basic_load_1row(2, src, dst)
#define load_1row_3col(src, dst) basic_load_1row(3, src, dst)
#define load_1row_4col(src, dst) basic_load_1row(4, src, dst)
#define load_2row_1col(src, src2, dst, dst2) basic_load_2row(1, src, src2, dst, dst2)
#define load_2row_2col(src, src2, dst, dst2) basic_load_2row(2, src, src2, dst, dst2)
#define load_2row_3col(src, src2, dst, dst2) basic_load_2row(3, src, src2, dst, dst2)
#define load_2row_4col(src, src2, dst, dst2) basic_load_2row(4, src, src2, dst, dst2)
#define load_3row_1col(src, src2, src3, dst, dst2, dst3) basic_load_3row(1, src, src2, src3, dst, dst2, dst3)
#define load_3row_2col(src, src2, src3, dst, dst2, dst3) basic_load_3row(2, src, src2, src3, dst, dst2, dst3)
#define load_3row_3col(src, src2, src3, dst, dst2, dst3) basic_load_3row(3, src, src2, src3, dst, dst2, dst3)
#define load_3row_4col(src, src2, src3, dst, dst2, dst3) basic_load_3row(4, src, src2, src3, dst, dst2, dst3)
#define load_4row_1col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    basic_load_4row(1, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define load_4row_2col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    basic_load_4row(2, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define load_4row_3col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    basic_load_4row(3, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define load_4row_4col(src, src2, src3, src4, dst, dst2, dst3, dst4) \
    basic_load_4row(4, src, src2, src3, src4, dst, dst2, dst3, dst4)
#define load_5row_1col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    basic_load_5row(1, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define load_5row_2col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    basic_load_5row(2, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define load_5row_3col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    basic_load_5row(3, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)
#define load_5row_4col(src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5) \
    basic_load_5row(4, src, src2, src3, src4, src5, dst, dst2, dst3, dst4, dst5)

/* ---------------------------------- Reuse ---------------------------------- */
#define basic_reuse_1row(col, src_31, dst_31)    \
    block_cnt = channel_div4 * col;              \
    while (block_cnt > 0) {                      \
        q31_assign2(src_31, dst_31) block_cnt--; \
    }
#define basic_reuse_2row(col, src_31, src2_31, dst_31, dst2_31)                \
    block_cnt = channel_div4 * col;                                            \
    while (block_cnt > 0) {                                                    \
        q31_assign2(src_31, dst_31) q31_assign2(src2_31, dst2_31) block_cnt--; \
    }
#define basic_reuse_3row(col, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)                            \
    block_cnt = channel_div4 * col;                                                                          \
    while (block_cnt > 0) {                                                                                  \
        q31_assign2(src_31, dst_31) q31_assign2(src2_31, dst2_31) q31_assign2(src3_31, dst3_31) block_cnt--; \
    }
#define basic_reuse_4row(col, src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31) \
    block_cnt = channel_div4 * col;                                                                 \
    while (block_cnt > 0) {                                                                         \
        q31_assign2(src_31, dst_31) q31_assign2(src2_31, dst2_31) q31_assign2(src3_31, dst3_31)     \
            q31_assign2(src4_31, dst4_31) block_cnt--;                                              \
    }
#define basic_reuse_5row(col, src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31) \
    block_cnt = channel_div4 * col;                                                                                   \
    while (block_cnt > 0) {                                                                                           \
        q31_assign2(src_31, dst_31) q31_assign2(src2_31, dst2_31) q31_assign2(src3_31, dst3_31)                       \
            q31_assign2(src4_31, dst4_31) q31_assign2(src5_31, dst5_31) block_cnt--;                                  \
    }

#define reuse_1row_1col(src_31, dst_31) basic_reuse_1row(1, src_31, dst_31)
#define reuse_1row_2col(src_31, dst_31) basic_reuse_1row(2, src_31, dst_31)
#define reuse_1row_3col(src_31, dst_31) basic_reuse_1row(3, src_31, dst_31)
#define reuse_1row_4col(src_31, dst_31) basic_reuse_1row(4, src_31, dst_31)
#define reuse_1row_5col(src_31, dst_31) basic_reuse_1row(5, src_31, dst_31)
#define reuse_1row_6col(src_31, dst_31) basic_reuse_1row(6, src_31, dst_31)
#define reuse_2row_1col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(1, src_31, src2_31, dst_31, dst2_31)
#define reuse_2row_2col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(2, src_31, src2_31, dst_31, dst2_31)
#define reuse_2row_3col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(3, src_31, src2_31, dst_31, dst2_31)
#define reuse_2row_4col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(4, src_31, src2_31, dst_31, dst2_31)
#define reuse_2row_5col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(5, src_31, src2_31, dst_31, dst2_31)
#define reuse_2row_6col(src_31, src2_31, dst_31, dst2_31) basic_reuse_2row(6, src_31, src2_31, dst_31, dst2_31)
#define reuse_3row_1col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(1, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_3row_2col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(2, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_3row_3col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(3, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_3row_4col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(4, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_3row_5col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(5, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_3row_6col(src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31) \
    basic_reuse_3row(6, src_31, src2_31, src3_31, dst_31, dst2_31, dst3_31)
#define reuse_4row_3col(src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31) \
    basic_reuse_4row(3, src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31)
#define reuse_4row_4col(src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31) \
    basic_reuse_4row(4, src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31)
#define reuse_4row_5col(src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31) \
    basic_reuse_4row(5, src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31)
#define reuse_4row_6col(src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31) \
    basic_reuse_4row(6, src_31, src2_31, src3_31, src4_31, dst_31, dst2_31, dst3_31, dst4_31)
#define reuse_5row_3col(src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31) \
    basic_reuse_5row(3, src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31)
#define reuse_5row_4col(src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31) \
    basic_reuse_5row(4, src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31)
#define reuse_5row_5col(src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31) \
    basic_reuse_5row(5, src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31)
#define reuse_5row_6col(src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31) \
    basic_reuse_5row(6, src_31, src2_31, src3_31, src4_31, src5_31, dst_31, dst2_31, dst3_31, dst4_31, dst5_31)
#endif /* ARMNN_INCLUDE_IMG2COL_ELEMENT_H_ */
