1、功能说明

    /* 简单描述工程功能 */
        这个例程配置并演示MCU跑分测试


2、使用环境

        软件开发环境：KEIL MDK-ARM V5.34.0.0
                      IAR EWARM 8.50.1
    
        硬件环境：基于评估板N32G401C8L7-STB V1.0开发


3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
        SystemClock：72MHz

        USART：TX - PA9，RX - PA10，波特率115200
        SysTick：时钟滴答配置为1MS

    /* 描述Demo的测试步骤和现象 */
        1.编译后下载程序复位运行；
        2.通过串口打印出跑分结果；


4、注意事项
    例程中的coremask运行时间大约需要20秒。你可以通过改变ITERATIONS宏的值来改变运行时间。运行时间不应少于10s。

1. Function description
    /* A brief description of the engineering function */
    This routine configures and demonstrates MCU running tests

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
        
3. How to use   
    /* Describe the related module configuration method; For example: clock, I/O, etc. */
    USART: TX-PA9, TX-PA10, baud rate 115200
    SysTick: clock tick is set to 1MS
    /* Describes the test steps and symptoms of Demo */
    1. Reset and run the downloaded program after compilation;
    2. Print the running result through the serial port;

4. Attention
    The coremask running time in the demo takes about 20s. You can change the running time by changing the value of the ITERATIONS macro. The running time should not be less than 10s.