#include "ADC1.h"

#include <stdio.h>

static uint8_t ADC1_Channel_Num = 0;

void ADC1_Init(void)
{
#ifndef __ADC_INIT__
#define __ADC_INIT__
    // 使能ADC1时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 6分频，72M/6=12M
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // ADC1配置
    ADC1->CR1 |= ADC_CR1_SCAN;   // 扫描模式
    ADC1->CR2 &= ~ADC_CR2_ALIGN; // 右对齐
    ADC1->CR2 |= ADC_CR2_CONT;   // 连续转换模式
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1; // 13.5周期

    ADC1->CR2 |= ADC_CR2_EXTTRIG; // 允许外部触发
    ADC1->CR2 |= ADC_CR2_EXTSEL;  // 外部触发选择：SWSTART

    // DMA
    ADC1->CR2 |= ADC_CR2_DMA; // DMA模式
#endif
    return;
}

void ADC1_Init_Channel10(void)
{
    ADC1_Init();
    ADC1_Channel_Num++;
    // PC0模拟输入
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;
    GPIOC->CRL &= ~GPIO_CRL_MODE0;

    ADC1->SQR3 &= ~(0x1f << ((ADC1_Channel_Num - 1) * 5)); // 清空原来的通道
    ADC1->SQR3 |= 10 << ((ADC1_Channel_Num - 1) * 5);      // 通道10
    ADC1->SQR1 &= ~ADC_SQR1_L;                             // 清空原来的通道
    ADC1->SQR1 |= (ADC1_Channel_Num - 1) << 20;            // ADC1_Channel_Num个转换
}

void ADC1_Init_Channel12(void)
{
    ADC1_Init();
    // PC2模拟输入 0000
    ADC1_Channel_Num++;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRL &= ~GPIO_CRL_CNF2;
    GPIOC->CRL &= ~GPIO_CRL_MODE2;

    ADC1->SQR3 &= ~(0x1f << ((ADC1_Channel_Num - 1) * 5)); // 清空原来的通道
    ADC1->SQR3 |= 12 << ((ADC1_Channel_Num - 1) * 5);      // 通道12
    ADC1->SQR1 &= ~ADC_SQR1_L;                             // 清空原来的通道
    ADC1->SQR1 |= (ADC1_Channel_Num - 1) << 20;            //  ADC1_Channel_Num个转换
    // printf("ADC1_Channel_Num = %d\n", ADC1_Channel_Num);
    // printf("ADC1->SQR3 = %x\n", ADC1->SQR3);
    // printf("ADC1->SQR1 = %x\n", ADC1->SQR1);
}

void ADC1_Start(void)
{
    // 上电
    ADC1->CR2 |= ADC_CR2_ADON;
    // 校准
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    // 开始转换
    // ADC1->CR2 |= ADC_CR2_ADON;  //上电触发
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC))
        ;
}

double ADC1_GetVoltage(void)
{
    return (double)ADC1->DR * 3.3 / 0xfff;
}
