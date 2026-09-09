1、功能说明

    此例程展示了作为从机使用双地址模式收发数据。   

2、使用环境

   软件开发环境：
        IDE工具：KEIL MDK-ARM 5.34.0.0
                IAR EWARM 8.50.1
    
    硬件环境：
        开发板：N32G401C8L7-STB V1.0
          
3、使用说明

    1、主时钟：72MHz
    2、I2C 配置：
            SCL --> PB6
            SDA --> PB7         
            CLOCK:100KHz
            
    3、UART1配置：
            TX --> PA9
            RX --> PA10         
            波特率：115200
        

    4、测试步骤与现象
        a，跳线连接主机I2C
        b，编译下载代码复位运行
        c，从串口看打印信息，验证结果

4、注意事项
    SCL及SDA必须接上拉

1. Function description

     This routine demonstrates using dual address mode to send and receive data as a slave.

2. Development environment

    Software development environment:
         IDE tool: KEIL MDK-ARM V5.34.0.0
                       IAR EWARM 8.50.1 
    
     Hardware environment:
         Development board: N32G401C8L7-STB V1.0


3. How to use

     1. Main clock: 72MHz
     2. I2C1 configuration:
             SCL --> PB6
             SDA --> PB7
             CLOCK: 100KHz
            
     3. USART1 configuration:
             TX --> PA9
             RX --> PA10
             Baud rate: 115200
        

     4. Test steps and phenomena
         a, jumper to connect the host I2C1
         b, compile and download the code to reset and run
         c, see the print information from the serial port, and verify the result

4. Attention
     SCL and SDA must be pulled up