1、功能说明
    1、TIM1 一个周期后同时改变周期和占空比
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB_CLK=72M,APB2_CLK=36M,TIM1 CLK=36M,DMA CLK=36M
        2、端口配置：
            PA8选择为TIM1 CH1输出
        3、TIM：
            TIM1 CH1 输出，周期触发DMA burst传输，加载AR,REPCNT,CCDAT1寄存器，改变占空比和周期和重复计数器
        4、DMA：
            DMA1_CH5通道正常模式搬运3个半字SRC_Buffer[3]变量到TIM1 DMA寄存器            
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM1 CH1的波形
        2、TIM1的第一个周期结束后，后面的波形为DMA搬运的改变周期和占空比的波形
        3、进调试模式，修改SRC_Buffer[6]数组数据，然后将DmaAgain修改为1，即可以改变周期和占空比的波形
        
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。

1. Function description
     1. Change the period and duty cycle at the same time after one cycle of TIM1
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware environment: Developed based on N32G401C8L7_STB V1.0
3. How to use
     System Configuration;
         1. Clock source:
                     HSE=8M,PLL=72M,AHB_CLK=72M,APB2_CLK=36M,TIM1 CLK=36M,DMA CLK=36M
         2. Port configuration:
                     PA8 is selected as TIM1 CH1 output
         3. TIM:
                     TIM1 CH1 output, period trigger DMA burst transfer, load AR, REPCNT, CCDAT1 registers, change duty cycle and period and repeat counter
         4. DMA:
                     DMA1_CH5 channel normal mode transfers 3 half-word SRC_Buffer[3] variables to TIM1 DMA register
     Instructions:
         1. Open the debug mode after compiling, and observe the waveform of TIM1 CH1 with an oscilloscope or logic analyzer
         2. After the first cycle of TIM1 ends, the following waveforms are the waveforms of the change cycle and duty cycle of DMA transfer
         3. Enter the debug mode, modify the SRC_Buffer [6] array data, and then modify the DmaAgain to 1, it will change the period and duty cycle waveforms
         
4. Attention
   By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.