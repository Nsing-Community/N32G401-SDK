1、功能说明
    1、TIM3 CH1 CH2 CH3 CH4 达到CC值后，输出ACTIVE电平
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM3 CLK=36M      
        2、端口配置：
                    PA6选择为TIM3 CH1输出
                    PA7选择为TIM3 CH2输入
                    PB0选择为TIM3 CH3输出
                    PB1选择为TIM4 CH4输出     
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM3 CH1 CH2 CH3 CH4的波形
        2、定时器运行到CC1 CC2 CC3 CC4之后，对应通道的输出变为Active
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。



1. Function description
     1. After TIM3 CH1 CH2 CH3 CH4 reaches the CC value, it outputs ACTIVE level
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware Environment: Developed based on N32G401C8L7_STB V1.0 EVB
3. How to use
     System Configuration;
         1. Clock source:
                     HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM3 CLK=36M
         2. Port configuration:
                     PA6 is selected as TIM3 CH1 output
                     PA7 is selected as TIM3 CH2 output
                     PB0 is selected as TIM3 CH3 output
                     PB1 is selected as TIM3 CH4 output
     Instructions:
         1. Open the debug mode after compiling, and observe the waveforms of TIM2 CH1 CH2 CH3 CH4 with an oscilloscope or logic analyzer
         2. After the timer runs to CC1 CC2 CC3 CC4, the output of the corresponding channel becomes Active
4. Attention
       By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.