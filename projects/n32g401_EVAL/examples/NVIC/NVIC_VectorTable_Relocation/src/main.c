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

/**
**  NVIC VectorTable Relocation
**/

__IO uint8_t Key_Status = DISABLE;

#define SP_PROCESS_SIZE          0x200 /* Process stack size */
#define SP_PROCESS               0x02  /* Process stack */
#define SP_MAIN                  0x00  /* Main stack */
#define THREAD_MODE_PRIVILEGED   0x00  /* Thread mode has privileged access */
#define THREAD_MODE_UNPRIVILEGED 0x01  /* Thread mode has unprivileged access */


/**
*\*\name   main
*\*\fun    Main program.
*\*\return none
**/
int main(void)
{    
    /* log Init */
    log_init();
    log_info("NVIC VectorTable Relocation \r\n");

    KeyInputExtiInit(KEY_INPUT_PORT, KEY_INPUT_PIN);

    while (1)
    {
        if (Key_Status == ENABLE)
        {
            log_info("Sets the vector table location and Offset \r\n");

            /* Vector Table Relocation in Internal SRAM. */
            NVIC_Vector_Table_Set(NVIC_VECTTAB_RAM, VECT_TAB_SRAM_OFFSET);

            while (1)
                ;
        }
    }
  
}

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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    printf("EXTI0 IRQHandler Start \r\n");
    if (EXTI_Interrupt_Status_Get(KEY_INPUT_EXTI_LINE) != RESET)
    {
#ifdef VECT_TAB_SRAM
        log_info("EXTI0 IRQHandler is executed(SRAM) \r\n");
#else
        /* Key_Status */
        Key_Status = ENABLE;
        log_info("EXTI0 IRQHandler is executed(FLASH) \r\n");
#endif
        /* Clears the SEL Button EXTI line pending bits */
        EXTI_Flag_Status_Clear(KEY_INPUT_EXTI_LINE);
    }
    log_info("EXTI0 IRQHandler End \r\n");
}


