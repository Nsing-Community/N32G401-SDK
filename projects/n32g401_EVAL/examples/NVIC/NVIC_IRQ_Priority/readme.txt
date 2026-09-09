1、功能说明

    /* 简单描述工程功能 */
        这个例程配置并演示NVIC优先级设置


2、使用环境

        软件开发环境：KEIL MDK-ARM V5.34.0.0
                      IAR EWARM 8.50.1
    
        硬件环境：基于评估板N32G401C8L7-STB V1.0开发


3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
        SystemClock：72MHz

        USART: TX-PA9, RX-PA10，波特率115200
        EXTI：PA0为浮空输入模式，外部中断线 - EXTI_LINE0，开启外部中断，优先级为1
        SysTick：中断优先级设置为0

    /* 描述Demo的测试步骤和现象 */
        1.编译后下载程序复位运行；
        2.第一次按键可以看到SysTick中断可以抢占EXTI中断，然后将SysTick中断优先级改为2，再次按键SysTick中断不可以抢占EXTI中断；


4、注意事项


1. Function description
    /* A brief description of the engineering function */
    This routine configures and demonstrates NVIC priority Settings

2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
        
3. How to use    
    /* Describe the related module configuration method; For example: clock, I/O, etc. */
    SystemClock: 72MHZ
    USART: TX-PA9, RX-PA10, baud rate 115200
    EXTI: PA0 is floating input mode, external interrupt line -exti_line0, external interrupt is enabled, and the priority is 1
    SysTick: Set the interrupt priority to 0
    
    /* Describes the test steps and symptoms of Demo */
    1. Reset and run the downloaded program after compilation;
    2. The first time you press the key, you can see that the SysTick interrupt can preempt the EXTI interrupt, then change the SysTick interrupt priority to 2, and press the key again the SysTick interrupt cannot preempt the EXTI interrupt.

4. Attention
    None