/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_cmplx_q15.c
 * Description:  Q15 complex-by-complex multiplication
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "riscv_math.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup CmplxByCmplxMult
  @{
 */

/**
  @brief         Q15 complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector
  @param[in]     pSrcB       points to second input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.15 by 1.15 multiplications and finally output is converted into 3.13 format.
 */

void riscv_cmplx_mult_cmplx_q15(
  const q15_t * pSrcA,
  const q15_t * pSrcB,
        q15_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */
        q15_t a, b, c, d;                              /* Temporary variables */
#if __RISCV_XLEN == 64
        q31_t RESA,RESB;
#endif /* __RISCV_XLEN == 64 */
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
#if defined(RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    RESA = read_q15x2_ia((q15_t **) &pSrcA);
    RESB = read_q15x2_ia((q15_t **) &pSrcB);
    *pDst++ = (q15_t) (__RV_SMALDRS(0,RESA,RESB) >> 17);
    *pDst++ = (q15_t) (__RV_SMALXDA(0,RESA,RESB) >> 17);

    RESA = read_q15x2_ia((q15_t **) &pSrcA);
    RESB = read_q15x2_ia((q15_t **) &pSrcB);
    *pDst++ = (q15_t) (__RV_SMALDRS(0,RESA,RESB) >> 17);
    *pDst++ = (q15_t) (__RV_SMALXDA(0,RESA,RESB) >> 17);

    RESA = read_q15x2_ia((q15_t **) &pSrcA);
    RESB = read_q15x2_ia((q15_t **) &pSrcB);
    *pDst++ = (q15_t) (__RV_SMALDRS(0,RESA,RESB) >> 17);
    *pDst++ = (q15_t) (__RV_SMALXDA(0,RESA,RESB) >> 17);

    RESA = read_q15x2_ia((q15_t **) &pSrcA);
    RESB = read_q15x2_ia((q15_t **) &pSrcB);
    *pDst++ = (q15_t) (__RV_SMALDRS(0,RESA,RESB) >> 17);
    *pDst++ = (q15_t) (__RV_SMALXDA(0,RESA,RESB) >> 17);

#else

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in 3.13 format in destination buffer. */
#if defined(RISCV_MATH_DSP)
	*pDst++ = (q15_t) ( (__SMBB16(a, c) >> 17) - (__SMBB16(b, d) >> 17) );
	*pDst++ = (q15_t) ( (__SMBB16(a, d) >> 17) + (__SMBB16(b, c) >> 17) );
#else
	*pDst++ = (q15_t) ( (((q31_t) a * c) >> 17) - (((q31_t) b * d) >> 17) );
    *pDst++ = (q15_t) ( (((q31_t) a * d) >> 17) + (((q31_t) b * c) >> 17) );
#endif

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

#if defined(RISCV_MATH_DSP)
	*pDst++ = (q15_t) ( (__SMBB16(a, c) >> 17) - (__SMBB16(b, d) >> 17) );
	*pDst++ = (q15_t) ( (__SMBB16(a, d) >> 17) + (__SMBB16(b, c) >> 17) );
#else
	*pDst++ = (q15_t) ( (((q31_t) a * c) >> 17) - (((q31_t) b * d) >> 17) );
    *pDst++ = (q15_t) ( (((q31_t) a * d) >> 17) + (((q31_t) b * c) >> 17) );
#endif

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

#if defined(RISCV_MATH_DSP)
	*pDst++ = (q15_t) ( (__SMBB16(a, c) >> 17) - (__SMBB16(b, d) >> 17) );
	*pDst++ = (q15_t) ( (__SMBB16(a, d) >> 17) + (__SMBB16(b, c) >> 17) );
#else
	*pDst++ = (q15_t) ( (((q31_t) a * c) >> 17) - (((q31_t) b * d) >> 17) );
    *pDst++ = (q15_t) ( (((q31_t) a * d) >> 17) + (((q31_t) b * c) >> 17) );
#endif

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

#if defined(RISCV_MATH_DSP)
	*pDst++ = (q15_t) ( (__SMBB16(a, c) >> 17) - (__SMBB16(b, d) >> 17) );
	*pDst++ = (q15_t) ( (__SMBB16(a, d) >> 17) + (__SMBB16(b, c) >> 17) );
#else
	*pDst++ = (q15_t) ( (((q31_t) a * c) >> 17) - (((q31_t) b * d) >> 17) );
    *pDst++ = (q15_t) ( (((q31_t) a * d) >> 17) + (((q31_t) b * c) >> 17) );
#endif
#endif
#endif /* __RISCV_XLEN == 64 */
    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = numSamples % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in 3.13 format in destination buffer. */
#if defined(RISCV_MATH_DSP)
	*pDst++ = (q15_t) ( (__SMBB16(a, c) >> 17) - (__SMBB16(b, d) >> 17) );
	*pDst++ = (q15_t) ( (__SMBB16(a, d) >> 17) + (__SMBB16(b, c) >> 17) );
#else
	*pDst++ = (q15_t) ( (((q31_t) a * c) >> 17) - (((q31_t) b * d) >> 17) );
	*pDst++ = (q15_t) ( (((q31_t) a * d) >> 17) + (((q31_t) b * c) >> 17) );
#endif

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of CmplxByCmplxMult group
 */
