1、功能说明
     1.LPTIM定时产生EXTI20中断，唤醒CPU进行IO翻转
2、使用环境
     软件开发环境：KEIL MDK-ARM V5.34.0.0
                   IAR EWARM V8.50.1
     硬件环境：基于N32G401C8L7-STB V1.0开发
3、使用说明
     系统配置;
          1.时钟源：
              HSE = 8M, PLL = 72M, AHB = 72M, APB1 = 18M, APB2 =36M, LPTIM CLK = LSI 40K
          2.中断：
              LPTIM周期触发EXTI20中断，唤醒CPU
          3.端口配置：
              PB7被选为IO输出
          4. LPTIM：
              LPTIM连续计时模式
     指示：
          1.编译后下载复位运行，可以观察到PA1脚有一个周期翻转，CPU继续进入低功耗模式
4、注意事项
     无


1. Function description
    1. LPTIM generates EXTI20 interrupt regularly, wakes up the CPU for IO flipping
2. Development environment
    Software development environment: KEIL MDK-ARM V5.34.0.0
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0
3. How to use
    System Configuration;
         1. Clock source:
            HSE = 8M, PLL = 72M, AHB = 72M, APB1 = 18M, APB2 = 36M, LPTIM CLK = LSI 40K
         2. Interrupt:
            LPTIM cycle triggers EXTI20 interrupt, wakes up CPU
         3. Port configuration:
            PB7 is selected as IO output
         4. LPTIM:
            LPTIM Continuous Timing Mode
    Instructions:
         1. Download after compiling and run, and it can be observed that the PA1 pin has a cycle flip, and the CPU continues to enter the low-power mode
4. Attention
    none   