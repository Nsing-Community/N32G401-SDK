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
*\*\file timer_common.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "timer_common.h"

static TIM_TimeBaseInitType TIM_TimeBaseStructure;

/**
*\*\name    Common_TIM_Base_Initialize.
*\*\param   TIMx :
*\*\          - TIM1
*\*\          - TIM2
*\*\          - TIM3
*\*\          - TIM4
*\*\          - TIM5
*\*\          - TIM6
*\*\param   period
*\*\          - [1, 0xffff]
*\*\param   prescaler
*\*\          - [0, 0xffff]
*\*\return  none
**/
void Common_TIM_Base_Initialize(TIM_Module *TIMx, uint16_t period, uint16_t prescaler)
{
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = period;
    TIM_TimeBaseStructure.Prescaler = prescaler;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_Base_Initialize(TIMx, &TIM_TimeBaseStructure);   
}

/**
*\*\name    Common_TIM_NVIC_Initialize.
*\*\param   IRQ_number :
*\*\          - TIM1_BRK_IRQn
*\*\          - TIM1_UP_IRQn
*\*\          - TIM1_TRG_COM_IRQn
*\*\          - TIM1_CC_IRQn
*\*\          - TIM2_IRQn
*\*\          - TIM3_IRQn
*\*\          - TIM4_IRQn
*\*\          - TIM8_BRK_IRQn
*\*\          - TIM8_UP_IRQn
*\*\          - TIM8_TRG_COM_IRQn
*\*\          - TIM8_CC_IRQn
*\*\          - TIM5_IRQn
*\*\          - TIM6_IRQ
*\*\param   command
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return  none
**/
void Common_TIM_NVIC_Initialize(IRQn_Type IRQ_number, FunctionalState command)
{
    NVIC_InitType NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = IRQ_number;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    
    if(ENABLE == command)
    {
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    }
    
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name    Common_TIM_DMA_Config.
*\*\param   chan_handle
*\*\param   dma_param
*\*\param   req_remap
*\*\          - DMA_REMAP_TIM1_CH1
*\*\          - DMA_REMAP_TIM1_CH2
*\*\          - DMA_REMAP_TIM1_CH3
*\*\          - DMA_REMAP_TIM1_CH4
*\*\          - DMA_REMAP_TIM1_COM
*\*\          - DMA_REMAP_TIM1_UP
*\*\          - DMA_REMAP_TIM1_TRIG
*\*\          - DMA_REMAP_TIM2_CH1
*\*\          - DMA_REMAP_TIM2_CH2
*\*\          - DMA_REMAP_TIM2_CH3
*\*\          - DMA_REMAP_TIM2_CH4
*\*\          - DMA_REMAP_TIM2_UP
*\*\          - DMA_REMAP_TIM3_CH1
*\*\          - DMA_REMAP_TIM3_CH3
*\*\          - DMA_REMAP_TIM3_CH4
*\*\          - DMA_REMAP_TIM3_UP
*\*\          - DMA_REMAP_TIM3_TRIG
*\*\          - DMA_REMAP_TIM4_CH1
*\*\          - DMA_REMAP_TIM4_CH2
*\*\          - DMA_REMAP_TIM4_CH3
*\*\          - DMA_REMAP_TIM4_UP
*\*\          - DMA_REMAP_TIM5_CH1
*\*\          - DMA_REMAP_TIM5_CH2
*\*\          - DMA_REMAP_TIM5_CH3
*\*\          - DMA_REMAP_TIM5_CH4
*\*\          - DMA_REMAP_TIM5_UP
*\*\          - DMA_REMAP_TIM5_TRIG
*\*\          - DMA_REMAP_TIM6_UP
*\*\          - DMA_REMAP_TIM8_CH1
*\*\          - DMA_REMAP_TIM8_CH2
*\*\          - DMA_REMAP_TIM8_CH3
*\*\          - DMA_REMAP_TIM8_CH4
*\*\          - DMA_REMAP_TIM8_COM
*\*\          - DMA_REMAP_TIM8_UP
*\*\          - DMA_REMAP_TIM8_TRIG
*\*\return  none
**/
void Common_TIM_DMA_Config(DMA_ChannelType *chan_handle, DMA_InitType *dma_param, uint32_t req_remap)
{
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);
    
    DMA_Reset(chan_handle);

    DMA_Initializes(chan_handle, dma_param);

    DMA_Channel_Request_Remap(chan_handle, req_remap);
}

/**
*\*\name    Common_TIM_DMA_Config.
*\*\param   chan_handle
*\*\param   src_addr
*\*\param   dst_addr
*\*\param   len
*\*\return  none
**/
void Common_TIM_DMA_Start(DMA_ChannelType *chan_handle, uint32_t src_addr, uint32_t dst_addr, uint32_t len)
{
    DMA_Channel_Disable(chan_handle);

    chan_handle->TXNUM = len;
    if(chan_handle->CHCFG & DMA_DIR_PERIPH_DST)
    {
        chan_handle->PADDR = dst_addr;
        chan_handle->MADDR = src_addr;
    }
    else
    {
        chan_handle->PADDR = src_addr;
        chan_handle->MADDR = dst_addr;
    }

    DMA_Channel_Enable(chan_handle);
}
