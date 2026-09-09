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
*\*\file smartcard.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/

#include <stdio.h>
#include <stdlib.h>
#include "smartcard.h"
#include <string.h>
#include "log.h"
#include "delay.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables definition and initialization ----------------------------*/
SC_ATR SC_A2R;
uint8_t SC_ATR_Table[40];
static __IO uint8_t SCData = 0;
static uint32_t F_Table[16] = {0, 372, 558, 744, 1116, 1488, 1860, 0,
                               0, 512, 768, 1024, 1536, 2048, 0, 0
                              };
static uint32_t D_Table[16] = {0, 1, 2, 4, 8, 16, 32, 64,
                               12, 20, 0, 0, 0, 0, 0, 0
                              };
static int T_lenght_count = 0;
extern uint8_t F_HotReset;



/* Private function prototypes -----------------------------------------------*/
/* Transport Layer -----------------------------------------------------------*/
/*--------------APDU-----------*/
// static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus);

/*------------ ATR ------------*/
// static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length);  /* Ask ATR */
// static uint8_t SC_decode_Answer2reset(uint8_t *card);  /* Decode ATR */

/* Physical Port Layer -------------------------------------------------------*/
// static void SC1_Init(SC_InitStructure *SC_InitCfg);
// void SC1_DeInit(void);

static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes all peripheral used for Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_Init(SC_InitStructure *SC_InitCfg)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    USART_ClockInitType USART_ClockInitStructure;
    RCC_ClocksType RCC_ClocksStatus;
    /* Enable GPIO clocks */
    RCC_AHB_Peripheral_Clock_Enable(SC_USART_GPIO_CLK | SC_PIN_RESET_GPIO_CLK);
    /* Enable USART clock */
    SC_USART_APBxClkCmd(SC_USART_CLK);
    /* Initialize GPIO_InitStructure */
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Configure USART CK pin as alternate function push-pull */
    GPIO_InitStructure.Pin              = SC_USART_PIN_CK;
    GPIO_InitStructure.GPIO_Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate   = SC_USART_Clk_GPIO_AF;
    GPIO_InitStructure.GPIO_Current =  GPIO_DS_12MA;
    GPIO_Peripheral_Initialize(SC_USART_GPIO, &GPIO_InitStructure);
    /* Configure USART Tx pin as alternate function open-drain */
    GPIO_InitStructure.Pin              = SC_USART_PIN_TX;
    GPIO_InitStructure.GPIO_Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Alternate   = SC_USART_Tx_GPIO_AF;
    GPIO_InitStructure.GPIO_Current =  GPIO_DS_12MA;
    GPIO_Peripheral_Initialize(SC_USART_GPIO, &GPIO_InitStructure);
    /* Configure Smartcard Reset pin */
    GPIO_InitStructure.Pin = SC_PIN_RESET;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Peripheral_Initialize(SC_PIN_RESET_GPIO, &GPIO_InitStructure);
    /* SC_USART configuration ----------------------------------------------------*/
    /* SC_USART configured as follow:
        - Word Length = 9 Bits
        - 0.5 Stop Bit
        - Even parity
        - BaudRate = 9677 baud
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
        - USART Clock enabled
    */
    /* SC_USART Clock set to 2.7MHz (PCLK1 = 27 MHZ / 10) */
    USART_Prescaler_Set(SC_USART, SC_InitCfg->Clk_Div >> 1);
    /* USART Guard Time set to 16 Bit */
    USART_Guard_Time_Set(SC_USART, SC_InitCfg->GT);
    USART_Clock_Structure_Initializes(&USART_ClockInitStructure);
    USART_ClockInitStructure.Clock      = USART_CLK_ENABLE;
    USART_ClockInitStructure.Polarity   = USART_CLKPOL_LOW;
    USART_ClockInitStructure.Phase      = USART_CLKPHA_1EDGE;
    USART_ClockInitStructure.LastBit    = USART_CLKLB_ENABLE;
    USART_Clock_Initializes(SC_USART, &USART_ClockInitStructure);
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClocksStatus);
    USART_Structure_Initializes(&USART_InitStructure);    
    USART_InitStructure.BaudRate = RCC_ClocksStatus.Pclk1Freq / (372 * ((SC_USART->GTP & (uint16_t)0x00FF) * 2));
    log_info(" Pclk1 =%d,Clk_Div = %d,bps =%d\r\n", RCC_ClocksStatus.Pclk1Freq, (372 * ((SC_USART->GTP & (uint16_t)0x00FF) *2)),USART_InitStructure.BaudRate);
    USART_InitStructure.WordLength = USART_WL_9B;
    if(SC_InitCfg->StopBits == 1)
    {
        USART_InitStructure.StopBits = USART_STPB_0_5;
    }
    else if(SC_InitCfg->StopBits == 3)
    {
        USART_InitStructure.StopBits = USART_STPB_1_5;
    }
    else
    {
        log_info("\r\n StopBits value is error !!! \r\n");
    }
    if(SC_InitCfg->Parity == 1)
    {
        USART_InitStructure.Parity = USART_PE_EVEN;
    }
    else if(SC_InitCfg->Parity == 2)
    {
        USART_InitStructure.Parity = USART_PE_ODD;
    }
    else
    {
        log_info("\r\n Parity value is error !!! \r\n");
    }
    USART_InitStructure.Mode                  = USART_MODE_RX | USART_MODE_TX;
    USART_InitStructure.HardwareFlowControl   = USART_HFCTRL_NONE;
    USART_Initializes(SC_USART, &USART_InitStructure);
    /* Enable the SC_USART Parity Error Interrupt */
    USART_Interrput_Enable(SC_USART, USART_INT_PEF);
    /* Enable the SC_USART Framing Error Interrupt */
    USART_Interrput_Enable(SC_USART, USART_INT_ERRF);
    /* Enable SC_USART */
    USART_Enable(SC_USART);
    /* Enable the NACK Transmission */
    if(SC_InitCfg->NackEn == 0)
    {
        USART_Smart_Card_NACK_Disable(SC_USART);
    }
    else if(SC_InitCfg->NackEn == 1)
    {
        USART_Smart_Card_NACK_Enable(SC_USART);
    }
    else
    {
        log_info("\r\n NACK Enable value is error !!! \r\n");
    }
    /* Enable the Smartcard Interface */
    USART_Smart_Card_Enable(SC_USART);
    /* Set RSTIN HIGH */
    SC_Reset(Bit_SET);
}

