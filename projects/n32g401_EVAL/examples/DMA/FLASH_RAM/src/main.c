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

#include <string.h>

#define BUFFER_SIZE (sizeof(SRC_Const_Buffer))

DMA_InitType DMA_InitStructure;
__IO uint32_t CurrDataCounterBegin = 0;
__IO uint32_t CurrDataCounterEnd   = 0x01; /* This variable should not be initialized to 0 */

const uint32_t SRC_Const_Buffer[] = {
    0x72aa0305, 0x839916bf, 0x913782c5, 0x7e3bd953, 0x4174430c, 0x40d59981, 0xbccff537, 0xd22bca1a,
    0xda039c07, 0xff38219a, 0x17f16c33, 0x0056b9cf, 0x4e75ab0a, 0x0751b137, 0xbb051ec9, 0xcac01e06,
    0x5de647ba, 0xf2bd2108, 0x60a32be0, 0xbf80a68f, 0x299b4a55, 0xd37b1965, 0x84a3ca5b, 0x0f08f335,
    0x21c4084b, 0x738688ca, 0xe7971a03, 0xadcd6bfc, 0xd4bb55ae, 0x0ea4ed88, 0x131711a3, 0x4e92f42f,
};

uint32_t DST_Buffer[BUFFER_SIZE];

void RCC_Configuration(void);
void NVIC_Configuration(void);

/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    log_init();
    log_info("DMA Flash to RAM Demo\n");

    /* System Clocks Configuration */
    RCC_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* DMA channel6 configuration */
    DMA_Reset(DMA_CHANNEL_USED);
	DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.PeriphAddr     = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.MemAddr        = (uint32_t)DST_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = BUFFER_SIZE;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_ENABLE;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_WORD;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_WORD;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_ENABLE;
    DMA_Initializes(DMA_CHANNEL_USED, &DMA_InitStructure);

    /* Enable DMA Channel6 Transfer Complete interrupt */
    DMA_Interrupts_Enable(DMA_CHANNEL_USED, DMA_INT_TXC);

    /* Get Current Data Counter value before transfer begins */
    CurrDataCounterBegin = DMA_Current_Data_Transfer_Number_Get(DMA_CHANNEL_USED);

    /* Enable DMA Channel6 transfer */
    DMA_Channel_Enable(DMA_CHANNEL_USED);

    /* Wait the end of transmission */
    while (CurrDataCounterEnd != 0)
    {
    }

    /* Check if the transmitted and received data are equal */
    if (memcmp(SRC_Const_Buffer, DST_Buffer, BUFFER_SIZE) == 0)
    {
        log_info("DMA Flash to RAM passed\n");
    }
    else
    {
        log_error("DMA Flash to RAM failed\n");
    }

    while (1)
    {
    }
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\param   none
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable peripheral clocks */
    /* Enable DMA clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configure the nested vectored interrupt controller.
*\*\param   none
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Enable DMA channel6 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_IRQN;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name    Buffercmp.
*\*\fun     Compares two buffers.
*\*\param   pBuffer buffers to be compared
*\*\param   pBuffer1 buffers to be compared
*\*\param   BufferLength buffer's length
*\*\return  PASSED or FAILED
**/
Status Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer != *pBuffer1)
        {
            return FAILED;
        }

        pBuffer++;
        pBuffer1++;
    }

    return PASSED;
}



