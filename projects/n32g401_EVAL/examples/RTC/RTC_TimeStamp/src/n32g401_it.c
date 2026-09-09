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
*\*\file n32g401_it.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/


#include "n32g401.h"
#include "n32g401_it.h"
#include "main.h"
#include "log.h"


/** Cortex-M4 Processor Exceptions Handlers **/

/**
*\*\name    NMI_Handler.
*\*\fun     This function handles NMI exception.
*\*\param   none
*\*\return  none 
**/
void NMI_Handler(void)
{
}

/**
*\*\name    HardFault_Handler.
*\*\fun     This function handles Hard Fault exception.
*\*\param   none
*\*\return  none 
**/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
*\*\name    MemManage_Handler.
*\*\fun     This function handles Memory Manage exception.
*\*\param   none
*\*\return  none 
**/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
*\*\name    BusFault_Handler.
*\*\fun     This function handles Bus Fault exception.
*\*\param   none
*\*\return  none 
**/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
*\*\name    UsageFault_Handler.
*\*\fun     This function handles Usage Fault exception.
*\*\param   none
*\*\return  none 
**/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
*\*\name    SVC_Handler.
*\*\fun     This function handles SVCall exception.
*\*\param   none
*\*\return  none 
**/
void SVC_Handler(void)
{
}

/**
*\*\name    DebugMon_Handler.
*\*\fun     This function handles Debug Monitor exception.
*\*\param   none
*\*\return  none 
**/
void DebugMon_Handler(void)
{
}

uint8_t Delay_100Ms_Cnt = 0;
extern volatile uint32_t RTC_Delay_Flag;

#ifdef RTC_DELAY_USE_TIM6
/**
*\*\name    TIM6_IRQHandler.
*\*\fun     This function handles TIM6 global interrupt request.
*\*\param   none
*\*\return  none 
**/
void TIM6_IRQHandler(void)
{
    if (TIM_Interrupt_Status_Get(TIM6, TIM_INT_UPDATE) != RESET)
    {
        Delay_100Ms_Cnt++;

        if(Delay_100Ms_Cnt == 11)
        {
            RTC_Delay_Flag = 1;
            Delay_100Ms_Cnt = 0;
            /* Disable the TIM6 Counter */
            TIM6->CTRL1 &= (uint32_t)(~((uint32_t)TIM_CTRL1_CNTEN));
        }
        TIM_Interrupt_Status_Clear(TIM6, TIM_INT_UPDATE);
    }
}
#else
/**
*\*\name    SysTick_Handler.
*\*\fun     This function handles SysTick Handler.
*\*\param   none
*\*\return  none 
**/
void SysTick_Handler(void)
{
    Delay_100Ms_Cnt++;
    if(Delay_100Ms_Cnt == 11)
    {
        RTC_Delay_Flag = 1;
        Delay_100Ms_Cnt = 0;
        /* Disable the SysTick Counter */
        SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
    }
}
#endif

/** N32G401 Peripherals Interrupt Handlers, interrupt handler's name please refer to the startup file (startup_n32g401.s) **/

/**
*\*\name    RTC_TAMPER_STAMP_IRQHandler.
*\*\fun     Display the current TimeStamp (time and date) and tamper on the Hyperterminal.
*\*\param   none
*\*\return  none 
**/

void RTC_TAMPER_STAMP_IRQHandler(void)
{
    EXTI_Interrupt_Status_Clear(EXTI_LINE18);
    if (RTC_Interrupt_Status_Get(RTC_INT_TST) != RESET)
    {
        /* Every interval delay (1s) displays calendar and clock stamp register contents */
        RTC_Date_Show();
        RTC_Time_Show();
        RTC_TimeStamp_Show();
        RTC_Interrupt_Status_Clear(RTC_INT_TST);
    }
}
