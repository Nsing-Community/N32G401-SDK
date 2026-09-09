1、功能说明
    1、COMP1的输出刹车TIM1 TIM8的互补信号，COMP OUT变低后恢复TIM1 TIM8波形
2、使用环境
    
       软件开发环境：  
          IDE工具：KEIL MDK-ARM V5.34.0.0
                         IAR EWARM 8.50.1

       硬件环境：      
          开发板：N32G401C8L7-STB V1.0

3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,COMP CLK=18M,TIM1 CLK=72M,TIM8 CLK=72M
        2、端口配置：
            PB10选择为模拟功能COMP INP
            PA5选择为模拟功能COMP INM
            PA11选择为模拟功能COMP OUT
            PB11选择为IO输出
            PB12选择为IO输出
            PA0选择为TIM1 CH1输出
            PA8选择为TIM1 CH2输出
            PA10选择为TIM1 CH3输出
            PA1 选择为TIM1 CH1N输出
            PB14选择为TIM1 CH2N输出
            PB15选择为TIM1 CH3N输出
            PB4选择为TIM1 breakin输入
            PA2选择为TIM8 CH1输出
            PA3选择为TIM8 CH2输出
            PA4选择为TIM8 CH3输出
            PA7选择为TIM8 CH1N输出
            PB3选择为TIM8 CH2N输出
            PB1选择为TIM8 CH3N输出
        3、TIM：
            TIM1开启CH1 CH2 CH3 CH1N CH2N CH3N输出,COMP作为刹车输入
            TIM8开启CH1 CH2 CH3 CH1N CH2N CH3N输出,COMP作为刹车输入
        4、COMP：
            COMP1输出触发TIM1 TIM8刹车，无输出时恢复TIM1 TIM8输出
    使用方法：
        1、编译后下载复位运行，将PB11连接到PB10，PB12连接到PA5，利用示波器或者逻辑分析仪观察TIM1 TIM8输出波形
        2、当软件输出PB11电平大于PB12时，TIM波形消失，相反时，波形正常输出
4、注意事项
    无

1. Function description
    1. COMP1 output brake TIM1 TIM8 complementary signal, 
    after the COMP OUT becomes low restore TIM1 TIM8 waveform
2. Development environment
    Software development environment:   KEIL MDK-ARM V5.34.0.0
                                                                IAR EWARM 8.50.1
    Hardware environment: 
        the evaluation board:   N32G401C8L7-STB V1.0

3. How to use
    System configuration：
       1. Clock source:
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,COMP CLK=18M,TIM1 CLK=72M,TIM8 CLK=72M
       2. Port Configuration:
            PB10 is selected as the analog function COMP INP
            PA5 is selected as the analog function COMP INM
            PA11 is selected as analog feature COMP OUT
            PB11 Select IO output
            PB12 select IO output
            PA0 select TIM1 CH1 output
            PA8 select TIM1 CH2 output
            PA10 Select TIM1 CH3 output
            PA1 select TIM1 CH1N output
            PB14 selected as TIM1 CH2N output
            PB15 is selected as TIM1 CH3N output
            PB4 select TIM1 breakin input
            PA2 select TIM8 CH1 output
            PA3 select TIM8 CH2 output
            PA4 select TIM8 CH3 output
            PA7 select TIM8 CH1N output
            PB3 is selected as TIM8 CH2N output
            PB1 is selected as TIM8 CH3N output
       3. TIM:
            TIM1 starts CH1 CH2 CH3 CH1N CH2N CH3N output,COMP as brake input
            TIM8 starts CH1 CH2 CH3 CH1N CH2N CH3N output,COMP as brake input
       4. COMP：
            COMP1 output triggers TIM1 TIM8 brake, and resumes TIM1 TIM8 output when there is no output
    Usage:
            1. Download after compilation and run, connect PB11 to PB10 and PB12 to PA5, and observe 
            the output waveform of TIM1 and TIM8 with oscilloscope or logic analyzer
            2. When the software output PB11 level is greater than PB12, TIM waveform disappears; 
            otherwise, waveform output is normal
4. Attention
    No