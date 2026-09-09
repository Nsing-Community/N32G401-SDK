1. 功能描述

    1. IO控制LED闪烁


2. 使用环境

    软件开发环境:KEIL MDK-ARM V5.34.0.0
                     IAR EWARM 8.50.1
    硬件环境:基于N32G401C8L7-STB V1.0评估板开发


3.使用说明

    /*描述相关模块配置方法;例如:时钟，I/O等*/
    SystemClock: 72MHz
    GPIO: PA1、PA7、PA9控制LED (D1、D2、D3)闪烁

    /*描述Demo的测试步骤和现象*/
    1. 编译完成后，下载程序复位并运行;
    2. LED (D1, D2, D3)闪烁;

4. 注意事项
    无


1. Function description

    1. This example shows the IO control LED blinking


2. Use environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                          IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0


3. Instructions for use

    /* Describe related module configuration methods; for example: clock, I/O, etc. */
        SystemClock：72MHz
        GPIO: PA1, PA7, PA9 control LED (D1, D2, D3) to blink

    /* Describe the test steps and phenomena of the Demo */
        1. After compiling, download the program to reset and run;
        2. LED (D1, D2, D3) flashes;

4. Matters needing attention
    none