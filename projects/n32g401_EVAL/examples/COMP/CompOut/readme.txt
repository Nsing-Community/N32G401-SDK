1、功能说明
    1、COMP1的输出PA11受输入INP PB10和INM PA5的影响
2、使用环境
    
       软件开发环境：  
          IDE工具：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM 8.50.1

       硬件环境：      
          开发板：N32G401C8L7-STB V1.0

3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,COMP CLK=18M 
        2、端口配置：
            PB10选择为模拟功能COMP INP
            PA5选择为模拟功能COMP INM
            PA11选择为模拟功能COMP OUT
            PB11选择为IO输出
            PB12选择为IO输出
        3、COMP：
            COMP1输入PB10，PA5，输出PA11
    使用方法：
        1、编译后打开调试模式，将PB11连接到PB10，PB12连接到PA5，利用示波器或者逻辑分析仪观察PA11输出波形
        2、当软件输出PB11电平大于PB12时，PA11输出高电平，相反时，输出低电平
4、注意事项
    无

1. Function description
    1、COMP1 output PA11 is affected by INP PB10 and INM PA5
2. Development environment
    Software development environment:   KEIL MDK-ARM V5.34.0.0
                                        IAR EWARM 8.50.1
    Hardware environment: 
        the evaluation board:   N32G401C8L7-STB V1.0

3. How to use
    System configuration：
       1. Clock source:
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,COMP CLK=18M 
       2. Port Configuration:
            PB10 is selected as the analog function COMP INP
            PA5 is selected as the simulation function COMP INM
            PA11 is selected as emulation feature COMP OUT
            PB11 Select IO output
            PB12 select IO output
       3. COMP：
            COMP1 inputs PB10, PA5, and outputs PA11
    Usage:
       1. After compiling, open the debugging mode, connect PB11 to PB10 and PB12 to PA5, 
          and observe the output waveform of PA11 using oscilloscope or logic analyzer.
       2. When the software outputs PB11 level greater than PB12, PA11 outputs high level; otherwise, PA11 outputs low level
4. Attention
    No