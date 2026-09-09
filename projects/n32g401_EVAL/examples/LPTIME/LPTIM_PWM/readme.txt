1、功能说明
     1.LPTIM输出PWM信号
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     系统配置;
         1.时钟源：
           HSE = 8M, PLL = 72M, AHB = 72M, APB1 = 18M, APB2 = 36M, LPTIM CLK = LSI 40K
         2.端口配置：
           PB2选作LPTIM输出
         3.LPTIM：
           LPTIM 4频LSI，输出PWM信号
     指示：
         1.编译后打开debug模式，可以观察到PB2脚的PWM信号
         2.将PB2引脚连接到LED，以便您可以看到LED闪烁。
4、注意事项
     无


1. Function description
    1. LPTIM output PWM signal
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    System Configuration;
        1. Clock source:
           HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,LPTIM CLK=LSI 40K
        2. Port configuration:
           PB2 selected as LPTIM output
        3. LPTIM:
           LPTIM 4-frequency LSI, output PWM signal
    Instructions:
        1. Open the debug mode after compiling, and you  observe the PWM signal of the PB2 pin
        2. Connect the PB2 pin to the LED so that you  see the LED blink.
4. Attention
    none