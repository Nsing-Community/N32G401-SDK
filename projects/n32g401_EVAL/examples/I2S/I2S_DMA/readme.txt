1、功能说明
    1. I2S使用DMA收发数据
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     /* 描述相关模块配置方法； 例如：时钟、I/O等 */
     1.系统时钟：72MHz
     2.GPIO：I2S1: WS--PA4  <--> I2S2: WS--PB12
             I2S1: SK--PA5  <--> I2S2: SK--PB13
             I2S1: MCK--PA6 <--> I2S2: MCK--PB14
             I2S1: SD--PA7  <--> I2S2: SD--PB15
     /* 描述Demo的测试步骤和现象 */
     1.编译完成后，下载程序复位运行；
     2.I2S1使用DMA发送数据，I2S2使用DMA接收数据，发送完成后，比较发送和接收的数据，
       I2S1使用DMA接收数据，I2S2使用DMA发送数据，发送完成后，比较发送和接收的数据，
       如果通过则使用串口打印PASS，如果失败则打印ERR；
4、注意事项
     无

1. Function description
    1. I2S uses DMA to send and receive data
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    1.SystemClock：72MHz
    2.GPIO：I2S1: WS--PA4  <--> I2S2: WS--PB12 
            I2S1: SK--PA5  <--> I2S2: SK--PB13
            I2S1: MCK--PA6 <--> I2S2: MCK--PB14
            I2S1: SD--PA7  <--> I2S2: SD--PB15  
    /* Describe the test steps and phenomena of the Demo */
    1.After compiling, download the program to reset and run;
    2.I2S1 uses DMA to send data, I2S2 uses DMA to receive data, after the transmission is completed, compare the sent and received data,
      I2S1 uses DMA to receive data, I2S2 uses DMA to send data, after the transmission is completed, compare the sent and received data,
      If it passes, use the serial port to print PASS, and if it fails, print ERR;
4. Attention
    none