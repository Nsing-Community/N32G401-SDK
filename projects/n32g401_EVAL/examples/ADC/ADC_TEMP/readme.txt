1、功能说明
    1、ADC采样转换内部温度传感器的模拟电压，并转换为温度值
2、使用环境
    
       软件开发环境：  
          IDE工具：KEIL MDK-ARM V5.34.0.0
                         IAR EWARM 8.50.1

       硬件环境：      
          开发板：N32G401C8L7-STB V1.0

3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,ADC CLK=72M/16,ADC 1M CLK=HSE/8 
        2、DMA：
            DMA_CH1通道回环模式搬运一个半字的ADC转换结果到ADCConvertedValue变量
        3、ADC：
            ADC连续转换、软件触发、12位数据右对齐，转换通道17即内部温度传感器的模拟电压数据
        4、端口配置：
            PA9选择为USART1的TX引脚
            PA10选择为USART1的RX引脚
        5、USART：
            USART1 115200波特率、8位数据位、1位停止位、无奇偶校验位、无硬件流控、发送和接收使能
        6、功能函数：
            TempValue = TempCal(ADCConvertedValue)函数将温度ADC原始格式数据转为度的单位的格式
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue,TempValue添加到watch窗口观察
        2、将串口工具连接到PA9引脚，并打开串口接收工具
        3、全速运行，可以看到温度变量的数值在常温下接近25度左右，同时串口工具显示实时芯片内的温度值
4、注意事项
    当系统采用HSE时钟时（一般HSI也是打开的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8)可以配置为HSE或者HSI
    当系统采样HSI时钟时（一般HSE是关闭的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8)只能配置为HSI


1. Function description
    1. ADC samples and converts the analog voltage of the internal temperature sensor to the temperature value
2. Development environment
    Software development environment:   KEIL MDK-ARM V5.34.0.0
                                                                IAR EWARM 8.50.1
    Hardware environment: 
        the evaluation board:   N32G401C8L7-STB V1.0

3. How to use
    System configuration：
       1. Clock source:
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,ADC CLK=72M/16,ADC 1M CLK=HSE/8 
        2、DMA：
            DMA_CH1 channel loopback mode carries a half-word ADC conversion result to the ADCConvertedValue variable
        3、ADC：
            ADC continuous conversion, software triggered, 12 bit data right aligned, conversion channel 17 is the analog
            voltage data of the internal temperature sensor
        4. Port Configuration:
            PA9 selects the TX pin of USART1
            PA10 selects the RX pin for USART1
        5、USART：
           USART1 115200 Baud rate, 8 data bits, 1 Stop bit, no parity bit, no hardware flow control, send and receive enabled
        6、Functions:
            TempValue = TempCal(ADCConvertedValue)，The temperature ADC function converts the raw format data into degrees
    Usage:
        1、compiled to open the debug mode, variable ADCConvertedValue, TempValue added to the watch window to observe
        2、Connect the serial port tool to the PA9 pin and open the serial port receiver tool
        3、Running at full speed, it  be seen that the value of the temperature variable is close to 25 degrees at room temperature, 
           and the serial port tool displays the real-time temperature value in the chip
4、Precautions
       When the system uses the HSE clock (HSI is also normally turned on), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, 
       RCC_ADC1MCLK_DIV8) be configured as HSE or HSI.
       When the system samples the HSI clock (generally, HSE is disabled), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, 
       RCC_ADC1MCLK_DIV8)  only be set to HSI.