1、功能说明
    1. 进入和退出SLEEP模式。

2、使用环境
    软件开发环境：
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    硬件环境：
        基于评估板N32G401C8L7-STB V1.0开发

3、使用说明
    系统配置：
    1. 时钟源：HSE+PLL
    2. 时钟频率：72MHz
    3. 唤醒源：PA0
    4. 指示灯：PA1

    使用方法：
    在KEIL中编译后，烧录到评估板上。通电后，由PA1控制的指示灯熄灭。每隔一段时间按一次唤醒按钮PA0。
    PA1控制的指示灯电平会翻转一次。

4. 注意事项
    在评估功耗时，注意移除LED进行测量。


1. Function description
    1. Enter and exit the SLEEP mode.

2. Development environment
    Software development environment: 
        KEIL MDK-ARM V5.34.0.0
        IAR EWARM 8.50.1

    Hardware environment: 
        Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use
    System configuration:
    1. Clock source: HSE+PLL
    2. Clock frequency: 72MHz
    3. Wake up source: PA0
    4. Indicator light: PA1

    Usage:
    After compiling in KEIL, it is burned to the evaluation board. After powering on, the indicator light controlled by PA1 is off. Press the wake button PA0 every once in a while,
    The PA1 control indicator level will be flipped once.

4. Attention
    When evaluating power consumption, take care to remove the LED to measure.