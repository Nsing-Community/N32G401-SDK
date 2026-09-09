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
*\*\file main.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g401.h"

#define SPI_MASTER                SPI1
#define SPI_MASTER_CLK            RCC_APB2_PERIPH_SPI1
#define SPI_MASTER_GPIO           GPIOA
#define SPI_MASTER_GPIO_CLK       RCC_AHB_PERIPH_GPIOA
#define SPI_MASTER_PIN_NSS        GPIO_PIN_4
#define SPI_MASTER_PIN_SCK        GPIO_PIN_5
#define SPI_MASTER_PIN_MISO       GPIO_PIN_6
#define SPI_MASTER_PIN_MOSI       GPIO_PIN_7
#define SPI_MASTER_DMA            DMA
#define SPI_MASTER_DMA_CLK        RCC_AHB_PERIPH_DMA
#define SPI_MASTER_RX_DMA_CHANNEL DMA_CH2
#define SPI_MASTER_RX_DMA_FLAG    DMA_CH2_TXCF
#define SPI_MASTER_TX_DMA_CHANNEL DMA_CH3
#define SPI_MASTER_TX_DMA_FLAG    DMA_CH3_TXCF

#define SPI_SLAVE                SPI2
#define SPI_SLAVE_CLK            RCC_APB2_PERIPH_SPI2
#define SPI_SLAVE_GPIO           GPIOB
#define SPI_SLAVE_GPIO_CLK       RCC_AHB_PERIPH_GPIOB
#define SPI_SLAVE_PIN_NSS        GPIO_PIN_12
#define SPI_SLAVE_PIN_SCK        GPIO_PIN_13
#define SPI_SLAVE_PIN_MISO       GPIO_PIN_14
#define SPI_SLAVE_PIN_MOSI       GPIO_PIN_15
#define SPI_SLAVE_DMA            DMA
#define SPI_SLAVE_DMA_CLK        RCC_AHB_PERIPH_DMA
#define SPI_SLAVE_RX_DMA_CHANNEL DMA_CH4
#define SPI_SLAVE_RX_DMA_FLAG    DMA_CH4_TXCF
#define SPI_SLAVE_TX_DMA_CHANNEL DMA_CH5
#define SPI_SLAVE_TX_DMA_FLAG    DMA_CH5_TXCF

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

void RCC_Configuration(void);
void GPIO_Configuration(void);
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

