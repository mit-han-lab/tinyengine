/*
 * comm_buffer.h
 *
 * Copyright 2018 Dimitris Tassopoulos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef COMM_BUFFER_H_
#define COMM_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @brief This is a struct that is used for UART comms
 */
struct tp_comm_buffer {
    /* tx vars */
    uint8_t *tx_buffer;
    size_t tx_buffer_size;
    uint8_t tx_ready;
    uint16_t tx_ptr_in;
    uint16_t tx_ptr_out;
    uint16_t tx_length;
    uint8_t tx_int_en;
    /* rx vars */
    uint8_t *rx_buffer;
    size_t rx_buffer_size;
    uint8_t rx_ready;
    uint8_t rx_ready_tmr;
    uint16_t rx_ptr_in;
    uint16_t rx_ptr_out;
    uint16_t rx_length;
};

#define DECLARE_COMM_BUFFER(NAME, TX_SIZE, RX_SIZE)                                                                    \
    uint8_t comm_tx_buffer_##NAME[TX_SIZE];                                                                            \
    uint8_t comm_rx_buffer_##NAME[RX_SIZE];                                                                            \
    volatile struct tp_comm_buffer NAME = {.tx_buffer = comm_tx_buffer_##NAME,                                         \
                                           .tx_buffer_size = TX_SIZE,                                                  \
                                           .rx_buffer = comm_rx_buffer_##NAME,                                         \
                                           .rx_buffer_size = RX_SIZE}

#ifdef __cplusplus
}
#endif

#endif /* COMM_BUFFER_H_ */
