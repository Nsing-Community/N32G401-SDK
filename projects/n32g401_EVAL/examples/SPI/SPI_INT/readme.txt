1、功能说明
     1.SPI在单线模式下使用中断发送和接收数据。
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     /* 描述相关模块配置方法； 例如：时钟、I/O 等 */
     1.系统时钟：72MHz
     2.GPIO：SPI1：NSS--PA4  <--> SPI2：NSS--PB12
             SPI1：SCK--PA5  <--> SPI2：SCK--PB13
             SPI1：MOSI--PA7 <--> SPI2：MISO--PB14
     /* 描述Demo的测试步骤和现象 */
     1.编译完成后，下载程序复位运行；
     2. SPI1使用中断发送数据，SPI2使用中断接收数据，发送完成后，
        比较发送和接收的数据，通过串口打印PASSS，失败打印ERR；
4、注意事项
      如果使用“单线”模式，主机使用的是MOSI引脚，从机使用的是MISO引脚
      如果CLKPOL设置为HIGH，设置SPI_SCK GPIO需要设置为上拉GPIO_PULL_UP，其他情况设置为下拉GPIO_PULL_DOWN

1. Function description
    1. SPI uses interrupt to send and receive data in single mode.
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    /* Describe related module configuration methods; for example: clock, I/O, etc. */
    1、SystemClock：72MHz
    2、GPIO:SPI1: NSS--PA4  <--> SPI2: NSS--PB12 
            SPI1: SCK--PA5  <--> SPI2: SCK--PB13
            SPI1: MOSI--PA7 <--> SPI2: MISO--PB14
    /* Describe the test steps and phenomena of the Demo */
    1. After compiling, download the program to reset and run;
    2. SPI1 uses interrupt to send data, SPI2 uses interrupt to receive data, after the transmission is completed, 
       compare the sent and received data, use the serial port to print PASSS if it passes, and print ERR if it fails;
4. Attention
     The "single wire" data lines are MOSI pins on the master side and MISO pins on the slave side
     If CLKPOL is HIGH, set SPI_SCK GPIO to GPIO_PULL_UP, otherwise set to GPIO_PULL_DOWN