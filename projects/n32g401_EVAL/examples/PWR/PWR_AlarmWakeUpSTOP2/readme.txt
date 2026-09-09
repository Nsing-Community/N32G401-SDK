1、功能说明
    1. 使用RTC闹钟来唤醒STOP2。
    2. 串口打印进入退出状态

2、使用环境
    软件开发环境：
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    硬件环境：
        基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    系统配置：
    1. RTC时钟源：LSI/LSE
    2. 低功耗模式：STOP2
    3. 串行端口配置：
        - 串行端口：USART1 (TX: PA9 RX: PA10) :
        - 数据位：8
        - 停止位：1
        - 奇偶校验：无
        - 波特率：115200

    使用方法：
    在KEIL中编译后，烧到评估板上，上电后，过一段时间，串口打印出start low power，表示进入低功耗模式。
    如果通过串口打印出Exit low power，表明闹钟唤醒了STOP2。

4. 注意事项
    无


1. Function description
    1. Use the RTC alarm clock to wake up STOP2.
    2. Serial port printing enters the exit state

2. Development environment
    Software development environment: 
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    Hardware environment: 
        Developed based on the evaluation board N32G401C8L7-STB V1.0


3. How to use
    System configuration;
    1. RTC clock source: LSI/LSE
    2. Low power mode: STOP2
    3. Serial port configuration
        - Serial port: USART1 (TX: PA9 RX: PA10) :
        - Data bit: 8
        - Stop bit: 1
        - Parity check: None
        - Baud rate: 115200

    Usage:
    After compiling in KEIL, burn to the evaluation board, power on, after a while, the serial port prints start low power, indicating that it enters the low power mode.
    If Exit low power is printed through the serial port, it indicates that the alarm clock woke up STOP2.

4. Attention
    None
