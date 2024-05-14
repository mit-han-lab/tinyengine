#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "math.h"

#define min(a,b) ((a)<(b)?(a):(b))

void matmulSoftmax_4x2_S(
  const int8_t *  pInBuffer,
  const int8_t *  pWeight,
  int8_t *        pOutBuffer,
  const uint16_t  dim_sequence,
  const uint16_t  projections,
  const uint16_t  heads,
  const int16_t   requant_div,
  const int16_t   requant_mul
) 
{
  for (int i = 0; i < dim_sequence; i++) {
    // Compute dot product
    for (int j = 0; j < projections; j++) {
      int dotp = 0;
      for (int k = 0; k < heads; k++) {
        dotp += pInBuffer[k] * pWeight[k];
      }

      // Requantize and clip to 8-bit
      dotp = dotp * requant_mul;
      dotp = dotp / requant_div;
      dotp = __SSAT(dotp, 8);

      // Apply softmax
      int exp_dotp = exp(dotp);
      int sum_exp = 0;
      for (int k = 0; k < heads; k++) {
        sum_exp += exp(pInBuffer[k]);
      }
      if (sum_exp != 0) {
        pOutBuffer[j] = (int8_t) (exp_dotp / sum_exp);
      } else {
        pOutBuffer[j] = 0;
      }
    }

    // Update pointers
    pInBuffer += heads;
    pOutBuffer += projections;
  }
}
