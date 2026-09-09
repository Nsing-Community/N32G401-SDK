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
*\*\file      main.c
*\*\author    Nations
*\*\version   v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved. 
*/
#include "main.h"

/** N32G401_StdPeriph_Examples */

/* SPI_DMA */

#define BufferSize 32
#define SPICRC

SPI_InitType SPI_InitStructure;
uint16_t SPI_Master_Buffer_Tx[BufferSize] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F10,
                                             0x1112, 0x1314, 0x1516, 0x1718, 0x191A, 0x1B1C, 0x1D1E, 0x1F20,
                                             0x2122, 0x2324, 0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                             0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C, 0x3D3E, 0x3F40};
uint16_t SPI_Slave_Buffer_Tx[BufferSize] = {0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C, 0x5D5E, 0x5F60,
                                            0x6162, 0x6364, 0x6566, 0x6768, 0x696A, 0x6B6C, 0x6D6E, 0x6F70,
                                            0x7172, 0x7374, 0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F80,
                                            0x8182, 0x8384, 0x8586, 0x8788, 0x898A, 0x8B8C, 0x8D8E, 0x8F90};
uint16_t SPI_Master_Buffer_Rx[BufferSize], SPI_Slave_Buffer_Rx[BufferSize];
uint32_t TxIdx = 0, RxIdx = 0;
__IO uint16_t CRC1Value = 0, CRC2Value = 0;

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);                                     
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/*  Main program. */
int main(void)
{
    /* At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_N32G401.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_N32G401.c file
     */

    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();
    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();
    /* log configuration ------------------------------------------------------*/
    log_init();    
    log_info("This is SPI DMA demo!\r\n");
    /* Initializes the variable */
    SPI_I2S_Reset(SPI_MASTER);
    SPI_I2S_Reset(SPI_SLAVE);
    /* DMA configuration ------------------------------------------------------*/
    DMA_Configuration();

    /* SPI_MASTER configuration ------------------------------------------------------*/
    SPI_Initializes_Structure(&SPI_InitStructure);
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_16BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    /* It is recommended that the SPI master mode of the C version chips should not exceed 18MHz */
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_8;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = 7;
    SPI_Initializes(SPI_MASTER, &SPI_InitStructure);
    SPI_Set_Nss_Level(SPI_MASTER, SPI_NSS_HIGH);

    /* SPI_SLAVE configuration ------------------------------------------------------*/
    SPI_InitStructure.SpiMode = SPI_MODE_SLAVE;
    SPI_Initializes(SPI_SLAVE, &SPI_InitStructure);
    SPI_Set_Nss_Level(SPI_SLAVE, SPI_NSS_LOW);
    
    SPI_I2S_DMA_Transfer_Enable(SPI_SLAVE, SPI_I2S_DMA_TX);
    SPI_I2S_DMA_Transfer_Enable(SPI_SLAVE, SPI_I2S_DMA_RX);
    SPI_I2S_DMA_Transfer_Enable(SPI_MASTER, SPI_I2S_DMA_TX);
    SPI_I2S_DMA_Transfer_Enable(SPI_MASTER, SPI_I2S_DMA_RX);

    /* Enable SPI_MASTER CRC calculation */
    SPI_CRC_Enable(SPI_MASTER);
    /* Enable SPI_SLAVE CRC calculation */
    SPI_CRC_Enable(SPI_SLAVE);

    /* Enable SPI_SLAVE */
    SPI_ON(SPI_SLAVE);
    /* Enable SPI_MASTER */
    SPI_ON(SPI_MASTER);
    /* Wait for data sending complete*/        
    while(DMA_Flag_Status_Get(DMA, DMA_CH1_TXCF) == RESET)
        ;
    while(DMA_Flag_Status_Get(DMA, DMA_CH2_TXCF) == RESET)
        ;
    while(DMA_Flag_Status_Get(DMA, DMA_CH3_TXCF) == RESET)
        ;
    while(DMA_Flag_Status_Get(DMA, DMA_CH4_TXCF) == RESET)
        ;

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(SPI_Slave_Buffer_Rx, SPI_Master_Buffer_Tx, BufferSize);
    TransferStatus2 = Buffercmp(SPI_Master_Buffer_Rx, SPI_Slave_Buffer_Tx, BufferSize);

    if(TransferStatus1 == PASSED && TransferStatus2 == PASSED)
    {
        log_info("Test PASS!\r\n");
    }
    else
    {
        log_info("Test ERR!\r\n");
    }
    while(1)
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

    /* Enable peripheral clocks --------------------------------------------------*/
    /* spi clock enable */
    RCC_APB2_Peripheral_Clock_Enable(SPI_MASTER_PERIPH | SPI_SLAVE_PERIPH);

    /* GPIO Periph clock enable */
    RCC_AHB_Peripheral_Clock_Enable(SPI_MASTER_PERIPH_GPIO | SPI_SLAVE_PERIPH_GPIO | RCC_AHB_PERIPH_DMA);
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
    /* Configure slave pins: NSS, SCK, MISO and MOSI */
    /* Confugure SPI pins as Input Floating */
    GPIO_InitStructure.Pin        = SPI_SLAVE_CLK_PIN | SPI_SLAVE_MOSI_PIN | SPI_SLAVE_MISO_PIN | SPI_SLAVE_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = SPI_SLAVE_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(SPI_SLAVE_GPIO, &GPIO_InitStructure);
    
    /* Configure master pins: NSS, SCK, MISO and MOSI */
    /* Confugure SPI pins as Alternate Function Push Pull */
    GPIO_InitStructure.Pin        = SPI_MASTER_MOSI_PIN | SPI_MASTER_MISO_PIN | SPI_MASTER_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(SPI_MASTER_GPIO, &GPIO_InitStructure);
    /* If CLKPOL is HIGH, set SPI_SCK GPIO to GPIO_PULL_UP, otherwise set to GPIO_PULL_DOWN */
    GPIO_InitStructure.Pin            = SPI_MASTER_CLK_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_DOWN;
    GPIO_InitStructure.GPIO_Alternate = SPI_MASTER_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(SPI_MASTER_GPIO, &GPIO_InitStructure);

}

