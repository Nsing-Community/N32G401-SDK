1、功能说明
    1、TIM3 CH2捕获引脚通过CH1下降沿和CH2上升沿计算占空比和频率
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,APB1 CLK=18M,TIM3 CLK=36M                        
        2、中断：
                    TIM3 CC2比较中断打开，抢断优先级0，子优先级0
        3、端口配置：
                    PA7选择为TIM3的CH2输入
                    PA3选择为IO输出
        4、TIM：
                    TIM3 CH1下降沿捕获CH2信号，CH2上升沿捕获CH2信号               
    使用方法：
        1、编译后打开调试模式，连接PA3和PA7，将Frequency、DutyCycle添加到watch窗口
        2、程序运行后，PA3发送的脉冲数据可以被捕获到占空比和频率到变量
        3、进调试模式，将gOnePulsEn修改为1，从Frequency、DutyCycle可以看到PA3发送的脉冲频率和占空比
        
4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。



1. Function description
     1. TIM3 CH2 pin CH2 rising edge, CH1 falling edge to calculate the frequency
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware environment: Developed based on N32G401C8L7_STB V1.0
3. How to use
     System Configuration;
         1. Clock source:
                     HSE=8M,PLL=72M,APB1 CLK=18M,TIM3 CLK=36M
         2. Interrupt:
                     TIM3 CC2 compare interrupt is turned on, steal priority 0, sub priority 1
         3. Port configuration:
                     PA7 is selected as TIM3 CH2 input
                     PA3 is selected as IO output
         4. TIM:
                     TIM3 CH2 rising edge, CH1 falling edge capture interrupt is turned on, the minimum capture frequency range is ((TIM3 CLK/2)/0xffff)Hz, the maximum frequency is ((TIM3 CLK)/2)Hz
     Instructions:
         1. Open the debug mode after compiling, connect PA3 and PA7, and add the variables TIM3Freq and gOnePulsEn to the watch window
         2. The default gOnePulsEn=0, manually give gOnePulsEn=1 each time, then you can see the frequency value calculated by TIM3Freq
         3. Enter the debugging mode, gOnePulsEn modified to 1, from the Frequency, DutyCycle can see the PA3 send pulse frequency and duty cycle

4. Attention
     By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.