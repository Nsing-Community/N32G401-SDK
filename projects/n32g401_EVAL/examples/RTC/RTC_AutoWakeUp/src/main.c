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

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "log.h"
#include "n32g401_rtc.h"
#include "n32g401_exti.h"
#include "delay.h"

#define    USER_WRITE_BKP_DAT1_DATA       0xA5A5
#define    RTC_CLOCK_TRY_COUNT            250

#define     RTC_CLK_HSE128      1
#define     RTC_CLK_LSE         2
#define     RTC_CLK_LSI         3

#define     RTC_CLK_FIRST_CONFIG    0
#define     RTC_CLK_LAST_CONFIG     1


RTC_DateType RTC_DateStructure;
RTC_DateType RTC_DateDefault;
RTC_TimeType RTC_TimeStructure;
RTC_TimeType RTC_TimeDefault;
RTC_InitType RTC_InitStructure;
RTC_AlarmType RTC_AlarmStructure;
RTC_AlarmType RTC_AlarmDefault;
uint32_t SynchPrediv, AsynchPrediv;

ErrorStatus RTC_CLKSource_Config(uint8_t ClkSrc, uint8_t FirstLastCfg);
ErrorStatus RTC_Date_Regulate(RTC_DateType* RTC_DateStruct);
ErrorStatus RTC_Time_Regulate(RTC_TimeType* RTC_TimeStruct);
ErrorStatus RTC_Calendar_Config(FunctionalState delay_cmd);
ErrorStatus RTC_Alarm_Regulate(uint32_t RTC_Alarm);
void RTC_Date_And_Time_Default_Value(void);
static void RTC_Prescaler_Config(RTC_InitType *RTC_InitStructure);

void EXTI19_RTCWKUP_Configuration(void);
void WakeUp_Clock_Select(uint8_t WKUPClkSrcSel);



/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{
    uint8_t WKUPOutIrq   = 2;
    uint8_t AlarmOutType = 2;

    /* Initialize USART,TX: PA9 */
    log_init();
    log_info("RTC Init");
    /* RTC date time default value */
    RTC_Date_And_Time_Default_Value();
    
     /* Enable the PWR clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_PWR);
    
    /* Allow access to RTC */
    PWR_RTC_Backup_Access_Enable();
    
    /* RTC clock source select */
    if(SUCCESS == RTC_CLKSource_Config(RTC_CLK_LSE, RTC_CLK_FIRST_CONFIG))
    {
        if (USER_WRITE_BKP_DAT1_DATA != RTC_Backup_Register_Read(RTC_BACKUP_REGISTER_1) )
        {
            /* RTC calendar regulate */
            RTC_Calendar_Config(DISABLE);

            /* wake up clock select */
            WakeUp_Clock_Select(5);
            /* wake up timer value */
            RTC_WakeUp_Counter_Set(4);

            /* Write backup register */
            RTC_Backup_Register_Write(RTC_BACKUP_REGISTER_1, USER_WRITE_BKP_DAT1_DATA);

            log_info("\r\n RTC Init Success\r\n");
        }
    }
    else
    {
        log_info("\r\n RTC Init Faile\r\n");
    }

    if (WKUPOutIrq == 0x02) // 1.WakeUp interrupt test
    {
        EXTI19_RTCWKUP_Configuration();
        /* Enable the RTC Wakeup Interrupt */
        RTC_Interrupts_Enable(RTC_INT_WUT);
        RTC_WakeUp_Enable();
        while (1)
            ;
    }
    else if (WKUPOutIrq == 0x01) // 2.WakeUp output test
    {
        /* Set the output type (open-drain output + push-pull output).
        When setting OD, external  should be added pull-up resistor to PC13
        */
        if (AlarmOutType == 0x01)
        {
            RTC_Output_Mode_Config(RTC_OUTPUT_OPENDRAIN);
        }
        else if (AlarmOutType == 0x02)
        {
            RTC_Output_Mode_Config(RTC_OUTPUT_PUSHPULL);
        }
        else
        {
            log_info("\r\n The AlarmOutType value is error! \r\n");
        }

        RTC_Output_Config(RTC_OUTPUT_WKUP, RTC_OUTPOL_LOW);

        RTC_WakeUp_Enable();
        while (1)
        {
            if (RTC_Flag_Status_Get(RTC_FLAG_WTF) == SET)
            {
                RTC_Flag_Clear(RTC_FLAG_WTF);
            }
        }
    }
}

