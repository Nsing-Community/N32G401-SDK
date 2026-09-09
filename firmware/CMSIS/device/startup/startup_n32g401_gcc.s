/*********************************************************************************************************
     Copyright (c) 2022, Nations Technologies Inc.
 
     All rights reserved.

     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
 referred to as NATIONS). This software, and the product of NATIONS described herein 
 (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
 of the People's Republic of China and other applicable jurisdictions worldwide.

     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
 intellectual property rights. Names and brands of third party may be mentioned or referred 
 thereto (if any) for identification purposes only.

     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
 improvements to this software at any time without notice. Please contact NATIONS and obtain 
 the latest version of this software before placing orders.

     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
 no responsibility for the accuracy and reliability of this software.
 
     It is the responsibility of the user of this software to properly design, program, and test 
 the functionality and safety of any application made of this information and any resulting product. 
 In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
 consequential damages arising in any way out of the use of this software or the Product.

     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
 malfunction or failure of which may cause loss of human life, bodily injury or severe property 
 damage. Such applications are deemed, "Insecure Usage".

     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
 harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
 to any customer's Insecure Usage.

     Any express or implied warranty with regard to this software or the Product, including,but not 
 limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
 are disclaimed to the fullest extent permitted by law.

     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
 or otherwise distribute this software for any purposes, in whole or in part.

     NATIONS products and technologies shall not be used for or incorporated into any products or systems
 whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
 User shall comply with any applicable export control laws and regulations promulgated and administered by 
 the governments of any countries asserting jurisdiction over the parties or transactions.
 ************************************************************************************************************/


  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system intitialization function.*/
  bl  System_Initializes
/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
\*fun      This is the code that gets called when the processor receives an
\*         unexpected interrupt.  This simply enters an infinite loop, preserving
\*         the system state for examination by a debugger.
**/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/**
\* The minimal vector table for a Cortex M0.  Note that the proper constructs
\* must be placed on this to ensure that it ends up at physical address 0x00000000.
**/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler  
  .word  WWDG_IRQHandler            /* Window Watchdog					*/
  .word  PVD_IRQHandler             /* PVD through EXTI Line detect     */
  .word  RTC_TAMPER_STAMP_IRQHandler/* EXTI Line18                      */
  .word  RTC_WKUP_IRQHandler        /* RTC_WKUP                         */
  .word  FLASH_IRQHandler           /* Flash                            */
  .word  RCC_IRQHandler             /* RCC                              */
  .word  EXTI0_IRQHandler           /* EXTI Line 0                      */
  .word  EXTI1_IRQHandler           /* EXTI Line 1                      */
  .word  EXTI2_IRQHandler           /* EXTI Line 2                      */
  .word  EXTI3_IRQHandler           /* EXTI Line 3                      */
  .word  EXTI4_IRQHandler           /* EXTI Line 4                      */
  .word  DMA_Channel1_IRQHandler    /* DMA Channel 1                    */
  .word  DMA_Channel2_IRQHandler    /* DMA Channel 2                    */
  .word  DMA_Channel3_IRQHandler    /* DMA Channel 3                    */
  .word  DMA_Channel4_IRQHandler    /* DMA Channel 4                    */
  .word  DMA_Channel5_IRQHandler    /* DMA Channel 5                    */
  .word  DMA_Channel6_IRQHandler    /* DMA Channel 6                    */
  .word  DMA_Channel7_IRQHandler    /* DMA Channel 7                    */
  .word  DMA_Channel8_IRQHandler    /* DMA Channel 8                    */
  .word  ADC_IRQHandler             /* ADC                              */
  .word  MMU_IRQHandler             /* MMU                              */
  .word  COMP_1_2_3_IRQHandler      /* COMP1 & COMP2 & COMP3            */
  .word  EXTI9_5_IRQHandler         /* EXTI Line 9..5                   */
  .word  TIM1_BRK_IRQHandler        /* TIM1 Break                       */
  .word  TIM1_UP_IRQHandler         /* TIM1 Update                      */
  .word  TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation     */
  .word  TIM1_CC_IRQHandler         /* TIM1 Capture Compare             */
  .word  TIM2_IRQHandler            /* TIM2                             */
  .word  TIM3_IRQHandler            /* TIM3                             */
  .word  TIM4_IRQHandler            /* TIM4                             */
  .word  I2C1_EV_IRQHandler         /* I2C1 Event                       */
  .word  I2C1_ER_IRQHandler         /* I2C1 Error                       */
  .word  I2C2_EV_IRQHandler         /* I2C2 Event                       */
  .word  I2C2_ER_IRQHandler         /* I2C2 Error                       */
  .word  SPI1_IRQHandler            /* SPI1                             */
  .word  SPI2_IRQHandler            /* SPI2/I2S2                        */
  .word  USART1_IRQHandler          /* USART1                           */
  .word  USART2_IRQHandler          /* USART2                           */
  .word  UART3_IRQHandler           /* UART3                            */
  .word  EXTI15_10_IRQHandler       /* EXTI Line 15..10                 */
  .word  RTCAlarm_IRQHandler        /* RTC Alarm through EXTI Line      */
  .word  LPTIM_WKUP_IRQHandler      /* LPTIM_WKUP through EXTI line 20  */
  .word  TIM8_BRK_IRQHandler        /* TIM8 Break                       */
  .word  TIM8_UP_IRQHandler         /* TIM8 Update                      */
  .word  TIM8_TRG_COM_IRQHandler    /* TIM8 Trigger and Commutation     */
  .word  TIM8_CC_IRQHandler         /* TIM8 Capture Compare             */
  .word  UART4_IRQHandler           /* UART4                            */
  .word  TIM5_IRQHandler            /* TIM5                             */
  .word  TIM6_IRQHandler            /* TIM6                             */

