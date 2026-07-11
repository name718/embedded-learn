#include "Driver_ADC_double.h"

void Driver_ADC_double_Init(void)
{
    /* 1. 打开时钟 ADC  PC  ADC预分频 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 实现6分频
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 配置引脚模式 PC0  PC2  模拟输入 0000
    GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOC->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);

    /* 2. CR1 CR2基础设置 */
    // 2.1 需要使用扫描
    ADC1->CR1 |= ADC_CR1_SCAN;
    // 2.2 连续测量
    ADC1->CR2 |= ADC_CR2_CONT;

    // 2.3 使用右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 2.4 采样时间 010 10通道和12通道
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0 | ADC_SMPR1_SMP12_2 | ADC_SMPR1_SMP12_0);
    ADC1->SMPR1 |= (ADC_SMPR1_SMP10_1 | ADC_SMPR1_SMP12_1);

    /* 3. 设置序列 */
    // 3.1 设置序列的通道个数
    ADC1->SQR1 &= ~(ADC_SQR1_L);
    ADC1->SQR1 |= (ADC_SQR1_L_0);

    // 3.2 在序列的第一个位置 配置通道10
    ADC1->SQR3 &= ~(ADC_SQR3_SQ1);
    ADC1->SQR3 |= 10 << 0;
    // 3.3 在序列的第二个位置 配置通道12
    ADC1->SQR3 &= ~(ADC_SQR3_SQ2);
    ADC1->SQR3 |= 12 << 5;
}

void Driver_ADC_DMA_Init(void)
{
    /* 1. 开时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. 配置DMA参数 */
    // 2.1 不使用存储器到存储器
    DMA1_Channel1->CCR &= ~DMA_CCR1_MEM2MEM;
    // 2.2 数据宽度 16位 01
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;

    // 2.3 设置地址自增  存储器自增  外设不自增
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;

    // 2.4 循环模式
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 2.5 传输方向
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // DMA的配置是双向的  需要打开ADC的DMA功能
    ADC1->CR2 |= ADC_CR2_DMA;
}

// DMA数据传输的方向:  从外设读  写到存储器
void Driver_ADC_DMA_Start(uint32_t desAddr, uint16_t dataLen)
{
    /* 1. 启动DMA */
    DMA1_Channel1->CMAR = desAddr;
    DMA1_Channel1->CPAR = (uint32_t) & (ADC1->DR);
    DMA1_Channel1->CNDTR = dataLen;
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    /* 2. 启动ADC */
    // 2.1 上电  0->1 上电唤醒
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2.2 AD校验
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    // 2.3 启动转换  二次上电 开启转换
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2.4 等待第一次转换完成
    while ((ADC1->SR & ADC_SR_EOC) == 0)
        ;
}
