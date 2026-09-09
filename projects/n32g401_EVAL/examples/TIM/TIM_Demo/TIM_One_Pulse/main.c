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
static OCInitType TIM_OCInitStructure;
static uint16_t PrescalerValue = 0;
uint16_t gSendTrigEn = 0;
uint32_t GPTIMClockFrequency = 0;

void GPIO_Config(void);
void One_Trig_Send(void);

/**
*\*\brief  Main program
**/
int main(void)
{
    /* System Clocks Configuration */
    GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(GPTIM, RCC_HCLK_DIV4);

    /* Configure the GPIO ports */
    GPIO_Config();

    /* GPTIM configuration: One Pulse mode 
     The external signal is connected to GPTIM_CH2 pin (PA.07),
     The Rising edge is used as active edge,
     The One Pulse signal is output on GPTIM_CH1 pin (PA.06)
     The Pulse defines the delay value
     The (Period -  Pulse) defines the One Pulse value.
     TIM3CLK = 36MHz, we want to get TIM3 counter clock at 18 MHz:
     - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
     The Autoreload value is 65535 (GPTIM->AR), so the maximum frequency value
     to trigger the GPTIM input is 18000000/65535 = 274.7HZ


     delay =  CCDAT1/GPTIM counter clock = 909us.
     The (Period - Pulse) defines the One Pulse value,
     One Pulse value = (Period - Pulse) / GPTIM counter clock = 2.73 ms.

                                                                 */

    PrescalerValue = (uint16_t)(GPTIMClockFrequency / 18000000) - 1;

    /* Time base configuration, period = 65535, prescaler = prescaler */
    Common_TIM_Base_Initialize(GPTIM, 65535, PrescalerValue);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    /* TIM Configuration in PWM Mode */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 16383;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    TIM_Output_Channel1_Initialize(GPTIM, &TIM_OCInitStructure);
    
    TIM_Input_Struct_Initialize(&TIM_ICInitStructure);    
    TIM_ICInitStructure.Channel     = TIM_CH_2;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;

    TIM_Input_Channel_Initialize(GPTIM, &TIM_ICInitStructure);

    /* One Pulse Mode selection */
    TIM_One_Pulse_Mode_Select(GPTIM, TIM_OPMODE_SINGLE);

    /* Input Trigger selection */
    TIM_Trigger_Source_Select(GPTIM, TIM_TRIG_SEL_TI2FP2);

    TIM_Slave_Mode_Select(GPTIM, TIM_SLAVE_MODE_TRIG);

    while(1)
    {
        One_Trig_Send();
    }
}


/**
*\*\brief  Send one trig by self.
**/
void One_Trig_Send(void)
{
    if (gSendTrigEn)
    {
        gSendTrigEn = 0;
        GPIO_Pins_Set(GPIOA, GPIO_PIN_3);
        {
            uint32_t i=0;
            while (i++ < 10)
                ;
        }
        GPIO_PBC_Pins_Reset(GPIOA, GPIO_PIN_3);
    }
}

/**
*\*\brief  Configure the GPIOA Pins.
**/
void GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* TIM4_CH1 pin (PA.06) configuration as out*/
    GPIO_InitStructure.Pin        = TIM3_REMAP0_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH1_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH1_PORT, &GPIO_InitStructure);

    /* TIM4_CH2 pin (PA.07) configuration as trig*/
    GPIO_InitStructure.Pin       = TIM3_REMAP0_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH2_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH2_PORT, &GPIO_InitStructure);

    /* pin (PA.03) configuration as send trig*/
    GPIO_InitStructure.Pin       = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}
