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
*\*\version v1.0.1
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);
void COMP_Configuratoin(void);
void NVIC_Configuration(void);
void ChangeVmVp(void);


/**
*\*\name    Main program.
*\*\return  none
**/
int main(void)
{
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();

    /* COMP configuration ------------------------------------------------------*/
    COMP_Configuratoin();
    
    /* interrupt configuration ------------------------------------------------------*/
    COMP_Interrupt_Status_OneComp_Clear(COMP1);
    while (1)
    {
        ChangeVmVp();
    }
}
/**
*\*\name    ChangeVmVp.
*\*\fun     Self Generate Puls ,by skip line connect to vp and vm if need.
*\*\return  none
**/
void ChangeVmVp(void)
{
    GPIO_Pins_Set(GPIOB, GPIO_PIN_11);
    GPIO_PBSC_Pins_Reset(GPIOB, GPIO_PIN_12);
    {
        uint32_t i = 0;
        while (i++ < 10000)
            ;
    }
    GPIO_PBSC_Pins_Reset(GPIOB, GPIO_PIN_11);
    GPIO_Pins_Set(GPIOB, GPIO_PIN_12);
    {
        uint32_t i = 0;
        while (i++ < 10000)
            ;
    }
}

/**
*\*\name    COMP_Configuratoin.
*\*\fun     Configures the comp module.
*\*\return  none
**/
void COMP_Configuratoin(void)
{
    COMP_InitType COMP_Initial;

    /*Set dac3,dac2,dac1. */
    COMP_Voltage_Reference_Config(63, true, 32, true, 16, true);
    /*Initial comp*/
    COMP_Initializes_Structure(&COMP_Initial);
    COMP_Initial.InpSel     = COMP1_INPSEL_PB10;
    COMP_Initial.InmSel     = COMP1_INMSEL_PA5;
    COMP_Initial.SampWindow = 18;       //(0~31)
    COMP_Initial.Threshold  = 12;       //Thresh should be greater than half of SampWindow and should be less than SampWindow at the same time.
    COMP_Initializes(COMP1, &COMP_Initial);
    /*Enable comp Interrupt*/
    COMP_Interrupt_Enable(COMP1_INTEN);
    /*enable comp1*/
    COMP_ON(COMP1);
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable COMP clocks */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_COMP | RCC_APB1_PERIPH_COMP_FILT);
    /* Enable GPIOA, GPIOB, GPIOC and GPIOD clocks */
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB | RCC_AHB_PERIPH_GPIOC | RCC_AHB_PERIPH_GPIOD);
}

/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /*INP*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;
    GPIO_InitStructure.Pin        = GPIO_PIN_10;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    /*INM*/
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    /*OUT*/
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin       = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_COMP1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);

    /*PB11,P12 as connect to INP,INM by external skip line*/
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.Pin       = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures Vector Table base location.
*\*\return  none
**/
void NVIC_Configuration(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    EXTI_Interrupt_Status_Clear(EXTI_LINE21);
    EXTI_Structure_Initializes(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE21;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);
    /* Configures the priority grouping */
    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Configure and enable COMP interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = COMP_1_2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}
