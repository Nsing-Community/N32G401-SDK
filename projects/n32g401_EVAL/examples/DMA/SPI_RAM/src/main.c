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
#include "n32g401.h"
#include "log.h"

#include <string.h>

#define BUFFER_SIZE    32
#define CRC_POLYNOMIAL 7

SPI_InitType SPI_InitStructure;
DMA_InitType DMA_InitStructure;
uint8_t SPI_Master_Rx_Buffer[BUFFER_SIZE], SPI_Slave_Rx_Buffer[BUFFER_SIZE];
volatile uint8_t SPI_MASTERCRCValue = 0, SPI_SLAVECRCValue = 0;
volatile Status TransferStatus1 = FAILED, TransferStatus2 = FAILED;

uint8_t SPI_Master_Tx_Buffer[BUFFER_SIZE] = {0xf1, 0x2e, 0xf1, 0xaf, 0x08, 0xa2, 0x41, 0xb4, 0xc8, 0x19, 0x38,
                                             0xb7, 0xd9, 0xdb, 0x9a, 0x64, 0x1f, 0xd6, 0x99, 0x9c, 0x0f, 0xae,
                                             0x84, 0xab, 0xda, 0x12, 0x95, 0x6c, 0xdb, 0xec, 0x06, 0x08};

uint8_t SPI_Slave_Tx_Buffer[BUFFER_SIZE] = {0x2c, 0x99, 0xd7, 0x26, 0xb0, 0xe5, 0xb2, 0xfc, 0xee, 0x88, 0x3f,
                                            0xde, 0xa4, 0x37, 0x87, 0xc9, 0xb2, 0x9c, 0xce, 0xc8, 0x2c, 0x22,
                                            0x6b, 0xfe, 0xba, 0x49, 0x94, 0x0a, 0x47, 0x5a, 0xb7, 0x89};

