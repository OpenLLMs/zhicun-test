/*****************************************************************************
 *                   SEGGER Microcontroller GmbH & Co. KG                    *
 *            Solutions for real time microcontroller applications           *
 *****************************************************************************
 *                                                                           *
 *               (c) 2017 SEGGER Microcontroller GmbH & Co. KG               *
 *                                                                           *
 *           Internet: www.segger.com   Support: support@segger.com          *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 *                         Preprocessor Definitions                          *
 *                         ------------------------                          *
 * NO_FPU_ENABLE                                                             *
 *                                                                           *
 *   If defined, FPU will not be enabled.                                    *
 *                                                                           *
 * NO_STACK_INIT                                                             *
 *                                                                           *
 *   If defined, the stack pointer will not be initialised.                  *
 *                                                                           *
 * NO_SYSTEM_INIT                                                            *
 *                                                                           *
 *   If defined, the SystemInit() function will not be called. By default    *
 *   SystemInit() is called after reset to enable the clocks and memories to *
 *   be initialised prior to any C startup initialisation.                   *
 *                                                                           *
 * NO_VTOR_CONFIG                                                            *
 *                                                                           *
 *   If defined, the vector table offset register will not be configured.    *
 *                                                                           *
 * MEMORY_INIT                                                               *
 *                                                                           *
 *   If defined, the MemoryInit() function will be called. By default        *
 *   MemoryInit() is called after SystemInit() to enable an external memory  *
 *   controller.                                                             *
 *                                                                           *
 * STACK_INIT_VAL                                                            *
 *                                                                           *
 *   If defined, specifies the initial stack pointer value. If undefined,    *
 *   the stack pointer will be initialised to point to the end of the        *
 *   RAM segment.                                                            *
 *                                                                           *
 * VECTORS_IN_RAM                                                            *
 *                                                                           *
 *   If defined, the exception vectors will be copied from Flash to RAM.     *
 *                                                                           *
 *****************************************************************************/

  .syntax unified

  .global Reset_Handler
  .extern _vectors

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08
  .equ FPU_CPACR_REG, 0xE000ED88

#ifndef STACK_INIT_VAL
#define STACK_INIT_VAL __RAM1_segment_end__
#endif

Reset_Handler:
#ifndef NO_STACK_INIT
  /* Initialise main stack */
  ldr r0, =STACK_INIT_VAL
  bic r0, #0x7
  mov sp, r0
#endif

#ifndef NO_SYSTEM_INIT
  /* Initialise system */
  ldr r0, =SystemInit
  blx r0
#endif

#ifdef SDRAM_INIT
  ldr r0, =TM_SDRAM_Init
  blx r0
#endif

  
  /* restore original stack pointer */
  LDR R0, =STACK_INIT_VAL         
  MSR MSP, R0

#ifdef VECTORS_IN_RAM
  /* Copy exception vectors into RAM */
  ldr r0, =__vectors_start__
  ldr r1, =__vectors_end__
  ldr r2, =__vectors_ram_start__
1:
  cmp r0, r1
  beq 2f
  ldr r3, [r0]
  str r3, [r2]
  adds r0, r0, #4
  adds r2, r2, #4
  b 1b
2:
#endif

#ifndef NO_VTOR_CONFIG
  /* Configure vector table offset register */
  ldr r0, =VTOR_REG
#ifdef VECTORS_IN_RAM
  ldr r1, =_vectors_ram
#else
  ldr r1, =_vectors
#endif
  str r1, [r0]
#endif

#if (defined(__ARM_ARCH_FPV4_SP_D16__) || defined(__ARM_ARCH_FPV5_D16__)) && !defined(NO_FPU_ENABLE)
  /* Enable FPU */
  ldr r0, =FPU_CPACR_REG
  ldr r1, [r0]
  orr r1, r1, #(0xF << 20)
  str r1, [r0]
  dsb
  isb
#endif

  /* Jump to program start */
  b _start


