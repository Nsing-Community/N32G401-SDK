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
*\*\file bsp_Tim_38K.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "bsp_Tim_38K.h"

void LPTIM_OutputIoInit(void);

void ADVANCE_TIM_Init(void)
{
    RCC_LPTIM_Clock_Config(ADVANCE_TIM_CLK);
    RCC_LPTIM_Enable();

    /* Init output IO */
    LPTIM_Prescaler_Set(LPTIM,ADVANCE_TIM_Prescaler);
    LPTIM_OutputIoInit();
    /* Init lptim module */
    LPTIM->INTEN = 0x7F;

    LPTIM_Waveform_Set(LPTIM,LPTIM_OUTPUT_WAVEFORM_PWM);
    /* output wave */ 
    LPTIM_Polarity_Set(LPTIM,LPTIM_OUTPUT_POLARITY_REGULAR);
    /* config the prescaler */
    LPTIM_Interrupt_Enable(LPTIM, LPTIM_INTEN_CMPUPDIE);

    LPTIM_ON(LPTIM);
    /* config ARR ande compare register */ 
    LPTIM_Auto_Reload_Set(LPTIM, ADVANCE_TIM_Period);
    LPTIM_Compare_Set(LPTIM, ADVANCE_TIM_Pulse);
    LPTIM_Counter_Start(LPTIM,LPTIM_OPERATING_MODE_CONTINUOUS);
}
/**
 * @brief  output IO Initaliza.
 * @param NONE.
 *   This parameter can be one of following parameters:
 *     @arg NONE
 */
void LPTIM_OutputIoInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Enable the GPIO Clock */
    RCC_AHB_Peripheral_Clock_Enable(PORTx_CLK);

    /* Configure the GPIO pin */
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = PORTx_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_LPTIM;
    GPIO_Peripheral_Initialize(PORTx, &GPIO_InitStructure);
}
