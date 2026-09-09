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
* damage. Such applications are deemed, â€œInsecure Usageâ€?.
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
#include "n32g401.h"
#include "log.h"

GPIO_InitType GPIO_InitStructure;
RCC_ClocksType RCC_ClockFreq;
ErrorStatus HSEStartUpStatus;
ErrorStatus HSIStartUpStatus;

void NVIC_Configuration(void);

#define SYSCLK_SOURCE_HSI 1
#define SYSCLK_SOURCE_HSE 2
#define SYSCLK_SOURCE_HSI_PLL 3
#define SYSCLK_SOURCE_HSE_PLL 4

#ifndef SYSCLK_SOURCE_SELECT
#define SYSCLK_SOURCE_SELECT SYSCLK_SOURCE_HSE_PLL /*select sysclk source */
#endif


#define SYSCLK_USE_HSIDIV2_PLL 0
#define SYSCLK_USE_HSEDIV2_PLL 1
#define HSE_Value   (8000000)

ErrorStatus SetSysClockToHSI(void);
ErrorStatus SetSysClockToHSE(void);
ErrorStatus SetSysClockToPLL(uint32_t freq, uint8_t src);
/**
*\*\name    PrintfClockInfo.
*\*\fun     Printf clock information.
*\*\param   none
*\*\return  none 
**/
void PrintfClockInfo(const char* msg)
{
    uint32_t TimeDelay = 0xFFFF;
    /* reinit after sysclk changed */
    log_init(); 
    
    log_info("--------------------------------\n");
    log_info("%s:\n", msg);
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClockFreq);
    log_info("SYSCLK: %d\n", RCC_ClockFreq.SysclkFreq);
    log_info("HCLK: %d\n", RCC_ClockFreq.HclkFreq);
    log_info("PCLK1: %d\n", RCC_ClockFreq.Pclk1Freq);
    log_info("PCLK2: %d\n", RCC_ClockFreq.Pclk2Freq);
    
    /* Wait for printing to complete */
    while(--TimeDelay) 
    {
    }
}

/**
*\*\name    main.
*\*\fun     main function.
*\*\param   none
*\*\return  none 
**/
int main(void)
{
    
    log_init();
    log_info("-----------------\nRCC_ClockConfig Demo.\n");

    PrintfClockInfo("After reset"); 
    
#if SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSI
    
    if(SetSysClockToHSI() == ERROR)
    {
        log_info("Clock configuration failure!\n");
    }
    PrintfClockInfo("HSI, 8MHz");
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSE  
    
    if(SetSysClockToHSE() == ERROR)
    {
        log_info("Clock configuration failure!\n");
    }
    PrintfClockInfo("HSE, 8MHz");
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSI_PLL 
    
    if(SetSysClockToPLL(48000000, SYSCLK_USE_HSIDIV2_PLL) == ERROR)
    {
        log_info("Clock configuration failure!\n");
    }
    PrintfClockInfo("HSIDIV2->PLL, 48M");
    
    
#elif SYSCLK_SOURCE_SELECT == SYSCLK_SOURCE_HSE_PLL 
   
    if(SetSysClockToPLL(32000000, SYSCLK_USE_HSEDIV2_PLL) == ERROR)
    {
        log_info("Clock configuration failure!\n");
    }
    PrintfClockInfo("HSEDIV2->PLL, 32M");
    
#endif

    /* Enable Clock Security System(CSS): this will generate an NMI exception
       when HSE clock fails */
    RCC_Clock_Security_System_Enable();

    /* NVIC configuration
     * ------------------------------------------------------*/
    NVIC_Configuration();

    /* Output HSE clock on MCO pin
     * ---------------------------------------------*/
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin             = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode       = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate  = GPIO_AF9_MCO;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    RCC_MCO_Source_Config(RCC_MCO_SYSCLK);

   
    while (1);
}

