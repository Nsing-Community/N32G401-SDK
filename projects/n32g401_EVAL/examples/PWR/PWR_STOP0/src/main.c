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
#include "log.h"
#include <stdio.h>
#include <stdint.h>

extern void PLL_TrimValueLoad(void);

/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    /* Initialize USART */
    log_init();
    /* Enable PWR Clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_PWR);
    /* Initialize LEDs on n32g401-EVAL board */
    LEDInit(LED1);
    LEDInit(LED2);
    /* Initialize Key button Interrupt to wakeUp stop */
    KeyInputExtiInit(KEY_INPUT_PORT, KEY_INPUT_PIN);
    /* Set the LED in different status */
    LedOff(LED1);
    LedOn(LED2);
    /* Clear the EXIT Interrupt flag */
    EXTI_Interrupt_Status_Clear(EXTI_LINE0);
    while (1)
    {
        /* Insert a long delay */
        delay(1000);
        /* Set the LED */
        Ledlink(LED1);
        log_info("Entry STOP mode\n");
        
        /* Request to enter STOP0 mode with regulator in low power mode*/
        PWR_STOP0_Mode_Enter(PWR_REGULATOR_LOWPOWER, PWR_STOP0_ENTRY_WFI);

        /* Configures system clock after wake-up from STOP0: enable HSE, PLL and select
           PLL as system clock source (HSE and PLL are disabled in STOP mode) */
        SYSCLKConfig_STOP(RCC_CFG_PLLMULFCT9);
        
        /* When MR is in low power mode, it takes more time to exit STOP0 mode. */
        delay(100);
        log_info("Exit STOP mode\n");
    }
}

/**
*\*\name    Ledlink.
*\*\fun     Toggles the selected Led.
*\*\param   Pin
*\*\return  none
**/
void Ledlink(uint16_t Pin)
{
    GPIOA->POD ^= Pin;
}

/**
*\*\name    LedOn.
*\*\fun     Turns selected Led on.
*\*\param   Pin
*\*\return  none
**/
void LedOn(uint16_t Pin)
{
    GPIOA->PBSC = Pin;
}

/**
*\*\name    LedOff.
*\*\fun     Turns selected Led Off.
*\*\param   Pin
*\*\return  none
**/
void LedOff(uint16_t Pin)
{
    GPIOA->PBC = Pin;
}

/**
*\*\name    LEDInit.
*\*\fun     Configures LED GPIO.
*\*\param   Pin
*\*\return  none
**/
void LEDInit(uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;

    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    delay.
*\*\fun     Delay function.
*\*\param   nCount
*\*\return  none
**/
void delay(vu32 nCount)
{
    vu32 index = 0;
    for (index = (34000 * nCount); index != 0; index--)
    {
    }
}

/**
*\*\name    SYSCLKConfig_STOP.
*\*\fun     Configures system clock after wake-up from STOP: enable HSE, PLL
*\*\        and select PLL as system clock source.
*\*\param   RCC_PLLMULL
*\*\return  none
**/
void SYSCLKConfig_STOP(uint32_t RCC_PLLMULL)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration */
    /* Enable HSE */
    RCC->CTRL |= ((uint32_t)RCC_CTRL_HSEEN);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTRL & RCC_CTRL_HSERDF;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTRL & RCC_CTRL_HSERDF) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    PLL_TrimValueLoad();

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Enable Prefetch Buffer */
        FLASH->AC |= FLASH_AC_PRFTBFEN;

        /* Flash 3 wait state */
        FLASH_Latency_Set(FLASH_LATENCY_3);

        /* HCLK = SYSCLK */
        RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV2; // RCC_CFG_APB2PRES_DIV1

        /* PCLK1 = HCLK */
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV4; // RCC_CFG_APB1PRES_DIV2

        /*  PLL configuration: PLLCLK = HSE * 9 = 72MHz */
        RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_PLLSRC | RCC_CFG_PLLHSEPRES | RCC_CFG_PLLMULFCT));
        RCC->CFG |= (uint32_t)(RCC_CFG_PLLSRC_HSE | RCC_PLLMULL);

        /* Enable PLL */
        RCC->CTRL |= RCC_CTRL_PLLEN;

        /* Wait till PLL is ready */
        while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0)
        {
            /* If this flag is always not set, it means PLL is failed, user can add here some code to deal with this error */
        }

        /* Select PLL as system clock source */
        RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
        RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_PLL;

        /* Wait till PLL is used as system clock source */
        while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x08)
        {
            /* If this bits are always not set, it means system clock select PLL failed, user can add here some code to deal with this error */
        }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */
    }
}

/**
*\*\name    KeyInputExtiInit.
*\*\fun     Configures key GPIO.
*\*\param   GPIOx
*\*\param   Pin
*\*\return  none
**/
void KeyInputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable the GPIO Clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /*Configure the GPIO pin as input floating*/
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);

    /*Configure key EXTI Line to key input  Pin*/
    GPIO_EXTI_Line_Set(EXTI_LINE_SOURCE0, AFIO_EXTI_PA0);

    EXTI_Structure_Initializes(&EXTI_InitStructure);
    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line = EXTI_LINE0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Initializes(&NVIC_InitStructure);
}

