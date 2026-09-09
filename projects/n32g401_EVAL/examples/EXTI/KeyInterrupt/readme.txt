1、功能说明

    此例程展示通过外部触发中断，控制 LED 闪烁


2、使用环境

    软件开发环境：
    IDE工具：KEIL MDK-ARM V5.34.0.0
             IAR EWARM 8.50.1
      
    /* 硬件环境：工程对应的开发硬件平台 */
    开发板：N32G401C8L7-STB V1.0    

3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
    SystemClock：72MHz
    GPIO：PA5 选择作为外部中断入口，PA7 控制 LED(PD3) 闪烁


    /* 描述Demo的测试步骤和现象 */
    1.编译后下载程序复位运行；
    2.按下松开 KEY2 按键，LED 闪烁；


4、注意事项
    无


1. Function description
   This routine shows an external trigger interrupt to control the LED flashing

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0               
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use

    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    SystemClock: 72MHz
    GPIO: PA5 is selected as external interrupt entry, PA7 controls LED(D3) to blink

    /* Describe the test steps and phenomena of the Demo */
    1. After compiling, download the program to reset and run;
    2. Press and release the KEY2 button, the LED flashes;


4. Attention
    none