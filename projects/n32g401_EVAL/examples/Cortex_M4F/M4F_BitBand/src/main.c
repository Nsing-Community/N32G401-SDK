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


#include "main.h"
#include "log.h"
#include <stdio.h>

/* RAM address */
#define RAM_BASE    ((uint32_t)0x20000000)
/* RAM bitband address */
#define RAM_BB_BASE ((uint32_t)0x22000000)

__IO uint32_t Data = 0, DataAddr = 0, DataBitValue = 0;

#define Data_ResetBit_BB(DataAddr, BitNumber)                                                                          \
    (*(__IO uint32_t*)(RAM_BB_BASE | ((DataAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)

#define Data_SetBit_BB(DataAddr, BitNumber)                                                                            \
    (*(__IO uint32_t*)(RAM_BB_BASE | ((DataAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define Data_GetBit_BB(DataAddr, BitNumber) (Data >> BitNumber)

int main(void)
{
    uint8_t i;
    
    /* USART Init */
    log_init();
    
    printf("Cortex-M4F BitBand \r\n");
    
    /* A mapping formula shows how to reference each word in the alias region to a
       corresponding bit in the bit-band region. The mapping formula is:
       bit_byte_addr = bit_band_base + (byte_offset x 32) + (bit_number x 4)

       Parameter Description:
         bit_byte_addr: The address of the byte in the alias memory region that maps to the targeted bit.
         bit_band_base: The starting address of the alias region
         byte_offset:   The number of the byte in the bit-band region that contains the targeted bit
         bit_number:    The bit position (0-7) of the targeted bit */

    /* Get the variable address */
    DataAddr = (uint32_t)&Data;

    /* Modify variable bit using bit-band access */

    for (i = 0; i < 8; i++)
    {
        /* Modify Data variable bit i */
        Data_SetBit_BB(DataAddr, i); /* Data = (0x00000001 << i) */
        printf("Set bit %d of variable Data to 1  = 0x%02X \r\n", i, Data);

        Data_ResetBit_BB(DataAddr, i); /* Data = 0x00000000 */
        printf("Set bit %d of variable Data to 0  = 0x%02X \r\n", i, Data);
    }
    while (1)
    {
    }
  
}



