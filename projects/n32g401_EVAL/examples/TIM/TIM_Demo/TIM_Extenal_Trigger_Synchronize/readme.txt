1、功能说明
    1、TIM1 CH2门控TIM1 CH1、TIM3 CH1和TIM4 CH1
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=18M,TIM1 CLK=36M,TIM3 CLK=36M,TIM4 CLK=36M       
        2、端口配置：
                    PA8选择为TIM1 CH1输出
                    PA9选择为TIM1 CH2输入
                    PA0选择为TIM3 CH1输出
                    PA6选择为TIM4 CH1输出
        4、TIM：
            TIM1 CH2门控TIM1 CH1、TIM3 CH1和TIM4 CH1         
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM1 CH1,TIM3 CH1,TIM4 CH1的波形
        2、TIM1 CH2高电平定时器开始计数，低电平停止
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。


1. Function description
    1. TIM1 CH2 gates TIM1's CH1, TIM2's CH1 and TIM3's CH1
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on N32G401C8L7_STB V1.0
3. How to use
    System Configuration;
        1. Clock source:
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=18M,TIM1 CLK=36M,TIM3 CLK=36M,TIM4 CLK=36M  
        2. Port configuration:
                    PA8 is selected as TIM1 CH1 output
                    PA9 is selected as TIM1 CH2 input
                    PA0 is selected as TIM2 CH1 output
                    PA6 is selected as TIM3 CH1 output
        3. TIM:
                    TIM1 CH2 gate CH1, gate TIM2, TIM3
    Instructions:
        1. After compiling, open the debug mode, and use an oscilloscope or logic analyzer to observe the waveforms of TIM1 CH1, TIM2, and TIM3 CH1
        2. TIM1 CH2 high level timer starts counting, low level stops
4. Attention
        By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.