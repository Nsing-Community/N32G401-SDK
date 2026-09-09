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


ADC_InitType ADC_InitStructure;
DMA_InitType DMA_InitStructure;
TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
__IO uint16_t ADC_RegularConvertedValueTab[10], ADC_InjectedConvertedValueTab[10];

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

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

    /* TIM1 configuration ------------------------------------------------------*/
    /* Time Base configuration */
    TIM_Base_Initialize(TIM1, &TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = 0x8FF; // 0xFF;
    TIM_TimeBaseStructure.Prescaler = 0x4;
    TIM_TimeBaseStructure.ClkDiv    = 0x0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM1, &TIM_TimeBaseStructure);
    /* TIM1 channel1 configuration in PWM mode */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 0x7F;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_LOW;
    TIM_Output_Channel1_Initialize(TIM1, &TIM_OCInitStructure);
    TIM_Output_Channel2_Initialize(TIM1, &TIM_OCInitStructure);
    TIM_Output_Channel4_Initialize(TIM1, &TIM_OCInitStructure);

    /* DMA Channel1 Configuration ----------------------------------------------*/
    DMA_Reset(DMA_CH1);
		DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)&ADC_RegularConvertedValueTab;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = 10;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
    DMA_Initializes(DMA_CH1, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH1, DMA_REMAP_ADC);
    /* Enable DMA channel1 */
    DMA_Channel_Enable(DMA_CH1);
    
    /* ADC configuration ------------------------------------------------------*/
		ADC_Initializes_Structure(&ADC_InitStructure);
    ADC_InitStructure.MultiChEn      = DISABLE;
    ADC_InitStructure.ContinueConvEn = DISABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_T1_CC2;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;
    ADC_Initializes(&ADC_InitStructure);
    
    /* Set injected sequencer length */
    ADC_Injected_Channels_Number_Config(ADC_INJECTED_LEN_1);
    /* ADC injected channel Configuration */
    ADC_Channel_Sample_Time_Config(ADC_Channel_01_PA0, ADC_SAMP_TIME_55CYCLES5);
    ADC_Injected_Sequence_Conversion_Number_Config(ADC_Channel_01_PA0, ADC_INJECTED_NUMBER_4);

    /* ADC injected external trigger configuration */
    ADC_Injected_Group_External_Trigger_Source_Config(ADC_EXT_TRIGCONV_INJECTED_SWSTRJCH);
    /* Enable automatic injected conversion start after regular one */
    ADC_Injected_Group_Autoconversion_Enable();
    /* Enable JEOC interrupt */
    ADC_Interrupts_Enable(ADC_INT_JENDCA);

    /* ADC regular channel configuration */
    ADC_Channel_Sample_Time_Config(ADC_Channel_02_PA1, ADC_SAMP_TIME_55CYCLES5);
    ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_02_PA1, ADC_REGULAR_NUMBER_1);
    
    /* Enable ADC external trigger */
    ADC_External_Trigger_Conversion_Config(ADC_EXTTRIGCONV_REGULAR_ENABLE);
    /* Enable ADC */
    ADC_ON();
    /* Check ADC Ready */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET)
        ;
    /* Start ADC calibration */
    ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);
    /* Check the end of ADC calibration */
    while (ADC_Calibration_Operation(ADC_CALIBRATION_STS))
        ;
	/* Enable ADC DMA */
    ADC_DMA_Transfer_Enable();
    /* TIM1 counter enable */
    TIM_On(TIM1);
    /* TIM1 main Output Enable */
    TIM_PWM_Output_Enable(TIM1);

#if 0
    /* Test on channel1 transfer complete flag */
    while(!DMA_GetFlagStatus(DMA_FLAG_TC1,DMA));
    /* Clear channel1 transfer complete flag */
    DMA_ClearFlag(DMA_FLAG_TC1,DMA);
    /* TIM1 counter disable */
    TIM_Enable(TIM1, DISABLE);
#endif

    while (1)
    {
    }
}

/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable peripheral clocks ------------------------------------------------*/
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_TIM1);
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_GPIOA|RCC_AHB_PERIPH_GPIOB|RCC_AHB_PERIPH_ADC);
    /* RCC_ADCHCLK_DIV16*/
    ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);  //selsect HSE as RCC ADC1M CLK Source
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
    /* Configure adc input as analog input -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_7 |GPIO_PIN_5 |GPIO_PIN_4 |GPIO_PIN_3 |GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin       = GPIO_PIN_14 |GPIO_PIN_13 |GPIO_PIN_12 |GPIO_PIN_11 |GPIO_PIN_10 |GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
    
    /* Configure PA6 as GPIO output -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    /* Configure PA8 as TIM1 CH1 PWM output -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    /* Configure PA11 as TIM1 CH4 PWM output -------------------------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM1;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

/**
*\*\name    NVIC_Configuration.
*\*\fun     Configures Vector Table base location.
*\*\return  none
**/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    /* Configures the priority grouping */
    NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}


