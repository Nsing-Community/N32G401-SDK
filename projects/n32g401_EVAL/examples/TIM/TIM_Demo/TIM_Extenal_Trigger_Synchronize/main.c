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

static TIM_ICInitType TIM_ICInitStructure;

/**
 * @brief  Configure the GPIO Pins.
 */
static void GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* GPIOA Configuration: PA.08(TIM1 CH1) ,PA.00(TIM2 CH1)and PA.06(TIM3 CH1) as alternate function push-pull */
    GPIO_InitStructure.Pin        = TIM1_REMAP0_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH1_AF;
    GPIO_Peripheral_Initialize(TIM1_REMAP0_CH1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = TIM2_REMAP0_CH1_PIN;
    GPIO_InitStructure.GPIO_Alternate = TIM2_REMAP0_CH1_AF;
    GPIO_Peripheral_Initialize(TIM2_REMAP0_CH1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = TIM3_REMAP0_CH1_PIN;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH1_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH1_PORT, &GPIO_InitStructure);

    /* GPIOA Configuration: PA.09(TIM1 CH2) */
    GPIO_InitStructure.Pin       = TIM1_REMAP0_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH2_AF;
    GPIO_Peripheral_Initialize(TIM1_REMAP0_CH2_PORT, &GPIO_InitStructure);
}

int main(void)
{
    Common_ADTIM_RCC_Initialize(ADTIM, RCC_HCLK_DIV4);

    Common_GPTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);

    Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);

    GPIO_Config();

    /* Timers synchronisation in cascade mode with an external trigger 
    1/TIM1 is configured as Master Timer:
     - Toggle Mode is used
     - The TIM1 Enable event is used as Trigger Output

    2/TIM1 is configured as Slave Timer for an external Trigger connected
     to TIM1 TI2 pin (TIM1 CH2 configured as input pin):
     - The TIM1 TI2FP2 is used as Trigger Input
     - Rising edge is used to start and stop the TIM1: Gated Mode.

    3/TIM2 is slave for TIM1
     - Toggle Mode is used
     - The ITR0(TIM1) is used as input trigger
     - Gated mode is used, so start and stop of slave counter
       are controlled by the Master trigger output signal(TIM1 enable event).

    4/TIM3 is slave for TIM2,
     - Toggle Mode is used
     - The ITR1(TIM2) is used as input trigger
     - Gated mode is used, so start and stop of slave counter
       are controlled by the Master trigger output signal(TIM2 enable event).

    * For N32G401 devices:
      The TIMxCLK is fixed to 36 MHZ, the Prescaler is equal to 3 so the TIMx clock
      counter is equal to 9 MHz.
      The Two Timers are running at:
      TIMx frequency = TIMx clock counter/ 2*(TIMx_Period + 1) = 50KHz.

    The starts and stops of the TIM1 counters are controlled by the
    external trigger.
    The TIM2 starts and stops are controlled by the TIM1, and the TIM3
    starts and stops are controlled by the TIM2.
    -------------------------------------------------------------------- */
    /* Time base configuration */

    Common_TIM_Base_Initialize(ADTIM, 89, 3);

    Common_TIM_Base_Initialize(TIM2, 89, 3);

    Common_TIM_Base_Initialize(TIM3, 89, 3);

    /* Channel 1 Configuration in toggle mode */
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_TOGGLE;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 64;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    TIM_Output_Channel1_Initialize(ADTIM, &TIM_OCInitStructure);

    /* ADTIM Input Capture Configuration */
    TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
    TIM_ICInitStructure.Channel     = TIM_CH_2;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(ADTIM, &TIM_ICInitStructure);

    /* ADTIM Input trigger configuration: External Trigger connected to TI2 */
    TIM_Trigger_Source_Select(ADTIM, TIM_TRIG_SEL_TI2FP2);
    TIM_Slave_Mode_Select(ADTIM, TIM_SLAVE_MODE_GATED);

    /* Select the Master Slave Mode */
    TIM_Master_Slave_Mode_Set(ADTIM, TIM_MASTER_SLAVE_MODE_ENABLE);

    /* Master Mode selection: ADTIM */
    TIM_Output_Trigger_Select(ADTIM, TIM_TRGO_SRC_ENABLE);

    /* Slaves Configuration: Toggle Mode */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_TOGGLE;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_Output_Channel1_Initialize(TIM2, &TIM_OCInitStructure);

    TIM_Output_Channel1_Initialize(TIM3, &TIM_OCInitStructure);

    /* Slave Mode selection: TIM2 */
    TIM_Slave_Mode_Select(TIM2, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM2, TIM_TRIG_SEL_IN_TR0);

    /* Select the Master Slave Mode */
    TIM_Master_Slave_Mode_Set(TIM2, TIM_MASTER_SLAVE_MODE_ENABLE);

    /* Master Mode selection: TIM2 */
    TIM_Output_Trigger_Select(TIM2, TIM_TRGO_SRC_ENABLE);

    /* Slave Mode selection: TIM3 */
    TIM_Slave_Mode_Select(TIM3, TIM_SLAVE_MODE_GATED);
    TIM_Trigger_Source_Select(TIM3, TIM_TRIG_SEL_IN_TR1);

    /* ADTIM Main Output Enable */
    TIM_PWM_Output_Enable(ADTIM);

    /* TIM enable counter */
    TIM_On(ADTIM);
    TIM_On(TIM2);
    TIM_On(TIM3);

    while (1)
    {
    }
}
