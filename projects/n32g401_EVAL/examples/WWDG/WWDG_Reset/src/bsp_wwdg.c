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
*\*\file bsp_wwdg.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "bsp_wwdg.h"

/**
 *\*\name   WWDG_Config.
 *\*\fun    Configure WWDG.
 *\*\param  prescaler :
 *\*\          - WWDG_PRESCALER_DIV1  (32MHz/4096/1 = 7813Hz(~128us))
 *\*\          - WWDG_PRESCALER_DIV2  (32MHz/4096/2 = 3906Hz(~256us))
 *\*\          - WWDG_PRESCALER_DIV4  (32MHz/4096/4 = 1953Hz(~512us))
 *\*\          - WWDG_PRESCALER_DIV8  (32MHz/4096/8 = 977Hz(~1024us))
 *\*\param  window_value :
 *\*\          -0x40 ~ 0x3FFF
 *\*\param  counter_value :
 *\*\          -0x40 ~ 0x3FFF
**/
void WWDG_Config(uint32_t prescaler, uint16_t window_value, uint16_t counter_value)
{
    /* Enable WWDG clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_WWDG);

    /* WWDG clock counter = (PCLK1(32MHz)/4096)/prescaler */
    WWDG_Prescaler_Division_Set(prescaler);

    /* Set Window value to window_value; WWDG counter should be refreshed only when the counter
     is below window_value and greater than 0x40 otherwise a reset will be generated */
    WWDG_Window_Value_Set(window_value);

    /* Set the value of the down counter */
    WWDG_Counter_Value_Set(counter_value);

    /* Enable WWDG and set counter value to counter_value */
    WWDG_Enable(counter_value);
}

/**
 *\*\name    WWDG_Feed.
 *\*\fun     Feed the dog.
 *\*\param  counter_value :
 *\*\          -0x40 ~ 0x3FFF
 *\*\return  none
**/
void WWDG_Feed(uint16_t counter_value)
{
    WWDG_Counter_Value_Set(counter_value);
}

