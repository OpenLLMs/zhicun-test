/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fill_q15.c
 * Description:  Fills a constant value into a Q15 vector
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
  @ingroup groupSupport
 */

/**
  @addtogroup Fill
  @{
 */

/**
  @brief         Fills a constant value into a Q15 vector.
  @param[in]     value      input value to be filled
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_fill_q15(
  q15_t value,
  q15_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined (RISCV_MATH_LOOPUNROLL)
  q31_t packedValue;                             /* value packed to 32 bits */

  /* Packing two 16 bit values to 32 bit value in order to use SIMD */
  //packedValue = __PKHBT(value, value, 16U);
  packedValue = __PKBB16(value, value);
#if __RISCV_XLEN == 64
  q63_t packedValue64;                             /* value packed to 32 bits */
  packedValue64 = __RV_PKBB32(packedValue,packedValue);
#endif /* __RISCV_XLEN == 64 */
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = value */
#if __RISCV_XLEN == 64
    write_q15x4_ia (&pDst, packedValue64);
#else
    /* fill 2 times 2 samples at a time */
    write_q15x2_ia (&pDst, packedValue);
    write_q15x2_ia (&pDst, packedValue);
#endif /* __RISCV_XLEN == 64 */

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = value */

    /* Fill value in destination buffer */
    *pDst++ = value;

    /* Decrement loop counter */
    blkCnt--;
  }
}

/**
  @} end of Fill group
 */
