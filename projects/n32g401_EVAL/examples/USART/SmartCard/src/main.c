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
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include <stdio.h>
#include "main.h"
#include "smartcard.h"
uint8_t F_HotReset = 0;

static void System_Init(void)
{
    log_init();
    log_info("System Init Finish!\r\n");
}
void SC__HotReset_handle(void)
{
    SC_InitStructure SC_InitCfg;
    SC_State SCState = SC_POWER_OFF;
    SC_ADPU_Commands SC_ADPU;
    SC_ADPU_Responce SC_Responce;
    USART_InitType USART_InitStructure;
    RCC_ClocksType RCC_ClocksStatus;
    F_HotReset = 1;
    
    log_info("USARTx Smart_Card Hot_Reset\r\n");
    SC_Reset(Bit_RESET);
    systick_delay_ms(10);
    SC_Reset(Bit_SET);
    SC_InitCfg.Clk_Div   = 10;    //SC_USART Clock set to (PCLK1 = SC_USART MHZ / Clk_Div)MHz 
    SC_InitCfg.GT        = 16;
    SC_InitCfg.StopBits  = 3;
    SC_InitCfg.Parity    = 1;
    SC_InitCfg.NackEn    = 0;    // 0 - Disable NACK, 1 - Enable NACK
    SCState = SC_POWER_ON;
    SC_APDU_Init(&SC_ADPU, 0x00, SC_GET_A2R, 0x00, 0x00, 0x00, NULL, NULL);
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClocksStatus);
    /*reset BaudRate*/
    USART_Structure_Initializes(&USART_InitStructure);
    USART_InitStructure.BaudRate            = RCC_ClocksStatus.Pclk1Freq / (372 * ((SC_USART->GTP & (uint16_t)0x00FF) * 2));
    USART_InitStructure.WordLength          = USART_WL_9B;
    USART_InitStructure.StopBits            = USART_STPB_1_5;
    USART_InitStructure.Parity              = USART_PE_EVEN;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_Initializes(SC_USART, &USART_InitStructure);
    while(SCState != SC_ACTIVE_ON_T0)
    {
        SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);
    }
    log_info("USARTx Smart_Card Hot_Reset Finish\r\n");
    F_HotReset = 0;
}
void Test_PSAM(void)
{
    int i = 0;
    SC_InitStructure SC_InitCfg;
    SC_State SCState = SC_POWER_OFF;
    SC_ADPU_Commands SC_ADPU;
    SC_ADPU_Responce SC_Responce;
    log_info("USARTx Smart_Card Test Start\r\n");
    SC_InitCfg.Clk_Div   = 10;    //SC_USART Clock set to (PCLK1 = SC_USART MHZ / Clk_Div) MHz
    SC_InitCfg.GT        = 16;
    SC_InitCfg.StopBits  = 3;
    SC_InitCfg.Parity    = 1;
    SC_InitCfg.NackEn    = 0;    // 0 - Disable NACK, 1 - Enable NACK
    SCState = SC_POWER_ON;
    SC_APDU_Init(&SC_ADPU, 0x00, SC_GET_A2R, 0x00, 0x00, 0x00, NULL, 0);
    while(SCState != SC_ACTIVE_ON_T0)
    {
        SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);
    }
    /* Apply the Procedure Type Selection (PTS) */
    SC_PTSConfig();
    SC_APDU_Init(&SC_ADPU, 0x00, 0x84, 0x00, 0x00, 0x00, NULL, 8);
    SC_Handler(&SCState, &SC_ADPU, &SC_Responce, &SC_InitCfg);
    for(i = 0; i < SC_ADPU.Body.LE; i++)
    {
        log_info("\r\n SC_Responce.Data[%d] = 0x%02x \r\n", i, SC_Responce.Data[i]);
    }
    printf("\r\n SC_Responce.SW1 = 0x%02x \r\n", SC_Responce.SW1);
    printf("\r\n SC_Responce.SW2 = 0x%02x \r\n", SC_Responce.SW2);
    if((SC_Responce.SW1 == 0x90) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard command normally completed \r\n");
        log_info("get rand success!\r\n");
        for(i = 0; i < SC_ADPU.Body.LE; i++)
        {
            printf("%02x ", SC_Responce.Data[i]);
        }
        log_info("\r\n");
    }
    else if((SC_Responce.SW1 == 0x6E) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard CLA not supported \r\n");
    }
    else if((SC_Responce.SW1 == 0x6D) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard CLA supported, but INS not programmed or invalid \r\n");
    }
    else if((SC_Responce.SW1 == 0x6B) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard CLA INS supported, but P1 P2 incorrect \r\n");
    }
    else if((SC_Responce.SW1 == 0x67) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard CLA INS P1 P2 supported, but P3 incorrect \r\n");
    }
    else if((SC_Responce.SW1 == 0x6F) && (SC_Responce.SW2 == 0x00))
    {
        log_info("\r\n SmartCard command not supported and no precise diagnosis given \r\n");
    }
    SC__HotReset_handle();
    SC_DeInit();
}
/**
 * @brief  Main program
 */
int main(void)
{
    System_Init();
    Test_PSAM();
    log_info("PSAM test finish\r\n");
    while (1)
    {
    }
}
