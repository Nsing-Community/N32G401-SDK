1、功能说明

    该测例演示了USARTy与USARTz间通过查询检测标识实现的基础通信。
    首先，USARTy发送TxBuffer1数据至USARTz，USARTz接收数据存至RxBuffer1。
比较接收数据与发送数据，比较结果存入TransferStatus变量。
    随后，USARTz发送TxBuffer2数据至USARTy，USARTy接收数据存至RxBuffer2。
比较接收数据与发送数据，比较结果存入TransferStatus变量。
    USARTy和USARTz可以是USART1和USART2。


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
    - USART1_Tx.PA9 <-------> UART4_Rx.PB1
    - USART1_Rx.PA10 <-------> UART4_Tx.PB0

    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，依次查看变量TransferStatus，其中，
      PASSED为测试通过，FAILED为测试异常


4、注意事项

    需先将开发板NS-LINK的MCU_TX和MCU_RX跳线帽断开



1. Function description

    This test example demonstrates the basic communication between USARTy and USARTz by querying the detection flag.
    First, USARTy sends TxBuffer1 data to USARTz, and USARTz receives data and stores it in RxBuffer1.
    Compare the received data with the sent data, and store the comparison result into the TransferStatus variable.
    Then, USARTz sends TxBuffer2 data to USARTy, and USARTy receives data and stores it in RxBuffer2.
    Compare the received data with the sent data, and store the comparison result into the TransferStatus variable.
    USARTy and USARTz can be USART1 and USART2.


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
    - USART1_Tx.PA9 <-------> UART4_Rx.PB1
    - USART1_Rx.PA10 <-------> UART4_Tx.PB0

    
    Test steps and phenomena:
    - Demo is compiled in KEIL environment and downloaded to MCU
    - Reset operation, check the variable TransferStatus twice, where,
      PASSED is the test passed, FAILED is the test abnormal


4. Attention
    the MCU_TX and MCU_RX jumper cap of the development board NS-LINK needs to be disconnected first