1、功能说明

    /* 简单描述工程功能 */
    这个例程配置并演示设置不同的系统时钟并通过MCO PA8输出时钟


2、使用环境

    /* 软件开发环境：当前工程使用的软件工具名称及版本号 */
    IDE工具：KEIL MDK-ARM 5.34.0.0
             IAR  EWARM 8.50.1
      
    /* 硬件环境：工程对应的开发硬件平台 */
    开发板：N32G401C8L7-STB V1.0     

       
3、使用说明

    /* 描述相关模块配置方法；例如:时钟，I/O等 */
    USART：TX - PA9，波特率115200
    GPIO：PA8 - 复用为MC0时钟输出

    /* 描述Demo的测试步骤和现象 */
    1.编译完成后，下载程序复位运行;
    2.将系统时钟分别配置为HSI、HSE、PLL，使用串口打印出当前的SYSCLK、HCLK、PCLK1、PCLK2等信息，并使用PA8多路复用引脚输出时钟，用示波器查看;


4、注意事项
   PLL输出频率必须大于等于32MHz

1. Function description

     /* Briefly describe the project function */
     This example configures and demonstrates setting different system clocks and outputting them from PA8 with MCO


2. Development environment

     /* Software development environment: the name and version number of the software tool used in the current project */
     IDE tool: KEIL MDK-ARM V5.34.0.0
                   IAR  EWARM 8.50.1
      
     /* Hardware environment: the development hardware platform corresponding to the project */
     Development board: N32G401C8L7-STB V1.0
     
3. How to use

     /* Describe related module configuration methods; for example: clock, I/O, etc. */
     USART: TX - PA9, baud rate 115200
     GPIO: PA8 - multiplexed as MC0 clock output

     /* Describe the test steps and phenomena of the Demo */
     1. After compiling, download the program to reset and run;
     2. Configure the system clock as HSI, HSE, PLL respectively, use the serial port to print out the current SYSCLK, HCLK, PCLK1, PCLK2 
         and other information, and use the PA8 multiplexing pin to output the clock, and view it with an oscilloscope;


4. Attention
    The PLL frequency must be greater than or equal to 32MHz