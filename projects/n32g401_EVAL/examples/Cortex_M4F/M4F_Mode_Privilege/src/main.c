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

/**
**  Cortex-M4F Mode Privilege
**/

#define SP_PROCESS_SIZE          0x200 /* Process stack size */
#define SP_PROCESS               0x02  /* Process stack */
#define SP_MAIN                  0x00  /* Main stack */
#define THREAD_MODE_PRIVILEGED   0x00  /* Thread mode has privileged access */
#define THREAD_MODE_UNPRIVILEGED 0x01  /* Thread mode has unprivileged access */

/* clang-format off */
#if defined ( __CC_ARM   )
  __ASM void __SVC(void) 
  { 
    SVC 0x01 
    BX R14
  }
#elif defined ( __ICCARM__ )
  static __INLINE  void __SVC()                     { __ASM ("svc 0x01");}
#elif defined   (  __GNUC__  )
  static __INLINE void __SVC()                      { __ASM volatile ("svc 0x01");}
#endif
/* clang-format on */

__IO uint8_t PSPMemAlloc[SP_PROCESS_SIZE];
__IO uint32_t Index = 0, PSPValue = 0, CurrentStack = 0, ThreadMode = 0;

  
/**
*\*\name   main
*\*\fun    Main program.
*\*\return none
**/
int main(void)
{    
    /* USART Init */
    log_init();
    
    printf("Cortex-M4F Mode Privilege \r\n");
    
    /* Switch Thread mode Stack from Main to Process */
    /* Initialize memory reserved for Process Stack */
    for (Index = 0; Index < SP_PROCESS_SIZE; Index++)
    {
        PSPMemAlloc[Index] = 0x00;
    }

    /* Set Process stack value */
    __set_PSP((uint32_t)PSPMemAlloc + SP_PROCESS_SIZE);

    /* Select Process Stack as Thread mode Stack */
    __set_CONTROL(SP_PROCESS);

    /* Get the Thread mode stack used */
    if ((__get_CONTROL() & 0x02) == SP_MAIN)
    {
        /* Main stack is used as the current stack */
        CurrentStack = SP_MAIN;
    }
    else
    {
        /* Process stack is used as the current stack */
        CurrentStack = SP_PROCESS;

        /* Get process stack pointer value */
        PSPValue = __get_PSP();
    }

    /* Switch Thread mode from privileged to unprivileged */
    /* Thread mode has unprivileged access */
    __set_CONTROL(THREAD_MODE_UNPRIVILEGED | SP_PROCESS);

    /* Unprivileged access mainly affect ability to:
        - Use or not use certain instructions such as MSR fields
        - Access System Control Space (SCS) registers such as NVIC and SysTick */

    /* Check Thread mode privilege status */
    if ((__get_CONTROL() & 0x01) == THREAD_MODE_PRIVILEGED)
    {
        /* Thread mode has privileged access  */
        ThreadMode = THREAD_MODE_PRIVILEGED;
    }
    else
    {
        /* Thread mode has unprivileged access*/
        ThreadMode = THREAD_MODE_UNPRIVILEGED;
    }

    /* Switch back Thread mode from unprivileged to privileged */
    /* Try to switch back Thread mode to privileged (Not possible, this can be
       done only in Handler mode) */
    __set_CONTROL(THREAD_MODE_PRIVILEGED | SP_PROCESS);

    /* Generate a system call exception, and in the INTSTS switch back Thread mode to privileged */
    __SVC();

    /* Check Thread mode privilege status */
    if ((__get_CONTROL() & 0x01) == THREAD_MODE_PRIVILEGED)
    {
        /* Thread mode has privileged access */
        ThreadMode = THREAD_MODE_PRIVILEGED;
    }
    else
    {
        /* Thread mode has unprivileged access */
        ThreadMode = THREAD_MODE_UNPRIVILEGED;
    }

    while (1)
    {
    }
  
}