/**
*\*\name    WakeUp_Clock_Select
*\*\fun     Wake up clock config.
*\*\param   WKUPClkSrcSel
*\*\                - 1     RTC_WKUPCLK_RTCCLK_DIV16
*\*\                - 2     RTC_WKUPCLK_RTCCLK_DIV8
*\*\                - 3     RTC_WKUPCLK_RTCCLK_DIV4
*\*\                - 4     RTC_WKUPCLK_RTCCLK_DIV2
*\*\                - 5     RTC_WKUPCLK_CK_SPRE_16BITS
*\*\                - 6     RTC_WKUPCLK_CK_SPRE_17BITS
*\*\return  none
**/
void WakeUp_Clock_Select(uint8_t WKUPClkSrcSel)
{
    /* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
    if (WKUPClkSrcSel == 0x01)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_RTCCLK_DIV16);
    else if (WKUPClkSrcSel == 0x02)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_RTCCLK_DIV8);
    else if (WKUPClkSrcSel == 0x03)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_RTCCLK_DIV4);
    else if (WKUPClkSrcSel == 0x04)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_RTCCLK_DIV2);
    else if (WKUPClkSrcSel == 0x05)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_CK_SPRE_16BITS);
    else if (WKUPClkSrcSel == 0x06)
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_CK_SPRE_17BITS);
    else
        RTC_WakeUp_Clock_Select(RTC_WKUPCLK_CK_SPRE_16BITS);
}

/**
*\*\name    EXTI19_RTCWKUP_Configuration
*\*\fun     Config RTC wake up Interrupt.
*\*\param   none
*\*\return  none
**/
void EXTI19_RTCWKUP_Configuration(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Configure Button EXTI line */
    EXTI_Interrupt_Status_Clear(EXTI_LINE19);
	EXTI_Structure_Initializes(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE19;
#ifdef __TEST_SEVONPEND_WFE_NVIC_DIS__
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
#else
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
#endif
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

/**
*\*\name    RTC_Date_Show.
*\*\fun     Display the current Date on the Hyperterminal.
*\*\param   none
*\*\return  none
**/
void RTC_Date_Show(void)
{
    /* Get the current Date */
    RTC_Date_Get(RTC_FORMAT_BIN, &RTC_DateStructure);
    log_info("\n\r //=========== Current Date Display ==============// \n\r");
    log_info("\n\r The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r",
             RTC_DateStructure.WeekDay,
             RTC_DateStructure.Date,
             RTC_DateStructure.Month,
             RTC_DateStructure.Year);
}

/**
*\*\name    RTC_Time_Show.
*\*\fun     Display the current time on the Hyperterminal.
*\*\param   none
*\*\return  none
**/
void RTC_Time_Show(void)
{
    /* Get the current Time and Date */
    RTC_Time_Get(RTC_FORMAT_BIN, &RTC_TimeStructure);
    log_info("\n\r //============ Current Time Display ===============// \n\r");
    log_info("\n\r The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r",
             RTC_TimeStructure.Hours,
             RTC_TimeStructure.Minutes,
             RTC_TimeStructure.Seconds);
    /* Unfreeze the RTC DAT Register */
    (void)RTC->DATE;
}

/**
*\*\name    RTC_Date_And_Time_Default_Value.
*\*\fun     RTC initalize default value.
*\*\param   none
*\*\return  none
**/
void RTC_Date_And_Time_Default_Value(void)
{ // Date
    RTC_DateDefault.WeekDay = 3;
    RTC_DateDefault.Date    = 20;
    RTC_DateDefault.Month   = 11;
    RTC_DateDefault.Year    = 19;
    // Time
    RTC_TimeDefault.H12     = RTC_AM_H12;
    RTC_TimeDefault.Hours   = 4;
    RTC_TimeDefault.Minutes = 5;
    RTC_TimeDefault.Seconds = 0;
}

/**
*\*\name    RTC_Calendar_Config.
*\*\fun     RTC date regulate with the default value.
*\*\param   delay_cmd
*\*\            - ENABLE
*\*\            - DISABLE
*\*\return  ERROR or SUCCESS
**/
ErrorStatus RTC_Calendar_Config(FunctionalState delay_cmd)
{
    /* RTC prescaler regulate */
    RTC_Prescaler_Config(&RTC_InitStructure);
    /* RTC date time and alarm regulate */
    RTC_Date_Regulate(&RTC_DateStructure);
    RTC_Time_Regulate(&RTC_TimeStructure);

    /* Initializes RTC calendar */
    if (RTC_Calendar_Initializes(RTC_FORMAT_SELECT, &RTC_InitStructure, &RTC_DateStructure, &RTC_TimeStructure, delay_cmd) == ERROR)
    {
        log_info("\n\r>> !! RTC Set Calendar failed. !! <<\n\r");
        return ERROR;
    }
    else
    {
        log_info("\n\r>> !! RTC Set Calendar success. !! <<\n\r");
        RTC_Date_Show();
        RTC_Time_Show();
        return SUCCESS;
    }
}

/**
*\*\name    RTC_Date_Regulate.
*\*\fun     RTC date regulate with the default value.
*\*\param   RTC_DateStruct : RTC_DateStruct pointer to a RTC_DateType structure that contains
*\*\                       the date configuration information for the RTC.
*\*\            - WeekDay 
*\*\                - RTC_WEEKDAY_MONDAY
*\*\                - RTC_WEEKDAY_TUESDAY
*\*\                - RTC_WEEKDAY_WEDNESDAY
*\*\                - RTC_WEEKDAY_THURSDAY
*\*\                - RTC_WEEKDAY_FRIDAY
*\*\                - RTC_WEEKDAY_SATURDAY
*\*\                - RTC_WEEKDAY_SUNDAY
*\*\            - Month 
*\*\                - RTC_MONTH_JANUARY
*\*\                - RTC_MONTH_FEBRURY
*\*\                - RTC_MONTH_MARCH
*\*\                - RTC_MONTH_APRIL
*\*\                - RTC_MONTH_MAY
*\*\                - RTC_MONTH_JUNE
*\*\                - RTC_MONTH_JULY
*\*\                - RTC_MONTH_AUGUST
*\*\                - RTC_MONTH_SEPTEMBER
*\*\                - RTC_MONTH_OCTOBER
*\*\                - RTC_MONTH_NOVEMBER
*\*\                - RTC_MONTH_DECEMBER
*\*\            - Date    the value in the 1-31 range 
*\*\            - Year    the value in the 0-99 range
*\*\return  ERROR or SUCCESS
**/
ErrorStatus RTC_Date_Regulate(RTC_DateType* RTC_DateStruct)
{
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
    log_info("\n\r //=============Date Settings================// \n\r");

    log_info("\n\r Please Set WeekDay (01-07) \n\r");
    tmp_hh = RTC_DateDefault.WeekDay;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStruct->WeekDay = tmp_hh;
    }
    log_info(": %0.2d\n\r", tmp_hh);

    tmp_hh = 0xFF;
    log_info("\n\r Please Set Date (01-31) \n\r");
    tmp_hh = RTC_DateDefault.Date;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStruct->Date = tmp_hh;
    }
    log_info(": %0.2d\n\r", tmp_hh);

    log_info("\n\r Please Set Month (01-12)\n\r");
    tmp_mm = RTC_DateDefault.Month;
    if (tmp_mm == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStruct->Month = tmp_mm;
    }
    log_info(": %0.2d\n\r", tmp_mm);

    log_info("\n\r Please Set Year (00-99)\n\r");
    tmp_ss = RTC_DateDefault.Year;
    if (tmp_ss == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStruct->Year = tmp_ss;
    }
    log_info(": %0.2d\n\r", tmp_ss);
    
    return SUCCESS;
}

/**
*\*\name    RTC_Time_Regulate.
*\*\fun     RTC time regulate with the default value.
*\*\param   RTC_TimeStruct : RTC_TimeStruct pointer to a RTC_TimeType structure that contains
*\*\        the time configuration information for the RTC.When the user does not need to configure this 
*\*\        parameter, note that NULL can be passed in.
*\*\            - Hours
*\*\                - if RTC_AM_H12 is select the value in the 0-23 range
*\*\                - if RTC_PM_H12 is select the value in the 1-12 range
*\*\            - Minutes  the value set in the 0-59 range
*\*\            - Seconds  the value set in the 0-59 range
*\*\            - H12
*\*\                - RTC_AM_H12
*\*\                - RTC_PM_H12
*\*\return  ERROR or SUCCESS
**/
ErrorStatus RTC_Time_Regulate(RTC_TimeType* RTC_TimeStruct)
{
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
    log_info("\n\r //==============Time Settings=================// \n\r");

    RTC_TimeStructure.H12 = RTC_TimeDefault.H12;

    log_info("\n\r Please Set Hours \n\r");
    tmp_hh = RTC_TimeDefault.Hours;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStruct->Hours = tmp_hh;
    }
    log_info(": %0.2d\n\r", tmp_hh);

    log_info("\n\r Please Set Minutes \n\r");
    tmp_mm = RTC_TimeDefault.Minutes;
    if (tmp_mm == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStruct->Minutes = tmp_mm;
    }
    log_info(": %0.2d\n\r", tmp_mm);

    log_info("\n\r Please Set Seconds \n\r");
    tmp_ss = RTC_TimeDefault.Seconds;
    if (tmp_ss == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStruct->Seconds = tmp_ss;
    }
    log_info(": %0.2d\n\r", tmp_ss);
    
    return SUCCESS;
}


