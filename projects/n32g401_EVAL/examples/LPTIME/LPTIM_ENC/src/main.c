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


#define COUNT_NUM        20
/* LPTIM_ENC */

void LedBlink(GPIO_Module* GPIOx, uint16_t Pin);
void LEDInit(uint16_t Pin);
void LedOn(uint16_t Pin);
void LedOff(uint16_t Pin);
void Ledlink(uint16_t Pin);
void delay(vu32 nCount);


void EncInputIoConfig(void);
void Lptim_EncInit(void);
void EncWaveOutput(uint16_t count);
uint16_t encCNT = 0;
/* Main program */
int main(void)
{
    /* At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_n32g401.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_n32g401.c file.
     */
    /* Init log */
    log_init();
    log_info("This is LPTIM encoder mode demo\r\n");
    /* Init LED GPIO */
    LEDInit(LED1);
    LEDInit(LED2);
    /* Enable the LSI source */
    RCC_LPTIM_Enable();
    RCC_LSI_Enable();
    while(RCC_LSI_Stable_Wait() != SUCCESS);
    RCC_LPTIM_Clock_Config(RCC_LPTIMCLK_SRC_LSI);

    /* LPTIM input1 IO Init and encoder*/
    LPTIM_Prescaler_Set(LPTIM,LPTIM_PRESCALER_DIV1);
    EncInputIoConfig();
    Lptim_EncInit();
    /* LPTIM start count*/
    LPTIM_Counter_Start(LPTIM,LPTIM_OPERATING_MODE_CONTINUOUS); 
    /* Great 20 square waves ,and encCNT should be equal to 40*/
    EncWaveOutput(COUNT_NUM);
    encCNT = LPTIM->CNT; 
    if(encCNT == 2 * COUNT_NUM)
    {
        log_info("Test PASS!\r\n");
    }
    else
    {
        log_info("Test ERR!\r\n");
    }
    while (1)
    {
    }
}

/**
*\*\name    EncInputIoConfig.
*\*\fun     encode input IO Initaliza.
*\*\param   none
*\*\return  none
**/
void EncInputIoConfig(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Enable the GPIO Clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);

    /* Configure the GPIO pin */ 
    GPIO_InitStructure.Pin        = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate =  GPIO_AF3_LPTIM;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    /* Configure the GPIOpin */
    GPIO_InitStructure.Pin        = GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_INPUT;
    GPIO_InitStructure.GPIO_Alternate =  GPIO_AF6_LPTIM;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

/**
*\*\name    Lptim_EncInit.
*\*\fun     encode module Initaliza.
*\*\param   none
*\*\return  none
**/
void Lptim_EncInit(void)
{
    /* Config LPTIM */
    LPTIM_Clock_Source_Set(LPTIM,LPTIM_CLK_SOURCE_INTERNAL);
    LPTIM->CFG &=~(LPTIM_CFG_NENC|LPTIM_CFG_ENC);
    LPTIM_Encoder_Mode_Enable(LPTIM);
    /* ENC MODE1 */
    LPTIM_Encoder_Mode_Set(LPTIM,LPTIM_ENCODER_MODE_RISING);

    LPTIM_ON(LPTIM);
    LPTIM_Auto_Reload_Set(LPTIM,15000);
    LPTIM_Compare_Set(LPTIM,10000);    
}


/**
*\*\name    Ledlink.
*\*\fun     Toggles the selected Led.
*\*\param   LED1
*\*\param   LED2
*\*\param   LED3
*\*\return  none
**/
void Ledlink(uint16_t Pin)
{
    GPIOA->POD ^= Pin;
}
/**
*\*\name    LedOn.
*\*\fun     Turns selected Led on.
*\*\param   LED1
*\*\param   LED2
*\*\param   LED3
*\*\return  none
**/
void LedOn(uint16_t Pin)
{
    GPIOA->PBC = Pin;
}
/**
*\*\name    LedOff.
*\*\fun     Turns selected Led Off.
*\*\param   LED1
*\*\param   LED2
*\*\param   LED3
*\*\return  none
**/
void LedOff(uint16_t Pin)
{
    GPIOA->PBSC = Pin;
}
/**
*\*\name    LEDInit.
*\*\fun     Configures LED GPIO.
*\*\param   LED1
*\*\param   LED2
*\*\param   LED3
*\*\return  none
**/
void LEDInit(uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Enable the GPIO_LED Clock */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}
/**
*\*\name    delay.
*\*\fun     Delay by counting .
*\*\param   nCount
*\*\return  none
**/
void delay(vu32 nCount)
{
    vu32 index = 0;
    for (index = (34000 * nCount); index != 0; index--)
    {
    }
}

/**
*\*\name    EncWaveOutput.
*\*\fun     Great Encode square  waves.(PA1 and PA7)
*\*\param   count        the number of waves.
*\*\return  none
**/
void EncWaveOutput(uint16_t count)
{
    while(count--)
    {
        GPIO_PBSC_Pins_Reset(GPIOA, GPIO_PIN_1);
        delay(2);
        GPIO_PBSC_Pins_Reset(GPIOA, GPIO_PIN_7);
        delay(2);
        GPIO_Pins_Set(GPIOA, GPIO_PIN_1);
        delay(2);
        GPIO_Pins_Set(GPIOA, GPIO_PIN_7);
        delay(2);
    }
}

