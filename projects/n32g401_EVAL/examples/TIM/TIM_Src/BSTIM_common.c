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
*\*\file BSTIM_common.c
*\*\author Nations 
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "BSTIM_common.h"

/**
*\*\name    Common_BSTIM_RCC_Initialize.
*\*\param   TIMx :
*\*\          - TIM6
*\*\param   hclk_division
*\*\          - RCC_HCLK_DIV1
*\*\          - RCC_HCLK_DIV2
*\*\          - RCC_HCLK_DIV4
*\*\          - RCC_HCLK_DIV8
*\*\          - RCC_HCLK_DIV16
*\*\return  uint32_t
**/
uint32_t Common_BSTIM_RCC_Initialize(TIM_Module *TIMx, uint32_t hclk_division)
{
    uint32_t BSTIM_clock;

    RCC_ClocksType RCC_Clocks;

    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB| RCC_AHB_PERIPH_GPIOC 
                                       | RCC_AHB_PERIPH_GPIOD);
    
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    
    RCC_Pclk1_Config(hclk_division);

    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM6);

    RCC_Clocks_Frequencies_Value_Get(&RCC_Clocks);

    if(RCC_Clocks.HclkFreq > RCC_Clocks.Pclk1Freq) 
    {
        BSTIM_clock = RCC_Clocks.Pclk1Freq * 2; 
    }
    else
    {
        BSTIM_clock = RCC_Clocks.Pclk1Freq;
    }
    return BSTIM_clock;
}
