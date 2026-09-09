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
*\*\file lin_master.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#include "main.h"
#include "lin_slave.h"

LIN_STATE LinRxState = IDLE;
uint8_t LINRxDataIndex = 0;
LIN_EX_MSG LINRxDataBuffer[2];//Double buffering receives data, which can reduce the probability of data error
uint8_t IDType[64] = {ID_TYPE_SR};
uint8_t GotMsgFlag = 0;
LIN_EX_MSG *pLINMsg;
LIN_EX_MSG LINTxMsg;

void LIN_SendBytes(uint8_t *pBuf, uint8_t Len)
{
    while(Len--)
    {
        while(USART_Flag_Status_Get(USARTx, USART_FLAG_TXC ) == RESET);
        USART_Data_Send(USARTx, *pBuf++);
    }
    while(USART_Flag_Status_Get(USARTx, USART_FLAG_TXC ) == RESET);
}

uint8_t LIN_GetCheckSum(uint8_t *pData, uint8_t len)
{
    uint16_t check_sum_temp = 0;
    uint8_t i;
    for(i = 0; i < len; i++)
    {
        check_sum_temp += pData[i];
        if(check_sum_temp > 0xFF)
        {
            check_sum_temp -= 0xFF;
        }
    }
    return (~check_sum_temp) & 0xFF;
}

void LIN_SetResp(uint8_t ID, uint8_t *pData, uint8_t Len, uint8_t CheckType)
{
    uint8_t i = 0;
    uint8_t CheckSum = 0;
    if(Len > 8)
    {
        Len = 8;
    }
    LINTxMsg.PID = GET_PID(ID);
    for(i = 0; i < Len; i++)
    {
        LINTxMsg.Data[i] = pData[i];
    }
    if(CheckType)
    {
        CheckSum = LIN_GetCheckSum(&LINTxMsg.PID, Len + 1);
    }
    else
    {
        CheckSum = LIN_GetCheckSum(LINTxMsg.Data, Len);
    }
    if(Len < 8)
    {
        LINTxMsg.Data[Len] = CheckSum;
    }
    else
    {
        LINTxMsg.Check = CheckSum;
    }
    LINTxMsg.DataLen = Len;
}

void LIN_EX_RxAsync(uint8_t data)
{
    switch(LinRxState)
    {
    case IDLE:
        break;
    case SYNCH:
        if(data == 0x55)
        {
            LINRxDataBuffer[LINRxDataIndex].Sync = 0x55;
            LinRxState = ID_LEN;
        }
        else
        {
            LinRxState = IDLE;
        }
        break;
    case ID_LEN:
        if(GET_PID(data) == data)
        {
            LINRxDataBuffer[LINRxDataIndex].PID = data;
            LINRxDataBuffer[LINRxDataIndex].DataLen = 0;
            if(IDType[data & 0x3F] == ID_TYPE_SR)
            {
                LinRxState = DATA_GET;
            }
            else
            {
                //Receives the frame header sent by the host and sends data in slave mode
                if(((LINTxMsg.PID & 0x3F) == (data & 0x3F)) && (LINTxMsg.DataLen > 0))
                {
                    LIN_SendBytes(LINTxMsg.Data, LINTxMsg.DataLen + 1);
                    LINTxMsg.DataLen = 0;
                }
                LinRxState = IDLE;
            }
        }
        else
        {
            LinRxState = IDLE;
        }
        break;
    case DATA_GET:
        LINRxDataBuffer[LINRxDataIndex].Data[LINRxDataBuffer[LINRxDataIndex].DataLen] = data;
        LINRxDataBuffer[LINRxDataIndex].Check = data;
        LINRxDataBuffer[LINRxDataIndex].DataLen++;
        if(LINRxDataBuffer[LINRxDataIndex].DataLen >= 8)
        {
            LinRxState = CHECKSUM;
        }
        else
        {
            LinRxState = DATA_GET;
        }
        break;
    case CHECKSUM:
        LINRxDataBuffer[LINRxDataIndex].Check = data;
        pLINMsg = &LINRxDataBuffer[LINRxDataIndex];
        GotMsgFlag = 1;
        LINRxDataIndex = (LINRxDataIndex + 1) % 2;
        LinRxState = IDLE;
        break;
    default:
        break;
    }
}

void LIN_IRQHandler(void)
{
    if(USART_Interrupt_Status_Get(USARTx, USART_INT_LINBD) == SET)
    {
        USART_Interrupt_Status_Clear(USARTx, USART_INT_LINBD);//Clear the LIN interval field detection flag bit
        //Read the status register and data register and clear the FE flag
        USARTx->STS;
        USARTx->DAT;
        LinRxState = SYNCH;
        return;
    }
    
    if (USART_Interrupt_Status_Get(USARTx, USART_INT_RXDNE)  == SET)
    {
        USART_Interrupt_Status_Clear(USARTx, USART_INT_RXDNE); //Clear the receive interrupt flag bit
        if(USART_Interrupt_Status_Get(USARTx, USART_INT_FEF) == RESET)
        {
            LIN_EX_RxAsync((uint8_t)USART_Data_Receive(USARTx));
        }
    }
	/* Determine if an error flag still exists and clear the error flag */
    if ((USART_Flag_Status_Get(USARTx, USART_FLAG_OREF) != RESET)  || \
        (USART_Flag_Status_Get(USARTx, USART_FLAG_NEF) != RESET)  || \
        (USART_Flag_Status_Get(USARTx, USART_FLAG_PEF) != RESET)  || \
        (USART_Flag_Status_Get(USARTx, USART_FLAG_FEF) != RESET))
    {
        /*Read the sts register first,and the read the DAT register to clear the all error flag*/
        (void)USARTx->STS;
        (void)USARTx->DAT;
        /* Under normal circumstances, all error flags will be cleared when the upper data is read and will not be executed here; 
           users can add their own processing according to the actual scenario. */
    }
}


/**
*\*\name    BOOT_ExecutiveCommand.
*\*\fun     Executes the command sent by the master.
*\*\param   pData
*\*\param   pFunResp
*\*\return  none
**/
void BOOT_ExecutiveCommand(uint8_t *pData, FUN_RESP pFunResp)
#if 1
{
    int i = 0;
    log_info("BOOT_ExecutiveCommand\r\n");
    for( i = 0; i < 8; i++)
    {
        log_info("pData[%d] = %02x \r\n", i, pData[i]);
        pData[i] = 0x1;
    }
    pFunResp(pData, 8);
}
#endif
/**
  * @}
  */

/*********************************END OF FILE**********************************/
