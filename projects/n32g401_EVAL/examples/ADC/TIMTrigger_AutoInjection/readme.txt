1、功能说明
    1、ADC规则采样PA0、自动注入采样PA1引脚的模拟电压，在TIM1 CC2事件下触发采样
    2、其中规则转换结果通过DMA_CH1通道读取到变量ADC_RegularConvertedValueTab[10]数组
        注入转换结果通过转换结束中断读取到变量ADC_InjectedConvertedValueTab[10]数组
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
        2、中断：
            ADC注入转换结果完成中断打开，中断优先级分组2，抢断优先级0，子优先级0
            中断函数中将注入结果读取到ADC_InjectedConvertedValueTab[10]数组，并翻转PA6电平
        3、端口配置：
            PA0选择为模拟功能ADC转换通道
            PA1选择为模拟功能ADC转换通道
            PA6选择为通用IO输出
            PA8选择为TIM1 CH1 PWM输出
            PA11选择为TIM1 CH4 PWM输出
        4、DMA：
            DMA_CH1通道循环模式搬运10个半字的ADC转换结果到ADC_RegularConvertedValueTab[10]数组
        5、ADC：
            ADC TIM1 CC2触发、12位数据右对齐，规则转换通道PA0，自动注入转换通道PA1的模拟电压数据
        6、TIM：
            TIM1开启CH1 CH4输出，CH2用作触发ADC转换
    使用方法：
        1、编译后打开调试模式，将变量ADC_RegularConvertedValueTab[10],ADC_InjectedConvertedValueTab[10]添加到watch窗口观察
        2、通过改变PA0 PA1引脚的电压，在每个CC2事件发生时转换一次规则和注入通道，变量储存在对应数组中。同时在PA8 PA11可以看到TIM1 CH1和CH4的 PWM 波形
4、注意事项
    当系统采用HSE时钟时（一般HSI也是打开的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8)可以配置为HSE或者HSI
    当系统采样HSI时钟时（一般HSE是关闭的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8)只能配置为HSI




1. Function description
   1. ADC samples PA0 regularly, automatically inject the analog voltage of PA1 pin to sample, and trigger sampling under TIM1 CC2 event
   2. The result of rule conversion is read into variable ADC_RegularConvertedValueTab[10] array through DMA_CH1 channel
       End into the transformation results by transforming the interrupt read into variable ADC_InjectedConvertedValueTab [10] array
2. Development environment
    Software development environment:   KEIL MDK-ARM V5.34.0.0
                                                                IAR EWARM 8.50.1
    Hardware environment: 
        the evaluation board:   N32G401C8L7-STB V1.0

3. How to use
    System configuration：
       1. Clock source:
              HSE=8M,PLL=72M,AHB=72M,APB1=18M,APB2=36M,ADC CLK=72M/16,ADC 1M CLK=HSE/8
       2. Interruption:
              ADC injection conversion results complete interrupt open, the priority group 2, preemption priority 0, sub priority 0
              Interrupt function will be injected into the result read ADC_InjectedConvertedValueTab [10] array, and flip PA6 level
       3. Port Configuration:
              PA0 is selected as the analog function ADC conversion channel
              PA1 is selected as the analog function ADC conversion channel
              PA6 is selected as general I/O output
              PA8 is selected as TIM1 CH1 PWM output
              PA11 is selected as TIM1 CH4 PWM output
       4. DMA:
              DMA_CH1 channel loopback mode carries the 10 half-word ADC regular channel conversion results into the ADC_RegularConvertedValueTab[10] array
       5. ADC:
              ADC TIM1 CC2 triggered, 12-bit data right aligned, regular conversion channel PA0, automatic injection of conversion channel PA1 analog voltage data
       6. TIM:
              TIM1 turns on the CH1 CH4 output, and CH2 is used to trigger the ADC conversion
    Usage:
       1. compiled to open the debug mode, variable ADC_RegularConvertedValueTab [10], ADC_InjectedConvertedValueTab [10] added to the watch window
       2. By changing the voltage of PA0 PA1 pin, the rule and injection channel are converted once for each CC2 event, and the variables are stored in the corresponding array. 
           Meanwhile, PWM waveforms of TIM1, CH1 and CH4  be seen in PA8, PA11.
4. Attention
       When the system uses the HSE clock (HSI is also normally turned on), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, 
       RCC_ADC1MCLK_DIV8) be configured as HSE or HSI.
       When the system samples the HSI clock (generally, HSE is disabled), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, 
       RCC_ADC1MCLK_DIV8)  only be set to HSI.