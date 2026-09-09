1、功能说明
    1、ADC采样转换PA0 PA1引脚的模拟电压
    2、采用软件触发一次，采集一次的方式
2、使用环境
    
       软件开发环境：  
          IDE工具：KEIL MDK-ARM V5.34.0.0
                         IAR EWARM 8.50.1

       硬件环境：      
          开发板：N32G401C8L7-STB V1.0

3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,ADC CLK=72M/16,ADC 1M CLK=HSE/8M 
        2、端口配置：
            PA0选择为模拟功能ADC转换通道1
            PA1选择为模拟功能ADC转换通道2
        3、ADC：
            ADC软件触发转换、12位数据右对齐，转换PA0 PA1的模拟电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue添加到watch窗口观察
        2、通过改变PA0 PA1引脚的电压，可以看到转换结果变量同步改变
4、注意事项
    当系统采用HSE时钟时（一般HSI也是打开的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8)可以配置为HSE或者HSI
    当系统采样HSI时钟时（一般HSE是关闭的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8)只能配置为HSI




1. Function description
    1. ADC sampling converts the analog voltage of PA0 PA1 pin
    2. Use the software to trigger once and collect once
2. Development environment
    Software development environment:   KEIL MDK-ARM V5.34.0.0
                                                                IAR EWARM 8.50.1
    Hardware environment: 
        the evaluation board:   N32G401C8L7-STB V1.0

3. How to use
    System configuration：
       1. Clock source:
              HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,ADC CLK=72M/16,ADC 1M CLK=HSE/8M 
       2. Port Configuration:
              PA0 is selected as the analog function ADC conversion channel 1
              PA1 is selected as the analog function ADC conversion channel 2
       3. the ADC:
             ADC software triggers conversion, right-aligned 12-bit data, and converts analog voltage data of PA0 PA1
    Usage:
       1. Open the debugging mode after compilation, and add the variable ADCConvertedValue to the Watch window for observation
       2. By changing the voltage of PA0 PA1 pin, it  be seen that the conversion result variable changes synchronously
4. Attention
       When the system uses the HSE clock (HSI is also normally turned on), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, 
       RCC_ADC1MCLK_DIV8) be configured as HSE or HSI.
       When the system samples the HSI clock (generally, HSE is disabled), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, 
       RCC_ADC1MCLK_DIV8)  only be set to HSI.