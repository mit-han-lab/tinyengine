/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   yoloOutput.c
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
#include "yoloOutput.h"

#include "tinyengine_function.h"

#define MAX_BOX_PER_CLASS 10
#define MAX_NUM_CLASSES 4

static inline float sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }

static int boxcnt[MAX_NUM_CLASSES] = {0};  // this will be reset before postprocessing for each image
int postprocess_single(float *data, int w, int h, float stride, int num_classes, const int anchors[3][2],
                       const int num_anchors, det_box **box, const float VALID_THRESHOLD) {
    int num_preds = 5 + num_classes;

    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            for (size_t anchor_id = 0; anchor_id < num_anchors; anchor_id++) {
                float center_x, center_y, scale_x, scale_y;
                float xmin, ymin;
                int need_parse = 1;

                float *point_data = data + ((y * w + x) * num_anchors + anchor_id) * num_preds;

                float confidence = sigmoid(point_data[4]);

                for (size_t class_id = 0; class_id < num_classes; class_id++) {
                    float score = sigmoid(point_data[5 + class_id]) * confidence;

                    if (score >= VALID_THRESHOLD && boxcnt[class_id] < MAX_BOX_PER_CLASS) {
                        if (need_parse) {
                            center_x = (sigmoid(point_data[0]) + x) * stride;
                            center_y = (sigmoid(point_data[1]) + y) * stride;
                            scale_x = exp(point_data[2]) * anchors[anchor_id][0];
                            scale_y = exp(point_data[3]) * anchors[anchor_id][1];

                            xmin = center_x - scale_x / 2.0f;
                            ymin = center_y - scale_y / 2.0f;
                            need_parse = 0;
                        }

                        box[class_id][boxcnt[class_id]].x0 = xmin;
                        box[class_id][boxcnt[class_id]].x1 = xmin + scale_x;
                        box[class_id][boxcnt[class_id]].y0 = ymin;
                        box[class_id][boxcnt[class_id]].y1 = ymin + scale_y;
                        box[class_id][boxcnt[class_id]].score = score;
                        boxcnt[class_id]++;
                    }
                }
            }
        }
    }
}

void data_requantize(int8_t *input_data, float *output_data, int size, int8_t y_zero, float y_scale) {
    int i;
    for (i = 0; i < size; i++) {
        int temp_int = (int)input_data[i] - (int)y_zero;
        output_data[i] = temp_int * y_scale;
    }
}

unsigned char suppress[MAX_BOX_PER_CLASS];
int nms_impl(det_box *candidates, det_box *results, float NMS_threshold, int candicate_num) {
    for (int i = 0; i < candicate_num; i++) suppress[i] = 0;

    int nms_boxes = 0;

    while (1) {
        // find the maximum
        int max_index = -1;
        float max_score = 0;
        for (int i = 0; i < candicate_num; i++) {
            if (!suppress[i]) {
                if (candidates[i].score > max_score) {
                    max_score = candidates[i].score;
                    max_index = i;
                }
            }
        }
        if (max_index == -1)  // cannot find any box
            break;
        else {  // add that box
            results[nms_boxes].score = candidates[max_index].score;
            results[nms_boxes].x0 = candidates[max_index].x0;
            results[nms_boxes].y0 = candidates[max_index].y0;
            results[nms_boxes].x1 = candidates[max_index].x1;
            results[nms_boxes].y1 = candidates[max_index].y1;
            nms_boxes++;
            suppress[max_index] = 1;  // suppress since we add it to the output boxes
        }

        // get the box
        float x1 = candidates[max_index].x0;
        float y1 = candidates[max_index].y0;
        float x2 = candidates[max_index].x1;
        float y2 = candidates[max_index].y1;
        float area_max = (x2 - x1) * (y2 - y1);
        for (int i = 0; i < candicate_num; i++) {
            if (!suppress[i]) {
                float xx1 = TN_MAX(0.f, TN_MAX(x1, candidates[i].x0));
                float yy1 = TN_MAX(0.f, TN_MAX(y1, candidates[i].y0));
                float xx2 = TN_MAX(0.f, TN_MIN(x2, candidates[i].x1));
                float yy2 = TN_MAX(0.f, TN_MIN(y2, candidates[i].y1));

                float b_w = TN_MAX(0.0f, xx2 - xx1 + 0.001f);  // avoid zero division
                float b_h = TN_MAX(0.0f, yy2 - yy1 + 0.001f);

                float inter = b_w * b_h;

                float area_i = (candidates[i].x1 - candidates[i].x0) * (candidates[i].y1 - candidates[i].y0);

                float ovr = inter / (area_max + area_i - inter);

                // filtering highly-overlapped boxes
                if (ovr > NMS_threshold) suppress[i] = 1;
            }
        }
    }
    return nms_boxes;
}

det_box **postprocessing(signed char *input_data[3], signed char y_zero[3], float y_scale[3], unsigned char *data_buf,
                         int w, int h, int output_c, int num_classes, const int anchors[3][3][2], int outputs,
                         const float NMS_threshold, const float VALID_THRESHOLD, int *box_ret, det_box **ret_box) {
    // assign buffers for boxes, for 4 num_class
    det_box *candidates[4];
    int i;
    for (i = 0; i < num_classes; i++) {
        candidates[i] = (det_box *)(data_buf);
        data_buf += sizeof(det_box) * MAX_BOX_PER_CLASS;
    }
    for (i = 0; i < num_classes; i++) {
        ret_box[i] = (det_box *)(data_buf);
        data_buf += sizeof(det_box) * MAX_BOX_PER_CLASS;
    }
    // initialize var
    for (i = 0; i < MAX_NUM_CLASSES; i++) {
        boxcnt[i] = 0;
    }

    // Yoloputput
    const int strides[] = {32, 16, 8};
    for (i = 0; i < 3; i++) {
        // preprocessing the input data
        int size = (w / strides[i]) * (h / strides[i]) * output_c;
        float *out = (float *)data_buf;
        data_requantize(input_data[i], out, size, y_zero[i], y_scale[i]);
        postprocess_single(out, w / strides[i], h / strides[i], strides[i], num_classes, anchors[i], 3, candidates,
                           VALID_THRESHOLD);
    }
    for (i = 0; i < num_classes; i++) {
        box_ret[i] = nms_impl(candidates[i], ret_box[i], NMS_threshold, boxcnt[i]);
    }
}
