/* ----------------------------------------------------------------------
 * Project: TinyEngine (platform-independent version)
 * Title:   main.cc
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 *
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Song Han, songhan@mit.edu
 * -------------------------------------------------------------------- */

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "batch_model.h"
#include "image_digits.h"

#define OUTPUT_CH 10
#define IMAGE_PER_ROW 5

using namespace cv;
using namespace std;

float interval_to_ms(struct timeval *start, struct timeval *end) {
    float us_seconds = (end->tv_sec - start->tv_sec) * 1000000 + (end->tv_usec - start->tv_usec);
    return us_seconds / 1000;
}

signed char out_int[OUTPUT_CH];

void invoke_givenimg(signed char *out_int8, int imp_choice) {
    batch_inference(1, imp_choice);
    signed char *output = (signed char *)getOutput();
    for (int i = 0; i < OUTPUT_CH; i++) {
        out_int8[i] = output[i];
    }
}

Mat makeMultipleImages(vector<Mat> &vecMat, int windowHeight, int nRows) {
    int N = vecMat.size();
    nRows = nRows > N ? N : nRows;
    int edgeThickness = 100;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
    int maxRowLength = 0;

    vector<int> resizeWidth;
    for (int i = 0; i < N;) {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++) {
            double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
            int temp = int(ceil(resizeHeight * aspectRatio));

            resizeWidth.push_back(temp);
            thisRowLen += temp;

            if (++i == N) break;
        }

        if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
            maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
        }
    }

    int windowWidth = maxRowLength;
    Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(0, 0, 0));

    for (int k = 0, i = 0; i < nRows; i++) {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;

        for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
            int x = x_end;
            Rect roi(x, y, resizeWidth[k], resizeHeight);
            Size s = canvasImage(roi).size();
            // change the number of channels to three
            Mat target_ROI(s, CV_8UC3);

            if (vecMat[k].channels() != canvasImage.channels()) {
                if (vecMat[k].channels() == 1) {
                    cvtColor(vecMat[k], target_ROI, COLOR_GRAY2BGR);
                }
            } else {
                vecMat[k].copyTo(target_ROI);
            }

            resize(target_ROI, target_ROI, s);
            if (target_ROI.type() != canvasImage.type()) {
                target_ROI.convertTo(target_ROI, canvasImage.type());
            }
            target_ROI.copyTo(canvasImage(roi));
            x_end += resizeWidth[k] + edgeThickness;
        }
    }

    return canvasImage;
}

