1、功能说明
    1、通过设定唤醒时间触发中断。
    2、通过唤醒标志位来配置IO输出

2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    
    系统配置:
        1、RTC时钟源：LSE 32.768kHz
        2、唤醒IO输出：PC13
        3、串口配置：
                    - 串口为USART1（TX：PA9）:
                    - 数据位：8
                    - 停止位：1
                    - 奇偶校验：无
                    - 波特率： 115200 
    使用方法：
        1、编译后烧录到评估板，上电后，串口每隔5s会打印:Stop2 Wakeup From RTC
        2、设定WKUPOutIrq = 1，编译后烧录到评估板，上电后，PC13每隔5S翻转一次(4s高电平，1s低电平)。

4、注意事项
    首次唤醒时间不精确，后续唤醒时间正常，可忽略第一次唤醒


1. Function description
    1. Trigger the wakeup interrupt when wake-up time arrived.
    2. Wake-up output

2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0



3. How to use

    System Configuration;
        1. RTC clock source: LSE  32.768kHz
        2. Wake-up output port: PC13
        3. Serial port configuration:
                            - Serial port is USART1 (TX: PA9)
                            - Data bits: 8
                            - Stop bit: 1
                            - Parity: none
                            - Baud rate: 115200


    Instructions:
        1. After compiling, burn it to the evaluation board. After power-on, the serial port will print I am in rtc_wkup every 5 seconds.
        2. Set WKUPOutIrq =1, and program it to the evaluation board after compiling. After power-on, PC13 Output high level for 4 seconds and low level for 1 second with a period of 5 seconds.


4. Attention
    The first wake-up time is inaccurate, and the subsequent wake-up time is normal, ignore first wake-up