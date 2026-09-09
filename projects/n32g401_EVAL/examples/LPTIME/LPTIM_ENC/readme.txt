1、功能说明
     1.LPTIM统计IN1 IN2正交编码的上升沿个数
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     系统配置;
         1.时钟源：
            HSE = 8M,PLL = 72M,AHB = 72M,APB1 = 18M,APB2 = 36M,LPTIM CLK = LSI 40K
         2.端口配置：
            PB5 被选为 LPTIME IN1 输入
            PB7 被选为 LPTIME IN2 输入
            PA1被选为IO输出
            PA7被选为IO输出
         3. LPTIM：
            LPTIM正交编码器模式，使用内部LSI时钟连续计数IN1 IN2的上升沿个数
     指示：
         1.编译后打开debug模式，连接PA1和PB5，PA7和PB7，在watch window中加入变量encCNT
         2.程序运行后，PA1 PA7输出20个脉冲周期，encCNT等于40
4、注意事项
     无


1. Function description
    1. LPTIM counts the number of rising edges of IN1 IN2 quadrature encoding
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    System Configuration;
        1. Clock source:
           HSE = 8M, PLL = 72M, AHB = 72M, APB1 = 18M, APB2 = 36M, LPTIM CLK = LSI 40K
        2. Port configuration:
           PB5 is selected as LPTIME IN1 input
           PB7 is selected as LPTIME IN2 input
           PA1 is selected as IO output
           PA7 is selected as IO output
        3. LPTIM:
           LPTIM quadrature encoder mode, use the internal LSI clock to continuously count the number of rising edges of IN1 IN2
    Instructions:
        1. Open the debug mode after compiling, connect PA1 and PB5, PA7 and PB7, and add the variable encCNT to the watch window
        2. After the program runs, PA1 PA7 outputs 20 pulse cycles, and encCNT is equal to 40
4. Attention
    none