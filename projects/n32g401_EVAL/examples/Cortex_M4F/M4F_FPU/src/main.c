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
#include "string.h"
#include "julia_fpu.h"

/**
**  Cortex-M4F FPU
**/


#define SCREEN_X_SIZE    ((uint16_t)90)
#define SCREEN_Y_SIZE    ((uint8_t)60)
#define PBAR_X_POS       ((uint16_t)0x100)
#define PBAR_Y_POS_H     ((uint8_t)0)
#define PBAR_Y_POS_L     ((uint8_t)227)
#define PBAR_COLOR       ((uint16_t)0xf79e)
#define ANIMATION_LENGHT ((uint32_t)26)

#define SUBMODE_FPU_USED_MODE     ((uint8_t)0)
#define SUBMODE_FPU_NOT_USED_MODE ((uint8_t)1)

#if (__FPU_USED == 1)
#define SCORE_FPU_MODE    "FPU On"
#define PROGRESS_FPU_MODE "FPU ON"
#else
#define SCORE_FPU_MODE    "FPU Off"
#define PROGRESS_FPU_MODE "FPU OFF"
#endif /* __FPU_USED */

const int16_t animation_zoom[ANIMATION_LENGHT] = {120, 110,  100,  150,  200,  275,  350,  450,  600,
                                                  800, 1000, 1200, 1500, 2000, 1500, 1200, 1000, 800,
                                                  600, 450,  350,  275,  200,  150,  100,  110};
TIM_TimeBaseInitType TIM_TimeBaseStructure;
uint8_t buffer[SCREEN_X_SIZE * SCREEN_Y_SIZE];
uint8_t text[50];
__IO FlagStatus key_pressed;
__IO FlagStatus switch_mode;

                                                  
/**
*\*\name   main
*\*\fun    Main program.
*\*\return none
**/  
int main(void)
{
    /* Animation pointer */
    uint8_t animation_pointer;
    /* Benchmark result */
    uint32_t score_fpu;
    
    /* USART Init */
    log_init();
    
    printf("Cortex-M4F FPU \r\n");
    
    /* Initialise global variables */
    switch_mode       = RESET;
    animation_pointer = 0;
    /* Initialise local variables */
    score_fpu = 0;

    /* Clear Screen */
    memset(buffer, 0x00, sizeof(buffer));

    InitTIMER();

    /* Start generating the fractal */
    while (1)
    {
        /* Start the timer */
        StartTIMER();

        /* Make the calculation */
        GenerateJulia_fpu(SCREEN_X_SIZE,
                          SCREEN_Y_SIZE,
                          SCREEN_X_SIZE / 2,
                          SCREEN_Y_SIZE / 2,
                          animation_zoom[animation_pointer],
                          buffer);

        /* Get elapsed time */
        score_fpu = StopTIMER();

        printf("%s : %dms\r\n", SCORE_FPU_MODE, score_fpu/2);
        break;
    }
    while(1);
  
}


void InitTIMER(void)
{
    RCC_ClocksType RCC_Clocks;

    /* TIM1 clock enable */
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_TIM1);

    RCC_Clocks_Frequencies_Value_Get(&RCC_Clocks);

    /* DeInit TIM1 */
    TIM_Reset(TIM1);

    /* Time Base configuration */
    TIM_TimeBaseStructure.Prescaler = ((RCC_Clocks.Pclk2Freq / 1000));
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.RepetCnt  = 0;

    TIM_Base_Initialize(TIM1, &TIM_TimeBaseStructure);
}


void StartTIMER(void)
{
    /* Disable TIM1 counter */
    TIM_Off(TIM1);

    /* Initialize counter */
    TIM1->CNT = 0x0000;

    /* TIM1 counter enable */
    TIM_On(TIM1);
}

uint32_t StopTIMER(void)
{
    /* TIM1 counter enable */
    TIM_Off(TIM1);

    /* Return counter value */
    return (uint32_t)TIM1->CNT;
}
