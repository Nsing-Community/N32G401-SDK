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
#include "GPTIM_common.h"
#include "ADTIM_common.h"
#include "timer_common.h"
#include "main.h"

uint16_t CCR1_Val       = 1000;
uint16_t CCR2_Val       = 500;
uint16_t CCR3_Val       = 250;
uint16_t CCR4_Val       = 125;
uint16_t PrescalerValue = 0;
uint32_t GPTIMClockFrequency = 0;

OCInitType TIM_OCInitStructure;

int main(void)
{


    /* System Clocks Configuration */
    GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(GPTIM, RCC_HCLK_DIV4);

    /* Configure the GPIO ports */
    Common_GPTIM_GPIO_Initialize(GPTIM);

    /*
    Generate 4 signals with 4 different delays:
    TIM3_CH1 delay = CCR1_Val/TIM3 counter clock = 500 ms
    TIM3_CH2 delay = CCR2_Val/TIM3 counter clock = 250 ms
    TIM3_CH3 delay = CCR3_Val/TIM3 counter clock = 125 ms
    TIM3_CH4 delay = CCR4_Val/TIM3 counter clock = 62.5 ms
    */
    PrescalerValue = (uint16_t)(GPTIMClockFrequency / 2000) - 1;
    /* TIM Base init , Period = 65535, Prescaler = PrescalerValue */
    Common_TIM_Base_Initialize(GPTIM, 65535, PrescalerValue);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);

    TIM_OCInitStructure.OcMode      = TIM_OCMODE_ACTIVE;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    
    /* Output Compare Active Mode configuration: Channel1 */
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = CCR1_Val;
    TIM_Output_Channel1_Initialize(GPTIM, &TIM_OCInitStructure);

    TIM_Output_Channel1_Preload_Set(GPTIM, TIM_OC_PRELOAD_DISABLE);

    /* Output Compare Active Mode configuration: Channel2 */
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = CCR2_Val;
    TIM_Output_Channel2_Initialize(GPTIM, &TIM_OCInitStructure);

    TIM_Output_Channel2_Preload_Set(GPTIM, TIM_OC_PRELOAD_DISABLE);

    /* Output Compare Active Mode configuration: Channel3 */
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = CCR3_Val;
    TIM_Output_Channel3_Initialize(GPTIM, &TIM_OCInitStructure);

    TIM_Output_Channel3_Preload_Set(GPTIM, TIM_OC_PRELOAD_DISABLE);

    /* Output Compare Active Mode configuration: Channel4 */
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = CCR4_Val;
    TIM_Output_Channel4_Initialize(GPTIM, &TIM_OCInitStructure);

    TIM_Output_Channel4_Preload_Set(GPTIM, TIM_OC_PRELOAD_DISABLE);

    TIM_Auto_Reload_Preload_Enable(GPTIM);

    TIM_On(GPTIM);

    while(1)
    {

    }
}
