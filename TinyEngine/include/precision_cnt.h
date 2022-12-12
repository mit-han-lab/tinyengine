/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   precision_cnt.h
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

#ifndef TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_PRECISION_CNT_H_
#define TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_PRECISION_CNT_H_

/* MIX precision */
#define INPUT_PRE 8
#define KERNEL_PRE 8
#define OUTPUT_PRE 8
#define input_scaler (8 / INPUT_PRE)
#define weight_scaler (8 / KERNEL_PRE)
#define output_scaler (8 / OUTPUT_PRE)


#endif /* TINYENGINE_SOURCE_CONVOLUTIONFUNCTIONS_MIX_PRECISION_CNT_H_ */
