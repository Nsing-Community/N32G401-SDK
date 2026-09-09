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
*\*\version v1.1.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"


TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
TIM_BDTRInitType TIM_BDTRInitStructure;
uint16_t TimerPeriod   = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0;

void RCC_Configuration(void);
void GPIO_CompConfiguration(void);
void GPIO_TimConfiguration(void);
void COMP_Configuratoin(void);
void TIM_Intial(TIM_Module* TIMx);
void NVIC_Configuration(void);
void ChangeVmVp(void);


/**
*\*\name    Main program.
*\*\return  none
**/
int main(void)
{
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_CompConfiguration();
    GPIO_TimConfiguration();

    /* tim pwm configuration ------------------------------------------------------*/
    TIM_Intial(TIM1);
    TIM_Intial(TIM8);

    /* COMP configuration ------------------------------------------------------*/
    COMP_Configuratoin();
    while (1)
    {
        ChangeVmVp();
    }
}

/**
*\*\name    ChangeVmVp.
*\*\fun     Self Generate Puls ,by skip line connect to vp and vm if need.
*\*\return  none
**/
void ChangeVmVp(void)
{
    GPIO_Pins_Set(GPIOB, GPIO_PIN_11);
    GPIO_PBSC_Pins_Reset(GPIOB, GPIO_PIN_12);
    {
        uint32_t i = 0;
        while (i++ < 10000)
            ;
    }
    GPIO_PBSC_Pins_Reset(GPIOB, GPIO_PIN_11);
    GPIO_Pins_Set(GPIOB, GPIO_PIN_12);
    {
        uint32_t i = 0;
        while (i++ < 10000)
            ;
    }
}
/**
*\*\name    COMP_Configuratoin.
*\*\fun     Configures the comp module.
*\*\return  none
**/
void COMP_Configuratoin(void)
{
    COMP_InitType COMP_Initial;

    /*Set dac3,dac2,dac1 */
    COMP_Voltage_Reference_Config(63, true, 32, true, 16, true);
    /*Initial comp*/
    COMP_Initializes_Structure(&COMP_Initial);
    COMP_Initial.InpSel     = COMP1_INPSEL_PB10;
    COMP_Initial.InmSel     = COMP1_INMSEL_PA5;
    COMP_Initial.SampWindow = 18;       //(0~31)
    COMP_Initial.Threshold  = 12;       //Thresh should be greater than half of SampWindow and should be less than SampWindow at the same time.
    COMP_Initializes(COMP1, &COMP_Initial);
    /*trig initial as tim1&tim8 break*/
    COMP_Output_Trigger_Config(COMP1, COMP1_OUTSEL_TIM1_BKIN_TIM8_BKIN);
    /*enable comp1*/
    COMP_ON(COMP1);
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable COMP clocks */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_COMP | RCC_APB1_PERIPH_COMP_FILT);
    /* Enable GPIOA, GPIOB, GPIOC and GPIOD clocks */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB | RCC_AHB_PERIPH_GPIOC | RCC_AHB_PERIPH_GPIOD);
    /* Enable TIM1 TIM8 AFIO clocks */
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_TIM1 | RCC_APB2_PERIPH_TIM8 | RCC_APB2_PERIPH_AFIO);
    
}
/**
*\*\name    TIM_Intial.
*\*\fun     Configures timx program.
*\*\return  none
**/
void TIM_Intial(TIM_Module* TIMx)
{
    /* TIMx Configuration ---------------------------------------------------
    Generate 6 PWM signals with 3 different duty cycles:
    TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
    SystemCoreClock is set to 64 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 32 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The objective is to generate 6 PWM signal at 17.57 KHz:
     - TIMx_Period = (SystemCoreClock / 17570) - 1
    The channel 1 and channel 1N duty cycle is set to 50%
    The channel 2 and channel 2N duty cycle is set to 37.5%
    The channel 3 and channel 3N duty cycle is set to 25%
    The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
    ----------------------------------------------------------------------- */
    /* Compute the value to be set in AR regiter to generate signal frequency at 17.57 Khz */
    TimerPeriod = (SystemClockFrequency / 17570 ) - 1;
    /* Compute CCDAT1 value to generate a duty cycle at 50% for channel 1 and 1N */
    Channel1Pulse = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);
    /* Compute CCDAT2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
    Channel2Pulse = (uint16_t)(((uint32_t)375 * (TimerPeriod - 1)) / 1000);
    /* Compute CCDAT3 value to generate a duty cycle at 25%  for channel 3 and 3N */
    Channel3Pulse = (uint16_t)(((uint32_t)25 * (TimerPeriod - 1)) / 100);

    /* Time Base configuration */
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = TimerPeriod;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.RepetCnt  = 0;

    TIM_Base_Initialize(TIMx, &TIM_TimeBaseStructure);

    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel1Pulse;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(TIMx, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel3Pulse;
    TIM_Output_Channel3_Initialize(TIMx, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel2Pulse;
    TIM_Output_Channel2_Initialize(TIMx, &TIM_OCInitStructure);

    /* TIM1 break */
    TIM_Break_And_Dead_Time_Struct_Initialize(&TIM_BDTRInitStructure);
    TIM_BDTRInitStructure.OssrState       = TIM_OSSR_STATE_ENABLE;
    TIM_BDTRInitStructure.OssiState       = TIM_OSSI_STATE_ENABLE;
    TIM_BDTRInitStructure.LockLevel       = TIM_LOCK_LEVEL_OFF;
    TIM_BDTRInitStructure.DeadTime        = 1;
    TIM_BDTRInitStructure.Break           = TIM_BREAK_IN_ENABLE;
    TIM_BDTRInitStructure.BreakPolarity   = TIM_BREAK_POLARITY_HIGH;
    TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
    TIM_BDTRInitStructure.IomBreakEn      = false;
    TIM_BDTRInitStructure.LockUpBreakEn   = false;
    TIM_BDTRInitStructure.PvdBreakEn      = false;
    TIM_Break_And_Dead_Time_Set(TIMx, &TIM_BDTRInitStructure);

    /* TIM1 counter enable */
    TIM_On(TIMx);

    /* TIM1 Main Output Enable */
    TIM_PWM_Output_Enable(TIMx);
}

/**
*\*\name    GPIO_CompConfiguration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_CompConfiguration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /*INP*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.Pin        = GPIO_PIN_10;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    /*INM*/
    GPIO_InitStructure.Pin       = GPIO_PIN_5;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    /*OUT*/
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin       = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_COMP1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);

    /*PB11,PB12 as connect to INP,INM by external skip line*/
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.Pin       = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);    
}
/**
*\*\name    GPIO_TimConfiguration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_TimConfiguration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /*TIM1 CHx:PA0,PA8,PA10     CHxN:PA1,PB114,PB15 */
    /* Tim1 Configuration: Channel 1, 2 and 3 as alternate function push-pull */
    GPIO_InitStructure.Pin        = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF10_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    /* Tim1 Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
    GPIO_InitStructure.Pin        = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.Pin        = GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);

    /*iom break if need*/
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_TIM1;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    
    /*TIM8 CHx:PA2,PA3,PA4     CHxN:PA7,PB3,PB1 */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF9_TIM8;
    GPIO_InitStructure.Pin       = GPIO_PIN_2 | GPIO_PIN_4;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF10_TIM8;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_TIM8;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_TIM8;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM8;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures Vector Table base location.
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    /* Configures the priority grouping */
    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Configure and enable COMP interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = COMP_1_2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}
