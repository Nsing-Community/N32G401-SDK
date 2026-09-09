1、功能说明

    该测例演示了USART2与智能卡间基础通信，查询智能卡随机数


2、使用环境

    软件开发环境：KEIL MDK-ARM V5.34.0.0
                  IAR EWARM 8.50.1

        硬件环境：最小系统板N32G401C8L7-STB_V1.0，智能卡，智能卡座


3、使用说明
    
    系统时钟配置如下：
    - 时钟源 = HSE + PLL
    - 系统时钟 = 72MHz
    - 智能卡工作时钟 = 2.7MHz
    
    USART2配置如下：
    - 波特率 =  (根据设置的智能卡工作时钟配置)
    - 字长 = 8数据位
    - 1.5停止位
    - 校验控制 even
    - 硬件流控制禁用（RTS和CTS信号）
    - 接收器和发送器使能

    打印信息USART1配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 硬件流控制禁用（RTS和CTS信号）
    - 接收器和发送器使能    
    
    USART引脚连接如下：

    - USART2_Tx.PA2    <-------> SMART_CARD I/O
    - USART2_CLK.PA4   <-------> SMART_CARD CLK
    - SC_PIN_RESET.PB7 <-------> SMART_CARD RST
    - 3V3              <-------> SMART_CARD Vcc
    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，查看串口打印信息


4、注意事项
无

1. Function description

The test example demonstrates the basic communication between USART2 and smart card to query the random number of smart card


2. Development environment

Software development environment: KEIL MDK-ARM V5.34.0.0
                                  IAR EWARM 8.50.1

Hardware environment: minimum system board N32G401C8L7-STB_V1.0, smart card, seat of card


3. How to use

The system clock configuration is as follows:
- Clock source = HSE + PLL
- System clock = 72MHz
- Smartcard clock = 2.7MHz

USART2 configuration is as follows:
- Baud rate = (based on the set smart card working clock)
- Word length = 8 data bits
- 1.5 Stop bit
- Check control even
- Hardware flow control disabled (RTS and CTS signals)
- Enable receiver and transmitter

USART1 is configured as follows:
- baud rate = 115200 baud
- Word length = 8 data bits
-1 Stop bit
- Check control is disabled
- Hardware flow control disabled (RTS and CTS signals)
- Enable receiver and transmitter

The USART pins are connected as follows:

- USART2_Tx.PA2    <-------> SMART_CARD I/O
- USART2_CLK.PA4   <-------> SMART_CARD CLK
- SC_PIN_RESET.PB7 <-------> SMART_CARD RST
- 3V3              <-------> SMART_CARD Vcc

Test procedure and phenomenon:
- Demo is compiled in KEIL environment and downloaded to MCU
- Reset and run. View the serial port information


4. Attention
None
