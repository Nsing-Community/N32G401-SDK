/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32g401_it.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "main.h"
#include "n32g401.h"
#include "n32g401_it.h"
#include "n32g401_STLparam.h"
#include "n32g401_STLclassBvar.h"
#include "n32g401_STLlib.h"
#include <stdio.h>
volatile static uint16_t tmpCC3_last;	/* keep last TIM2/Chn4 captured value */


/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 */
void PendSV_Handler(void)
{
}

/**
* @brief This function handles System tick timer.
 *1.get 2ms SysTick to test main check
 *2.test 
*/
//1ms
void SysTick_Handler(void)
{
    IncTick();
    
    /* Verify TickCounter integrity */
    if ((TickCounter ^ TickCounterInv) == 0xFFFFFFFFuL)
    {
        TickCounter++;
        TickCounterInv = ~TickCounter;

        if (TickCounter >= SYSTICK_10ms_TB)
        {
          uint32_t RamTestResult;

          /* Reset timebase counter */
          TickCounter = 0u;
          TickCounterInv = 0xFFFFFFFF;

          /* Set Flag read in main loop */
          TimeBaseFlag = 0xAAAAAAAAuL;
          TimeBaseFlagInv = 0x55555555uL;

         
      /*----------------------------------------------------------------------*/
      /*------------------     RAM test(in interrupt)  -----------------------*/
      /*----------------------------------------------------------------------*/
            ISRCtrlFlowCnt += RAM_MARCHC_ISR_CALLER;
            __disable_irq();
            RamTestResult = STL_TranspMarch();
            __enable_irq();
            ISRCtrlFlowCntInv -= RAM_MARCHC_ISR_CALLER; 
          switch ( RamTestResult )
          {
            case TEST_RUNNING:
              break;
            case TEST_OK:
              #ifdef STL_VERBOSE
               /* avoid any long string output here in the interrupt, '#' marks ram test completed ok */
                putchar((int)'#');      /* RAM OK mark */
              #endif  /* STL_VERBOSE */
              break;
            case TEST_FAILURE:
            case CLASS_B_DATA_FAIL:
            default:
              #ifdef STL_VERBOSE
                printf("\n\r >>>>>>>>>>>>>>>>>>>  RAM Error (March C- Run-time check)\n\r");
              #endif  /* STL_VERBOSE */
              FailSafePOR();
              break;
          } /* End of the switch */

          /* Do we reached the end of RAM test? */
          /* Verify 1st ISRCtrlFlowCnt integrity */
          if ((ISRCtrlFlowCnt ^ ISRCtrlFlowCntInv) == 0xFFFFFFFFuL)
          {
            if (RamTestResult == TEST_OK)
            {
              if (ISRCtrlFlowCnt != RAM_TEST_COMPLETED)
              {
                  #ifdef STL_VERBOSE
                    printf("\n\r Control Flow error (RAM test) \n\r");
                  #endif  /* STL_VERBOSE */
                  FailSafePOR();
              }
              else  /* Full RAM was scanned */
              {
                 ISRCtrlFlowCnt = 0u;
                 ISRCtrlFlowCntInv = 0xFFFFFFFFuL;
              }
            } /* End of RAM completed if*/
          } /* End of control flow monitoring */
          else
          {
              #ifdef STL_VERBOSE
                printf("\n\r Control Flow error in ISR \n\r");
              #endif  /* STL_VERBOSE */
              FailSafePOR();
          }         
       
        } /* End of the 20 ms timebase interrupt */
    }

}

/******************************************************************************/
/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  : None
  * @retval : None
  */
void TIM2_IRQHandler(void)
{
  uint16_t tmpCC3_last_cpy;
	
  if ((TIM2->STS & TIM_STS_CC3ITF) != 0u )
  {
    /* store previous captured value */
    tmpCC3_last_cpy = tmpCC3_last; 
    /* get currently captured value */
    tmpCC3_last = (uint16_t)(TIM2->CCDAT3);
    /* The CC4IF flag is already cleared here be reading CCR4 register */

    /* overight results only in case the data is required */
    if (LSIPeriodFlag == 0u)
    {
      /* take correct measurement only */
      if ((TIM2->STS & TIM_STS_CC3OCF) == 0u)
      {
        /* Compute period length */
        PeriodValue = ((uint32_t)(tmpCC3_last) - (uint32_t)(tmpCC3_last_cpy))& 0xFFFFuL;      
        PeriodValueInv = ~PeriodValue;
      
        /* Set Flag tested at main loop */
        LSIPeriodFlag = 0xAAAAAAAAuL;
        LSIPeriodFlagInv = 0x55555555uL;
      }
      else
      {
        /* ignore computation in case of IC overflow */
        TIM2->STS &= (~TIM_STS_CC3OCF);
      }
    }
    /* ignore computation in case data is not required */
  }
}

/******************************************************************************/
/**
  * @brief Configure TIM2 to measure LSI period
  * @param  : None
  * @retval : ErrorStatus = (ERROR, SUCCESS)
  */
ErrorStatus SelfTest_InitClock_Xcross_Measurement(void)
{
  ErrorStatus result = SUCCESS;

  TIM_TimeBaseInitType TIM_BaseStructure;
  TIM_ICInitType TIM_ICStructure;
  NVIC_InitType NVIC_InitStructure; 
 
  /*## Enable peripherals and GPIO Clocks ####################################*/
  /* TIMx Peripheral clock enable */
  RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM2);
  /*## Configure the NVIC for TIMx ###########################################*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Initializes(&NVIC_InitStructure);
 
  /* TIM2 configuration: Input Capture mode ---------------------
  The LSI oscillator is connected to TIM2 CH3.
  The Rising edge is used as active edge, input divided by 8
  The TIM2 CC4 is used to compute the frequency value. 
  ------------------------------------------------------------ */
	
	TIM_Base_Struct_Initialize(&TIM_BaseStructure);
	
  TIM_BaseStructure.Prescaler = 0;
  TIM_BaseStructure.CntMode = TIM_CNT_MODE_UP;  
  TIM_BaseStructure.Period = 0xFFFF;  
  TIM_BaseStructure.ClkDiv = 0;
  TIM_BaseStructure.RepetCnt = 0;
	TIM_BaseStructure.CapCh3Sel = true;
  TIM_Base_Initialize(TIM2, &TIM_BaseStructure);
	
	//TIM_Base_Channel3(TIM2, 1);

  /* Configure the TIM2 Input Capture of channel 4 */
  TIM_ICStructure.Channel = TIM_CH_3;
  TIM_ICStructure.IcPolarity = TIM_IC_POLARITY_RISING;
  TIM_ICStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
  TIM_ICStructure.IcPrescaler = TIM_IC_PSC_DIV8; // IcPrescaler = TIM_IC_PSC_DIV8,limit should * 8
  TIM_ICStructure.IcFilter = 0x00;
  TIM_Input_Channel_Initialize(TIM2,&TIM_ICStructure);
  /* Reset the flags */
  LSIPeriodFlag = 0u;
  /* Start the TIM Input Capture measurement in interrupt mode */
  TIM_Interrupt_Enable(TIM2, TIM_INT_CC3);
  TIM_On(TIM2);
  return(result);
}

/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA_Channel1_2_IRQnHandler(void)
{
  
}

/**
* @brief This function handles TIM1 break, update, trigger and commutation interrupts.
*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  
}



/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
 
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C)  *****END OF FILE****/
