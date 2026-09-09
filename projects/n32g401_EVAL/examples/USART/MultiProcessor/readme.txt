1、功能说明

    该测例演示了如何使用USART多处理器模式。USARTy和USARTz可以是USART1
和USART2。
    首先，分别设置USARTy和USARTz的地址为0x1和0x2。USARTy连续给USARTz
发送字符0x33。USARTz收到0x33，便翻转LED1的引脚。
    一旦KEY1_INT_EXTI_LINE线检测到上升沿，则产生EXTI0中断，在
EXTI0_IRQHandler中断处理函数中(the ControlFlag = 0)，USARTz进入静默
模式，在静默模式中，LED引脚停止翻转，直到KEY1_INT_EXTI_LINE线检测到
上升沿(the ControlFlag = 1)。在EXTI0_IRQHandler中断处理函数中，USARTy
发送地址0x102唤醒USARTz。LED引脚重新启动翻转。


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
    - 字长 = 9数据位
    - 1停止位
    - 校验控制禁用
    - 硬件流控制禁用（RTS和CTS信号）
    - 接收器和发送器使能  
    
    
    USART引脚连接如下：    
    - USART1_Tx.PA9 <-------> UART4_Rx.PB1
    - USART1_Rx.PA10 <-------> UART4_Tx.PB0
    
    KEY1_INT_EXTI_LINE.PA6 <-------> KEY3
    
    LED1 <-------> PA1

    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，观察LED1是否处于闪烁状态
    - 按按键KEY，观察LED1是否停止闪烁
    - 再次按按键KEY，观察LED1是否恢复闪烁


4、注意事项
   需先将开发板NS-LINK的MCU_TX和MCU_RX跳线帽断开


1. Function description
    This test example demonstrates how to use the USART multiprocessor mode. USARTy and USARTz can be USART1 and USART2.
    First, set the addresses of USARTy and USARTz to 0x1 and 0x2, respectively. USARTy continuously gives USARTz send the character
    0x33. USARTz receives 0x33 and flips the pin of LED1.
    Once a rising edge is detected on the KEY1_INT_EXTI_LINE line, an EXTI0 interrupt will be generated.
    In the EXTI0_IRQHandler interrupt handler (the ControlFlag = 0), USARTz goes silent mode, in silent mode, the LED pin stops toggling.
    toggling until the KEY1_INT_EXTI_LINE line detects rising edge (the ControlFlag = 1). In the EXTI0_IRQHandler interrupt handler, 
    USARTysend address 0x102 to wake up USARTz. The LED pin restarts toggling.


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
    - Word length = 9 data bits
    - 1 stop bit
    - checksum control disabled
    - Hardware flow control disabled (RTS and CTS signals)
    - Receiver and transmitter enable
    
    
    The USART pins are connected as follows:
    - USART1_Tx.PA9 <-------> UART4_Rx.PB1
    - USART1_Rx.PA10 <-------> UART4_Tx.PB0
    
    KEY1_INT_EXTI_LINE.PA6 <-------> KEY3
    
    LED1 <-------> PA1

    
    Test steps and phenomena:
    - Demo is compiled in KEIL environment and downloaded to MCU
    - Reset operation and observe whether LED1 are blinking
    - Press the button KEY and observe whether LED1 stop flashing
    - Press the button KEY again and observe whether LED1 resume to flash


4. Attention
    the MCU_TX and MCU_RX jumper cap of the development board NS-LINK needs to be disconnected first