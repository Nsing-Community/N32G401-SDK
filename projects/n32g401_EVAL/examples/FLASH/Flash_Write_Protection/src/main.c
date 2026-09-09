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

#define FLASH_PAGE_SIZE        ((uint16_t)0x800)
#define FLASH_WRITE_START_ADDR ((uint32_t)0x08008000)
#define FLASH_WRITE_END_ADDR   ((uint32_t)0x08010000)

#define FLASH_PROTECTED_PAGES FLASH_WRP_Pages16to17

#define WRITE_PROTECTION_DISABLE

#if defined WRITE_PROTECTION_ENABLE
static uint32_t IT_flag = 0;
#endif

int main(void)
{    
    uint8_t Test_Result = 0;
    uint32_t WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0;
#ifdef WRITE_PROTECTION_DISABLE
    uint32_t ProgramData = 0xCDEF89AB;
#elif defined WRITE_PROTECTION_ENABLE
    uint32_t ProgramData1 = 0xAABBCCDD;
#endif

    
    NVIC_InitType NVIC_InitStructure;
    /* USART Init */
    log_init();

    NVIC_InitStructure.NVIC_IRQChannel                   = FLASH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
    
    printf("Flash Write Protection Test Start\r\n");
    /* Write Protection */

    /* Unlocks the FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Get pages already write protection */
    WRPR_Value = FLASH_Option_Bytes_Write_Protection_Get();

#ifdef WRITE_PROTECTION_DISABLE

    printf("Write Protection Disable\r\n");

    /* Get pages already write protected */
    ProtectedPages = ~(WRPR_Value | FLASH_PROTECTED_PAGES);

    /* Check if desired pages are already write protected */
    if ((WRPR_Value | (~FLASH_PROTECTED_PAGES)) != 0xFFFFFFFF)
    {
        /* Erase all the option Bytes */
        FLASH_Option_Bytes_Erase();
        
        FLASH_Option_Bytes_User_RDP1_Program(FLASH_OB_RDP1_DISABLE,FLASH_OB_IWDG_SW,FLASH_OB_STOP_NORST,FLASH_OB_STDBY_NORST,FLASH_OB_IWDG_STOP0_NOFRZ,FLASH_OB_IWDG_STOP2_NOFRZ,FLASH_OB_IWDG_STDBY_NOFRZ,FLASH_OB_IWDG_SLEEP_NOFRZ);

        /* Check if there is write protected pages */
        if (ProtectedPages != 0x0)
        {
            /* Restore write protected pages */
            FLASH_Write_Protection_Enable(ProtectedPages);
        }
        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
    }
    else
    {
        /* FLASH Write Protection Test */
        printf("Flash Page Erase/Program\r\n");
        /* Clear All pending flags */
        FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

        /* Erase */
        if (FLASH_EOP == FLASH_One_Page_Erase(FLASH_WRITE_START_ADDR))
        {
            /* Clear All pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);

            /* Program */
            if (FLASH_EOP == FLASH_Word_Program(FLASH_WRITE_START_ADDR, ProgramData))
            {
                /* Check */
                if (ProgramData != (*(__IO uint32_t*)FLASH_WRITE_START_ADDR))
                {
                    /* Test Fail */
                    Test_Result = FAILED;
                }
                else
                {
                    /* Test PASSED */
                    Test_Result = PASSED;
                }
            }
            else
            {
                /* Test Fail */
                Test_Result = FAILED;
            }
        }
        else
        {
            /* Test Fail */
            Test_Result = FAILED;
        }
               
    }
#elif defined WRITE_PROTECTION_ENABLE

    printf("Write Protection Enable\r\n");

    /* Set write protected pages */
    ProtectedPages = (~WRPR_Value) | FLASH_PROTECTED_PAGES;

    /* Check if desired pages are not yet write protected */
    if (((~WRPR_Value) & FLASH_PROTECTED_PAGES) != FLASH_PROTECTED_PAGES)
    {
        /* Erase all the option Bytes */
        FLASH_Option_Bytes_Erase();
        
        FLASH_Option_Bytes_User_RDP1_Program(FLASH_OB_RDP1_DISABLE,FLASH_OB_IWDG_SW,FLASH_OB_STOP_NORST,FLASH_OB_STDBY_NORST,FLASH_OB_IWDG_STOP0_NOFRZ,FLASH_OB_IWDG_STOP2_NOFRZ,FLASH_OB_IWDG_STDBY_NOFRZ,FLASH_OB_IWDG_SLEEP_NOFRZ);

        /* Enable the pages write protection */
        FLASH_Write_Protection_Enable(ProtectedPages);

        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
    }
    else
    {
        /* FLASH Write Protection Test */
        printf("Flash Page Erase/Program\r\n");
        /* Clear All pending flags */
        FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
        
        FLASH_Interrupt_Enable(FLASH_INT_ERROR);

        FLASH_One_Page_Erase(FLASH_WRITE_START_ADDR);
        /* Erase */
        if (IT_flag == 1)
        {
            /* Clear All pending flags */
            FLASH_Flag_Status_Clear(FLASH_STS_CLRFLAG);
            
            IT_flag = 0;
            
            FLASH_Word_Program(FLASH_WRITE_START_ADDR, ProgramData1);
            /* Program */
            if (IT_flag == 1)
            {
                /* Check */
                if (ProgramData1 == (*(__IO uint32_t*)FLASH_WRITE_START_ADDR))
                {
                    /* Test Fail */
                    Test_Result = FAILED;
                }
                else
                {
                    /* Test PASSED */
                    Test_Result = PASSED;
                }
            }
            else
            {
                /* Test Fail */
                Test_Result = FAILED;
            }
        }
        else
        {
            /* Test Fail */
            Test_Result = FAILED;
        }
    }

#endif
    FLASH_Lock();

    if (Test_Result == FAILED)
    {
        printf("Test_Result = FAILED\r\n");
    }
    else
    {
        printf("Test_Result = PASSED\r\n");
    }

    while (1)
    {
    }  
}

#if defined WRITE_PROTECTION_ENABLE
void FLASH_IRQHandler(void)
{
    if(FLASH_Flag_Status_Get(FLASH_FLAG_WRPERR) == SET)
    {
        printf("FLASH_Flag_Status_Get FLASH_FLAG_WRPERR Pass.\n");
        IT_flag = 1;
        FLASH_Flag_Status_Clear(FLASH_FLAG_WRPERR);
        if(FLASH_Flag_Status_Get(FLASH_FLAG_WRPERR) != SET)
        {
            printf("FLASH_Flag_Status_Clear FLASH_FLAG_WRPERR Pass.\n");
        }
        else
        {
            printf("FLASH_Flag_Status_Clear FLASH_FLAG_WRPERR Fail.\n");
        }
    }
    else
    {
        printf("FLASH_Flag_Status_Get FLASH_FLAG_WRPERR Fail.\n");
    }
}
#endif
