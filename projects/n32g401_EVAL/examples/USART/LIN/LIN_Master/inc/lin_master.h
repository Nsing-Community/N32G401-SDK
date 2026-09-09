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
*\*\file lin_master.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef _LIN_MASTER_H
#define _LIN_MASTER_H
#include <stdio.h>
#include "n32g401.h"
#include "log.h"

#define SC_RECEIVE_TIMEOUT 0x4000  /* Direction to reader */

typedef enum
{
    CLASSIC = 0,
    ENHANCED = !CLASSIC
} ChecksumType;

typedef struct M_LIN_EX_MSG
{
    ChecksumType CheckType;     //LIN data check type
    unsigned char DataLen;      //Number of valid data bytes in a data segment
    unsigned char Sync;         
    unsigned char PID;          
    unsigned char Data[8];      
    unsigned char Check;        
} M_LIN_EX_MSG;

void SetFrameMsg(M_LIN_EX_MSG *dst_Msg, M_LIN_EX_MSG src_Msg);
void SetFramePID(M_LIN_EX_MSG *src_Msg);
uint8_t MasterGetCheckSum(uint8_t *pData, uint8_t len);
void SetFrameChecksum(M_LIN_EX_MSG *Msg);
void MasterSendBytes(uint8_t *pBuf, uint8_t Len);
void MasterSendFrame(M_LIN_EX_MSG     Msg);
void FrameHandle(void);
static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut);
uint32_t Master_RecData(uint8_t *pdata, uint8_t length);
ErrorStatus WaitFrameRes(uint8_t *dst_data, uint8_t length);
void TestMasterReqFrame(void);
void TestSlaveResFrame(void);
void TestLinMaster(void);

#endif