/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    log_init();

    log_info("----------------------\nDMA SPI to RAM Demo.\n");

    /* System Clocks Configuration */
    RCC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* SPI_MASTER configuration */
	SPI_Initializes_Structure(&SPI_InitStructure);
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_8;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = CRC_POLYNOMIAL;
    SPI_Initializes(SPI_MASTER, &SPI_InitStructure);
    SPI_Set_Nss_Level(SPI_MASTER,SPI_NSS_HIGH);

    /* SPI_SLAVE configuration */
    SPI_InitStructure.SpiMode = SPI_MODE_SLAVE;
    SPI_Initializes(SPI_SLAVE, &SPI_InitStructure);
    SPI_Set_Nss_Level(SPI_SLAVE,SPI_NSS_LOW);

    /* SPI_MASTER_RX_DMA_CHANNEL configuration */
    DMA_Reset(SPI_MASTER_RX_DMA_CHANNEL);
	DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&SPI1->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)SPI_Master_Rx_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = BUFFER_SIZE;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
    DMA_Initializes(SPI_MASTER_RX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_Channel_Request_Remap(SPI_MASTER_RX_DMA_CHANNEL,DMA_REMAP_SPI1_RX);

    /* SPI_MASTER_TX_DMA_CHANNEL configuration */
    DMA_Reset(SPI_MASTER_TX_DMA_CHANNEL);
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI1->DAT;
    DMA_InitStructure.MemAddr    = (uint32_t)SPI_Master_Tx_Buffer;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.Priority   = DMA_CH_PRIORITY_LOW;
    DMA_Initializes(SPI_MASTER_TX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_Channel_Request_Remap(SPI_MASTER_TX_DMA_CHANNEL,DMA_REMAP_SPI1_TX);

    /* SPI_SLAVE_RX_DMA_CHANNEL configuration */
    DMA_Reset(SPI_SLAVE_RX_DMA_CHANNEL);
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI2->DAT;
    DMA_InitStructure.MemAddr    = (uint32_t)SPI_Slave_Rx_Buffer;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.Priority   = DMA_CH_PRIORITY_HIGHEST;
    DMA_Initializes(SPI_SLAVE_RX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_Channel_Request_Remap(SPI_SLAVE_RX_DMA_CHANNEL,DMA_REMAP_SPI2_RX);

    /* SPI_SLAVE_TX_DMA_CHANNEL configuration */
    DMA_Reset(SPI_SLAVE_TX_DMA_CHANNEL);
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI2->DAT;
    DMA_InitStructure.MemAddr    = (uint32_t)SPI_Slave_Tx_Buffer;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.Priority   = DMA_CH_PRIORITY_MEDIUM;
    DMA_Initializes(SPI_SLAVE_TX_DMA_CHANNEL, &DMA_InitStructure);
    DMA_Channel_Request_Remap(SPI_SLAVE_TX_DMA_CHANNEL,DMA_REMAP_SPI2_TX);

    /* Enable SPI_MASTER DMA Tx request */
    SPI_I2S_DMA_Transfer_Enable(SPI_MASTER, SPI_I2S_DMA_TX);
    /* Enable SPI_MASTER DMA Rx request */
    SPI_I2S_DMA_Transfer_Enable(SPI_MASTER, SPI_I2S_DMA_RX);
    /* Enable SPI_SLAVE DMA Tx request */
    SPI_I2S_DMA_Transfer_Enable(SPI_SLAVE, SPI_I2S_DMA_TX);
    /* Enable SPI_SLAVE DMA Rx request */
    SPI_I2S_DMA_Transfer_Enable(SPI_SLAVE, SPI_I2S_DMA_RX);

    /* Enable SPI_MASTER CRC calculation */
    SPI_CRC_Enable(SPI_MASTER);
    /* Enable SPI_SLAVE CRC calculation */
    SPI_CRC_Enable(SPI_SLAVE);

    /* Enable SPI_SLAVE */
    SPI_ON(SPI_SLAVE);
    /* Enable SPI_MASTER */
    SPI_ON(SPI_MASTER);

    /* Enable DMA channels */
    DMA_Channel_Enable(SPI_MASTER_RX_DMA_CHANNEL);
    DMA_Channel_Enable(SPI_SLAVE_RX_DMA_CHANNEL);
    DMA_Channel_Enable(SPI_SLAVE_TX_DMA_CHANNEL);
    DMA_Channel_Enable(SPI_MASTER_TX_DMA_CHANNEL);

    /* Transfer complete */
    while (!DMA_Flag_Status_Get(SPI_MASTER_DMA, SPI_MASTER_RX_DMA_FLAG))
        ;
    while (!DMA_Flag_Status_Get(SPI_SLAVE_DMA, SPI_SLAVE_RX_DMA_FLAG))
        ;
    while (!DMA_Flag_Status_Get(SPI_SLAVE_DMA, SPI_SLAVE_TX_DMA_FLAG))
        ;
    while (!DMA_Flag_Status_Get(SPI_MASTER_DMA, SPI_MASTER_TX_DMA_FLAG))
        ;

    /* Wait for SPI_MASTER data reception: CRC transmitted by SPI_SLAVE */
    while (SPI_I2S_Flag_Status_Get(SPI_MASTER, SPI_I2S_FLAG_RNE) == RESET)
        ;
    /* Wait for SPI_SLAVE data reception: CRC transmitted by SPI_MASTER */
    while (SPI_I2S_Flag_Status_Get(SPI_SLAVE, SPI_I2S_FLAG_RNE) == RESET)
        ;

    /* Check the correctness of written dada */
    TransferStatus1 = Buffercmp(SPI_Slave_Rx_Buffer, SPI_Master_Tx_Buffer, BUFFER_SIZE);
    TransferStatus2 = Buffercmp(SPI_Master_Rx_Buffer, SPI_Slave_Tx_Buffer, BUFFER_SIZE);
    /* TransferStatus1, TransferStatus2 = PASSED, if the data transmitted and
       received are correct */
    /* TransferStatus1, TransferStatus2 = FAILED, if the data transmitted and
       received are different */

    /* Test on the SPI_MASTER CRCR ERROR flag */
    if ((SPI_I2S_Flag_Status_Get(SPI_MASTER, SPI_FLAG_CRCERR)) != RESET)
    {
        TransferStatus1 = FAILED;
    }
    /* Test on the SPI_SLAVE CRCR ERROR flag */
    if ((SPI_I2S_Flag_Status_Get(SPI_SLAVE, SPI_FLAG_CRCERR)) != RESET)
    {
        TransferStatus2 = FAILED;
    }

    /* Read SPI_MASTER received CRC value */
    SPI_MASTERCRCValue = SPI_I2S_Data_Get(SPI_MASTER);
    /* Read SPI_SLAVE received CRC value */
    SPI_SLAVECRCValue = SPI_I2S_Data_Get(SPI_SLAVE);

    if (TransferStatus1 != FAILED)
    {
        log_info("SPI master passed.\n");
    }
    else
    {
        log_error("SPI master failed.\n");
    }

    if (TransferStatus2 != FAILED)
    {
        log_info("SPI slave passed.\n");
    }
    else
    {
        log_error("SPI slave failed.\n");
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
    /* PCLK2 = HCLK/2 */
    RCC_Pclk2_Config(RCC_HCLK_DIV2);

    /* Enable peripheral clocks */
    /* Enable DMA clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);

    /* Enable SPI_MASTER clock and GPIO clock for SPI_MASTER and SPI_SLAVE */
    RCC_APB2_Peripheral_Clock_Enable(SPI_MASTER_CLK);
    RCC_AHB_Peripheral_Clock_Enable(SPI_MASTER_GPIO_CLK | SPI_SLAVE_GPIO_CLK);
    /* Enable SPI_SLAVE Periph clock */
    RCC_APB2_Peripheral_Clock_Enable(SPI_SLAVE_CLK);
}


/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\param   none
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure SPI_MASTER pins: SCK and MOSI */
    GPIO_InitStructure.Pin            = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_SPI1;
    GPIO_Peripheral_Initialize(SPI_MASTER_GPIO, &GPIO_InitStructure);
    /* Configure SPI_MASTER pins:  MISO */
    GPIO_InitStructure.Pin       = SPI_MASTER_PIN_MISO;
    GPIO_Peripheral_Initialize(SPI_MASTER_GPIO, &GPIO_InitStructure);
    /* Configure SPI_MASTER pins:  NSS */
    GPIO_InitStructure.Pin       = GPIO_PIN_4;
    GPIO_Peripheral_Initialize(SPI_MASTER_GPIO, &GPIO_InitStructure);

    /* Configure SPI_SLAVE pins: SCK and MOSI */
    GPIO_InitStructure.Pin       = SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_SPI2;
    GPIO_Peripheral_Initialize(SPI_SLAVE_GPIO, &GPIO_InitStructure);
    /* Configure SPI_SLAVE pins: MISO  */
    GPIO_InitStructure.Pin       = SPI_SLAVE_PIN_MISO;
    GPIO_Peripheral_Initialize(SPI_SLAVE_GPIO, &GPIO_InitStructure);
    /* Configure SPI_SLAVE pins: NSS  */
    GPIO_InitStructure.Pin       = SPI_SLAVE_PIN_NSS;
    GPIO_Peripheral_Initialize(SPI_SLAVE_GPIO, &GPIO_InitStructure);
}

/**
*\*\name    Buffercmp.
*\*\fun     Compares two buffers.
*\*\param   pBuffer buffers to be compared
*\*\param   pBuffer1 buffers to be compared
*\*\param   BufferLength buffer's length
*\*\return  PASSED or FAILED
**/
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
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