/**
  * @brief  Deinitializes all ressources used by the Smartcard interface.
  * @param  None
  * @retval None
  */
void SC_DeInit(void)
{
    /* Deinitializes the SC_USART */
    USART_Reset(SC_USART);
    GPIO_Reset(SC_PIN_RESET_GPIO);
    /* Disable GPIO clocks */
    RCC_APB2_Peripheral_Clock_Disable(SC_USART_GPIO_CLK);
    RCC_AHB_Peripheral_Clock_Disable(SC_PIN_RESET_GPIO_CLK);
    /* Disable SC_USART clock */
    SC_USART_APBxClkCmd(SC_USART_CLK);
}

/**
  * @brief  Sets or clears the Smartcard reset pin.
  * @param  ResetState: this parameter specifies the state of the Smartcard
  *   reset pin. BitVal must be one of the BitAction enum values:
  *     @arg Bit_RESET: to clear the port pin.
  *     @arg Bit_SET: to set the port pin.
  * @retval None
  */
void SC_Reset(Bit_OperateType ResetState)
{
    if(ResetState)
    {
        GPIO_Pins_Set(SC_PIN_RESET_GPIO, SC_PIN_RESET);
    }
    else
    {
        GPIO_Pins_Reset(SC_PIN_RESET_GPIO, SC_PIN_RESET);
    }
}

/**
  * @brief  Resends the byte that failed to be received (by the Smartcard) correctly.
  * @param  None
  * @retval None
  */
void SC_ParityErrorHandler(void)
{
    USART_Data_Send(SC_USART, SCData);
    while(USART_Flag_Status_Get(SC_USART, USART_FLAG_TXC) == RESET)
    {
    }
}

/**
 * @brief  Configures the DMA.
 */
