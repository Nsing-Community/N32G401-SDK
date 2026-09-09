/**
*     Copyright (c) 2022, Nations Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
* referred to as NATIONS). This software, and the product of NATIONS described herein 
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NATIONS and obtain 
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file main.c
*\*\author Nations 
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "timer_common.h"
#include "ADTIM_common.h"
#include "GPTIM_common.h"
#include "main.h"

static TIM_ICInitType TIM_ICInitStructure;

uint16_t gOnePulsEn = 0;

__IO uint16_t IC3ReadValueL = 0, IC3ReadValueH = 0;
__IO uint16_t CaptureNumber   = 0;
__IO uint32_t Capture         = 0, CaptureH         = 0, CaptureL         = 0;
__IO uint32_t TIM3Freq        = 0;
__IO uint32_t TIM3EnterIrqCnt = 0;
__IO uint32_t GPTIMClockFrequency = 0;


void GPIO_Config(TIM_Module *TIMx);

void Two_Rising_Edge_Generate(void);

/**
*\*brief  Main program
**/
int main(void)
{
    /* System Clocks Configuration */
    GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);

    /* NVIC configuration */
    Common_TIM_NVIC_Initialize(TIM3_IRQn, ENABLE);

    /* Configure the GPIO ports */
    GPIO_Config(TIM3);

    /* TIM3 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM3 CH2 pin (PA.07) ,Rising edge
     The external signal is connected to TIM3 CH1 pin (PA.07) ,Falling edge
     The TIM3 CCDAT1+CCDAT2 is used to compute the frequency value

    gOnePulsEn = 1,will do Two_Rising_Edge_Generate
    then
    see Capture that is dist time
    ------------------------------------------------------------ */
    
    TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
    TIM_ICInitStructure.Channel     = TIM_CH_2;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0x0;
    TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);
    
    TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
    TIM_ICInitStructure.Channel     = TIM_CH_1;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_FALLING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_INDIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0x0;
    TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);
    /* TIM enable counter */
    TIM_On(TIM3);

    TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC1);
    TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC2);
    /* Enable the CC1 and CC2 Interrupt Request */
    TIM_Interrupt_Enable(TIM3, TIM_INT_CC1 | TIM_INT_CC2);

    while (1)
    {
        Two_Rising_Edge_Generate();
    }
}

// generation two rising edge
void Two_Rising_Edge_Generate(void)
{
    if (gOnePulsEn)
    {
        gOnePulsEn = 0;
        GPIO_Pins_Set(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 500)
                ;
        }
        GPIO_PBC_Pins_Reset(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 500)
                ;
        }

        GPIO_Pins_Set(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 500)
                ;
        }
        GPIO_PBC_Pins_Reset(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 500)
                ;
        }
    }
}

/**
*\*brief  Configure the GPIOA Pins.
**/
void GPIO_Config(TIM_Module *TIMx)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    
    if(TIM3 == TIMx)
    {
        /* TIM3 channel 2 pin (PA.07) configuration */
        GPIO_InitStructure.Pin            = TIM3_REMAP0_CH2_PIN;
        GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
        GPIO_InitStructure.GPIO_Current   = GPIO_DS_4MA;
        GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH2_AF;
        GPIO_Peripheral_Initialize(TIM3_REMAP0_CH2_PORT, &GPIO_InitStructure);
    }
    else
    {
        /* none */
    }

    /* PA3 */
    GPIO_InitStructure.Pin       = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\brief  This function handles TIM3 global interrupt request.
**/
void TIM3_IRQHandler(void)
{
    uint16_t IC3ReadValueH_t = 0;
    uint32_t CaptureH_t = 0;
    /* TIM3 CH2 Rising Edge */
    if (TIM_Interrupt_Status_Get(TIM3, TIM_INT_CC2) == SET)
    {
        TIM3EnterIrqCnt++;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC2);
        if (CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            IC3ReadValueL = TIM_Compare_Capture2_Get(TIM3);
            CaptureNumber = 1;
        }
        else if (CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            IC3ReadValueL = TIM_Compare_Capture2_Get(TIM3);
            /*For remove IAR Warning[Pa082]*/
            IC3ReadValueH_t = IC3ReadValueH;
            /* Capture computation */
            if (IC3ReadValueL > IC3ReadValueH_t)
            {
                CaptureL = (IC3ReadValueL - IC3ReadValueH_t);
            }
            else
            {
                CaptureL = ((0xFFFF - IC3ReadValueH_t) + IC3ReadValueL);
            }
            /*For remove IAR Warning[Pa082]*/
            CaptureH_t = CaptureH;
            /* Period computation */
            Capture = CaptureH_t + CaptureL;
            /* Frequency computation */
            TIM3Freq      = (uint32_t)GPTIMClockFrequency / Capture;
        }
    }
    else if (TIM_Interrupt_Status_Get(TIM3, TIM_INT_CC1) == SET)
    {
        TIM3EnterIrqCnt++;
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC1);
        if (CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            IC3ReadValueH = TIM_Compare_Capture1_Get(TIM3);
            CaptureNumber = 1;
        }
        else if (CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            IC3ReadValueH = TIM_Compare_Capture1_Get(TIM3);
            /*For remove IAR Warning[Pa082]*/
            IC3ReadValueH_t = IC3ReadValueH;
            /* Capture computation */
            if (IC3ReadValueH_t > IC3ReadValueL)
            {
                CaptureH = (IC3ReadValueH_t - IC3ReadValueL);
            }
            else
            {
                CaptureH = ((0xFFFF - IC3ReadValueL) + IC3ReadValueH_t);
            }
        }
    }
}
/**
 * @}
 */

/**
 * @}
 */