/**
*\*\name    RTC_Prescaler_Config.
*\*\fun     RTC prescaler config.
*\*\param   RTC_InitStruct: pointer to a RTC_InitType structure. 
*\*\            - RTC_HourFormat
*\*\                - RTC_24HOUR_FORMAT
*\*\                - RTC_12HOUR_FORMAT
*\*\            - RTC_AsynchPrediv  the value in the 0-0x7F range
*\*\            - RTC_SynchPrediv   the value in the 0-0x7FFF range
*\*\return  none
**/
static void RTC_Prescaler_Config(RTC_InitType *RTC_InitStruct)
{
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStruct->RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStruct->RTC_SynchPrediv  = SynchPrediv;
    RTC_InitStruct->RTC_HourFormat   = RTC_24HOUR_FORMAT;
}

/**
*\*\name    RTC_CLKSource_Config.
*\*\fun     Configure the RTC peripheral by selecting the clock source.
*\*\param   ClkSrc    
*\*\            - RTC_CLK_HSE128    clock source select HSE/128
*\*\            - RTC_CLK_LSE       clock source select LSE
*\*\            - RTC_CLK_LSI       clock source select LSI
*\*\param   FirstLastCfg
*\*\            - RTC_CLK_FIRST_CONFIG
*\*\            - RTC_CLK_LAST_CONFIG
*\*\return  SUCCESS or ERROR
**/
ErrorStatus RTC_CLKSource_Config(uint8_t ClkSrc, uint8_t FirstLastCfg)
{
    ErrorStatus status=SUCCESS;
    uint16_t ready_count=0;
    /* Enable the PWR clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_PWR);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    
    RCC_Backup_Reset();
    
    /* Disable RTC clock */
    RCC_RTC_Clock_Disable();

    if (ClkSrc == RTC_CLK_HSE128)
    {
        log_info("\r\n RTC_ClkSrc Is Set HSE128! \r\n");
        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable HSE */
            RCC_LSI_Disable();
            RCC_HSE_Config(RCC_HSE_ENABLE);
            if (RCC_HSE_Stable_Wait() == ERROR)
            {
                status = ERROR;
                log_info("\r\n RTC_ClkSrc Set HSE/128 Faile! \r\n");
            }
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);
        }
        else
        {
            RCC_LSI_Disable();
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);

            /* Enable HSE */
            RCC_HSE_Config(RCC_HSE_ENABLE);

            if (RCC_HSE_Stable_Wait() == ERROR)
            {
                status = ERROR;
                log_info("\r\n RTC_ClkSrc Set HSE/128 Faile! \r\n");
            }
        }
        SynchPrediv  = 0x1E8; // 8M/128 = 62.5KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else if (ClkSrc == RTC_CLK_LSE)
    {
        log_info("\r\n RTC_ClkSrc Is Set LSE! \r\n");

        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_LSI_Disable(); // LSI is turned off here to ensure that only one clock is turned on

#if (_LSE_BYPASS_)
            RCC_LSE_Config(RCC_LSE_BYPASS, 0x1D7);
#else
            RCC_LSE_Config(RCC_LSE_ENABLE, 0x1D7);
#endif
            while (RCC_Flag_Status_Get(RCC_FLAG_LSERD) == RESET)
            {
                ready_count++;
                systick_delay_ms(10);
                if(ready_count >= RTC_CLOCK_TRY_COUNT)
                {
                    status = ERROR;
                    log_info("\r\n RTC_ClkSrc Set LSE Faile! \r\n");
                    break;
                }
            }
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSE);
        }
        else
        {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_LSI_Disable();
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSE);

