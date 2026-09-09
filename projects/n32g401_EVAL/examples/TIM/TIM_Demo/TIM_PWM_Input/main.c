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

__IO uint16_t IC2Value  = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;

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

    /* TIM3 configuration: PWM Input mode
     The external signal is connected to TIM3 CH2 pin (PA.07),
     The Rising edge is used as active edge,
     The TIM3 CCDAT2 is used to compute the frequency value
     The TIM3 CCDAT1 is used to compute the duty cycle value
                                                                 */
    TIM_Input_Struct_Initialize(&TIM_ICInitStructure);    
    TIM_ICInitStructure.Channel     = TIM_CH_2;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0x0;
    TIM_PWM_Input_Channel_Config(TIM3, &TIM_ICInitStructure);
    
    TIM_Trigger_Source_Select(TIM3, TIM_TRIG_SEL_TI2FP2);

    TIM_Slave_Mode_Select(TIM3, TIM_SLAVE_MODE_RESET);

    TIM_Master_Slave_Mode_Set(TIM3, TIM_MASTER_SLAVE_MODE_ENABLE);
    /* TIM enable counter */
    TIM_On(TIM3);

    TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC2);
    /* Enable the CC1 and CC2 Interrupt Request */
    TIM_Interrupt_Enable(TIM3, TIM_INT_CC2);

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
            while (i++ < 50)
                ;
        }

        GPIO_PBC_Pins_Reset(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 50)
                ;
        }

        GPIO_Pins_Set(GPIOA, GPIO_PIN_3);
        {
            uint32_t i = 0;
            while (i++ < 50)
                ;
        }

        GPIO_PBC_Pins_Reset(GPIOA, GPIO_PIN_3);
    }
}

/**
*\*brief  Configure the GPIOx Pins.
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
    /* Clear TIM3 Capture compare interrupt pending bit */
    TIM_Interrupt_Status_Clear(TIM3, TIM_INT_CC2);

    /* Get the Input Capture value */
    IC2Value = TIM_Compare_Capture2_Get(TIM3);

    if (IC2Value != 0)
    {
        /* Duty cycle computation */
        DutyCycle = (TIM_Compare_Capture1_Get(TIM3) * 100) / IC2Value;

        /* Frequency computation */
        Frequency = GPTIMClockFrequency / IC2Value;
    }
    else
    {
        DutyCycle = 0;
        Frequency = 0;
    }
}
/**
 * @}
 */

/**
 * @}
 */
