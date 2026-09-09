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

#include "main.h"
#include "log.h"
#include <stdio.h>

uint8_t Key_Status = DISABLE;

/**
*\*\name   main
*\*\fun    Main program.
*\*\return none
**/
int main(void)
{    
    /* log Init */
    log_init();
    log_info("NVIC IRQ Mask \r\n");

    /* TIM configuration */
    TIM_Config();
    /* KEY configuration */
    KeyInputExtiInit(KEY_INPUT_PORT, KEY_INPUT_PIN);

    while (1)
    {
        while (Key_Status == DISABLE)
        {
        }
        log_info("Disable irq \r\n");
        /* This instruction raises the execution priority to 0. This prevents all
           exceptions with configurable priority from activating, other than through
           the HardFault fault escalation mechanism. */
        __disable_irq();

        while(GPIO_Input_Pin_Data_Get(GPIOA,GPIO_PIN_0) == RESET)
        {
        
        }
        while(GPIO_Input_Pin_Data_Get(GPIOA,GPIO_PIN_0) == SET)
        {
        
        }

        log_info("enable irq \r\n");
        /* This instruction will allow all exceptions with configurable priority to
           be activated. */

        __enable_irq();
    }
  
}

/**
*\*\name   KeyInputExtiInit
*\*\fun    External key interrupt configuration.
*\*\return none
**/
void KeyInputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable the GPIO Clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);


    /*Configure the GPIO pin as input floating*/
	GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);

    /*Configure key EXTI Line to key input Pin*/
    GPIO_EXTI_Line_Set(KEY_INPUT_PORT_SOURCE, KEY_INPUT_PIN_SOURCE);

    /*Configure key EXTI line*/
	EXTI_Structure_Initializes(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = KEY_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = KEY_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name   KeyInputExtiInit
*\*\fun    TIM interrupt configuration.
*\*\return none
**/
void TIM_Config(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable TIM2 clocks */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM2);

    /* TIM2 configuration */
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = 0x4AF;
    TIM_TimeBaseStructure.Prescaler = ((SystemClockFrequency / 1200) - 1);
    TIM_TimeBaseStructure.ClkDiv    = 0x0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM2, &TIM_TimeBaseStructure);
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.OcMode = TIM_OCMODE_TIMING;
    TIM_OCInitStructure.Pulse  = 0x0;
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);

    /* Immediate load of TIM2 Precaler values */
    TIM_Base_Prescaler_Set(TIM2, ((SystemClockFrequency / 1200) - 1));
    TIM_Base_Reload_Mode_Set(TIM2, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* Clear TIM2 update pending flags */
    TIM_Flag_Clear(TIM2, TIM_FLAG_UPDATE);

    /* Configure two bits for preemption priority */
    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);

    /* Enable the TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    /* Enable TIM2 Update interrupts */
    TIM_Interrupt_Enable(TIM2, TIM_INT_UPDATE);

    /* TIM2 enable counters */
    TIM_On(TIM2);
}

/**
*\*\name    EXTI0_IRQHandler.
*\*\fun     This function handles EXTI0 exception.
*\*\param   none
*\*\return  none 
**/
void EXTI0_IRQHandler(void)
{
    printf("EXTI0 IRQHandler Start \r\n");
    if (EXTI_Interrupt_Status_Get(KEY_INPUT_EXTI_LINE) != RESET)
    {
        if (Key_Status == ENABLE)
        {
            Key_Status = DISABLE;
        }
        else
        {
            Key_Status = ENABLE;
        }
        /* Clears the SEL Button EXTI line pending bits. */
        EXTI_Flag_Status_Clear(KEY_INPUT_EXTI_LINE);
    }
    log_info("EXTI0 IRQHandler End \r\n");
}

/**
*\*\name    DebugMon_Handler.
*\*\fun     This function handles TIM2 exception.
*\*\param   none
*\*\return  none 
**/
void TIM2_IRQHandler(void)
{
    /* Clear TIM2 update interrupt */
    TIM_Flag_Clear(TIM2, TIM_INT_UPDATE);

    /* TIM2 IRQHandler */
    log_info("TIM2 IRQHandler \r\n");
}