/**
\* Provide weak aliases for each Exception handler to the Default_Handler.
\* As they are weak aliases, any function with the same name will override
\* this definition.
**/

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler
  
  .weak      BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak      UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler

  .weak      PVD_IRQHandler
  .thumb_set PVD_IRQHandler,Default_Handler
  
  .weak      RTC_TAMPER_STAMP_IRQHandler
  .thumb_set RTC_TAMPER_STAMP_IRQHandler,Default_Handler
  
  .weak      RTC_WKUP_IRQHandler
  .thumb_set RTC_WKUP_IRQHandler,Default_Handler
  
  .weak      FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler

  .weak      RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler

  .weak      EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Default_Handler
  
  .weak      EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Default_Handler
  
  .weak      EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Default_Handler
  
  .weak      EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Default_Handler

  .weak      EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Default_Handler

  .weak      DMA_Channel1_IRQHandler
  .thumb_set DMA_Channel1_IRQHandler,Default_Handler
  
  .weak      DMA_Channel2_IRQHandler
  .thumb_set DMA_Channel2_IRQHandler,Default_Handler
  
  .weak      DMA_Channel3_IRQHandler
  .thumb_set DMA_Channel3_IRQHandler,Default_Handler
  
  .weak      DMA_Channel4_IRQHandler
  .thumb_set DMA_Channel4_IRQHandler,Default_Handler
  
  .weak      DMA_Channel5_IRQHandler
  .thumb_set DMA_Channel5_IRQHandler,Default_Handler
  
  .weak      DMA_Channel6_IRQHandler
  .thumb_set DMA_Channel6_IRQHandler,Default_Handler
  
  .weak      DMA_Channel7_IRQHandler
  .thumb_set DMA_Channel7_IRQHandler,Default_Handler

  .weak      DMA_Channel8_IRQHandler
  .thumb_set DMA_Channel8_IRQHandler,Default_Handler
  
  .weak      ADC_IRQHandler
  .thumb_set ADC_IRQHandler,Default_Handler
  
  .weak      MMU_IRQHandler
  .thumb_set MMU_IRQHandler,Default_Handler

  .weak      COMP_1_2_3_IRQHandler
  .thumb_set COMP_1_2_3_IRQHandler,Default_Handler
  
  .weak      EXTI9_5_IRQHandler
  .thumb_set EXTI9_5_IRQHandler,Default_Handler
  
  .weak      TIM1_BRK_IRQHandler
  .thumb_set TIM1_BRK_IRQHandler,Default_Handler
  
  .weak      TIM1_UP_IRQHandler
  .thumb_set TIM1_UP_IRQHandler,Default_Handler

  .weak      TIM1_TRG_COM_IRQHandler
  .thumb_set TIM1_TRG_COM_IRQHandler,Default_Handler

  .weak      TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler

  .weak      TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler
  
  .weak      TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler
  
  .weak      TIM4_IRQHandler
  .thumb_set TIM4_IRQHandler,Default_Handler

  .weak      I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Default_Handler

  .weak      I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Default_Handler

  .weak      I2C2_EV_IRQHandler
  .thumb_set I2C2_EV_IRQHandler,Default_Handler

  .weak      I2C2_ER_IRQHandler
  .thumb_set I2C2_ER_IRQHandler,Default_Handler

  .weak      SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler
  
  .weak      SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler

  .weak      USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler

  .weak      USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler

  .weak      UART3_IRQHandler
  .thumb_set UART3_IRQHandler,Default_Handler
  
  .weak      EXTI15_10_IRQHandler
  .thumb_set EXTI15_10_IRQHandler,Default_Handler
  
  .weak      RTCAlarm_IRQHandler
  .thumb_set RTCAlarm_IRQHandler,Default_Handler
  
  .weak      LPTIM_WKUP_IRQHandler
  .thumb_set LPTIM_WKUP_IRQHandler,Default_Handler

  .weak      TIM8_BRK_IRQHandler
  .thumb_set TIM8_BRK_IRQHandler,Default_Handler

  .weak      TIM8_UP_IRQHandler
  .thumb_set TIM8_UP_IRQHandler,Default_Handler
  
  .weak      TIM8_TRG_COM_IRQHandler
  .thumb_set TIM8_TRG_COM_IRQHandler,Default_Handler
  
  .weak      TIM8_CC_IRQHandler
  .thumb_set TIM8_CC_IRQHandler,Default_Handler
  
  .weak      UART4_IRQHandler
  .thumb_set UART4_IRQHandler,Default_Handler
  
  .weak      TIM5_IRQHandler
  .thumb_set TIM5_IRQHandler,Default_Handler
  
  .weak      TIM6_IRQHandler
  .thumb_set TIM6_IRQHandler,Default_Handler
  

