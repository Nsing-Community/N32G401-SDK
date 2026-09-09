1、功能说明
    1、ADC规则通道采样PA3、PA4引脚的模拟电压，注入通道采样PA0、PA1引脚的模拟电压
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
            中断处理接收注入转换结果到ADC_InjectedConvertedValueTab[10]数组
        3、端口配置：
            PA3选择为模拟功能ADC转换通道
            PA4选择为模拟功能ADC转换通道
            PA0选择为模拟功能ADC转换通道
            PA1选择为模拟功能ADC转换通道
            PA11选择为外部EXTI事件上升沿触发
            PA15选择为外部EXTI事件上升沿触发
        4、DMA：
            DMA_CH1通道循环模式搬运10个半字的ADC规则通道转换结果到ADC_RegularConvertedValueTab[10]数组
        5、ADC：
            ADC规则通道扫描间断模式、EXTI11触发、12位数据右对齐，转换通道PA3和PA4的模拟电压数据
            ADC注入通道扫描模式、EXTI15触发、12位数据右对齐，转换通道PA0和PA1的模拟电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADC_RegularConvertedValueTab[10],ADC_InjectedConvertedValueTab[10]添加到watch窗口观察
        2、通过PA11给上升沿可以触发规则通道数据采样，PA15给上升沿可以触发注入通道数据采样
4、注意事项
    当系统采用HSE时钟时（一般HSI也是打开的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8)可以配置为HSE或者HSI
    当系统采样HSI时钟时（一般HSE是关闭的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8)只能配置为HSI



1. Function description
    1. ADC regular channel samples the analog voltage of pins PA3 and PA4, and injection channel samples the analog voltage of pins PA0 and PA1
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
              Interrupt handling receiving injection conversion results to ADC_InjectedConvertedValueTab [10] array
       3. Port Configuration:
              PA3 is selected as the analog function ADC conversion channel
              PA4 is selected as the analog function ADC conversion channel
              PA0 is selected as the analog function ADC conversion channel
              PA1 is selected as the analog function ADC conversion channel
              PA11 is selected as an external EXTI event rising edge trigger
              PA15 is selected as an external EXTI event rising edge trigger
       4. DMA:
              DMA_CH1 channel loopback mode carries the 10 half-word ADC regular channel conversion results into the ADC_RegularConvertedValueTab[10] array
       5. ADC:
              ADC regular channel s intermittent mode, EXTI11 trigger, 12 bit data right alignment, conversion channel PA3 and PA4 analog voltage data
              ADC injection channel s mode, EXTI15 trigger, 12 bit data right alignment, conversion channel PA0 and PA1 analog voltage data
    Usage:
      1, compiled to open the debug mode, variable ADC_RegularConvertedValueTab [10], ADC_InjectedConvertedValueTab [10] added to the watch window
      2. Regular channel data sampling  be triggered by PA11 rising edge, and injection channel data sampling  be triggered by PA15 rising edge
4. Attention
       When the system uses the HSE clock (HSI is also normally turned on), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, 
       RCC_ADC1MCLK_DIV8) be configured as HSE or HSI.
       When the system samples the HSI clock (generally, HSE is disabled), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, 
       RCC_ADC1MCLK_DIV8)  only be set to HSI.