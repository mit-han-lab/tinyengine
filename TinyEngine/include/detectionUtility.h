/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   detectionUtility.h
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

#ifndef TINYENGINE_INCLUDE_DETECTIONUTILITY_H_
#define TINYENGINE_INCLUDE_DETECTIONUTILITY_H_

int postProcessing(signed char *input, unsigned char* runtime_buffer,
		int y_zero, float y_scale, int shape_x, int shape_y, int shape_c, int resolution,
		int width, int height , float conf_thresh, float out_boxes[10][6]);


#endif /* TINYENGINE_INCLUDE_DETECTIONUTILITY_H_ */
