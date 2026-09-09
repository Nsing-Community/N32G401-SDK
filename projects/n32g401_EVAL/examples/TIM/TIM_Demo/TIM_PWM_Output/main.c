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
#include "timer_common.h"
#include "main.h"

static OCInitType TIM_OCInitStructure;

static TIM_BDTRInitType TIM_BDTRInitStructure;

uint16_t TimerPeriod   = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, channel4Pulse = 0;

uint32_t ADTIMClockFrequency = 0;;

int main(void)
{
    ADTIMClockFrequency = Common_ADTIM_RCC_Initialize(ADTIM, RCC_HCLK_DIV2);

    Common_ADTIM_GPIO_Initialize(ADTIM);

    Common_ADTIM_Break_IO_Initialize(ADTIM);

    /* 
    ADTIM Configuration to:

    1/ Generate 4 complementary PWM signals with 4 different duty cycles:
    ADTIMCLK is fixed to SystemCoreClock, the ADTIM Prescaler is equal to 0 so the
    ADTIM counter clock used is SystemCoreClock.
    * SystemCoreClock is set to 74 MHz.

    The objective is to generate PWM signal at 17.57 KHz:
    - ADTIM_Period = (SystemCoreClock / 17570) - 1
    
    The Three Duty cycles are computed as the following description:

    The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
    The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
    The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
    The channel 4 duty cycle is set to 12.5% so channel 4N is set to 87.5%.
    The Timer pulse is calculated as follows:
      - ChannelxPulse = DutyCycle * (ADTIM_Period - 1) / 100

    2/ Insert a dead time equal to 11/SystemCoreClock ns
    3/ Configure the break feature, active at High level, and using the automatic
     output enable feature
    4/ Use the Locking parameters level1.
                                                                                */

    /* Compute the value to be set in AR register to generate signal frequency at 17.57 Khz */
    TimerPeriod = (ADTIMClockFrequency / 17570) - 1;
    /* Compute CCDAT1 value to generate a duty cycle at 50% for channel 1 */
    Channel1Pulse = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);
    /* Compute CCDAT2 value to generate a duty cycle at 25%  for channel 2 */
    Channel2Pulse = (uint16_t)(((uint32_t)25 * (TimerPeriod - 1)) / 100);
    /* Compute CCDAT3 value to generate a duty cycle at 12.5%  for channel 3 */
    Channel3Pulse = (uint16_t)(((uint32_t)125 * (TimerPeriod - 1)) / 1000);
    /* Compute CCDAT4 value to generate a duty cycle at 12.5%  for channel 3 */
    channel4Pulse = (uint16_t)(((uint32_t)125 * (TimerPeriod - 1)) / 1000);

    /* TIM Base Init, Period = TimerPeriod, Prescaler = 0 */
    Common_TIM_Base_Initialize(ADTIM, TimerPeriod, 0);
    
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);

    /* Channel 1, 2 and 3 Configuration in PWM mode */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel1Pulse;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OCN_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(ADTIM, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel2Pulse;
    TIM_Output_Channel2_Initialize(ADTIM, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel3Pulse;
    TIM_Output_Channel3_Initialize(ADTIM, &TIM_OCInitStructure);

    /* Output Compare Active Mode configuration: Channel4 */
    /* The CH4N if only for ADTIM, not for ADTIM */
    TIM_OCInitStructure.Pulse = channel4Pulse;
    TIM_Output_Channel4_Initialize(ADTIM, &TIM_OCInitStructure);

    /* Automatic Output enable, Break, dead time and lock configuration*/
    TIM_Break_And_Dead_Time_Struct_Initialize(&TIM_BDTRInitStructure);
    
    TIM_BDTRInitStructure.OssrState       = TIM_OSSR_STATE_ENABLE;
    TIM_BDTRInitStructure.OssiState       = TIM_OSSI_STATE_ENABLE;
    TIM_BDTRInitStructure.LockLevel       = TIM_LOCK_LEVEL_1;
    TIM_BDTRInitStructure.DeadTime        = 11;
    TIM_BDTRInitStructure.Break           = TIM_BREAK_IN_ENABLE;
    TIM_BDTRInitStructure.BreakPolarity   = TIM_BREAK_POLARITY_HIGH;
    TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
    TIM_BDTRInitStructure.IomBreakEn      = true;

    TIM_Break_And_Dead_Time_Set(ADTIM, &TIM_BDTRInitStructure);

    TIM_On(ADTIM);
    TIM_PWM_Output_Enable(ADTIM);



    while(1)
    {

    }
}
