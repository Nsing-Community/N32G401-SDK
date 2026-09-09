1、功能说明
    1、TIM3 CH2上升沿触发CH1输出一个单脉冲
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.34.0.0
    硬件环境：      基于N32G401C8L7_STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM3 CLK=36M                        
        2、端口配置：
                    PA6选择为TIM3的CH1输出
                    PA7选择为TIM3的CH2输入
                    PA3选择为IO输出  
         3、TIM：
                    TIM3 配置CH2上升沿触发CH1输出一个单脉冲                 
    使用方法：
        1、编译后打开调试模式，PA3连接PA7，用示波器或者逻辑分析仪观察TIM3 的CH1 的波形
        2、程序发送PA3的上升沿，TIM3 CH1输出一个单脉冲
        3、进调试模式，将gSendTrigEn改成1，PA3产生一个上升沿，TIM3_CH1输出一个单脉冲

4、注意事项
     默认情况下，开发板的PA9和PA10跳线帽连接到NSLINK的虚拟串行端口。如果PA9和PA10未用作项目中的串行端口，而是用于其他用途，则必须拔掉串行端口跳线帽。
     
     

1. Function description
     1. The rising edge of TIM3 CH2 triggers CH1 to output a single pulse
2. Development environment
     Software development environment: KEIL MDK-ARM V5.34.0.0
     Hardware Environment: Developed based on N32G401C8L7-STB V1.0
3. How to use
     System Configuration;
         1. Clock source:
                     HSE=8M,PLL=72M,AHB=72M,APB1=18M,TIM3 CLK=36M
         2. Port configuration:
                     PA6 is selected as CH1 output of TIM3
                     PA7 is selected as the CH2 input of TIM3
                     PA3 is selected as IO output
         3. TIM:
                     TIM3 configures CH2 rising edge to trigger CH1 to output a single pulse
     Instructions:
         1. Open the debug mode after compiling, connect PA3 to PA7, and observe the waveform of CH1 of TIM3 with an oscilloscope or logic analyzer
         2. Add the variable gSendTrigEn to the watch window, the default gSendTrigEn=0, every time you manually modify gSendTrigEn=1, you will see a single pulse output on the TIM3 CH1 port
         3. Enter the debugging mode, Modify gSendTrigEn to 1, PA3 generate a rising edge, TIM3_CH1 output a single pulse
         
4. Attention
      By default, the PA9 and PA10 jumper caps of the development board are connected to the virtual serial port of NSLINK. If PA9 and PA10 are not used as serial ports in the project, and are used for other purposes, the serial port jumper caps must be unplugged.