void DMA_Configuration(uint8_t *TxBuffer,uint32_t TxBufferSize, uint8_t *RxBuffer,uint32_t RxBufferSize)
{
    DMA_InitType DMA_InitStructure;
    RCC_AHB_Peripheral_Clock_Enable(SC_USART_DMAx_CLK);
    /* USARTy TX DMA1 Channel (triggered by USARTy Tx event) Config */
    DMA_Reset(SC_USART_Tx_DMA_Channel);
    DMA_InitStructure.PeriphAddr     = SC_USART_DR_Base;
    DMA_InitStructure.MemAddr        = (uint32_t)TxBuffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = TxBufferSize;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGHEST;
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
    if(TxBufferSize != 0 ){
        DMA_Initializes(SC_USART_Tx_DMA_Channel, &DMA_InitStructure);
        DMA_Channel_Request_Remap(SC_USART_Tx_DMA_Channel, SC_USART_Tx_DMA_REMAP);

    }

    /* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
    DMA_Reset(SC_USART_Rx_DMA_Channel);
    DMA_InitStructure.PeriphAddr = SC_USART_DR_Base;
    DMA_InitStructure.MemAddr    = (uint32_t)RxBuffer;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize    = RxBufferSize;
    if(RxBufferSize != 0 ){
        DMA_Initializes(SC_USART_Rx_DMA_Channel, &DMA_InitStructure);
        DMA_Channel_Request_Remap(SC_USART_Rx_DMA_Channel, SC_USART_Rx_DMA_REMAP);
    }
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param BufferLength buffer's length
 * @return PASSED: pBuffer1 identical to pBuffer2
 *         FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

/**
  * @brief  Configures the IO speed (BaudRate) communication.
  * @param  None
  * @retval None
  */
void SC_PTSConfig(void)
{
    RCC_ClocksType RCC_ClocksStatus;
    uint32_t workingbaudrate = 0, apbclock = 0;
    uint8_t TxBuffer1[4] = {0};
    uint8_t RxBuffer1[4] = {0};
    USART_InitType USART_InitStructure;
    USART_ClockInitType USART_ClockInitStructure;
    /* Reconfigure the USART Baud Rate -------------------------------------------*/
    RCC_Clocks_Frequencies_Value_Get(&RCC_ClocksStatus);
    apbclock = RCC_ClocksStatus.Pclk1Freq;
    apbclock /= ((SC_USART->GTP & (uint16_t)0x00FF) * 2);
    /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
    in case of a framing error FE */

    if((SC_A2R.T0 & (uint8_t)0x10) == 0x10)
    {
        if(SC_A2R.T[0] != 0x11)
        {
//            /* Send PPSS */
            TxBuffer1[0] = 0xFF;
//            /* Send PPS0 */
            TxBuffer1[1] = 0x10;
//            /* PPS1~3  */
            TxBuffer1[2] = SC_A2R.T[0];
//            /* PCK */
            TxBuffer1[3] = (uint8_t)0xFF ^ (uint8_t)0x10 ^ (uint8_t)SC_A2R.T[0];
            DMA_Configuration(TxBuffer1, 4, RxBuffer1, 4);            
            /* Enable USARTy DMA Rx and TX request */
            USART_DMA_Transfer_Enable(SC_USART, USART_DMAREQ_RX | USART_DMAREQ_TX);

            /* Enable USARTy TX DMA1 Channel */
            DMA_Channel_Enable(SC_USART_Tx_DMA_Channel);
            /* Enable USARTy RX DMA1 Channel */
            DMA_Channel_Enable(SC_USART_Rx_DMA_Channel);

            /* Wait until USARTy TX DMA1 Channel Transfer Complete */
            while (DMA_Flag_Status_Get(SC_USART_DMAx, SC_USART_Tx_DMA_FLAG) == RESET)
            {
            }
            DMA_Flag_Status_Clear(SC_USART_DMAx, SC_USART_Tx_DMA_FLAG);
            /* Wait until USARTy RX DMA1 Channel Transfer Complete */
            while (DMA_Flag_Status_Get(SC_USART_DMAx, SC_USART_Rx_DMA_FLAG) == RESET)
            {
            }            
            DMA_Flag_Status_Clear(SC_USART_DMAx, SC_USART_Rx_DMA_FLAG);
            log_info(" locData =%02x %02x %02x %02x\r\n", RxBuffer1[0],RxBuffer1[1],RxBuffer1[2],RxBuffer1[3]);

            /* DISABLE USARTy DMA Rx and TX request */
            USART_DMA_Transfer_Enable(SC_USART, USART_DMAREQ_RX | USART_DMAREQ_TX);

            /* DISABLE USARTy TX DMA1 Channel */
            DMA_Channel_Disable(SC_USART_Tx_DMA_Channel);
            /* DISABLE USARTy RX DMA1 Channel */
            DMA_Channel_Disable(SC_USART_Rx_DMA_Channel);
            
            if(Buffercmp(TxBuffer1, RxBuffer1, 4) == 0x01)
            {
                log_info(" PTS Confirm success\r\n");
                /* DISABLE SC_USART */
                USART_Enable(SC_USART);
                workingbaudrate = apbclock * D_Table[(SC_A2R.T[0] & (uint8_t)0x0F)];
                workingbaudrate /= F_Table[((SC_A2R.T[0] >> 4) & (uint8_t)0x0F)];
                USART_ClockInitStructure.Clock          = USART_CLK_ENABLE;
                USART_ClockInitStructure.Polarity       = USART_CLKPOL_LOW;
                USART_ClockInitStructure.Phase          = USART_CLKPHA_1EDGE;
                USART_ClockInitStructure.LastBit        = USART_CLKLB_ENABLE;
                USART_Clock_Initializes(SC_USART, &USART_ClockInitStructure);
                log_info(" bps =%d\r\n", workingbaudrate);
                USART_InitStructure.BaudRate            = workingbaudrate;
                USART_InitStructure.WordLength          = USART_WL_9B;
                USART_InitStructure.StopBits            = USART_STPB_1_5;
                USART_InitStructure.Parity              = USART_PE_EVEN;
                USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
                USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
                USART_Initializes(SC_USART, &USART_InitStructure);
                log_info(" Pclk1 =%d,apbclock = %d,bps =%d\r\n", RCC_ClocksStatus.Pclk1Freq, apbclock,USART_InitStructure.BaudRate);
                /* Enable SC_USART */
                USART_Enable(SC_USART);
            }
            else
            {
                log_info(" PTS Confirm fail,HotReset\r\n");
                SC__HotReset_handle();
            }
                
        }   
    }    
}

/**
  * @brief  Manages the Smartcard transport layer: send APDU commands and receives
  *   the APDU responce.
  * @param  SC_ADPU: pointer to a SC_ADPU_Commands structure which will be initialized.
  * @param  SC_Response: pointer to a SC_ADPU_Responce structure which will be initialized.
  * @retval None
  */
static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus)
#if 1
{
    uint8_t locData = 0;
    uint8_t TxBuffer1[5] = {0};
    uint32_t i = 0;
    uint32_t number = 0;
    uint32_t Tx_len = 0;
    /* Reset responce buffer ---------------------------------------------------*/
    for(i = 0; i < LC_MAX; i++)
    {
        SC_ResponceStatus->Data[i] = 0;
    }
    log_info("\r\nSC_ADPU:0x%x", SC_ADPU->Header.CLA);
    log_info(" 0x%x", SC_ADPU->Header.INS);
    log_info(" 0x%x", SC_ADPU->Header.P1);
    log_info(" 0x%x", SC_ADPU->Header.P2);
    log_info(" 0x%x", SC_ADPU->Body.LC);
    log_info(" 0x%x\r\n", SC_ADPU->Body.LE);
    SC_ResponceStatus->SW1 = 0;
    SC_ResponceStatus->SW2 = 0;
    /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
    in case of a framing error FE */
    /* Send header -------------------------------------------------------------*/
    TxBuffer1[0] = SC_ADPU->Header.CLA;
    TxBuffer1[1] = SC_ADPU->Header.INS;
    TxBuffer1[2] = SC_ADPU->Header.P1;
    TxBuffer1[3] = SC_ADPU->Header.P2;
    Tx_len = 4;    
    if(SC_ADPU->Body.LC){
        TxBuffer1[4] = SC_ADPU->Body.LC;
        Tx_len++;
    }else if(SC_ADPU->Body.LE){
        TxBuffer1[4] = SC_ADPU->Body.LE;
        Tx_len++;
    }    
    DMA_Configuration(TxBuffer1, Tx_len, NULL, 0);            
    /* Enable USARTy DMA Rx and TX request */
    USART_DMA_Transfer_Enable(SC_USART, USART_DMAREQ_TX);
    /* Enable USARTy DMA Rx and TX request */
    /* Enable USARTy TX DMA1 Channel */
    DMA_Channel_Enable(SC_USART_Tx_DMA_Channel);
    
    /* Wait until USARTy TX DMA1 Channel Transfer Complete */
    while (DMA_Flag_Status_Get(SC_USART_DMAx, SC_USART_Tx_DMA_FLAG) == RESET)
    {
    }
    DMA_Flag_Status_Clear(SC_USART_DMAx, SC_USART_Tx_DMA_FLAG);

    USART_DMA_Transfer_Disable(SC_USART, USART_DMAREQ_TX);
    /* DISABLE USARTy TX DMA1 Channel */
    DMA_Channel_Disable(SC_USART_Tx_DMA_Channel);

    /* Wait until USARTy TX Transfer Complete */
    while(USART_Flag_Status_Get(SC_USART, USART_FLAG_TXC) == RESET)
    {
    }
    /* Flush the SC_USART DAT */
    USART_Data_Receive(SC_USART);
    /* --------------------------------------------------------
    Wait Procedure byte from card:
    1 - ACK
    2 - NULL
    3 - SW1; SW2
    -------------------------------------------------------- */
    SC_USART->CTRL1 &= ~USART_WL_9B;
    while(1)
    {
        if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
        {
            if(locData != 0x60)
            {
                if(((locData & (uint8_t)0xFE) == (((uint8_t)~(SC_ADPU->Header.INS)) & \
                                                  (uint8_t)0xFE)) || ((locData & (uint8_t)0xFE) == (SC_ADPU->Header.INS & (uint8_t)0xFE)))
                {
                    SC_ResponceStatus->Data[0] = locData;/* ACK received */
                    break;
                }
                else if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
                {
                    SC_ResponceStatus->SW1 = locData;
                    if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                    {
                        /* SW2 received */
                        if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
                        {
                            SC_ResponceStatus->SW2 = locData;
                        }
                    }
                    break;
                }
                else //NULL
                {
                    SC_ResponceStatus->Data[number] = locData;
                    number++;
                    for(; number < (SC_ADPU->Body.LC); number++)
                    {
                        if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                        {
                            if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
                            {
                                SC_ResponceStatus->Data[number] = locData;
                            }
                            //log_info("%x , %d\r\n", locData, number);
                        }
                    }
                    /* SW1 received */
                    if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                    {
                        if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
                        {
                            SC_ResponceStatus->SW1 = locData;
                        }
                    }
                    if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                    {
                        /* SW2 received */
                        if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
                        {
                            SC_ResponceStatus->SW2 = locData;
                        }
                    }
                    break;
                }
            }
        }
    }
    SC_USART->CTRL1 |= USART_WL_9B;
    /* If no status bytes received ---------------------------------------------*/
    if(SC_ResponceStatus->SW1 == 0x00)
    {
        /* Send body data to SC--------------------------------------------------*/
        if (SC_ADPU->Body.LC)
        {
            for(i = 0; i < SC_ADPU->Body.LC; i++)
            {
                SCData = SC_ADPU->Body.Data[i];
                USART_Data_Send(SC_USART, SCData);
                while(USART_Flag_Status_Get(SC_USART, USART_FLAG_TXC) == RESET)
                {
                }
            }
        }
        /* Or receive body data from SC ------------------------------------------*/
        else if (SC_ADPU->Body.LE)
        {
            SC_USART->CTRL1 &= ~USART_WL_9B;
            for(i = 0; i < SC_ADPU->Body.LE; i++)
            {
                if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
                {
                    SC_ResponceStatus->Data[i] = locData;
                }
            }
            SC_USART->CTRL1 |= USART_WL_9B;
        }
        /* Wait SW1 --------------------------------------------------------------*/
        SC_USART->CTRL1 &= ~USART_WL_9B;
        i = 0;
        while(i < 10)
        {
            if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
            {
                SC_ResponceStatus->SW1 = locData;
                i = 11;
            }
            else
            {
                i++;
            }
        }
        /* Wait SW2 ------------------------------------------------------------*/
        i = 0;
        while(i < 10)
        {
            if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
            {
                SC_ResponceStatus->SW2 = locData;
                i = 11;
            }
            else
            {
                i++;
            }
        }
        SC_USART->CTRL1 |= USART_WL_9B;
    }
}