#if (_LSE_BYPASS_)
            RCC_LSE_Config(RCC_LSE_BYPASS, 0x1D7);
#else
            RCC_LSE_Config(RCC_LSE_ENABLE, 0x1D7);
#endif

            while (RCC_Flag_Status_Get(RCC_FLAG_LSERD) == RESET)
            {
                ready_count++;
                systick_delay_ms(10);
                if(ready_count >= RTC_CLOCK_TRY_COUNT)
                {
                    status = ERROR;
                    log_info("\r\n RTC_ClkSrc Set LSE Faile! \r\n");
                    break;
                }
            }
        }
        SynchPrediv  = 0xFF; // 32.768KHz
        AsynchPrediv = 0x7F; // value range: 0-7F
    }
    else if (ClkSrc == RTC_CLK_LSI)
    {
        log_info("\r\n RTC_ClkSrc Is Set LSI! \r\n");
        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable the LSI OSC */
            RCC_LSI_Enable();

            while (RCC_Flag_Status_Get(RCC_FLAG_LSIRD) == RESET)
            {
                ready_count++;
                systick_delay_ms(10);
                if(ready_count >= RTC_CLOCK_TRY_COUNT)
                {
                    status = ERROR;
                    log_info("\r\n RTC_ClkSrc Set LSI Faile! \r\n");
                    break;
                }
            }
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSI);
        }
        else
        {
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_LSI);

            /* Enable the LSI OSC */
            RCC_LSI_Enable();

            while (RCC_Flag_Status_Get(RCC_FLAG_LSIRD) == RESET)
            {
                ready_count++;
                systick_delay_ms(10);
                if(ready_count >= RTC_CLOCK_TRY_COUNT)
                {
                    status = ERROR;
                    log_info("\r\n RTC_ClkSrc Set LSE Faile! \r\n");
                    break;
                }
            }
        }
        SynchPrediv  = 0x13B; // 39.64928KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else
    {
        log_info("\r\n RTC_ClkSrc Value is error! \r\n");
    }

    /* Enable the RTC Clock */
    RCC_RTC_Clock_Enable();
    RTC_Wait_For_Synchronization();
	/* Unfreeze the RTC DAT register */
	(void)RTC->DATE;
    
    return status;
}
