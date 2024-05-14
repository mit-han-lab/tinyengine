/* ----------------------------------------------------------------------
#
# File: iSoftmax.c
#
# Last edited: 10.10.2023
#
# Copyright (C) 2023, ETH Zurich and University of Bologna.
#
# Author:
# - Victor Jung, jungvi@iis.ee.ethz.ch, ETH Zurich
#
# ----------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/



//#include "pmsis.h"
//#include "../inc/pulp_nn_utils.h"
//#include "../inc/pulp_nn_kernels.h"
#include "stm32746g_discovery.h"
#include "stm32f7xx_hal.h"

void iSoftmax(
  int8_t * pInBuffer,
  uint8_t * pOutBuffer,
  const int32_t rowDimension,
  const int32_t coeffA,
  const int32_t coeffB,
  const int32_t coeffC,
  const int32_t log2,
  const uint32_t n_levels )
{

    int16_t xTilde;
    int8_t z, p;
    int8_t x_max = -128;
    uint32_t y_sum = 0;
    uint32_t y[rowDimension];
    
    for (int i=0; i<rowDimension; i++){
        if (pInBuffer[i] > x_max){
            x_max = pInBuffer[i];
        }
    }

    for (int i=0; i<rowDimension; i++){
        xTilde = pInBuffer[i] - x_max;
        z = -(xTilde / log2);
        p = xTilde + z * log2;
        y[i] = ((coeffA*(p+coeffB)*(p+coeffB) + coeffC)>>z)*(1-(z > 31 || z < 0));
        y_sum += y[i];
    }

    for (int i=0; i<rowDimension; i++){
        pOutBuffer[i] = (uint8_t)((y[i]*(n_levels-1))/(y_sum));
    }

}
