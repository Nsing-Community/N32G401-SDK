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
#include "BSTIM_common.h"
#include "main.h"

uint32_t BSTIMClockFrequency = 0;

void GPIO_Config(void);

/**
*\*\brief  Main program
**/
int main(void)
{
    /* System Clocks Configuration */
    BSTIMClockFrequency = Common_BSTIM_RCC_Initialize(TIM6, RCC_HCLK_DIV4);

    /* Configure the GPIO ports */
    GPIO_Config();

    /* NVIC Configuration */
    Common_TIM_NVIC_Initialize(TIM6_IRQn, ENABLE);

    /* Time base configuration, period = 65535, prescaler = prescaler */
    Common_TIM_Base_Initialize(TIM6, 65535, 0);

    TIM_Base_Reload_Mode_Set(TIM6, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    TIM_Interrupt_Status_Clear(TIM6, TIM_INT_UPDATE);
    TIM_Interrupt_Enable(TIM6, TIM_INT_UPDATE);

    TIM_On(TIM6);
    while (1)
    {
    }
}

/**
*\*\brief  Configure the GPIOx Pins.
**/
void GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    GPIO_InitStructure.Pin        = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;

    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

void TIM6_IRQHandler(void)
{
    if (TIM_Interrupt_Status_Get(TIM6, TIM_INT_UPDATE) != RESET)
    {
        TIM_Interrupt_Status_Clear(TIM6, TIM_INT_UPDATE);

        /* Pin PB.06 toggling */
        GPIO_Pin_Toggle(GPIOB, GPIO_PIN_6);
    }
}
