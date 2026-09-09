1、功能说明
    1、通过检测PA0 IO 上升沿触发入侵中断。

2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    
    系统配置:
        1、RTC时钟源：LSE 32.768kHz
        2、Tamper2 引脚：PA0
        3、串口配置：
                    - 串口为USART1（TX：PA9）:
                    - 数据位：8
                    - 停止位：1
                    - 奇偶校验：无
                    - 波特率： 115200 
    使用方法：
        1、编译后烧录到评估板，上电后，按下 WAKEUP 按键（PA0），串口输出RTC Tamper Interrupt。说明入侵检测发生了入侵中断
                
4、注意事项
    无


1. Function description
     1. Trigger the tamper interrupt by detecting the rising edge of PA0 IO.

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0​

3. How to use

     System Configuration;
         1. RTC clock source: LSE  32.768k
         2. Tamper2 port: PA0
         3. Serial port configuration:
                             - Serial port is USART1 (TX: PA9)
                             - Data bits: 8
                             - Stop bit: 1
                             - Parity: none
                             - Baud rate: 115200


     Instructions:
     1. After compiling and burning it to the evaluation board, after power-on, press WAKEUP key(PA0), and the serial port outputs RTC Tamper Interrupt, indicating that a tamper interrupt has occurred.


4. Attention
     none