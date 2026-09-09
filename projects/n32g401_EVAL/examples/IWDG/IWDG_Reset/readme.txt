1、功能说明
    1. IWDG复位功能


2、使用环境

    软件开发环境:KEIL MDK-ARM V5.34.0.0
                     IAR EWARM 8.50.1
    硬件环境:基于N32G401C8L7-STB V1.0评估板开发


3、使用说明

    系统配置:
    1. IWDG时钟源:LSI/32
    2. 超时值:250ms
    3. 指示灯:LED1(PA1) LED2(PA7) LED3(PA9)

    说明:
    1. 编译后，将其编程到评估板。上电后,指示灯LED2一直闪烁。这意味着IWDG正常地喂狗，代码正常地运行。大概运行3秒后冻结IWDG，IWDG停止计数，LED2关闭，LED3开始闪烁。
    2. 当SysTick_Delay_Ms(249)里面的值改成大于250后，重新编译下载，上电后由于喂狗超时，可以看到MCU一直复位。


4. 注意事项
    无


1. Function description
    1. IWDG reset function.


2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                          IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0


3. How to use

    System Configuration
        1. IWDG clock source: LSI/32
        2. Timeout value: 250ms
        3. Indicator light: LED1(PA1) LED2(PA7) LED3(PA9)

    Instructions:
    1. After compiling, program it to the evaluation board.  After powering on,LED2 kept flashing. The IWDG feeds the dog properly and the code runs properly, 
       After running for about 3 seconds, freeze IWDG, IWDG stops counting, LED2 off and LED3 starts flashing.
    2. When the parameter of SysTick_Delay_Ms(249) function is changed to more than 250, IWDG feed dog timeout and the entire system will always be reset.


4. Attention
      None

