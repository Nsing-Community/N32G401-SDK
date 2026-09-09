1、功能说明
    1、TIM6 利用更新中断，产生定时翻转IO
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM6 CLK=36M
        2、端口配置：
            PB6选择为IO输出
        3、TIM：
            TIM6使能周期中断
        4、中断：
            TIM6 更新中断打开，抢断优先级0，子优先级1
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察PB6的波形
        2、程序运行后，TIM6的周期中断来临翻转PB6电平
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。
     
     

1. Function description
     1. TIM6 uses update interrupt to generate timing flip IO
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware Environment: Developed based on N32G401C8L7-STB V1.0
3. How to use
     System Configuration;
         1. Clock source:
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM6 CLK=36M
         2. Port configuration:
                    PB6 is selected as IO output
         3. TIM:
                    TIM6 enables periodic interrupts
         4. Interrupt:
                    TIM6 update interrupt on, steal priority 0, sub priority 1
     Instructions:
         1. Open the debug mode after compiling, and observe the waveform of PB6 with an oscilloscope or logic analyzer
         2. After the program runs, the cycle of TIM6 is interrupted and the level of PB6 is flipped4.
Matters needing attention
      By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.