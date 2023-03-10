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

#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "model.h"

#define CROP_SIZE 720
#define DOWN_WIDTH 28
#define DOWN_HEIGHT 28
#define OUTPUT_CH 10

using namespace cv;
using namespace std;

float interval_to_ms(struct timeval *start, struct timeval *end) {
    float us_seconds = (end->tv_sec - start->tv_sec) * 1000000 + (end->tv_usec - start->tv_usec);
    return us_seconds / 1000;
}

signed char out_int[OUTPUT_CH];

void invoke_new_weights_givenimg(signed char *out_int8) {
    inference();
    signed char *output = (signed char *)getOutput();
    for (int i = 0; i < OUTPUT_CH; i++) {
        out_int8[i] = output[i];
    }
}
uint8_t input_g[] = {
    0, 0, 0, 0, 0, 0,  0,  0,  0,   4,   6,   0,   0,   0,   15,  0,   0,   0,   9,   0,   9, 2,  0,  1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  7,   0,   0,   0,   7,   0,   2,   0,   0,   17,  0,   15,  0, 0,  18, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   8,   0,   0,   10,  0,   2,   4,   3,   0,   0,   5,   0, 0,  9,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  25,  0,   0,   34,  75,  167, 245, 202, 95,  23,  5,   0,   0, 10, 0,  8, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  3,   0,   29,  158, 244, 255, 255, 245, 242, 206, 42,  3,   0, 0,  0,  6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   18,  114, 255, 255, 140, 123, 213, 255, 255, 162, 0,   4, 0,  3,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  2,   0,   137, 251, 93,  0,   6,   19,  135, 236, 254, 32,  9, 7,  1,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   10,  0,   19,  4,   8,   1,   0,   14,  236, 244, 137, 0, 1,  0,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  6,   0,   0,   4,   0,   9,   0,   3,   62,  216, 255, 192, 0, 0,  9,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   14,  24,  5,   0,   13,  8,   0,   0,   213, 255, 108, 7, 6,  0,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  8,   0,   6,   29,  4,   5,   0,   0,   52,  248, 245, 33,  5, 0,  0,  4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   12,  22,  3,   17,  129, 156, 123, 218, 246, 146, 9,   0, 0,  9,  1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  11,  0,   0,   7,   97,  252, 255, 244, 255, 187, 17,  1,   0, 0,  6,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   7,   0,   0,   61,  197, 243, 250, 244, 188, 4,   0,   4, 0,  0,  1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   0,   0,   13,  0,   11,  95,  255, 255, 237, 114, 2,   3, 0,  0,  6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   8,   0,   0,   0,   3,   0,   68,  255, 249, 213, 19,  0, 0,  8,  0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0,  0,  1,  0,   8,   0,   0,   13,  0,   0,   10,  139, 255, 255, 106, 3, 0,  4,  0, 0, 0, 0, 0,
    0, 2, 0, 0, 6, 10, 5,  4,  0,   3,   7,   19,  108, 175, 12,  0,   30,  237, 255, 98,  2, 0,  0,  7, 0, 0, 0, 0,
    0, 6, 2, 0, 1, 4,  0,  0,  5,   7,   118, 223, 253, 9,   2,   5,   24,  250, 255, 86,  8, 4,  0,  5, 0, 0, 0, 0,
    2, 5, 0, 0, 0, 0,  0,  6,  45,  200, 255, 251, 41,  8,   29,  53,  181, 255, 211, 47,  1, 6,  0,  2, 0, 0, 0, 0,
    3, 0, 0, 4, 6, 0,  10, 47, 182, 255, 247, 231, 203, 184, 243, 255, 255, 224, 101, 1,   0, 2,  3,  7, 0, 0, 0, 0,
    4, 0, 0, 5, 8, 0,  14, 55, 255, 248, 255, 249, 255, 247, 250, 241, 181, 83,  8,   0,   1, 4,  5,  0, 0, 0, 0, 0,
    1, 3, 0, 0, 1, 0,  1,  21, 126, 216, 234, 255, 220, 164, 195, 92,  34,  0,   0,   7,   8, 4,  1,  0, 0, 0, 0, 0,
    0, 6, 2, 0, 0, 3,  0,  0,  0,   4,   0,   0,   3,   4,   0,   7,   0,   0,   7,   0,   0, 0,  5,  4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,  0,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,  0,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,  0,  0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,  0,  0, 0, 0, 0, 0,
};
int8_t fake_quant_0[28 * 28] = {
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -124, -122, -128, -128, -128, -113, -128, -128, -128, -119,
    -128, -119, -126, -128, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -121, -128,
    -128, -128, -121, -128, -126, -128, -128, -111, -128, -113, -128, -128, -110, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -120, -128, -128, -118, -128, -126, -124, -125, -128, -128, -123,
    -128, -128, -119, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -103, -128, -128,
    -94,  -53,  39,   117,  74,   -33,  -105, -123, -128, -128, -118, -128, -120, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -125, -128, -99,  30,   116,  127,  127,  117,  114,  78,   -86,  -125, -128,
    -128, -128, -122, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -110, -14,  127,
    127,  12,   -5,   85,   127,  127,  34,   -128, -124, -128, -125, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -126, -128, 9,    123,  -35,  -128, -122, -109, 7,    108,  126,  -96,  -119, -121,
    -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -118, -128, -109, -124,
    -120, -127, -128, -114, 108,  116,  9,    -128, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -122, -128, -128, -124, -128, -119, -128, -125, -66,  88,   127,  64,   -128, -128, -119,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -114, -104, -123, -128, -115,
    -120, -128, -128, 85,   127,  -20,  -121, -122, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -120, -128, -122, -99,  -124, -123, -128, -128, -76,  120,  117,  -95,  -123, -128, -128, -124,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -116, -106, -125, -111, 1,    28,
    -5,   90,   118,  18,   -119, -128, -128, -119, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -117, -128, -128, -121, -31,  124,  127,  116,  127,  59,   -111, -127, -128, -128, -122, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -121, -128, -128, -67,  69,   115,  122,
    116,  60,   -124, -128, -124, -128, -128, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -115, -128, -117, -33,  127,  127,  109,  -14,  -126, -125, -128, -128, -122, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -120, -128, -128, -128, -125, -128, -60,  127,
    121,  85,   -109, -128, -128, -120, -128, -128, -128, -128, -128, -128, -125, -128, -128, -128, -128, -128, -127,
    -128, -120, -128, -128, -115, -128, -128, -118, 11,   127,  127,  -22,  -125, -128, -124, -128, -128, -128, -128,
    -128, -128, -126, -128, -128, -122, -118, -123, -124, -128, -125, -121, -109, -20,  47,   -116, -128, -98,  109,
    127,  -30,  -126, -128, -128, -121, -128, -128, -128, -128, -128, -122, -126, -128, -127, -124, -128, -128, -123,
    -121, -10,  95,   125,  -119, -126, -123, -104, 122,  127,  -42,  -120, -124, -128, -123, -128, -128, -128, -128,
    -126, -123, -128, -128, -128, -128, -128, -122, -83,  72,   127,  123,  -87,  -120, -99,  -75,  53,   127,  83,
    -81,  -127, -122, -128, -126, -128, -128, -128, -128, -125, -128, -128, -124, -122, -128, -118, -81,  54,   127,
    119,  103,  75,   56,   115,  127,  127,  96,   -27,  -127, -128, -126, -125, -121, -128, -128, -128, -128, -124,
    -128, -128, -123, -120, -128, -114, -73,  127,  120,  127,  121,  127,  119,  122,  113,  53,   -45,  -120, -128,
    -127, -124, -123, -128, -128, -128, -128, -128, -127, -125, -128, -128, -127, -128, -127, -107, -2,   88,   106,
    127,  92,   36,   67,   -36,  -94,  -128, -128, -121, -120, -124, -127, -128, -128, -128, -128, -128, -128, -122,
    -126, -128, -128, -125, -128, -128, -128, -124, -128, -128, -125, -124, -128, -121, -128, -128, -121, -128, -128,
    -128, -123, -124, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,
    -128, -128, -128, -128, -128,
};
int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "No video stream detected" << endl;
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
            cout << "[MIT Hanlab] Platform-independent TinyEngine VWW Demo, Start!" << endl;
            cout << "Original (Cropped) Image Width : " << myImage.size().width << endl;
            cout << "Original (Cropped) Image Height: " << myImage.size().height << endl;
            cout << "Original (Cropped) Image Channel: " << myImage.channels() << endl;
            cout << endl;
            cout << "Resized Image Width : " << resized_myImage.size().width << endl;
            cout << "Resized Image Height: " << resized_myImage.size().height << endl;
            cout << "Resized Image Channel: " << resized_myImage.channels() << endl;

            flag = 1;
        }

        /* Convert the OpenCV image from BGR to RGB565 */
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

                // float fp_input = (0.3 * (float)r + 0.59 * (float)g + 0.11 * (float)b) / 255.0;
                *input++ = (int8_t)((int)input_g[(i * num_col + j)] - 128);
            }
        }

        /* Inference for dectection */
        struct timeval start, end;
        gettimeofday(&start, NULL);
        invoke_new_weights_givenimg(out_int);
        gettimeofday(&end, NULL);
        float ms = interval_to_ms(&start, &end);

        FILE *f = fopen("temp.txt", "w");
        input = getInput();
        input += 100;
        for (int i = 0; i < 10; i++) {
            fprintf(f, "%d,", *input++);
        }
        fclose(f);
        return 0;

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

        int person = 0;
        uint8_t ui_red, ui_green, ui_blue;
        int max = -129;
        int max_idx = 0;
        for (int i = 0; i < OUTPUT_CH; i++) {
            if (out_int[i] > max) {
                max = out_int[i];
                max_idx = i;
            }
        }

        char buf_person[50];
        sprintf(buf_person, "Num: %d", max_idx);
        // sprintf(buf_person, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", out_int[0], out_int[1], out_int[2], out_int[3],
        // out_int[4], out_int[5], out_int[6], out_int[7], out_int[8], out_int[9]);
        // if (out_int[0] > out_int[1]) {
        //     person = 0;
        // } else {
        //     person = 1;
        // }
        // char buf_person[12];
        // if (person == 0) {
        //     strcpy(buf_person, "No Person!");
        //     ui_red = 151;
        //     ui_green = 41;
        //     ui_blue = 52;
        // } else {
        //     strcpy(buf_person, "Person!");
        //     ui_red = 41;
        //     ui_green = 98;
        //     ui_blue = 25;
        // }
        ui_red = 151;
        ui_green = 41;
        ui_blue = 52;

        putText(myImage, buf_time, Point(1, 22), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_peak_mem, Point(1, 44), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_model_size, Point(1, 66), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        putText(myImage, buf_person, Point(1, 88), FONT_HERSHEY_DUPLEX, 0.7, CV_RGB(ui_red, ui_green, ui_blue), 2);
        imshow("[MIT Hanlab] Platform-independent TinyEngine VWW Demo", myImage);

        char c = (char)waitKey(25);
        if (c == 27) {  // Press ESC to leave
            break;
        }
    }

    cap.release();
    return 0;
}
