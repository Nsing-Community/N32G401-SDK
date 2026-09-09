1、功能说明
    本例程提供了一种DMA的用法，用于在外设和RAM之间传输数据。
             
    初始化CLOCK，GPIO，PERIPH，然后启用SPI的DMA功能，再进行DMA。

    首先DMA_CH5将数据从Slave_Tx_Buffer传输到SPI2设备的TX数据寄存器，数据流从SPI2 TX
    到SPI1 RX，DMA_CH4从SPI1的RX寄存器传输数据到master_rx_buffer。

    同时，DMA_CH3从Master_Tx_Buffer传输数据到SPI1设备的TX数据寄存器，数据流从SPI1 TX发送。
    到SPI2 RX，DMA_CH2将数据从SPI2的RX寄存器传输到Slave_rx_Buffer。
    等待DMA传输完成。
    比较Slave_Rx_Buffer和Master_Tx_Buffer的数据一致性，将比较结果输出到USART1
    比较Master_Rx_Buffer和Slave_Tx_Buffer到USART1

2、使用环境
    软件开发环境：
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    硬件环境：
        基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    1. 时钟源：HSE+PLL
    2. 主时钟：72MHz
    3. DMA通道：DMA_CH2, DMA_CH3, DMA_CH4, DMA_CH5
    4. SPI1配置：
        NSS --> PA4 50MHz, AF_PP
        SCK --> PA5 50MHz, AF_PP
        MISO --> PA6 50MHz, AF_PP
        MOSI --> PA7 50MHz, AF_PP
        全双工
        主模式
        8位传输
        极性：在低电平/第二边缘开始
        可选择的软件片断
        大端在前MSB

    5. SPI2配置：
        NSS --> PB12 50MHz, AF_PP
        SCK --> PB13 50MHz, AF_PP
        MISO --> PB14 50MHz, AF_PP
        MOSI --> PB15 50MHz, AF_PP
        全双工
        从模式上看
        8位传输
        极性：从低电平/第二边缘开始
        选择软件的片断
        大端在前MSB

    6. USART1配置：
        TX --> PA9 50MHz, AF_PP
        波特率：115200
        数据位：8位
        停止位：1位
        无校验

    7. 测试步骤和现象
        A. 编译下载代码复位运行
        B. 查看从串口打印的信息并验证结果
        
4. 注意事项
    无


1. Function description
    This routine provides a DMA usage for transferring data between peripherals and RAM.
             
    Initialize CLOCK, GPIO, PERIPH, then enable DMA function of SPI, and then DMA.

    First DMA_CH5 transfers data from Slave_Tx_Buffer to the TX data register of SPI2 device, and the data stream is sent from SPI2 TX
    To SPI1 RX, DMA_CH4 transfers data from the RX register of SPI1 to master_rx_buffer.

    At the same time, DMA_CH3 transfers data from Master_Tx_Buffer to the TX data register of SPI1 device,and the data stream is sent from SPI1 TX
    To SPI2 RX, DMA_CH2 transfers data from SPI2's RX register to Slave_rx_Buffer.
    Wait for the DMA transfer to complete,
    Compare data consistency between Slave_Rx_Buffer and Master_Tx_Buffer, output the comparison result to USART1
    Compare Master_Rx_Buffer and Slave_Tx_Buffer to USART1

2. Development environment
    Software development environment: 
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    Hardware environment: 
        Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use
    1. Clock source: HSE+PLL
    2. Master clock: 72MHz
    3. DMA channels: DMA_CH2, DMA_CH3, DMA_CH4, DMA_CH5
    4. SPI1 configuration:
        NSS   -->  PA4          50MHz, AF_PP
        SCK   -->  PA5          50MHz, AF_PP
        MISO  -->  PA6          50MHz, AF_PP
        MOSI  -->  PA7          50MHz, AF_PP
        Full duplex
        Main mode
        8 bit transmission
        Polarity: start at low/second edge
        Piece of software to choose
        Big end in front MSB

    5. SPI2 Configuration:
        NSS   -->  PB12         50MHz, AF_PP
        SCK   -->  PB13         50MHz, AF_PP
        MISO  -->  PB14         50MHz, AF_PP
        MOSI  -->  PB15         50MHz, AF_PP
        Full duplex
        From the pattern
        8 bit transmission
        Polarity: start at low/second edge
        Piece of software to choose
        Big end in front MSB

    6. USART1 configuration:
        TX  -->  PA9            50MHz, AF_PP
        Baud rate: 115200
        Data bit: 8 bits
        Stop bit: 1bit
        No check

    7. Test steps and phenomena
        A. Compile download code reset run
        B. View the printed information from the serial port and verify the result
        
4. Attention
    None
