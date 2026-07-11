#include "Driver_tim6.h"

void Driver_TIM6_Init(void)
{
    /* 要实现的效果: 1s的定时中断 -> 翻转led灯的引脚 */
    // 使用72MHz的时钟 -> 进行预分频  ->  固定时间累加1次
    // 预分频值: 7200-1  -> 得到时钟的频率 72M/7200 = 10000Hz  -> 1s/10000  得到100us的周期 =>  cnt+1
    // 1s触发一次中断 -> 1s / 100us = 10000  => ARR 10000-1

    /* 1. 开启TIM6时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 2. 配置TIM6参数 */
    // 2.1 预装载和单脉冲
    TIM6->CR1 &= ~TIM_CR1_ARPE;
    TIM6->CR1 &= ~TIM_CR1_OPM;

    // 2.2 预分频的值
    TIM6->PSC = 7200 - 1;

    // 2.3 重装载的值
    TIM6->ARR = 10000 - 1;

    // 2.4 开启中断
    TIM6->DIER |= TIM_DIER_UIE;

    // 2.5 NVIC设置中断优先级
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 1);
    NVIC_EnableIRQ(TIM6_IRQn);

    // 2.6 开启定时器
    TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM6_IRQHandler(void)
{
    /* 1. 清空中断标志位 */
    TIM6->SR &= ~TIM_SR_UIF;

    /* 2. 1s触发1次 翻转LED2*/
    Driver_LED_Toggle(LED2);

    printf("1\n");
}
