1、功能说明
    1、TIM2周期门控TIM3，TIM3周期门控TIM4
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM2 CLK=36M,TIM3 CLK=36M,TIM4 CLK=36M
        2、端口配置：
            PA0选择为TIM2 CH1输出
            PA6选择为TIM3 CH1输出
            PB6选择为TIM4 CH1输出
        3、TIM：
            TIM2 的周期门控TIM3，TIM3的周期门控TIM4
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM2 CH1，TIM3 CH1，TIM4 CH1波形
        2、TIM3 4倍周期TIM2，TIM4 4倍周期TIM3
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。



1. Function description
     1. TIM2 cycle gate TIM3, TIM3 cycle gate TIM4
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware environment: Developed based on N32G401C8L7_STB V1.0
3. How to use
     System Configuration;
         1. Clock source:
                     HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM2 CLK=36M,TIM3 CLK=36M,TIM4 CLK=36M
         2. Port configuration:
                     PA0 is selected as TIM2 CH1 output
                     PA6 is selected as TIM3 CH1 output
                     PB6 is selected as TIM4 CH1 output
         3. TIM:
                     The period gate of TIM2 is TIM3, the period gate of TIM3 is TIM4
     Instructions:
         1. Open the debug mode after compiling, and observe the TIM2 CH1, TIM3 CH1, TIM4 CH1 waveforms with an oscilloscope or logic analyzer
         2. TIM3 4 times the period TIM2, TIM4 4 times the period TIM3
4. Attention
    By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.