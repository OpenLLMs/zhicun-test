/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_trans_q31.c
 * Description:  Q31 matrix transpose
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
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixTrans
  @{
 */

/**
  @brief         Q31 matrix transpose.
  @param[in]     pSrc      points to input matrix
  @param[out]    pDst      points to output matrix
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed
 */

riscv_status riscv_mat_trans_q31(
  const riscv_matrix_instance_q31 * pSrc,
        riscv_matrix_instance_q31 * pDst)
{
  q31_t *pIn = pSrc->pData;                      /* input data matrix pointer */
  q31_t *pOut = pDst->pData;                     /* output data matrix pointer */
  q31_t *px;                                     /* Temporary output data matrix pointer */
  uint16_t nRows = pSrc->numRows;                /* number of rows */
  uint16_t nCols = pSrc->numCols;                /* number of columns */
  uint32_t col, row = nRows, i = 0U;             /* Loop counters */
  riscv_status status;                             /* status of matrix transpose */

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numCols) ||
      (pSrc->numCols != pDst->numRows)   )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {
    /* Matrix transpose by exchanging the rows with columns */
    /* row loop */
    do
    {
      /* Pointer px is set to starting address of column being processed */
      px = pOut + i;

#if defined (RISCV_MATH_LOOPUNROLL)

      /* Loop unrolling: Compute 4 outputs at a time */
      col = nCols >> 2U;

      while (col > 0U)        /* column loop */
      {
        /* Read and store input element in destination */
        *px = *pIn++;
        /* Update pointer px to point to next row of transposed matrix */
        px += nRows;

        *px = *pIn++;
        px += nRows;

        *px = *pIn++;
        px += nRows;

        *px = *pIn++;
        px += nRows;

        /* Decrement column loop counter */
        col--;
      }

      /* Loop unrolling: Compute remaining outputs */
      col = nCols % 0x4U;

#else

      /* Initialize col with number of samples */
      col = nCols;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (col > 0U)
      {
        /* Read and store input element in destination */
        *px = *pIn++;

        /* Update pointer px to point to next row of transposed matrix */
        px += nRows;

        /* Decrement column loop counter */
        col--;
      }

      i++;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);          /* row loop end */

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixTrans group
 */
