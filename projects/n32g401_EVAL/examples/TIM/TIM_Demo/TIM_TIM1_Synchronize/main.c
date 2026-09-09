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

static OCInitType TIM_OCInitStructure;

static TIM_TimeBaseInitType TIM_TimeBaseStructure;

static TIM_BDTRInitType TIM_BDTRInitStructure;

int main(void)
{
    Common_ADTIM_RCC_Initialize(ADTIM, RCC_HCLK_DIV4);

    Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);

    Common_GPTIM_RCC_Initialize(TIM4, RCC_HCLK_DIV4);

    Common_ADTIM_GPIO_Initialize(TIM1);

    Common_GPTIM_GPIO_Initialize(TIM3);
    
    Common_GPTIM_GPIO_Initialize(TIM4);

    /* TIM1 and Timers(TIM3 and TIM4) synchronisation in parallel mode 
     1/TIM1 is configured as Master Timer:
     - PWM Mode is used
     - The TIM1 Update event is used as Trigger Output

     2/TIM3 and TIM4 are slaves for TIM1,
     - PWM Mode is used
     - The ITR0(TIM1) is used as input trigger for both slaves
     - Gated mode is used, so starts and stops of slaves counters
       are controlled by the Master trigger output signal(update event).

    The Master Timer TIM1 is running at:
    TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 140.625 KHz
    and the duty cycle is equal to: TIM1_CCR1/(TIM1_ARR + 1) = 50%

    The TIM3 is running at:
    (TIM1 frequency)/ ((TIM3 period +1)* (Repetition_Counter+1)) = 9.37 KHz and
    a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 33.3%

    The TIM4 is running at:
    (TIM1 frequency)/ ((TIM4 period +1)* (Repetition_Counter+1)) = 14.063 KHz and
    a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 50%
                                                                             */

    /* Time base configuration */
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = 255;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.RepetCnt  = 4;

    TIM_Base_Initialize(TIM1, &TIM_TimeBaseStructure);

    Common_TIM_Base_Initialize(TIM3, 2, 0);

    Common_TIM_Base_Initialize(TIM4, 1, 0);

    /* Channel 1 Configuration in PWM mode */
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 127;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(ADTIM, &TIM_OCInitStructure);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 1;
    TIM_Output_Channel1_Initialize(TIM3, &TIM_OCInitStructure);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 1;
    TIM_Output_Channel1_Initialize(TIM4, &TIM_OCInitStructure);


    /* Select the Master Slave Mode */
    TIM_Master_Slave_Mode_Set(ADTIM, TIM_MASTER_SLAVE_MODE_ENABLE);
    /* Master Mode selection: ADTIM */
    TIM_Output_Trigger_Select(ADTIM, TIM_TRGO_SRC_UPDATE);


    /* Slave Mode selection: TIM3 */
    TIM_Slave_Mode_Select(TIM3, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM3, TIM_TRIG_SEL_IN_TR0);

    /* Slave Mode selection: TIM4 */
    TIM_Slave_Mode_Select(TIM4, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM4, TIM_TRIG_SEL_IN_TR0);

    /* Automatic Output enable, Break, dead time and lock configuration*/
    TIM_Break_And_Dead_Time_Struct_Initialize(&TIM_BDTRInitStructure);
    TIM_BDTRInitStructure.OssrState       = TIM_OSSR_STATE_ENABLE;
    TIM_BDTRInitStructure.OssiState       = TIM_OSSI_STATE_ENABLE;
    TIM_BDTRInitStructure.LockLevel       = TIM_LOCK_LEVEL_1;
    TIM_BDTRInitStructure.DeadTime        = 5;
    TIM_BDTRInitStructure.Break           = TIM_BREAK_IN_DISABLE;
    TIM_BDTRInitStructure.BreakPolarity   = TIM_BREAK_POLARITY_HIGH;
    TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_DISABLE;

    TIM_Break_And_Dead_Time_Set(ADTIM, &TIM_BDTRInitStructure);

    /* ADTIM Main Output Enable */
    TIM_PWM_Output_Enable(ADTIM);
    /* TIM enable counter */
    TIM_On(ADTIM);
    TIM_On(TIM3);
    TIM_On(TIM4);

    while (1)
    {
    }
}
