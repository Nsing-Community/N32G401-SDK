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
#include "bsp_delay.h"

__IO uint32_t TimingDelay = 0;
__IO uint32_t LsiFreq     = 40000;
extern uint16_t CaptureNumber;

/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
    uint16_t count = 0;
    log_init();
    log_info("\r\n IWDG Demo Reset \r\n");

    /* Initialize the LEDs */
    LED_Initialize(LED1_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN);

    /* Check if the system has resumed from IWDG reset */
    if(RCC_Flag_Status_Get(RCC_FLAG_IWDGRST) != RESET)
    {
        /* IWDGRST flag set */
        /* Turn On LED1 */
        LED_On(LED1_GPIO_PORT, LED1_GPIO_PIN);
        log_info("\r\n Reset By IWDG \r\n");
        
        /* Clear reset flags */
        RCC_Reset_Flag_Clear();
    }
    else 
    {
        /* IWDG Reset flag is not set */
        /* Turn Off LED1 */
        LED_Off(LED1_GPIO_PORT, LED1_GPIO_PIN);
    }
    
#ifdef LSI_TIM_MEASURE
    /* Enable the LSI OSC */
    RCC_LSI_Enable();

    /* Wait till LSI is ready */
    while (RCC_Flag_Status_Get(RCC_FLAG_LSIRD) == RESET)
    {
        /* if this flag is alaways not set, User can add here some code to deal with this error */
    }
    /* TIM Configuration -------------------------------------------------------*/
    TIM2_ConfigForLSI();
    /* Wait until the TIM2 get 3 LSI edges */
    while (CaptureNumber != 3)
    {
    }
    /* Disable TIM2 CC3 Interrupt Request */
    TIM_Interrupt_Disable(TIM2, TIM_INT_CC3);
#endif  /* LSI_TIM_MEASURE */

    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
       dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_Write_Protection_Disable();
    /* IWDG counter clock: LSI/32 */
    IWDG_Prescaler_Division_Set(IWDG_CONFIG_PRESCALER_DIV32);
    /* Set counter reload value to obtain 250ms IWDG TimeOut.
       Counter Reload Value = 250ms/IWDG counter clock period
                            = 250ms / (LSI/32)
                            = 0.25s / (LsiFreq/32)
                            = LsiFreq/(32 * 4)
                            = LsiFreq/128 */
    IWDG_Counter_Reload(LsiFreq / 128);
    /* Reload IWDG counter */
    IWDG_Key_Reload();
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
    while (1)
    {
        /* Toggle LED2 */
        LED_Toggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
        /* Insert 249 ms delay */
        SysTick_Delay_Ms(249);
        /* Reload IWDG counter */
        IWDG_Key_Reload();
        count++;
        if(count > 11)
        {
            break;
        }
    }
    LED_Off(LED2_GPIO_PORT, LED2_GPIO_PIN);
    /* Freeze IWDG, IWDG stop counting */
    IWDG_Freeze_Enable();
    
    while(1)
    {
        /* Toggle LED3 */
        LED_Toggle(LED3_GPIO_PORT, LED3_GPIO_PIN);
        /* Insert 249 ms delay */
        SysTick_Delay_Ms(249);
    }
}


#ifdef LSI_TIM_MEASURE
/**
 *\*\name   TIM2_ConfigForLSI.
 *\*\fun    Configures TIM2 to measure the LSI oscillator frequency.
 *\*\param  none.
 *\*\return none.
**/
void TIM2_ConfigForLSI(void)
{
    NVIC_InitType NVIC_InitStructure;
    TIM_ICInitType TIM_ICInitStructure;

    /* Enable TIM2 clocks */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM2);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);

    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Enable the TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    /* Configure TIM2 prescaler */
    TIM_Base_Prescaler_Set(TIM2, 0);
    TIM_Base_Reload_Mode_Set(TIM2, TIM_PSC_RELOAD_MODE_IMMEDIATE);
    
    /* TIM2 configuration: Input Capture mode 
    The LSI oscillator is connected to TIM2 CH3
    The Rising edge is used as active edge,
    The TIM2 CCDAT3 is used to compute the frequency value */
	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
    TIM_ICInitStructure.Channel     = TIM_CH_3;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV8;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);

    TIM_Base_Channel3(TIM2, true);
    /* TIM2 Counter Enable */
    TIM_On(TIM2);

    /* Reset the flags */
    TIM2->STS = 0;

    /* Enable the CC3 Interrupt Request */
    TIM_Interrupt_Enable(TIM2, TIM_INT_CC3);
}
#endif  /* LSI_TIM_MEASURE */

