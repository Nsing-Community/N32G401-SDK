1、功能说明

    /* 简单描述工程功能 */
        这个例程配置并演示直接对FLASH进行读写操作


2、使用环境

        软件开发环境：KEIL MDK-ARM V5.34.0.0
                      IAR EWARM 8.50.1
    
        硬件环境：基于评估板N32G401C8L7-STB V1.0开发


3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
        SystemClock：72MHz

        USART：TX - PA9，RX - PA10，波特率115200

    /* 描述Demo的测试步骤和现象 */
        1.编译后下载程序复位运行；
        2.程序执行读保护使能，程序复位，flash进入读保护L1状态，通过打印信息观察到上述现象则测试通过；

4、注意事项
    无

1. Function description
    /* A brief description of the engineering function */
    This routine configures and demonstrates reading and writing directly to FLASH

2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
        
3. How to use  
    /* Describe the related module configuration method; For example: clock, I/O, etc. */
    SystemClock: 72MHZ
    USART: TX-PA9, RX-PA10, baud rate 115200
    
    /* Describes the test steps and symptoms of Demo */
	1. Reset and run the downloaded program after compilation;
	2. Program execution read protection enable, program reset, flash into the read protection L1 state, through the print information to observe the above phenomenon then the test passed.

4. Attention
    None