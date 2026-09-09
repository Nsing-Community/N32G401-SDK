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
*\*\file log.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "log.h"
#include "n32g401.h"
#include "n32g401_gpio.h"
#include "n32g401_usart.h"
#include "n32g401_rcc.h"

#define LOG_USARTx        USART1
#define LOG_PERIPH        RCC_APB2_PERIPH_USART1
#define LOG_GPIO          GPIOA
#define LOG_PERIPH_GPIO   RCC_AHB_PERIPH_GPIOA
#define LOG_TX_PIN        GPIO_PIN_9
#define LOG_RX_PIN        GPIO_PIN_10

/**
 *\*\name   log_init.
 *\*\fun    USART initialize.
 *\*\param  none.
 *\*\return none.
**/
void log_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    RCC_AHB_Peripheral_Clock_Enable(LOG_PERIPH_GPIO);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    RCC_APB2_Peripheral_Clock_Enable(LOG_PERIPH);

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    GPIO_InitStructure.Pin            = LOG_TX_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART1;
    GPIO_Peripheral_Initialize(LOG_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin            = LOG_RX_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART1;
    GPIO_Peripheral_Initialize(LOG_GPIO, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_TX | USART_MODE_RX;
    USART_Initializes(LOG_USARTx, &USART_InitStructure);
    
    /* Enable USART */
    USART_Enable(LOG_USARTx);
}


/**
 *\*\name   Usart_SendByte.
 *\*\fun    USART sends one byte of data.
 *\*\param  USARTx :
 *\*\          - USART1
 *\*\          - USART2
 *\*\param  byte :
 *\*\          - A byte of data to be sent
 *\*\return none.
**/
void Usart_SendByte(USART_Module* USARTx, uint8_t byte)
{
    USART_Data_Send(USARTx, byte);

    while (USART_Flag_Status_Get(USARTx, USART_FLAG_TXC) == RESET)
    {
    }
}


/**
 *\*\name   Usart_SendString.
 *\*\fun    USART sends a string.
 *\*\param  USARTx :
 *\*\          - USART1
 *\*\          - USART2
 *\*\param  str :
 *\*\          - The string to send
 *\*\return none.
**/
void Usart_SendString(USART_Module* USARTx, char *str)
{
    unsigned int k=0;

    do 
    {
       Usart_SendByte(USARTx, *(str + k) );
       k++;
    } while(*(str + k)!='\0');
  
    while(USART_Flag_Status_Get(USARTx, USART_FLAG_TXC) == RESET)
    {}
}


/**
 *\*\name   fputc.
 *\*\fun    Rewrite the C library printf function.
 *\*\param  
 *\*\return none.
**/
int fputc(int ch, FILE* f)
{
    /* Send a byte of data to the serial port */
    USART_Data_Send(LOG_USARTx, (uint8_t) ch);
        
    /* Waiting for sending */
    while (USART_Flag_Status_Get(LOG_USARTx, USART_FLAG_TXDE) == RESET);

    return (ch);
}


/**
 *\*\name   fgetc.
 *\*\fun    Rewrite the C library scanf function.
 *\*\param  
 *\*\return none.
**/
int fgetc(FILE* f)
{
    /* Waiting for usart port input data */
    while (USART_Flag_Status_Get(LOG_USARTx, USART_FLAG_RXDNE) == RESET);

    return (int)USART_Data_Receive(LOG_USARTx);
}


