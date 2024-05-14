#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "math.h"

#define min(a,b) ((a)<(b)?(a):(b))

void linearQK_4x2_H(
  const q7_t *  pInBuffer,
  const q7_t *  pWeight,
  const q15_t * pBiasBuffer,
  q7_t *        pOutBuffer,
  const uint16_t  dimSequence,
  const uint16_t  dimEmbedding,
  const uint16_t  dimProjections,
  const uint16_t  heads,
  const int16_t   requant_div,
  const int16_t   requant_mul
) 
{
  for (int i = 0; i < dimSequence; i++) {
    // Compute dot product and add bias
    for (int j = 0; j < dimProjections; j++) {
      int dotp = 0;
      for (int k = 0; k < dimEmbedding; k++) {
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
    pInBuffer += dimEmbedding;
    pOutBuffer += dimProjections;
  }
}
