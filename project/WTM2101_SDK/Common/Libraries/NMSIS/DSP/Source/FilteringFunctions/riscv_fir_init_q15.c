/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fir_init_q15.c
 * Description:  Q15 FIR filter initialization function
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
  @ingroup groupFilters
 */

/**
  @addtogroup FIR
  @{
 */

/**
  @brief         Initialization function for the Q15 FIR filter.
  @param[in,out] S          points to an instance of the Q15 FIR filter structure.
  @param[in] 	 numTaps    number of filter coefficients in the filter. Must be even and greater than or equal to 4.
  @param[in]     pCoeffs    points to the filter coefficients buffer.
  @param[in]     pState     points to the state buffer.
  @param[in]     blockSize  number of samples processed per call.
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : <code>numTaps</code> is not greater than or equal to 4 and even

  @par           Details
                   <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
  <pre>
      {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
  </pre>
                   Note that <code>numTaps</code> must be even and greater than or equal to 4.
                   To implement an odd length filter simply increase <code>numTaps</code> by 1 and set the last coefficient to zero.
                   For example, to implement a filter with <code>numTaps=3</code> and coefficients
  <pre>
      {0.3, -0.8, 0.3}
  </pre>
                   set <code>numTaps=4</code> and use the coefficients:
  <pre>
      {0.3, -0.8, 0.3, 0}.
  </pre>
                   Similarly, to implement a two point filter
  <pre>
      {0.3, -0.3}
  </pre>
                   set <code>numTaps=4</code> and use the coefficients:
  <pre>
      {0.3, -0.3, 0, 0}.
  </pre>
                   <code>pState</code> points to the array of state variables.
                   <code>pState</code> is of length <code>numTaps+blockSize</code>, when running on RISC-V Core with DSP enabled  and is of length <code>numTaps+blockSize-1</code>, when running on RISC-V Core without DSP where <code>blockSize</code> is the number of input samples processed by each call to <code>riscv_fir_q15()</code>.
 */

riscv_status riscv_fir_init_q15(
        riscv_fir_instance_q15 * S,
        uint16_t numTaps,
  const q15_t * pCoeffs,
        q15_t * pState,
        uint32_t blockSize)
{
  riscv_status status;

#if defined (RISCV_MATH_DSP)

  /* The Number of filter coefficients in the filter must be even and at least 4 */
  if (numTaps & 0x1U)
  {
    status = RISCV_MATH_ARGUMENT_ERROR;
  }
  else
  {
    /* Assign filter taps */
    S->numTaps = numTaps;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Clear the state buffer.  The size is always (blockSize + numTaps ) */
    memset(pState, 0, (numTaps + (blockSize)) * sizeof(q15_t));

    /* Assign state pointer */
    S->pState = pState;

    status = RISCV_MATH_SUCCESS;
  }

  return (status);

#else

  /* Assign filter taps */
  S->numTaps = numTaps;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Clear state buffer. The size is always (blockSize + numTaps - 1) */
  memset(pState, 0, (numTaps + (blockSize - 1U)) * sizeof(q15_t));

  /* Assign state pointer */
  S->pState = pState;

  status = RISCV_MATH_SUCCESS;

  return (status);

#endif /* #if defined (RISCV_MATH_DSP) */

}

/**
  @} end of FIR group
 */
