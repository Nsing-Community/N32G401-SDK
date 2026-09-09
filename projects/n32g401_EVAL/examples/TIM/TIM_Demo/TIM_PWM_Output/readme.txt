1、功能说明
    1、TIM1 输出4组互补PWM波形
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,AHB=72M,APB2_CLK=36M,TIM1 CLK=72M                       
        2、端口配置：
                    PA8选择为TIM1 CH1输出
                    PA9选择为TIM1 CH2输出
                    PA10选择为TIM1 CH3输出
                    PB13选择为TIM1 CH1N输出
                    PB14选择为TIM1 CH2N输出
                    PB15选择为TIM1 CH3N输出
                    PA11选择为TIM1 CH4输出
                    PB2选择为TIM1 CH4N输出  
                    PB12选择为TIM1 刹车输入                   
        3、TIM：
                    TIM1输出8路带死区的PWM，刹车IO为PB12               
    使用方法：
        1、编译后打开调试模式，连用示波器或者逻辑分析仪观察TIM1各通道的波形。
        2、程序运行后，PB12低电平时可以观察到4组互补PWM波形，PB12高电平时没有波形
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。
     


1. Function description
    1. TIM1 outputs 4 complementary waveforms
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on N32G401C8L7_STB V1.0
3. How to use
    System Configuration;
        1. Clock source:
                    HSE=8M,PLL=72M,AHB=72M,APB2_CLK=36M,TIM1 CLK=72M
        2. Port configuration:
                    PA8 is selected as TIM1 CH1 output
                    PA9 is selected as TIM1 CH2 output
                    PA10 is selected as TIM1 CH3 output
                    PB13 is selected as TIM1 CH1N output
                    PB14 is selected as TIM1 CH2N output
                    PB15 is selected as TIM1 CH3N output
                    PA11 is selected as TIM1 CH4 output
                    PB2 is selected as TIM1 CH4N output
                    PB12 is selected as TIM1 Breakin input
        3. TIM:
                    TIM1 8-way complementary with dead zone, IOM brake
    Instructions:
        1. Open the debug mode after compiling, and observe the waveform of TIM1 with an oscilloscope or logic analyzer
        2. After the program runs, four sets of complementary PWM waveforms  be observed at low level of PB12, and there are no waveforms at high level of PB12
4. Attention
        By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.