int main() {
    struct timeval start, end;
    float ms = 0, time_naive, time_unroll, time_unroll_simd, time_unroll_simd_tiling;
    char buf_result[NUM_IMAGES][2];

    /* Measure time */
    gettimeofday(&start, NULL);
    batch_inference(25, naive);
    gettimeofday(&end, NULL);
    time_naive = (interval_to_ms(&start, &end)) / NUM_IMAGES;

    gettimeofday(&start, NULL);
    batch_inference(25, unroll);
    gettimeofday(&end, NULL);
    time_unroll = (interval_to_ms(&start, &end)) / NUM_IMAGES;

    gettimeofday(&start, NULL);
    batch_inference(25, unroll_simd);
    gettimeofday(&end, NULL);
    time_unroll_simd = (interval_to_ms(&start, &end)) / NUM_IMAGES;

    gettimeofday(&start, NULL);
    batch_inference(25, unroll_simd_tiling);
    gettimeofday(&end, NULL);
    time_unroll_simd_tiling = (interval_to_ms(&start, &end)) / NUM_IMAGES;

    signed char *input = getInput();
    for (int i = 0; i < NUM_IMAGES; i++) {
        for (int j = 0; j < IMAGE_SIZE; j++) {
            *input++ = (int)mnist_images[i][j] - 128;
        }
    }
    batch_inference(NUM_IMAGES, unroll_simd_tiling);

    signed char *output = getOutput();
    for (int i = 0; i < NUM_IMAGES; i++) {
        int tmp_max = INT_MIN, result = INT_MIN;
        for (int j = 0; j < OUTPUT_CH; j++) {
            if (output[i * OUTPUT_CH + j] > tmp_max) {
                tmp_max = output[i * OUTPUT_CH + j];
                result = j;
            }
        }

        snprintf(buf_result[i], 2, "%d", result);
    }

    /* Generate UI information */
    char buf[3];
    gcvt(ms, 3, buf);
    char buf_time[100];
    snprintf(buf_time, 100, "Latency: %.2f(Naive), %.2f(Unroll), %.2f(Unroll+SIMD), %.2f(Unroll+SIMD+Tiling)",
             time_naive, time_unroll, time_unroll_simd, time_unroll_simd_tiling);

    char buf_model_size[30] = "Model size: ";
    gcvt(MODEL_SIZE / 1024, 6, buf);
    strcat(buf_model_size, buf);
    strcat(buf_model_size, " KB");

    vector<Mat> imgvector = {};
    for (int i = 0; i < NUM_IMAGES; i++) {
        Mat mnist_image_mat(28, 28, CV_8U, mnist_images[i]);
        imgvector.insert(imgvector.begin() + i, mnist_image_mat);
    }
    Mat multi_img = makeMultipleImages(imgvector, 1200, IMAGE_PER_ROW);

    uint8_t ui_red = 172, ui_green = 252, ui_blue = 152;
    putText(multi_img, buf_time, Point(5, 22), FONT_HERSHEY_DUPLEX, 0.9, CV_RGB(ui_red, ui_green, ui_blue), 2);
    putText(multi_img, buf_model_size, Point(5, 52), FONT_HERSHEY_DUPLEX, 0.9, CV_RGB(ui_red, ui_green, ui_blue), 2);

    for (int i = 0; i < NUM_IMAGES; i++) {
        if (i % IMAGE_PER_ROW == 0) {
            putText(multi_img, "Prediction:", Point(5, 240 + (i / IMAGE_PER_ROW) * 220), FONT_HERSHEY_DUPLEX, 0.7,
                    CV_RGB(ui_red, ui_green, ui_blue), 2);

            putText(multi_img, "Ground", Point(5, 272 + (i / IMAGE_PER_ROW) * 220), FONT_HERSHEY_DUPLEX, 0.7,
                    CV_RGB(ui_red, ui_green, ui_blue), 2);
            putText(multi_img, "truth:", Point(5, 297 + (i / IMAGE_PER_ROW) * 220), FONT_HERSHEY_DUPLEX, 0.7,
                    CV_RGB(ui_red, ui_green, ui_blue), 2);
        }

        char buf_label[2];
        snprintf(buf_label, 2, "%d", mnist_labels[i]);

        if (strcmp(buf_result[i], buf_label) != 0) {
            ui_red = 229;
            ui_green = 172;
            ui_blue = 170;
        }
        putText(multi_img, buf_result[i], Point(155 + (i % IMAGE_PER_ROW) * 220, 244 + (i / IMAGE_PER_ROW) * 220),
                FONT_HERSHEY_DUPLEX, 1.1, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(multi_img, buf_label, Point(155 + (i % IMAGE_PER_ROW) * 220, 290 + (i / IMAGE_PER_ROW) * 220),
                FONT_HERSHEY_DUPLEX, 1.1, CV_RGB(ui_red, ui_green, ui_blue), 2);

        ui_red = 172;
        ui_green = 252;
        ui_blue = 152;
    }
    namedWindow("[MIT Hanlab] Platform-independent TinyEngine MNIST Demo", WINDOW_AUTOSIZE);
    imshow("[MIT Hanlab] Platform-independent TinyEngine MNIST Demo", multi_img);
    waitKey(0);
    destroyWindow("[MIT Hanlab] Platform-independent TinyEngine MNIST Demo");

    return 0;
}
