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
#include <stdio.h>
#include "main.h"


uint32_t T_value=0;
uint32_t VTS_value=0,VTS_NewValue;

ADC_InitType ADC_InitStructure;
DMA_InitType DMA_InitStructure;
__IO uint16_t ADCConvertedValue;
USART_InitType USART_InitStructure;
__IO float TempValue;


void RCC_Configuration(void);
void Delay(__IO uint32_t nCount);
void USART_Config(void);



/*xx mv per celsius degree by datasheet define*/
#define AVG_SLOPE  0.004f



/**  Calculate temperature use float result.  **/
float TempratureCalculate(uint16_t TempAdVal)
{ 

    float Temperate,tempValue;

    FLASH_ICache_Disable();
    FLASH_ICache_Reset();
     /* Get the voltage new value */
    Get_NVR_Value(0x1FFFF520, &VTS_value);
    if(VTS_value == 0xFFFFFFFF)
    {
        /* Get the voltage value , temperature sensor under 3.3V and 25 celsius */
        Get_NVR_Value(0x1FFFF7D0, &VTS_value);
    }
    VTS_value = (uint16_t)(VTS_value>>16);
    
     /* Get the new trim temperature */
    Get_NVR_Value(0x1FFFF51C, &T_value);
    if(T_value == 0xFFFFFFFF)
    {
        /* Get the trim temperature */
        Get_NVR_Value(0x1FFFF040, &T_value);
    }
    T_value = (uint16_t)(T_value>>16);
    FLASH_ICache_Enable();

    /* Voltage value of temperature sensor */
    tempValue=TempAdVal*(3.3/4095);
    /* Get the temperature inside the chip */
    Temperate=(VTS_value-(tempValue*1000))/(AVG_SLOPE*1000)+T_value/1000-1.25f;
    return Temperate;
}



/**
*\*\name    Main program.
*\*\return  none
**/
int main(void)
{
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* DMA channel1 configuration ----------------------------------------------*/
    DMA_Structure_Initializes(&DMA_InitStructure);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)&ADCConvertedValue;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = 1;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_DISABLE;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;
    DMA_Initializes(DMA_CH1, &DMA_InitStructure);
    DMA_Channel_Request_Remap(DMA_CH1 ,DMA_REMAP_ADC);
    /* Enable DMA channel1 */
    DMA_Channel_Enable(DMA_CH1);

    /* ADC configuration ------------------------------------------------------*/
		ADC_Initializes_Structure(&ADC_InitStructure);
    ADC_InitStructure.MultiChEn      = ENABLE;
    ADC_InitStructure.ContinueConvEn = ENABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;
    ADC_Initializes(&ADC_InitStructure);
    /* ADC enable temperature  */
    ADC_Temperature_Sensor_And_Vrefint_Channel_Enable();
    
    /* ADC regular ADC_CH_TEMP_SENSOR configuration */
    ADC_Channel_Sample_Time_Config(ADC_CH_TEMP_SENSOR ,ADC_SAMP_TIME_55CYCLES5);
    ADC_Regular_Sequence_Conversion_Number_Config(ADC_CH_TEMP_SENSOR , ADC_REGULAR_NUMBER_1);
    
    /* Enable ADC */
    ADC_ON();
    /* Check ADC Ready */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET)
        ;
    /* Start ADC calibration */
    ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);
    /* Check the end of ADC calibration */
    while (ADC_Calibration_Operation(ADC_CALIBRATION_STS) == SET)
        ;
		/* Enable ADC DMA */
    ADC_DMA_Transfer_Enable();
		
    /* Start ADC Software Conversion */
    ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);
    
    /* Config Uart1 as Temperature output */
    USART_Config();
    


    while (1)
    {
 
        /* */
        TempValue = TempratureCalculate(ADCConvertedValue);
        printf("\r\n Temperature = %.1f C\r\n",TempValue);         
        Delay(10000);
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
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO|RCC_APB2_PERIPH_USART1);
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_GPIOA|RCC_AHB_PERIPH_GPIOB|RCC_AHB_PERIPH_ADC);
    /* RCC_ADCHCLK_DIV16 */
    ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);  //selsect HSE as RCC ADC1M CLK Source
}
/**
*\*\name    Delay.
*\*\fun      Delay funciton.
*\*\return  none
**/
void Delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
} 
/**
*\*\name    USART_Config
*\*\fun     uart configure as 115200 8-N-1
*\*\return  none
**/
void USART_Config(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    /* Initialize GPIO_InitStructure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure USARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Tx_GPIO_AF;
    GPIO_Peripheral_Initialize(USARTx_GPIO, &GPIO_InitStructure);
    
    /* Configure USARTx Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = USARTx_RxPin;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Rx_GPIO_AF;
    GPIO_Peripheral_Initialize(USARTx_GPIO, &GPIO_InitStructure);
    
    USART_Structure_Initializes(&USART_InitStructure);
    /* USARTy and USARTz configuration ------------------------------------------------------*/
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    
    /* Configure USARTx */
    USART_Initializes(USARTx, &USART_InitStructure);
    /* Enable the USARTx */
    USART_Enable(USARTx);
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE* f)
{
    USART_Data_Send(USARTx, (uint8_t)ch);
    while (USART_Flag_Status_Get(USARTx, USART_FLAG_TXDE) == RESET)
        ;

    return (ch);
}

