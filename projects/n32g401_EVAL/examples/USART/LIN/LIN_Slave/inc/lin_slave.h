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
#ifndef _LIN_SLAVE_H
#define _LIN_SLAVE_H
#include <stdio.h>
#include "n32g401.h"

typedef void (*FUN_RESP)(uint8_t *pData, uint8_t Len); 

/* Exported Functions --------------------------------------------------------*/
#define GET_PID(data) ((data&0x3F)|((((data&0x01)^((data>>1)&0x01)^((data>>2)&0x01)^((data>>4)&0x01))&0x01)<<6)|(((~(((data>>1)&0x01)^((data>>3)&0x01)^((data>>4)&0x01)^((data>>5)&0x01)))&0x01)<<7))
#define ID_TYPE_SR  0
#define ID_TYPE_SW  1
/* Define the frame ID for sending and receiving data. The frame ID must be consistent with the software configuration of the bit-computer; 
otherwise, the frame ID not work properly.
For the LIN bus, the data sending and receiving ID  be defined as one ID or different ID */
#define MSG_RECEIVE_ID  0x3C
#define MSG_SEND_ID     0x3D
// Define the data check type, 0- standard check, 1- enhanced check, only for LIN upgrade
#define CHECK_TYPE   0
//Firmware type value definition
#define FW_TYPE_BOOT     0x55
#define FW_TYPE_APP      0xAA
//Set the current firmware type to BOOT
#define FW_TYPE         FW_TYPE_APP

typedef enum
{
    IDLE,
    SYNCH,
    ID_LEN,
    DATA_GET,
    CHECKSUM
} LIN_STATE;

typedef struct _LIN_EX_MSG
{
    unsigned char DataLen;      //Number of valid data bytes in a data segment
    unsigned char Sync;         
    unsigned char PID;          
    unsigned char Data[8];      
    unsigned char Check;        
} LIN_EX_MSG;
void LIN_Configuration(int BaudRate);
void LIN_SetResp(uint8_t ID, uint8_t *pData, uint8_t Len, uint8_t CheckType);
void LIN_IRQHandler(void);
void BOOT_ExecutiveCommand(uint8_t *pData, FUN_RESP pFunResp);

#endif /*LIN_DRIVER_H*/

/*********************************END OF FILE**********************************/


