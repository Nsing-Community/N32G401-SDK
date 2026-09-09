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
#include "ADTIM_common.h"
#include "GPTIM_common.h"
#include "timer_common.h"
#include "main.h"

static OCInitType TIM_OCInitStructure;


int main(void)
{
    Common_GPTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);

    Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);
    
    Common_GPTIM_RCC_Initialize(TIM4, RCC_HCLK_DIV4);

    Common_GPTIM_GPIO_Initialize(TIM2);

    Common_GPTIM_GPIO_Initialize(TIM3);

    Common_GPTIM_GPIO_Initialize(TIM4);

    /**  Timers synchronisation in cascade mode 
     1/TIM2 is configured as Master Timer:
     - PWM Mode is used
     - The TIM2 Update event is used as Trigger Output

     2/TIM3 is slave for TIM2 and Master for TIM4,
     - PWM Mode is used
     - The ITR1(TIM2) is used as input trigger
     - Gated mode is used, so start and stop of slave counter
       are controlled by the Master trigger output signal(TIM2 update event).
     - The TIM3 Update event is used as Trigger Output.

      3/TIM4 is slave for TIM3,
     - PWM Mode is used
     - The ITR2(TIM3) is used as input trigger
     - Gated mode is used, so start and stop of slave counter
       are controlled by the Master trigger output signal(TIM3 update event).

       The TIMxCLK is fixed to 36 MHz, the TIM2 counter clock is 36 MHz.

       The Master Timer TIM2 is running at TIM2 frequency :
       TIM2 frequency = (TIM2 counter clock)/ (TIM2 period + 1) = 125 KHz
       and the duty cycle = TIM2_CCR1/(TIM2_ARR + 1) = 25%.

       The TIM3 is running:
       - At (TIM2 frequency)/ (TIM3 period + 1) = 31.25 KHz and a duty cycle
         equal to TIM3_CCR1/(TIM3_ARR + 1) = 25%

        The TIM4 is running:
      - At (TIM3 frequency)/ (TIM4 period + 1) = 7.8125 KHz and a duty cycle
        equal to TIM4_CCR1/(TIM4_ARR + 1) = 25%  **/

    /* Timer 2 base configuration, Period = 255, Prescaler = 0 */
    Common_TIM_Base_Initialize(TIM2, 287, 0);
    
    /* Timer 3 base configuration, Period = 3, Prescaler = 0 */
    Common_TIM_Base_Initialize(TIM3, 3, 0);

    /* Timer 4 base configuration, Period = 3, Prescaler = 0 */
    Common_TIM_Base_Initialize(TIM4, 3, 0);

    /* Master Configuration in PWM1 Mode */
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 64;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    TIM_Output_Channel1_Initialize(TIM2, &TIM_OCInitStructure);

    /* Select the Master Slave Mode */
    TIM_Master_Slave_Mode_Set(TIM2, TIM_MASTER_SLAVE_MODE_ENABLE);

    /* Master Mode selection: TIM2 */
    TIM_Output_Trigger_Select(TIM2, TIM_TRGO_SRC_UPDATE);

    /* Slaves Configuration: Toggle Mode */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 1;
    TIM_Output_Channel1_Initialize(TIM3, &TIM_OCInitStructure);

    TIM_Output_Channel1_Initialize(TIM4, &TIM_OCInitStructure);

    /* Slave Mode selection: TIM3 */
    TIM_Slave_Mode_Select(TIM3, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM3, TIM_TRIG_SEL_IN_TR1);

    /* Select the Master Slave Mode */
    TIM_Master_Slave_Mode_Set(TIM3, TIM_MASTER_SLAVE_MODE_ENABLE);

    /* Master Mode selection: TIM3 */
    TIM_Output_Trigger_Select(TIM3, TIM_TRGO_SRC_UPDATE);

    /* Slave Mode selection: TIM3 */
    TIM_Slave_Mode_Select(TIM4, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM4, TIM_TRIG_SEL_IN_TR2);

    /* TIM enable counter */
    TIM_On(TIM2);
    TIM_On(TIM3);
    TIM_On(TIM4);

    while (1)
    {
    }
}