/**
*\*\name    SetSysClockToHSI.
*\*\fun     Selects HSI as System clock source and configure HCLK, PCLK2
*\*\         and PCLK1 prescalers.
*\*\param   none
*\*\return  none 
**/
ErrorStatus SetSysClockToHSI(void)
{
    uint32_t timeout_value = 0xFFFFFF; 
    
    RCC_Reset();

    RCC_HSI_Enable();

    /* Wait till HSI is ready */
    HSIStartUpStatus = RCC_HSI_Stable_Wait();

    if (HSIStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_Prefetch_Buffer_Enable();

        /* Flash 0 wait state */
        FLASH_Latency_Set(FLASH_LATENCY_0);

        /* HCLK = SYSCLK */
        RCC_Hclk_Config(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_Pclk2_Config(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_Pclk1_Config(RCC_HCLK_DIV1);

        /* Select HSI as system clock source */
        RCC_Sysclk_Config(RCC_SYSCLK_SRC_HSI);
           
        /* Wait till HSI is used as system clock source */
        while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_HSI)
        {
            if ((timeout_value--) == 0)
            {
                return ERROR;
            }
        }
    }
    else
    {
        /* If HSI fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */
        return ERROR;
    }
    
    return SUCCESS;
}

/**
*\*\name    SetSysClockToHSE.
*\*\fun     Selects HSE as System clock source and configure HCLK, PCLK2
*\*\          and PCLK1 prescalers.
*\*\param   none
*\*\return  none 
**/
ErrorStatus SetSysClockToHSE(void)
{
    uint32_t timeout_value = 0xFFFFFF; 
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset();

    /* Enable HSE */
    RCC_HSE_Config(RCC_HSE_ENABLE);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_HSE_Stable_Wait();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_Prefetch_Buffer_Enable();

        if (HSE_Value <= 18000000)
        {
            /* Flash 0 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_0);
        }
        else
        {
            /* Flash 1 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_1);
        }

        /* HCLK = SYSCLK */
        RCC_Hclk_Config(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_Pclk2_Config(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_Pclk1_Config(RCC_HCLK_DIV1);

        /* Select HSE as system clock source */
        RCC_Sysclk_Config(RCC_SYSCLK_SRC_HSE);
       
        /* Wait till HSE is used as system clock source */
        while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_HSE)
        {
            if ((timeout_value--) == 0)
            {
                return ERROR;
            }
        }
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */
        return ERROR;
    }
    
    return SUCCESS;
}

/**
*\*\name    SetSysClockToPLL.
*\*\fun     Selects PLL clock as System clock source and configure HCLK, PCLK2
*\*\         and PCLK1 prescalers.
*\*\param   none
*\*\note    PLL frequency must be greater than or equal to 32MHz.
*\*\return  none 
**/
ErrorStatus SetSysClockToPLL(uint32_t freq, uint8_t src)
{
    uint32_t pllmul;
    uint32_t latency;
    uint32_t pclk1div, pclk2div;
    uint32_t timeout_value = 0xFFFFFF; 

    if (HSE_VALUE != 8000000)
    {
        /* HSE_VALUE == 8000000 is needed in this project! */
        return ERROR;
    }

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset();

    if (src == SYSCLK_USE_HSIDIV2_PLL) 
    {
        /* Enable HSI */
        RCC_HSI_Enable();

        /* Wait till HSI is ready */
        HSIStartUpStatus = RCC_HSI_Stable_Wait();

        if (HSIStartUpStatus != SUCCESS)
        {
            /* If HSI fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */
            return ERROR;
        }

    }
    else if (src == SYSCLK_USE_HSEDIV2_PLL) 
    {
        /* Enable HSE */
        RCC_HSE_Config(RCC_HSE_ENABLE);

        /* Wait till HSE is ready */
        HSEStartUpStatus = RCC_HSE_Stable_Wait();

        if (HSEStartUpStatus != SUCCESS)
        {
            /* If HSE fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */
            return ERROR;
        }

    }

    switch (freq)
    {
        case 32000000:
            latency  = FLASH_LATENCY_0;
            if(src == SYSCLK_USE_HSIDIV2_PLL)
            {
                pllmul = RCC_PLL_MUL_8;
            }
            else if(src == SYSCLK_USE_HSEDIV2_PLL)
            {
                pllmul = RCC_PLL_MUL_8;
            }
            pclk1div = RCC_HCLK_DIV2;
            pclk2div = RCC_HCLK_DIV1;
            break;
        case 48000000:
            latency  = FLASH_LATENCY_1;
            if(src == SYSCLK_USE_HSIDIV2_PLL)
            {
              pllmul = RCC_PLL_MUL_12;
            }
            else if(src == SYSCLK_USE_HSEDIV2_PLL)
            {
               pllmul = RCC_PLL_MUL_12;
            }
            pclk1div = RCC_HCLK_DIV2;
            pclk2div = RCC_HCLK_DIV1;
            break;
        default:
            return ERROR;
    }

    FLASH_Latency_Set(latency);

    /* HCLK = SYSCLK */
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);

    /* PCLK2 = HCLK */
    RCC_Pclk2_Config(pclk2div);

    /* PCLK1 = HCLK */
    RCC_Pclk1_Config(pclk1div);
    
    if(src == SYSCLK_USE_HSEDIV2_PLL)
    {
        RCC_PLL_Config(RCC_PLL_SRC_HSE_DIV2,pllmul);
    }
    else
    {
        RCC_PLL_Config(RCC_PLL_SRC_HSI_DIV2,pllmul);
    }

    /* Enable PLL */
    RCC_PLL_Enable();
    
     /* Wait till PLL is ready */
    while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0)
    {
        if ((timeout_value--) == 0)
        {
            return ERROR;
        }
    }
    /* Select PLL as system clock source */
    RCC_Sysclk_Config(RCC_SYSCLK_SRC_PLLCLK);

    /* Wait till PLL is used as system clock source */
    timeout_value = 0xFFFF;
    while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_PLL)
    {
        if ((timeout_value--) == 0)
        {
            return ERROR;
        }
    }
    
    return SUCCESS;
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     NVIC Configuration.
*\*\param   none
*\*\return  none 
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}


