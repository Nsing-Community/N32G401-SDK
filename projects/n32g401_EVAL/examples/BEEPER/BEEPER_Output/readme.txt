1、功能说明

    演示beeper模块的用法。

2、使用环境

    软件开发环境:KEIL MDK-ARM V5.34.0.0
                     IAR EWARM 8.50.1
    硬件环境:基于N32G401C8L7-STB V1.0评估板开发

3、使用说明

    1. 时钟源:APB2(36MHz) + PSC(32) + Div(1024)
    2. 输出端口:
    PA6 AF_PP
    PA7 AF_PP

    使用方法：
    a:编译并下载代码，进入调试模式
    b:用示波器查看波形，验证结果

4. 注意事项
    1. PA6和PA7的波形互补
    2. APB2时钟频率选择寄存器BEEPER_CTRL.PSC位是根据主频确定的，这样输出波形不会失真。


1. Function description

    This example demonstrates the usage of the beeper module.
    
2. Development environment

    Software development environment: KEIL MDK-ARM V5.34.0.0
                                      IAR EWARM 8.50.1
    Hardware environment: Developed based on the evaluation board N32G401C8L7-STB V1.0

3. How to use

    1. Clock source: APB2(36MHz) + PSC(32) + Div(1024)
    2. Output port:
       PA6      AF_PP
       PA7      AF_PP

    3. Test steps and phenomena
       a: Compile and download the code, enter Debug mode
       b: View the waveform with an oscilloscope to verify the results

4. Attention
    1. The waveforms of PA6 and PA7 are complementary
    2. The value of the APB2 clock frequency selection register BEEPER_CTRL.PSC bit is determined according to the main frequency, so that the 
       output waveform will not be distorted.