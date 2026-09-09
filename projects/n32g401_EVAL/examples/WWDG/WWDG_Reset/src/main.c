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
*\*\version v1.1.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"
#include "log.h"
#include "bsp_led.h"
#include "bsp_wwdg.h"
#include "bsp_delay.h"


/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
    uint8_t temp = 0;

    log_init();
    log_info("\r\n WWDG Demo Reset \r\n");

    /* Initialize the LED1/LED2 */
    LED_Initialize(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);

    /* Check if the system has resumed from WWDG reset */
    if(RCC_Flag_Status_Get(RCC_FLAG_WWDGRST) != RESET)
    {
        /* WWDGRST flag set */
        /* Turn On LED1 */
        LED_On(LED1_GPIO_PORT, LED1_GPIO_PIN);
        log_info("\r\n Reset By WWDG \r\n");
        
        /* Clear reset flags */
        RCC_Reset_Flag_Clear();
    }
    else 
    {
        /* WWDG Reset flag is not set */
        /* Turn Off LED1 */
        LED_Off(LED1_GPIO_PORT, LED1_GPIO_PIN);
    }

    /* Turn Off LED2 */
    LED_Off(LED2_GPIO_PORT, LED2_GPIO_PIN);

    /* Counter value to 16383, WWDG timeout = ~1024 us * 16320 = 16.7s
     In this case the refresh window is: ~1024 us * (16383 - 10000) = 6.6s < refresh window < ~1024 us * 16319 = 16.7s */
    WWDG_Config(WWDG_PRESCALER_DIV8, 0x2710, 0x3FFF);

    log_info("\r\n Set the upper window value to 0x2710.\r\n");

    while(temp < 3)
    {
        /* The program that needs to be monitored by WWDG should be written here. 
           The running time of the monitored program determines how large the window value should be set. 
        */
        
        /* The counter value is initialized to a maximum of 0x3FFF. 
           When the WWDG is turned on, the counter value will continue to decrease. 
           When the counter value is greater than the window value, if the dog is fed, the WWDG will be reset. 
           When the counter is reduced to 0x40 and the dog is not fed, the monitored program will Very dangerous, 
           the WWDG is reset when the counter is decremented to 0x3F again. 
           So to feed the dog when the value of the counter is between the window value and 0x40, 
           the lower window value of 0x40 is fixed.
        */
        
        /* Toggle LED2 */
        LED2_TOGGLE;
        
        /* Insertion delay. delay 7s */
        SysTick_Delay_Ms(7000);
        
        /* Feed the dog, reset the counter to the maximum value of 0x3FFF */
        WWDG_Feed(0x3FFF);
        
        log_info("\r\n Feed the dog i = %d times.\r\n", (temp + 1));

        temp++;
    }

    log_info("\r\n Do not Feed, system reset after 16.7s.\r\n");
    while(1)
    {
    }
}



