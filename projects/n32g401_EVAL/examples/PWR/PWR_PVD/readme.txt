1、功能说明
    1. PVD配置电压产生相应的中断。

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

    使用方法：
    在KEIL下编译后，烧到评估板上，通过仿真器进入调试模式，在PVD_IRQHandler()设置断点。
    全速运行。然后把电压调到接近PVD的设定电压档位，程序将在断点处停止。

4. 注意事项
    MSB位可以被扩展。详情请见用户手册中的MSB位的使用说明


1. Function description
    1. PVD configuration voltage produces the corresponding interrupt.

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
    Usage:
    After compiling under KEIL, burn to the evaluation board, go into debug mode through the emulator, and set breakpoint in PVD_IRQHandler().
    Run at full speed. Then turn the voltage close to the PVD setting voltage level, and the program will stop at the breakpoint.

4. Attention
    The MSB bit can be extended. For details, see the instructions for using the MSB bit in the user manual