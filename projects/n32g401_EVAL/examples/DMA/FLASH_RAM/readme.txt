1、功能说明
    该例程提供了一个DMA MemtoMem模式的用法，用于在FLASH和RAM之间传输数据。
    
    我们定义了一个静态数组SRC_Const_Buffer。数据被存储在FLASH的只读存储器中。
    配置DMA_CH1通道，将数据传输到RAM区域DST_Buffer。
    启用DMA传输完成中断，用于指示传输完成。
    
    等待数据传输完成，将DST_Buffer与SRC_Const_Buffer进行比较。
    将比较结果输出到串口。
    
2、使用环境
    软件开发环境：
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    硬件环境：
        基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    1.时钟源：HSE+PLL
    2.主时钟：72MHz
    3.DMA通道：DMA_CH1
    
    4.USART1配置：
        TX --> PA9 50MHz, AF_PP
        波特率：115200
        数据位：8位
        停止位：1位
        无校验
    5.测试步骤和现象：
        A. 编译下载代码复位运行
        B. 从串口查看打印的信息并验证结果
4. 注意事项
    无


1. Function description
    This routine provides a DMA MemtoMem mode usage for transferring data between FLASH and RAM.
    
    We define a static array SRC_Const_Buffer. The data is stored in a read-only memory in FLASH.
    Configure the DMA_CH1 channel for transferring data to the RAM area DST_Buffer,
    Enable DMA transfer completion interrupt, used to indicate that the transfer is complete,
    
    Wait for data transfer to complete and compare DST_Buffer with SRC_Const_Buffer.
    Output the comparison result to the serial port.
    
2. Development environment
    Software development environment: 
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    Hardware environment: 
        Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use
    1.Clock source: HSE+PLL
    2.Master clock: 72MHz
    3.DMA channel: DMA_CH1
    
    4.USART1 configuration:
        TX  -->  PA9            50MHz, AF_PP
        Baud rate: 115200
        Data bit: 8 bits
        Stop bit: 1bit
        No check
    5.Test steps and phenomena
        A. Compile download code reset run
        B. View the printed information from the serial port and verify the result
4. Attention
    None
