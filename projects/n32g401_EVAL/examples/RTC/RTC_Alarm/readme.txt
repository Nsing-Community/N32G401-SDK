1、功能说明
    1、通过设定闹钟时间来触发闹钟中断。
    2、通过闹钟标志位来配置IO输出

2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    
    系统配置:
        1、RTC时钟源：LSE 32.768kHz
        2、闹钟IO输出：PC13
        3、串口配置：
                    - 串口为USART1（TX：PA9）:
                    - 数据位：8
                    - 停止位：1
                    - 奇偶校验：无
                    - 波特率： 115200 
    使用方法：
        编译后烧录到评估板，重新上电，当闹钟设定的时间匹配后，串口会打印时间，同时，PC13输出高电平。

4、注意事项
    无

1. Function description
    1. Trigger the alarm interrupt when match the alarm time.
    2. Alarm output

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0​

3. How to use

    System Configuration;
        1. RTC clock source: LSE 32.768kHz
        2. Alarm clock IO output: PC13
        3. Serial port configuration:
                - Serial port is USART1 (TX:PA9)
                - Data bits: 8
                - Stop bit: 1
                - Parity: none
                - Baud rate: 115200


    Instructions:
        After compiling, burn it to the evaluation board, power on again, when the alarm time matches, serial port will print the time.
        At this time, alarm output high level on PC13 port.


4. Attention
    none