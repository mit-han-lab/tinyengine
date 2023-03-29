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
#include <stdio.h>
#include <sys/time.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "genModel.h"
#include "genNN.h"
#include "tinyengine_function.h"

using namespace cv;
using namespace std;

#define CROP_SIZE 720
#define DOWN_WIDTH 144
#define DOWN_HEIGHT 144
#define USE_CASE 0  // 0: VWW (Visual Wake Words), 1: MNIST
#if USE_CASE == 0
#define OUTPUT_CH 2
#elif USE_CASE == 1
#define OUTPUT_CH 10
#endif

float interval_to_ms(struct timeval *start, struct timeval *end) {
    float us_seconds = (end->tv_sec - start->tv_sec) * 1000000 + (end->tv_usec - start->tv_usec);
    return us_seconds / 1000;
}

void invoke_new_weights_givenimg(signed char *out_int8) {
    invoke_inf();
    signed char *output = (signed char *)getOutput();
    for (int i = 0; i < OUTPUT_CH; i++) {
        out_int8[i] = output[i];
    }
}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "No video stream is detected." << endl;
        system("pause");
        return -1;
    }

    int flag = 0;
    while (1) {
        Mat myImage;
        cap >> myImage;
        if (myImage.empty()) {
            break;
        }

        /* Crop image from rectangle to square */
        const int offsetW = (myImage.size().width - CROP_SIZE) / 2;
        const int offsetH = (myImage.size().height - CROP_SIZE) / 2;
        const Rect roi(offsetW, offsetH, CROP_SIZE, CROP_SIZE);
        myImage = myImage(roi).clone();

        /* Resize image */
        Mat resized_myImage = myImage.clone();
        resize(myImage, resized_myImage, Size(DOWN_WIDTH, DOWN_HEIGHT));

        if (flag == 0) {
            cout << "[MIT Hanlab] Platform-independent TinyEngine Demo, Start!" << endl;
            cout << "Original (Cropped) Image Width : " << myImage.size().width << endl;
            cout << "Original (Cropped) Image Height: " << myImage.size().height << endl;
            cout << "Original (Cropped) Image Channel: " << myImage.channels() << endl;
            cout << endl;
            cout << "Resized Image Width : " << resized_myImage.size().width << endl;
            cout << "Resized Image Height: " << resized_myImage.size().height << endl;
            cout << "Resized Image Channel: " << resized_myImage.channels() << endl;

            flag = 1;
        }

        /* Convert the OpenCV image from BGR to RGB */
        signed char *input = getInput();
        int num_row = resized_myImage.rows;
        int num_col = resized_myImage.cols;
        int num_channel = resized_myImage.channels();
        uint8_t *pixels = (uint8_t *)resized_myImage.data;
        for (int i = 0; i < num_row; i++) {
            for (int j = 0; j < num_col; j++) {
                uint8_t b = pixels[(i * num_col + j) * num_channel + 0];
                uint8_t g = pixels[(i * num_col + j) * num_channel + 1];
                uint8_t r = pixels[(i * num_col + j) * num_channel + 2];

                *input++ = (int)r - 128;
                *input++ = (int)g - 128;
                *input++ = (int)b - 128;
            }
        }

        /* Inference for dectection */
        struct timeval start, end;
        gettimeofday(&start, NULL);
        signed char out_int[OUTPUT_CH];
        invoke_new_weights_givenimg(out_int);
        gettimeofday(&end, NULL);
        float ms = interval_to_ms(&start, &end);

        /* Generate UI information */
        char buf[6];
        gcvt(ms, 6, buf);
        char buf_time[30] = "Inference time: ";
        strcat(buf_time, buf);
        strcat(buf_time, " ms");

        char buf_peak_mem[30] = "Peak memory: ";
        gcvt(PEAK_MEM / 1024, 6, buf);
        strcat(buf_peak_mem, buf);
        strcat(buf_peak_mem, " KB");

        char buf_model_size[30] = "Model size: ";
        gcvt(MODEL_SIZE / 1024, 6, buf);
        strcat(buf_model_size, buf);
        strcat(buf_model_size, " KB");

        uint8_t ui_red, ui_green, ui_blue;
        char buf_result[25];
        int tmp_max = INT_MIN, result = INT_MIN;
        for (int i = 0; i < OUTPUT_CH; i++) {
            if (out_int[i] > tmp_max) {
                tmp_max = out_int[i];
                result = i;
            }
        }
#if USE_CASE == 0  // VWW Case
        if (result == 0) {
            strcpy(buf_result, "No Person!");
            ui_red = 151;
            ui_green = 41;
            ui_blue = 52;
        } else {
            strcpy(buf_result, "Person!");
            ui_red = 41;
            ui_green = 98;
            ui_blue = 25;
        }
#elif USE_CASE == 1  // MNIST Case
        strcpy(buf_result, "Recognized Digit: ");
        sprintf(buf, "%d", result);
        strcat(buf_result, buf);
        ui_red = 151;
        ui_green = 41;
        ui_blue = 52;
#endif

        putText(myImage, buf_time, Point(1, 22), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_peak_mem, Point(1, 44), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_model_size, Point(1, 66), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_result, Point(1, 88), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        imshow("[MIT Hanlab] Platform-independent TinyEngine Demo", myImage);

        char c = (char)waitKey(25);
        if (c == 27) {  // Press ESC to leave
            break;
        }
    }

    cap.release();
    return 0;
}
