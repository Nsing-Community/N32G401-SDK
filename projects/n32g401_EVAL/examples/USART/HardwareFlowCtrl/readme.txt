1、功能说明

    该测例演示了USARTy与USARTz间使用硬件流控制的基础通信。USARTy和
USARTz可以是USART1和USART2。
    首先，USARTy利用CTS发送TxBuffer1数据，USARTz利用RTS接收
数据，存至RxBuffer2；随后，USARTz利用CTS发送TxBuffer2数据，USARTy
利用RTS接收数据，存至RxBuffer1。
    随后，比较接收数据与发送数据，比较结果分别存入变量TransferStatus1
和TransferStatus2。

2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1
    硬件环境：N32G401C8L7-STB V1.0


3、使用说明

    系统时钟配置如下：
    - 时钟源 = HSE + PLL
    - 系统时钟 = 72MHz
    
    USARTy配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - CTS硬件流控制使能
    - 发送器使能   
    
    USARTz配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - RTS硬件流控制使能
    - 接收器使能   
    
    
    USART引脚连接如下：    
    - USART1_Tx.PA9 <-------> USART2_Rx.PA3
    - USART1_Rx.PA10 <-------> USART2_Tx.PA2
    - USART1_CTS.PA11 <-------> USART2_RTS.PA1
    - USART1_RTS.PA12 <-------> USART2_CTS.PA0

    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，依次查看变量TransferStatus1和TransferStatus2，其中，
      PASSED为测试通过，FAILED为测试异常


4、注意事项

    需先将开发板NS-LINK的MCU_TX和MCU_RX跳线帽断开



1. Function description

    This test example demonstrates basic communication between USARTy and USARTz using hardware flow control. USARTy and USARTz can be USART1 and USART2.
    First, USARTy uses CTS to send TxBuffer1 data, USARTz uses RTS to receive.
    The data is stored in RxBuffer2; then, USARTz uses CTS to send TxBuffer2 data, USARTy use RTS to receive data and store it in RxBuffer1.
    Then, compare the received data with the sent data, and store the comparison results in the variable TransferStatus1 and TransferStatus2.

2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0



3. How to use
    The system clock configuration is as follows:
    - Clock Source = HSE + PLL
    - System Clock = 72MHz
    
    USARTy is configured as follows:
    - Baud rate = 115200 baud
    - Word length = 8 data bits
    - 1 stop bit
    - checksum control disabled
    - CTS hardware flow control enabled
    - Transmitter enable
    
    USARTz is configured as follows:
    - Baud rate = 115200 baud
    - Word length = 8 data bits
    - 1 stop bit
    - checksum control disabled
    - RTS hardware flow control enabled
    - Receiver enable
    
    
    The USART pins are connected as follows:
    - USART1_Tx.PA9 <-------> USART2_Rx.PA3
    - USART1_Rx.PA10 <-------> USART2_Tx.PA2
    - USART1_CTS.PA11 <-------> USART2_RTS.PA1
    - USART1_RTS.PA12 <-------> USART2_CTS.PA0

    
    Test steps and phenomena:
    - Demo is compiled in KEIL environment and downloaded to MCU
    - Reset operation, check the variables TransferStatus1 and TransferStatus2 in turn, where,
      PASSED is the test passed, FAILED is the test abnormal

4. Attention
    the MCU_TX and MCU_RX jumper cap of the development board NS-LINK needs to be disconnected first