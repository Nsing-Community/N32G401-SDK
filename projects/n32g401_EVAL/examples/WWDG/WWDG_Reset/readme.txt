1、功能说明
    1. WWDG复位功能。


2、使用环境

    软件开发环境:KEIL MDK-ARM V5.34.0.0
                     IAR EWARM 8.50.1
    硬件环境:基于N32G401C8L7-STB V1.0评估板开发


3、使用说明

    系统配置:
    1. WWDG时钟源:APB1
    2. 窗口值:6.6s < n < 16.7s
    3.指示灯:PA1(LED1) PA7(LED2)



    产品说明:
    1. 编译后，烧到评估板上。重新上电后，窗口连续显示“喂狗”，
    表示窗口值正常刷新，代码正常运行。


4. 注意事项
    无


1. Function description
    1. WWDG reset function.


2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0


3. How to use
    
    System Configuration:
        1. WWDG clock source: APB1
        2. Window value: 6.6s < n < 16.7s
        3. Indicator light: PA1(LED1) PA7(LED2) 
             


    Instructions:
        1. After compiling, burn it to the evaluation board. After power-on, the window continuously prints "Feed the dog", 
           indicating that the window value is refreshed normally and the code is running normally.


4. Attention
    None

