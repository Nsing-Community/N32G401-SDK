1、功能说明

    1. 这个例程显示读端口的高电平和低电平，并控制LED (D1、D2)闪烁
    2. 控制IO端口电平翻转

2、使用环境

    软件开发环境:KEIL MDK-ARM V5.34.0.0
                 IAR EWARM 8.50.1
    硬件环境:基于N32G401C8L7-STB V1.0评估板开发

3、使用说明

    /*描述相关模块配置方法;例如:时钟，I/O等*/
    SystemClock：72MHz
    GPIO:选择PA6作为读端口，PA1、PA7控制LED (D1、D2)闪烁


    /*描述Demo的测试步骤和现象*/
    1. 编译完成后，下载程序复位运行;
    2. 检查PA6引脚电平:
        如果是高电平，LED (D2)常亮;
        如果是低电平，则LED (D1)常亮。PA13/PA15/PB4/PA14/PB3依次接到PA9上，LED(D3)会闪烁

4. 注意事项
  当检测到PA6引脚处于高电平时，关闭JTAG，当PA6引脚处于低电平时，打开JTAG

1. Function description

    1. This routine shows the high and low levels of the read port and controls the LED (D1、D2) to flash
    2. Control IO port level flip

2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use

    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    SystemClock：72MHz
    GPIO: PA6 is selected as the read level port, PA1, PA7 control LED (D1, D2) to flash


    /* Describe the test steps and phenomena of the Demo */
    1. After compiling, download the program to reset and run;
    2. Check the level of the PA6 port:
       If it is high level, LED (D2) is always on;
       If it is low level, the LED (D1) is always on, and the. PA13/PA15/PB4/PA14/PB3 are connected to PA9 in turn, LED(D3) will blink

4. Attention
    When it is detected that the PA6 port is at a high level, close JTAG, and when the PA6 port is at a low level, open the JTAG