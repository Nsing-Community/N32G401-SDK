1、功能说明
    1. 进入和退出STOP0模式。

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
               
    使用方法：
    用KEIL编译后，烧录到评估板上，接上电流表，发现通电后电流从mA降到uA级别。
    按下唤醒按钮（PA0），电流恢复到mA水平，过了一会儿又降到uA。

4. 注意事项
    当评估功耗时，请注意移除LED来测量。


1. Function description
    1. Enter and exit STOP0 mode.

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
               
    Usage:
    After compiling in KEIL, it was burned to the evaluation board, connected to the ammeter, and found that the current dropped from mA to uA level after powering on.
    By pressing the wake button (PA0), the current reverted to mA level and after a while dropped back to uA.

4. Attention
    When evaluating power consumption, take care to remove the LED to measure