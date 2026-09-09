1、功能说明
    1、通过闹钟中断唤醒STANDBY模式。

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
        编译后烧录到评估板，上电，MCU 进入到STANDBY模式，当闹钟设定的时间匹配后，MCU会从STANDBY mode唤醒。

4、注意事项
    无

1. Function description
    1. Alarm interrupt wakeup from STANDBT mode.

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0​

3. How to use

    System Configuration;
        1. RTC clock source: LSE
        2. Serial port configuration:
                - Serial port is USART1 (TX:PA9)
                - Data bits: 8
                - Stop bit: 1
                - Parity: none
                - Baud rate: 115200


    Instructions:
        After compiling, burn it to the evaluation board, power on, MCU enter STANDBY mode, when the alarm time matches, MCU wakeup from STANDBY mode.


4. Attention
    none