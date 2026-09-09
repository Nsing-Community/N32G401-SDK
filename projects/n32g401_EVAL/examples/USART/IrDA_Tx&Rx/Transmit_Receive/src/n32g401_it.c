/*****************************************************************************
 * Copyright (c) 2022, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file n32g401.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include <stdio.h>
#include "main.h"
#include "bsp_Tim_38K.h"
#include "n32g401_it.h"
#include "main.h"


extern uint8_t RxBuffer1[];
extern uint8_t NbrOfDataToRead1;
extern uint8_t RxCounter;

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


/**
*\*\name    SysTick_Handler.
*\*\fun     This function handles SysTick Handler.
*\*\param   none
*\*\return  none 
**/
void SysTick_Handler(void)
{
}

/** N32G401 Peripherals Interrupt Handlers, interrupt handler's name please refer to the startup file (startup_n32g401.s) **/


/**
*\*\name    USARTz_IRQHandler.
*\*\fun     This function handles USARTz global interrupt request.
*\*\param   none
*\*\return  none 
**/
 void USARTz_IRQHandler(void)
 {
    if (USART_Interrupt_Status_Get(USARTz, USART_INT_RXDNE) != RESET)
    {
         /* Read one byte from the receive data register */
         RxBuffer1[RxCounter++] = USART_Data_Receive(USARTz);

         if (RxCounter == NbrOfDataToRead1)
         {
             /* Disable the USARTz Receive interrupt */
             USART_Interrput_Disable(USARTz, USART_INT_RXDNE);
         }
    }
		
    /* Determine if an error flag still exists and clear the error flag */
    if ((USART_Flag_Status_Get(USARTz, USART_FLAG_OREF) != RESET)  || \
        (USART_Flag_Status_Get(USARTz, USART_FLAG_NEF) != RESET)  || \
        (USART_Flag_Status_Get(USARTz, USART_FLAG_PEF) != RESET)  || \
        (USART_Flag_Status_Get(USARTz, USART_FLAG_FEF) != RESET))
    {
        /*Read the sts register first,and the read the DAT register to clear the all error flag*/
        (void)USARTz->STS;
        (void)USARTz->DAT;
        /* Under normal circumstances, all error flags will be cleared when the upper data is read and will not be executed here; 
           users can add their own processing according to the actual scenario. */
    }
 }

