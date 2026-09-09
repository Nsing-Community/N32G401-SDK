1、功能说明
     1.LPTIM统计IN1上升沿的个数
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     系统配置;
         1.时钟源：
            HSE = 8M,PLL = 72M,AHB = 72M,APB1 = 18M,APB2 = 36M,LPTIM CLK = LSI 40K
         2.端口配置：
            PB5被选为LPTIME IN1输入
            PA1被选为IO输出
         3. LPTIM：
            LPTIM外部计数模式，使用内部LSI时钟连续计数IN1上升沿的个数
     指示：
         1.编译后打开debug模式，连接PB5和PA1，在watch window中加入变量tempCNT
         2.程序运行后，PA1输出10个脉冲周期，tempCNT等于10
4、注意事项
     无


1. Function description
    1. LPTIM counts the number of rising edges of IN1
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    System Configuration;
        1. Clock source:
           HSE = 8M, PLL = 72M, AHB = 72M, APB1 = 18M, APB2 = 36M, LPTIM CLK = LSI 40K
        2. Port configuration:
           PB5 is selected as LPIME IN1 input
           PA1 is selected as IO output
        3. LPTIM:
           LPTIM external counting mode, use the internal LSI clock to continuously count the number of rising edges of IN1
    Instructions:
        1. Open the debug mode after compiling, connect PB5 and PA1, and add the variable tempCNT to the watch window
        2. After the program runs, PA1 outputs 10 pulse cycles, and tempCNT is equal to 10
4. Attention
    none