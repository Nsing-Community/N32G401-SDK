1、功能说明
    1.I2S收发数据完成以后切换成SPI收发数据，再切换成I2S收发数据
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     /* 描述相关模块配置方法；例如：时钟、I/O等 */
     1.系统时钟：72MHz
     2.GPIO：I2S1: WS--PA4  <--> I2S2: WS--PB12
             I2S1: SK--PA5  <--> I2S2: SK--PB13
             I2S1: MCK--PA6 <--> I2S2: MCK--PB14
             I2S1: SD--PA7  <--> I2S2: SD--PB15
     /* 描述Demo的测试步骤和现象 */
     1.编译完成后，下载程序复位运行；
     2.I2S1和I2S2初始化，I2S1发送数据，I2S2接收数据，运行完成后，查看TransferStatus1状态为PASSED，
       SPI1和SPI2初始化，SPI1发送数据，SPI2接收数据，运行完成后，查看TransferStatus2状态为PASSED，
       I2S1和I2S2初始化，I2S1发送数据，I2S2接收数据，运行完成后，查看TransferStatus3状态为PASSED，
       如果全部通过测试则使用串口打印PASS，如果失败则打印ERR；
4、注意事项
     无

1. Function description
     1. After I2S sends and receives data, switch to SPI to send and receive data, and then switch to I2S to send and receive data
2. Development environment
      Software development environment: KEIL MDK-ARM V5.34.0.0
      Hardware environment: developed based on N32G401C8L7-STB V1.0
3. How to use
      /* Describe the configuration method of related modules; for example: clock, I/O, etc. */
      1. System clock: 72MHz
      2. GPIO: I2S1: WS--PA4 <--> I2S2: WS--PB12
               I2S1: SK--PA5 <--> I2S2: SK--PB13
               I2S1: MCK--PA6 <--> I2S2: MCK--PB14
               I2S1: SD--PA7 <--> I2S2: SD--PB15
      /* Describe the test steps and phenomena of Demo */
      1. After the compilation is completed, the download program resets and runs;
      2. I2S1 and I2S2 are initialized, I2S1 sends data, and I2S2 receives data. After the operation is completed, check the TransferStatus1 status as PASSED.
         SPI1 and SPI2 are initialized, SPI1 sends data, SPI2 receives data, after the operation is completed, check the TransferStatus2 status as PASSED,
         I2S1 and I2S2 are initialized, I2S1 sends data, and I2S2 receives data. After the operation is completed, check the TransferStatus3 status as PASSED.
         If all the tests pass, print PASS through the serial port, and print ERR if it fails;
4. Attention
      none