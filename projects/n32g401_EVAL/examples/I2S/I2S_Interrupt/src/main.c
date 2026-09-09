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

/* N32G401_StdPeriph_Examples */



#define BufferSize 32


I2S_InitType I2S_InitStructure;

uint16_t I2S_Master_Buffer_Tx[BufferSize] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F10,
                                             0x1112, 0x1314, 0x1516, 0x1718, 0x191A, 0x1B1C, 0x1D1E, 0x1F20,
                                             0x2122, 0x2324, 0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                             0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C, 0x3D3E, 0x3F40};
uint16_t I2S_Slave_Buffer_Tx[BufferSize] = {0x4142, 0x4344, 0x4546, 0x4748, 0x494A, 0x4B4C, 0x4D4E, 0x4F50,
                                            0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C, 0x5D5E, 0x5F60,
                                            0x6162, 0x6364, 0x6566, 0x6768, 0x696A, 0x6B6C, 0x6D6E, 0x6F70,
                                            0x7172, 0x7374, 0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F90};
uint16_t I2S_Master_Buffer_Rx[BufferSize], I2S_Slave_Buffer_Rx[BufferSize];

volatile uint32_t TxIdx1 = 0, RxIdx1 = 0, TxIdx2 = 0, RxIdx2 = 0;

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/* Main program */
int main(void)
{
    /* At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_N32G401.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_N32G401.c file
     */
    /* System clocks configuration */
    RCC_Configuration();
    /* GPIO configuration */
    GPIO_Configuration();
    /* NVIC configuration*/
    NVIC_Configuration();
    /* log configuration*/
    log_init();    
    log_info("This is I2S Interrupt demo!\r\n");
    /* I2S_MASTER configuration*/
	I2S_Initializes_Structure(&I2S_InitStructure);
    I2S_InitStructure.I2sMode = I2S_MODE_MASTER_TX;
    I2S_InitStructure.AudioFrequency = I2S_AUDIO_FREQ_11K;
    I2S_InitStructure.CLKPOL = I2S_CLKPOL_HIGH;
    I2S_InitStructure.DataFormat = I2S_DATA_FMT_16BITS;
    I2S_InitStructure.MCLKEnable = I2S_MCLK_ENABLE;
    I2S_InitStructure.Standard = I2S_STD_PCM_LONGFRAME;
    I2S_Initializes(I2S_MASTER, &I2S_InitStructure);
    /* I2S_SLAVE configuration */
    I2S_InitStructure.I2sMode = I2S_MODE_SlAVE_RX;
    I2S_Initializes(I2S_SLAVE, &I2S_InitStructure);
    /* Enable the I2S DMA*/
    SPI_I2S_Interrupts_Enable(I2S_SLAVE, SPI_I2S_INT_RNE);
    SPI_I2S_Interrupts_Enable(I2S_MASTER, SPI_I2S_INT_TE); 
    /* Enable I2S */
    I2S_ON(I2S_SLAVE);
    I2S_ON(I2S_MASTER);
    /* Wait for the transmission to complete */
    while(RxIdx1 < BufferSize)
      ;

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(I2S_Slave_Buffer_Rx, I2S_Master_Buffer_Tx, BufferSize);

    /* Disable the I2S */
    I2S_OFF(I2S_MASTER);
    I2S_OFF(I2S_SLAVE);
    /* I2S_MASTER configuration */
    I2S_InitStructure.I2sMode = I2S_MODE_MASTER_RX;
    I2S_Initializes(I2S_MASTER, &I2S_InitStructure);
    /* I2S_SLAVE configuration */
    I2S_InitStructure.I2sMode = I2S_MODE_SlAVE_TX;
    I2S_Initializes(I2S_SLAVE, &I2S_InitStructure);
    /* Enable the I2S DMA*/
    SPI_I2S_Interrupts_Enable(I2S_SLAVE, SPI_I2S_INT_TE);
    SPI_I2S_Interrupts_Enable(I2S_MASTER, SPI_I2S_INT_RNE); 
    /* Enable I2S */
    I2S_ON(I2S_SLAVE);
    I2S_ON(I2S_MASTER);
    /* Wait for the transmission to complete */
    while(RxIdx2 < BufferSize)
      ;
    /* Check the received data with the send ones */
    TransferStatus2 = Buffercmp(I2S_Master_Buffer_Rx, I2S_Slave_Buffer_Tx, BufferSize);

    /* Disable the I2S */
    I2S_OFF(I2S_MASTER);
    I2S_OFF(I2S_SLAVE);

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
    /* I2S clock enable */
    RCC_APB2_Peripheral_Clock_Enable(I2S_MASTER_PERIPH | I2S_SLAVE_PERIPH);
    /* GPIO Periph clock enable */
    RCC_AHB_Peripheral_Clock_Enable(I2S_MASTER_PERIPH_GPIO | I2S_SLAVE_PERIPH_GPIO | RCC_AHB_PERIPH_DMA);
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
    /* Configure master pins: MCK, SD, CK and WS */
    /* Confugure all pins as Alternate Function Push Pull */
    GPIO_InitStructure.Pin        = I2S_MASTER_MCK_PIN | I2S_MASTER_SD_PIN | I2S_MASTER_CK_PIN | I2S_MASTER_WS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Alternate = I2S_MASTER_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(I2S_MASTER_GPIO, &GPIO_InitStructure);
  
    GPIO_InitStructure.Pin = I2S_MASTER_MCK_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_PULL_UP;
    GPIO_Peripheral_Initialize(I2S_MASTER_GPIO, &GPIO_InitStructure);
  
    /* Configure slave pins: MCK, SD, CK and WS */
    /* Confugure all pins as Input Floating */
    GPIO_InitStructure.Pin        = I2S_SLAVE_MCK_PIN | I2S_SLAVE_SD_PIN | I2S_SLAVE_CK_PIN | I2S_SLAVE_WS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = I2S_SLAVE_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(I2S_SLAVE_GPIO, &GPIO_InitStructure);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures the SPI&I2S interrupt.
*\*\param   none
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_Priority_Group_Set(NVIC_PER1_SUB3_PRIORITYGROUP);
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PER_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SUB_PRIORITY_2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PER_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SUB_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
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


