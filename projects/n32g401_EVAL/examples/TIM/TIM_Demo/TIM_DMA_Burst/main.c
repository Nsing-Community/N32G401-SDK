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
#include "timer_common.h"
#include "ADTIM_common.h"
#include "GPTIM_common.h"
#include "main.h"

#if ADTIM_NUM == 1

#define ADTIM_DMAR_ADDRESS ((uint32_t)0x40012C4C) 

#elif ADTIM_NUM == 8

#define ADTIM_DMAR_ADDRESS ((uint32_t)0x4001344C)

#endif 

DMA_InitType DMA_InitStructure;

OCInitType TIM_OCInitStructure;
uint32_t SRC_Buffer[6] = {0x00FF, 0x0000, 0x0055};
uint16_t DmaAgain      = 0;

uint16_t prescaler = 0;

uint32_t ADTIMClockFrequency = 0;

void GPIO_Config(TIM_Module *TIMx);

void Two_Rising_Edge_Generate(void);

/**
*\*brief  Main program
**/
int main(void)
{
    /* System Clocks Configuration */
    ADTIMClockFrequency = Common_ADTIM_RCC_Initialize(ADTIM, RCC_HCLK_DIV2);

    /* DMA configuration */
    DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = 3;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_WORD;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_WORD;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
    Common_TIM_DMA_Config(DMA_CH5, &DMA_InitStructure, DMA_REMAP_TIM1_UP);
    
    /* Configure the GPIO ports */
    Common_ADTIM_GPIO_Initialize(ADTIM);

    /* 
    ADTIM Configuration: generate 1 PWM signal using the DMA burst mode:
    The ADTIMCLK frequency is set to SystemCoreClock (Hz), to get ADTIM counter
    clock at 36 MHz the Prescaler is computed as following:
     - Prescaler = (ADTIMCLK / ADTIM counter clock) - 1
    SystemCoreClock is set to 72MHz 

    The ADTIM period is 8.79 KHz: ADTIM Frequency = ADTIM counter clock/(AR + 1)
                                               = 36 MHz / 4096 = 8.79KHz
    ADTIM Channel1 duty cycle = (ADTIM_CCR1/ ADTIM_ARR) = 33.33%
                                                                            */
    prescaler = (uint16_t)(ADTIMClockFrequency / 36000000) - 1;

    /* Time base configuration, period = 0xFFFF, prescaler = prescaler */
    Common_TIM_Base_Initialize(ADTIM, 0xFFE, prescaler);

    /* TIM Configuration in PWM Mode */
    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 0x547;
    TIM_Output_Channel1_Initialize(ADTIM, &TIM_OCInitStructure);
    
    /* ADTIM DADDR Base register and DMA Burst Length Config */
    TIM_Dma_Config(ADTIM, TIM_DMABASE_AR, TIM_DMABURST_LENGTH_3TRANSFERS);

    /* ADTIM DMA Update enable */
    TIM_Dma_Enable(ADTIM, TIM_DMA_UPDATE);

    /* TIM enable counter */
    TIM_On(ADTIM);

    TIM_PWM_Output_Enable(ADTIM);

    Common_TIM_DMA_Start(DMA_CH5, (uint32_t)SRC_Buffer, ADTIM_DMAR_ADDRESS, 3);
    
    while(!DMA_Flag_Status_Get(DMA, DMA_CH5_TXCF))
    {
        /* none */
    }

    DMA_Flag_Status_Clear(DMA, DMA_CH5_GLBF | DMA_CH5_TXCF | DMA_CH5_HTXF | DMA_CH5_ERRF);

    while(1)
    {
        if (DmaAgain)
        {
            DmaAgain = 0;

            Common_TIM_DMA_Config(DMA_CH5, &DMA_InitStructure, DMA_REMAP_TIM1_UP);

            TIM_Dma_Config(ADTIM, TIM_DMABASE_AR, TIM_DMABURST_LENGTH_3TRANSFERS);
            TIM_Dma_Enable(ADTIM, TIM_DMA_UPDATE);

            /* DMA Channel5 enable */
            Common_TIM_DMA_Start(DMA_CH5, (uint32_t)SRC_Buffer, ADTIM_DMAR_ADDRESS, 3);

            /* Wait until DMA Channel5 end of Transfer */
            while (!DMA_Flag_Status_Get(DMA, DMA_CH5_TXCF))
            {
            }

            DMA_Flag_Status_Clear(DMA, DMA_CH5_GLBF | DMA_CH5_TXCF | DMA_CH5_HTXF | DMA_CH5_ERRF);
        }
    }
}


