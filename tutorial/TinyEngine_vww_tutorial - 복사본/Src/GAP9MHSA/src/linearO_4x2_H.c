/* ----------------------------------------------------------------------
#
# File: linearO_4x2_H.c
#
# Last edited: 10.10.2023
#
# Copyright (C) 2023, ETH Zurich and University of Bologna.
#
# Author:
# - Victor Jung, jungvi@iis.ee.ethz.ch, ETH Zurich
# - Alessio Burrello alessio.burrello@unibo.it, UNIBO
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



#include "stm32746g_discovery.h"
#include "stm32f7xx_hal.h"
#include "../inc/undefined.h"

#define min(a,b) ((a)<(b)?(a):(b))


//linearO_4x2_H(I, W, B, O, 16, 16, 16, 8, 16, 385);

#include "arm_math.h"
#include "arm_nnfunctions.h"

#define min(a,b) ((a)<(b)?(a):(b))

void linearO_4x2_H(
  const q7_t * pInBuffer,
  const q7_t *  pWeight,
  const q15_t *  pBiasBuffer,
  q7_t *       pOutBuffer,
  const uint16_t  dim_sequence,
  const uint16_t  dim_embedding,
  const uint16_t  projections,
  const uint16_t  heads,
  const int16_t   requant_div,
  const int16_t   requant_mul
) 
{
  int seq_per_core = dim_sequence>>1;

  for (int i = 0; i < seq_per_core; i++) {
    // Compute dot product and add bias
    for (int j = 0; j < projections; j++) {
      int dotp = 0;
      for (int k = 0; k < dim_embedding; k++) {
        dotp += pInBuffer[k] * pWeight[k];
      }
      dotp += pBiasBuffer[j];

      // Requantize and clip to 8-bit
      dotp = dotp * requant_mul;
      dotp = dotp / requant_div;
      dotp = __SSAT(dotp, 8);

      // Store output
      pOutBuffer[j] = (q7_t) dotp;
    }

    // Update pointers
    pInBuffer += dim_embedding;
    pOutBuffer += projections;
  }
}