/**
*\*\name    DMA_Configuration.
*\*\fun     Configures the different DMA channel.
*\*\param   none
*\*\return  none
**/
void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;
  
    DMA_Reset(DMA_CH1);
    DMA_Reset(DMA_CH2);
    DMA_Reset(DMA_CH3);
    DMA_Reset(DMA_CH4);

    /* SPI_MASTER TX DMA config */
	DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.MemAddr = (uint32_t)&SPI_Master_Buffer_Tx[0];
    DMA_InitStructure.MemDataSize = DMA_MEM_DATA_WIDTH_HALFWORD;
    DMA_InitStructure.MemoryInc = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI_MASTER->DAT;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;
    DMA_InitStructure.PeriphInc = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.BufSize = BufferSize;
    DMA_InitStructure.CircularMode = DMA_CIRCULAR_MODE_DISABLE;
    DMA_InitStructure.Mem2Mem = DMA_MEM2MEM_DISABLE;
    DMA_InitStructure.Priority = DMA_CH_PRIORITY_MEDIUM;
    DMA_Initializes(DMA_CH1, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH1, SPI_MASTER_DMA_TX_CH);
    
    /* SPI_MASTER RX DMA config */
    DMA_InitStructure.MemAddr = (uint32_t)&SPI_Master_Buffer_Rx[0];
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.Priority = DMA_CH_PRIORITY_HIGHEST;
    DMA_Initializes(DMA_CH2, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH2, SPI_MASTER_DMA_RX_CH);
    
    /* SPI_Slave TX DMA config */
    DMA_InitStructure.MemAddr = (uint32_t)&SPI_Slave_Buffer_Tx[0];
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.PeriphAddr = (uint32_t)&SPI_SLAVE->DAT;
    DMA_InitStructure.Priority = DMA_CH_PRIORITY_HIGH;
    DMA_Initializes(DMA_CH3, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH3, SPI_SLAVE_DMA_TX_CH);
    
    /* SPI_MASTER RX DMA config */
    DMA_InitStructure.MemAddr = (uint32_t)&SPI_Slave_Buffer_Rx[0];
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.Priority = DMA_CH_PRIORITY_HIGHEST;
    DMA_Initializes(DMA_CH4, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH4, SPI_SLAVE_DMA_RX_CH);
    
    DMA_Channel_Enable(DMA_CH1);
    DMA_Channel_Enable(DMA_CH2);
    DMA_Channel_Enable(DMA_CH3);
    DMA_Channel_Enable(DMA_CH4);
}

/**
*\*\name    Buffercmp.
*\*\fun     Compares two buffers.
*\*\param   pBuffer1
*\*\param   pBuffer2
*\*\param   BufferLength
*\*\return  FAILED or PASSED
**/
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

