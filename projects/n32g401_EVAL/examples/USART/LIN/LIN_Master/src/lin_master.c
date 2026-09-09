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
#include "lin_master.h"

M_LIN_EX_MSG M_TxMsg;

void SetFrameMsg(M_LIN_EX_MSG *dst_Msg, M_LIN_EX_MSG src_Msg)
{
    int i = 0;
    Memset(dst_Msg, 0, sizeof(M_LIN_EX_MSG));
    dst_Msg->Check = src_Msg.Check;
    dst_Msg->DataLen = src_Msg.DataLen;
    dst_Msg->Sync = src_Msg.Sync;
    dst_Msg->PID = src_Msg.PID;
    for(i = 0; i < src_Msg.DataLen; i++)
    {
        dst_Msg->Data[i] = src_Msg.Data[i];
    }
    log_info("SetFrameMsg ID:0x%02x\r\n", dst_Msg->PID);
}

void SetFramePID(M_LIN_EX_MSG *src_Msg)
{
    uint8_t p0 = 0, p1 = 0;
    uint8_t LIN_ID = src_Msg->PID, PID = 0x00;
    p0 = (LIN_ID & 0x01) ^ ((LIN_ID & 0x02) >> 1) ^ ((LIN_ID & 0x04) >> 2) ^ ((LIN_ID & 0x10) >> 4);//????
    p0 = p0 & 0x01;
    p1 = ~(((LIN_ID & 0x02) >> 1) ^ ((LIN_ID & 0x08) >> 3) ^ ((LIN_ID & 0x10) >> 4) ^ ((LIN_ID & 0x20) >> 5));
    p1 = p1 & 0x01;
    PID = (p1 << 7) | (p0 << 6) | LIN_ID;
    src_Msg->PID = PID;
    log_info("p0 = %02x;p1 = %02x;PID = %02x\r\n", p0, p1, PID);
}

uint8_t MasterGetCheckSum(uint8_t *pData, uint8_t len)
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

void SetFrameChecksum(M_LIN_EX_MSG *Msg)
{
    uint8_t CheckSum = 0;
    uint8_t len = Msg->DataLen;
    if(Msg->Check)
    {
        CheckSum = MasterGetCheckSum(&Msg->PID, len + 1);
    }
    else
    {
        CheckSum = MasterGetCheckSum(Msg->Data, len);
    }
    if(len < 8)
    {
        Msg->Data[len] = CheckSum;
    }
    else
    {
        Msg->Check = CheckSum;
    }
}

void MasterSendBytes(uint8_t *pBuf, uint8_t Len)
{
    USART_Break_Frame_Send(USARTx);
    while(Len--)
    {
        while(USART_Flag_Status_Get(USARTx, USART_FLAG_TXC ) == RESET);
        USART_Data_Send(USARTx, *pBuf++);
    }
    while(USART_Flag_Status_Get(USARTx, USART_FLAG_TXC ) == RESET);
}

void MasterSendFrame(M_LIN_EX_MSG     Msg)
{
    if(Msg.DataLen)
    {
        MasterSendBytes(&Msg.Sync, Msg.DataLen + 3);
    }
    else
    {
        MasterSendBytes(&Msg.Sync, 2);
    }
}

void FrameHandle(void)
{
    uint8_t tmp_PID  = M_TxMsg.PID;
    SetFramePID(&M_TxMsg);
    switch (tmp_PID)
    {
    case 0x3C://Master request frame
        SetFrameChecksum(&M_TxMsg);
        break;
    case 0x3D://Slave reply frame
        M_TxMsg.DataLen = 0;
        break;
    default:
        break;
    }
    MasterSendFrame(M_TxMsg);
}

static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut)
{
    uint32_t Counter = 0;
    while((USART_Flag_Status_Get(USARTx, USART_FLAG_RXDNE) == RESET) && (Counter != TimeOut))
    {
        Counter++;
        /* Overflow data loss */
        if(USART_Flag_Status_Get(USARTx, USART_FLAG_OREF) != RESET)
        {
            USARTx->DAT;
        }
    }
    if(Counter != TimeOut)
    {
        *Data = (uint8_t)USART_Data_Receive(USARTx);
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

uint32_t Master_RecData(uint8_t *pdata, uint8_t length)
{
    int i = 0;
    uint8_t Data = 0;
    uint32_t number = 0;
    while(i < length)
    {
        i++;
        if((USART_ByteReceive(&Data, SC_RECEIVE_TIMEOUT)) == SUCCESS)
        {
            pdata[number] = Data;
            number++;
        }
    }
    return number;
}

ErrorStatus WaitFrameRes(uint8_t *dst_data, uint8_t length)
{
    //int i = 0;
    int datalen = 0;
    uint8_t recv_data[16];
    uint8_t CheckSum = 0;
    datalen = Master_RecData(recv_data, 16);
    if(datalen)
    {
#if 0
        log_info("recv_data:");
        for(i = 1; i < datalen; i++)
        {
            log_info("0x%x\r\n", recv_data[i]);
        }
#endif
        CheckSum = MasterGetCheckSum(recv_data, datalen - 1);
        log_info("CheckSum:0x%x\r\n", CheckSum);
        if(CheckSum == recv_data[datalen - 1])
        {
            if( (datalen - 2) > length)
            {
                Buffercopy(dst_data, &recv_data[0], length);
            }
            else
            {
                Buffercopy(dst_data, &recv_data[0], datalen - 1);
            }
            return SUCCESS;
        }
    }
    return ERROR;
}

void TestMasterReqFrame(void)
{
    int i = 0;
    M_LIN_EX_MSG CurLINTxMsg;
    CurLINTxMsg.Check = CLASSIC;
    CurLINTxMsg.DataLen = 8;
    CurLINTxMsg.Sync = 0x55;
    CurLINTxMsg.PID = 0x3C;
    for(i = 0; i < CurLINTxMsg.DataLen; i++)
    {
        CurLINTxMsg.Data[i] = 0x0F;
    }
    SetFrameMsg(&M_TxMsg, CurLINTxMsg);
    FrameHandle();

}

void TestSlaveResFrame(void)
{
    M_LIN_EX_MSG CurLINTxMsg;
    CurLINTxMsg.Check = CLASSIC;
    CurLINTxMsg.DataLen = 0;
    CurLINTxMsg.Sync = 0x55;
    CurLINTxMsg.PID = 0x3D;
    SetFrameMsg(&M_TxMsg, CurLINTxMsg);
    FrameHandle();
}

void TestLinMaster(void)
{
    int i = 0, count = 0;
    uint8_t recv_data[8];
    TestMasterReqFrame();
    delay_xms(20);
    while(count < 4)
    {
        Memset(recv_data, 0, 8);
        TestSlaveResFrame();
        if(WaitFrameRes(recv_data, 8) == SUCCESS)
        {
            log_info("recv_data:\r\n");
            for(i = 0; i < 8; i++)
            {
                log_info("recv_data[%d] = 0x%x\r\n", i, recv_data[i]);
            }
            break;
        }
        else
        {
            log_info("slave no response!!\r\n");
            count++;
        }
    }
}

