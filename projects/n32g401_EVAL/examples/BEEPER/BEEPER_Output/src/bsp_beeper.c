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
*\*\file bsp_beeper.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "bsp_beeper.h"
#include "bsp_delay.h"
#include "log.h"
#include "n32g401_beeper.h"

/**
 *\*\name   BEEPER_GPIO_Config.
 *\*\fun    BEEPER's GPIO configuration.
 *\*\param  none.
 *\*\return none.
**/
static void BEEPER_GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Turn on the port clock of the GPIO that controls the beeper */
    RCC_AHB_Peripheral_Clock_Enable(BEEPER1_GPIO_CLK | BEEPER2_GPIO_CLK);

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    GPIO_InitStructure.Pin            = BEEPER1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current   = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF12_BEEPER;
    GPIO_Peripheral_Initialize(BEEPER1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin            = BEEPER2_GPIO_PIN;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF12_BEEPER;
    GPIO_Peripheral_Initialize(BEEPER2_GPIO_PORT, &GPIO_InitStructure);
}

/**
*\*\name    BEEPER_Normal_Work.
*\*\fun     BEEPER normal operating mode and complementary mode configuration.
*\*\param   clock_selection:  
*\*\          - BEEPER_CLK_SOURCE_APB 
*\*\          - BEEPER_CLK_SOURCE_LSI 
*\*\          - BEEPER_CLK_SOURCE_LSE
*\*\param   prescale_factor:  if LSI or LSE clock is selected, this parameter is ignored
*\*\          - 0 ~ 63  if prescale_factor > 0 and prescale_factor < 64, the APB clock frequency is divided by (prescale_factor + 1)  
*\*\param   div_factor:  
*\*\          - 0 ~ 511  if div_factor is x, output frequency value divided by (x + 1) * 2
*\*\return  none. 
**/
void BEEPER_Normal_Work(uint32_t clock_selection, uint32_t prescale_factor, uint32_t div_factor)
{
    /* Enable BEEPER clock */
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_BEEPER);

    BEEPER_GPIO_Config();

    /* BEEPER initialization */
    BEEPER_Initialize(clock_selection, prescale_factor, div_factor);

    /* Enable complementary output */
    BEEPER_Inverted_Enable();

    /* Enable BEEPER */
    BEEPER_Enable();

    /* Delay 5s */
    SysTick_Delay_Ms(5000);

    /* Disable complementary output */
    BEEPER_Inverted_Disable();

    /* Disable BEEPER */
    BEEPER_Disable();

    log_info("The BEEPER output frequency is %dHz.\n", (64000000 / 32 / 1024));
}

