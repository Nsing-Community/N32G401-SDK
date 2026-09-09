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
 * @file julia_fpu.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "julia_fpu.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/**
 * @brief  Julia Calculation with FPU option disable
 * @param size_x screen width in pixel
 * @param size_y screen height in pixel
 * @param offset_x x offset for starting point
 * @param offset_y y offset for starting point
 * @param zoom Iteration zoom ratio
 * @param buffer pointer to output buffer where values are stored
 */
void GenerateJulia_fpu(uint16_t size_x,
                       uint16_t size_y,
                       uint16_t offset_x,
                       uint16_t offset_y,
                       uint16_t zoom,
                       uint8_t* buffer)
{
    float tmp1, tmp2;
    float num_real, num_img;
    float rayon;

    uint8_t i;
    uint16_t x, y;

    for (y = 0; y < size_y; y++)
    {
        for (x = 0; x < size_x; x++)
        {
            num_real = y - offset_y;
            num_real = num_real / zoom;
            num_img  = x - offset_x;
            num_img  = num_img / zoom;
            i        = 0;
            rayon    = 0;
            while ((i < ITERATION - 1) && (rayon < 4))
            {
                tmp1     = num_real * num_real;
                tmp2     = num_img * num_img;
                num_img  = 2 * num_real * num_img + IMG_CONSTANT;
                num_real = tmp1 - tmp2 + REAL_CONSTANT;
                rayon    = tmp1 + tmp2;
                i++;
            }
            /* Store the value in the buffer */
            buffer[x + y * size_x] = i;
        }
    }
}

/**
 * @}
 */
/**
 * @}
 */