#endif


/**
  * @brief  Requests the reset answer from card.
  * @param  SCState: pointer to an SC_State enumeration that will contain the Smartcard state.
  * @param  card: pointer to a buffer which will contain the card ATR.
  * @param  length: maximum ATR length
  * @retval None
  */
static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length)
{
    uint8_t Data = 0;
    uint32_t i = 0;
    uint32_t number = 0;
//    log_info("SC_AnswerReq *SCState = %d \r\n", *SCState);
    SC_USART->CTRL1 &= ~USART_WL_9B;
    switch(*SCState)
    {
        case SC_RESET_LOW:
            //log_info(" SC_RESET_LOW\r\n");
            /* Check responce with reset low ------------------------------------- */
            while(i < (length))
            {
                i++;
                if((USART_ByteReceive(&Data, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                {
                    card[number] = Data;
                    number++;
                    //log_info(" SC_AnswerReq SC_RESET_LOW Rece success card[%d] = %02x\r\n",i,card[i]);
                }
            }
            if(card[0])
            {
                (*SCState) = SC_ACTIVE;
                SC_Reset(Bit_SET);/* Reset High */
            }
            else
            {
                (*SCState) = SC_RESET_HIGH;
            }
            break;
        case SC_RESET_HIGH:
            //log_info(" SC_RESET_HIGH\r\n");
            /* Check responce with reset high --------------------------------------*/
            SC_Reset(Bit_SET); /* Reset High */
            while(i < length )
            {
                i++;
                if((USART_ByteReceive(&Data, SC_RECEIVE_TIMEOUT)) == SUCCESS)
                {
                    card[number] = Data;
                    number++;
                    //log_info(" SC_AnswerReq SC_RESET_LOW Rece success card[%d] = %02x\r\n",i,card[i]);
                }
            }

            if(card[0])
            {
                (*SCState) = SC_ACTIVE;
            }
            else
            {
                (*SCState) = SC_POWER_OFF;
            }
            break;
        case SC_ACTIVE:
    //        log_info(" SC_ACTIVE\r\n");
            break;
        case SC_POWER_OFF:
    //        log_info(" SC_POWER_OFF\r\n");
            /* Close Connection if no answer received ------------------------------*/
            SC_Reset(Bit_SET); /* Reset high - a bit is used as level shifter from 3.3 to 5 V */
            break;
        default:
            (*SCState) = SC_RESET_LOW;
    }
    SC_USART->CTRL1 |= USART_WL_9B;

}


/**
  * @brief  Receives a new data while the time out not elapsed.
  * @param  None
  * @retval An ErrorStatus enumuration value:
  *          - SUCCESS: New data has been received
  *          - ERROR: time out was elapsed and no further data is received
  */
static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut)
{
    uint32_t Counter = 0;
    while((USART_Flag_Status_Get(SC_USART, USART_FLAG_RXDNE) == RESET) && (Counter != TimeOut))
    {
        Counter++;
        /* Overflow data loss */
        if(USART_Flag_Status_Get(SC_USART, USART_FLAG_OREF) != RESET)
        {
            SC_USART->DAT;
        }
    }
    if(Counter != TimeOut)
    {
        *Data = (uint8_t)USART_Data_Receive(SC_USART);
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

void SC_APDU_Init(SC_ADPU_Commands *SC_ADPU, uint8_t SC_APDU_CLA, uint8_t SC_APDU_INS, uint8_t SC_APDU_P1, uint8_t SC_APDU_P2,
                  uint8_t SC_APDU_LC, uint8_t *SC_APDU_Data, uint8_t SC_APDU_LE)
{
    SC_ADPU->Header.CLA  = SC_APDU_CLA;
    SC_ADPU->Header.INS  = SC_APDU_INS;
    SC_ADPU->Header.P1   = SC_APDU_P1;
    SC_ADPU->Header.P2   = SC_APDU_P2;
    SC_ADPU->Body.LC     = SC_APDU_LC;
    if(SC_ADPU->Body.LC)
    {
        memcpy(SC_ADPU->Body.Data, SC_APDU_Data, SC_ADPU->Body.LC);
    }
    if(SC_ADPU->Body.LC < LC_MAX)
    {
        memset(SC_ADPU->Body.Data + SC_ADPU->Body.LC, 0, LC_MAX - SC_ADPU->Body.LC);
    }
    SC_ADPU->Body.LE = SC_APDU_LE;
}

static void atr_TS(unsigned char ch)
{
    printf("TS: %02X\r\n", ch);
    SC_A2R.TS = ch;

    if(ch == 0x3B)
    {
        printf("\t LSB \r\n");
    }
    else if(ch == 0x3F)
    {
        printf("\t MSB\r\n");
    }
    else
    {
        printf("\tATR error\r\n");
    }
}
static void atr_T0(unsigned char ch)
{
    printf("T0: %02X\r\n", ch);
    SC_A2R.T0 = ch;
    
    if ((ch & TA_BIT) == TA_BIT)
    {
        printf("\tTA1\r\n");
    }
    if ((ch & TB_BIT) == TB_BIT)
    {
        printf("\tTB1\r\n");
    }
    if ((ch & TC_BIT) == TC_BIT)
    {
        printf("\tTC1 \r\n");
    }
    if ((ch & TD_BIT) == TD_BIT)
    {
        printf("\tTD1 \r\n");
    }
    printf("\t Historical char: %d\r\n", ch & 0x0f); 
}
static void atr_TA1(unsigned char ch)
{
    printf("TA1: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tFi: %d\r\n", (ch >> 4) & 0x0f);
    printf("\tDi: %d\r\n", (ch & 0x0f));
    printf("\tFi/Di: %f\r\n", (F_Table[(ch>>4)&0x0f]!=0 && D_Table[ch&0x0f]!=0) ? (float)F_Table[(ch>>4)&0x0f]/(float)D_Table[ch&0x0f] : 0);
}
static void atr_TB1(unsigned char ch)
{
    printf("TB1: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\t P: %d\r\n", ch & 0x1f);
    printf("\t I: %d\r\n", (ch >> 5) & 0x03);
}
static void atr_TC1(unsigned char ch)
{
    printf("TC1: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\t time: %d\r\n", ch);
}
static void atr_TD1(unsigned char ch)
{
    printf("TD1: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;    
    if ((ch & TA_BIT) == TA_BIT)
    {
        printf("\tTA2 \r\n");
    }
    if ((ch & TB_BIT) == TB_BIT)
    {
        printf("\tTB2 \r\n");
    }
    if ((ch & TC_BIT) == TC_BIT)
    {
        printf("\tTC2 \r\n");
    }
    if ((ch & TD_BIT) == TD_BIT)
    {
        printf("\tTD2 \r\n");
    }
    printf("\t after massage: %d\r\n", ch & 0x0f);
}
static void atr_TA2(unsigned char ch)
{
    printf("TA2: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\t change operation mode: %s\r\n",(ch & 0x80) ? "Yes(0)" : "No(1)");
    printf("\t mode is %s\r\n",(ch & 0x80) ? "consult(0)" : "fixed(1)");
    printf("\t after massage: %d\r\n", ch & 0x0f);
}
static void atr_TB2(unsigned char ch)
{
    printf("TB2: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tPI2: %d\r\n", ch);
}
static void atr_TC2(unsigned char ch)
{
    printf("TC2: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tT=0, WI: %d\r\n", ch);
}
static void atr_TD2(unsigned char ch)
{
    printf("TD2: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    
    if ((ch & TA_BIT) == TA_BIT)
    {
        printf("\tTA3 \r\n");
    }
    if ((ch & TB_BIT) == TB_BIT)
    {
        printf("\tTB3 \r\n");
    }
    if ((ch & TC_BIT) == TC_BIT)
    {
        printf("\tTC3 \r\n");
    }
    if ((ch & TD_BIT) == TD_BIT)
    {
        printf("\tTD3 \r\n");
    }
    printf("\t after massage: %d\r\n", ch & 0x0f);
}
static void atr_TA3(unsigned char ch)
{
    printf("TA3: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tT=1, IFSI: %d\r\n", ch);
}
static void atr_TB3(unsigned char ch)
{
    printf("TB3: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tT=1, CWI: %d\r\n", ch & 0x0f);
    printf("\tT=1, BWI: %d\r\n", (ch >> 4) & 0x0f);
}
static void atr_TC3(unsigned char ch)
{
    printf("TC3: %02X\r\n", ch);
    SC_A2R.T[T_lenght_count] = ch;
    T_lenght_count++;
    printf("\tT=1, block error Parity tpye: %d\r\n", ch & 0x01);
}
static void atr_history(unsigned char *ch, int len)
{
    int i;
    printf("TKi:");
    SC_A2R.Hlength = len;
    memcpy(SC_A2R.H, ch, len);
    for(i = 0; i < len; i++)
    {
        printf(" %02X", SC_A2R.H[i]);
    }
    
    printf("\r\n");
}
static void atr_TCK(unsigned char ch)
{
    printf("TCK: %02X\r\n", ch);
}

/**
  * @brief  Decodes the Answer to reset received from card.
  * @param  card: pointer to the buffer containing the card ATR.
  * @retval None
  */
static uint8_t SC_decode_Answer2reset(unsigned char *atr, int len)
{
    uint8_t data;
    uint8_t TCK = 0;
    uint8_t K = 0;
    uint8_t Yi = 0;
    uint32_t k, state, index, length, protocol;    
    uint8_t *ptr;
    uint8_t hist_bytes[16];
    
    T_lenght_count = 0;
    length = len;
    ptr = atr;
    state = STATE_PARSE_TS;
    index = 0;
    k = 0;
    protocol = 0;
    
    while( ptr < (atr + length) )
    {
        data = *ptr++;
        if ( state != STATE_PARSE_TS )
        {
            TCK ^= data ;
        }
        
        switch( state )
        {
            case STATE_PARSE_TS:
                atr_TS(data);
                state = STATE_PARSE_T0;
                break;
            case STATE_PARSE_T0:
                atr_T0(data);
                K = data & 0x0F;
                Yi = data;
                if ( data & 0x10 )
                {
                    state = STATE_PARSE_TA;
                }
                else if ( data & 0x20 )
                {
                    state = STATE_PARSE_TB;
                }
                else
                {
                    if ( data & 0x40 )
                    {
                        state = STATE_PARSE_TC;
                    }
                    else if ( data & 0x80 )
                    {
                        state = STATE_PARSE_TD;
                    }
                    else
                    {
                        state = STATE_PARSE_HIST_BYTES;
                    }
                }
                break;
            case STATE_PARSE_TA :
                switch( index )
                {
                    case 0:            /* TA1 */            
                        atr_TA1(data);
                        break;
                    case 1:                    
                        atr_TA2(data);
                        break;
                    case 2:                    
                        atr_TA3(data);
                        break;
                }
                if ( Yi & 0x20 )
                {
                    state = STATE_PARSE_TB;
                }
                else if ( Yi & 0x40 )
                {
                    state = STATE_PARSE_TC;
                }
                else if ( Yi & 0x80 )
                {
                    state = STATE_PARSE_TD;
                }
                else
                {
                    state = STATE_PARSE_HIST_BYTES;
                }
                break;
            case STATE_PARSE_TB :
                switch( index )
                {
                    case 0:            /* TB1 */
                        atr_TB1(data);
                        break ;
                    case 1:            /* TB2 */
                        atr_TB2(data);
                        break ;
                    case 2:            /* TB3 */
                        atr_TB3(data);
                        break;
                }
                 if ( Yi & 0x40 )
                 {
                    state = STATE_PARSE_TC;
                 }
                else if ( Yi & 0x80 )
                {
                    state = STATE_PARSE_TD;
                }
                else
                {
                    state = STATE_PARSE_HIST_BYTES;
                }
                break;
            case STATE_PARSE_TC :
                switch( index )
                {
                    case 0:            /* TC1 */
                        atr_TC1(data);
                        break;
                    case 1:            /* TC2 */
                        atr_TC2(data);
                        break ;
                    case 2:            /* TC3 */
                        atr_TC3(data);
                        break ;
                }
                 if ( Yi & 0x80 )
                 {
                    state = STATE_PARSE_TD;
                 }
                else
                {
                    state = STATE_PARSE_HIST_BYTES;
                }
                break ;
            case STATE_PARSE_TD :
                Yi = data ;
                switch( index++ )
                {
                    case 0:
                        protocol = Yi & 0x0F;        
                        atr_TD1(data);
                        break;
                    case 1:
                        atr_TD2(data);
                        break;
                }
    
                if ( Yi & 0xF0 )
                {
                    if ( Yi & 0x10 )
                    {
                        /* TAx character present */
                        state = STATE_PARSE_TA;
                    }
                    else if ( Yi & 0x20 )
                    {
                        /* TBx character present */
                        state = STATE_PARSE_TB;
                    }
                    else if ( Yi & 0x40 )
                    {
                        /* TCx character present */
                        state = STATE_PARSE_TC;
                    }
                    else if ( Yi & 0x80 )
                    {
                        /* TDx character present */
                        state = STATE_PARSE_TD;
                    }
                    else
                    {
                        state = STATE_PARSE_HIST_BYTES;
                    }
                }
                else
                {
                    state = STATE_PARSE_HIST_BYTES;
                }
                break ;
            case STATE_PARSE_HIST_BYTES:
                if( K )
                {
                    if( k < K )
                    {
                        hist_bytes[k++] = data;
                        if(k == K)
                        {
                            if(protocol > 0)
                                state = STATE_PARSE_TCK;
                            else
                                ptr = atr + length;
                            
                            atr_history(hist_bytes, k);
                        }    
                    }
                    break;    
                }
            case STATE_PARSE_TCK:
                atr_TCK(data);
                if ( !TCK )
                {
                }
                atr_TCK(TCK);
                SC_A2R.TCK = data;
                ptr = atr + length;
                break ;
        }
        if( state == STATE_PARSE_HIST_BYTES && K == 0 && protocol == 0)
            break;
    }
    SC_A2R.Tlength = T_lenght_count;

    return 0;
}
/**
  * @brief  Handles all Smartcard states and serves to send and receive all
  *   communication data between Smartcard and reader.
  * @param  SCState: pointer to an SC_State enumeration that will contain the Smartcard state.
  * @param  SC_ADPU: pointer to an SC_ADPU_Commands structure that will be initialized.
  * @param  SC_Response: pointer to a SC_ADPU_Responce structure which will be initialized.
  * @retval None
  */
void SC_Handler(SC_State *SCState, SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_Response, SC_InitStructure *SC_InitCfg)
{
    uint32_t i = 0;
//    log_info("*SCState = %d \r\n", *SCState);
    switch(*SCState)
    {
    case SC_POWER_ON:
        if (SC_ADPU->Header.INS == SC_GET_A2R)
        {
            if(F_HotReset == 0)
            {
                /* Smartcard intialization ------------------------------------------*/
                SC_Init(SC_InitCfg);
            }
            /* Reset Data from SC buffer -----------------------------------------*/
            for (i = 0; i < 40; i++)
            {
                SC_ATR_Table[i] = 0;
            }
            /* Reset SC_A2R Structure --------------------------------------------*/
            SC_A2R.TS = 0;
            SC_A2R.T0 = 0;
            for (i = 0; i < SETUP_LENGTH; i++)
            {
                SC_A2R.T[i] = 0;
            }
            for (i = 0; i < HIST_LENGTH; i++)
            {
                SC_A2R.H[i] = 0;
            }
            SC_A2R.Tlength = 0;
            SC_A2R.Hlength = 0;
            /* Next State --------------------------------------------------------*/
            *SCState = SC_RESET_LOW;
        }
        break;
    case SC_RESET_LOW:
        if(SC_ADPU->Header.INS == SC_GET_A2R)
        {
            /* If card is detected then Power ON, Card Reset and wait for an answer) */
            if (1)
            {
                while(((*SCState) != SC_POWER_OFF) && ((*SCState) != SC_ACTIVE))
                {
                    SC_AnswerReq(SCState, SC_ATR_Table, 40); /* Check for answer to reset */
                    log_info("SC_ATR_Table:");
                    for(i = 0; i < 32; i++)
                    {
                        log_info(" %x \r", SC_ATR_Table[i]);
                    }
                }
            }
            else
            {
                (*SCState) = SC_POWER_OFF;
            }
        }
        printf("\n");
        break;
    case SC_ACTIVE:
        if (SC_ADPU->Header.INS == SC_GET_A2R)
        {
            SC_decode_Answer2reset(SC_ATR_Table, 40);
            (*SCState) = SC_ACTIVE_ON_T0;
        }
        break;
    case SC_ACTIVE_ON_T0:
        SC_SendData(SC_ADPU, SC_Response);
        break;
    case SC_POWER_OFF:
        SC_DeInit(); /* Disable Smartcard interface */
        break;
    default:
        (*SCState) = SC_POWER_OFF;
    }
}
