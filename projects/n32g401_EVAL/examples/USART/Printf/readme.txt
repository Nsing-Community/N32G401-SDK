1、功能说明

    该测例演示了USARTx与PC间通过查询检测标识实现的基础通信。
    重定向printf函数至USARTx，并使用printf函数输出消息至终端。
    USARTx可以是USART1或USART2。


2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
        硬件环境：N32G401C8L7-STB V1.0


3、使用说明

    系统时钟配置如下：
    - 时钟源 = HSE + PLL
    - 系统时钟 = 72MHz
    
    USART配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 硬件流控制禁用（RTS和CTS信号）
    - 接收器和发送器使能
    
    USART引脚连接如下：
    - USART1_Tx.PA9 
    或
    - USART2_Tx.PA6
    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，查看串口打印信息


4、注意事项




1. Function description

     This test example demonstrates the basic communication between USARTx and PC by querying the detection flag.
     Redirect the printf function to USARTx, and use the printf function to output messages to the terminal.
     USARTx can be USART1 or USART2.


2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0



3. How to use
     The system clock configuration is as follows:
     - Clock Source = HSE + PLL
     - System Clock = 72MHz
    
     USART is configured as follows:
     - Baud rate = 115200 baud
     - Word length = 8 data bits
     - 1 stop bit
     - checksum control disabled
     - Hardware flow control disabled (RTS and CTS signals)
     - Receiver and transmitter enable
    
     The USART pins are connected as follows:
     - USART1_Tx.PA9
     or
     - USART2_Tx.PA6
    
     Test steps and phenomena:
     - Demo is compiled in KEIL environment and downloaded to MCU
     - reset operation, view serial port printing information


4. Attention
    none