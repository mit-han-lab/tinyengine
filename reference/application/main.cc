#include <stdio.h>
#include <sys/time.h>

#include <iostream>

#include "genNN.h"
#include "tinyengine_function.h"

float interval_to_ms(struct timeval *start, struct timeval *end) {
    float us_seconds = (end->tv_sec - start->tv_sec) * 1000000 + (end->tv_usec - start->tv_usec);
    return us_seconds / 1000;
}

int main(void) {
    struct timeval start, end;
    int ms;

    gettimeofday(&start, NULL);
    invoke_inf();
    gettimeofday(&end, NULL);
    ms = interval_to_ms(&start, &end);

    std::cout << "inference time"
              << ": " << ms << " ms" << std::endl;
}
