#include "TIM6.h"

static TIM6_Callback callback = 0;
static uint16_t P;

void TIM6_Init(void)
{
    // 使能TIM6时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->CR1 &= ~TIM_CR1_ARPE; // 关闭自动重装载缓存
    TIM6->CR1 &= ~TIM_CR1_OPM;  // 关闭单脉冲模式, 计数器不停止

    TIM6->PSC = 7200 - 1; // 预分频系数
    TIM6->ARR = 5000 - 1; // 自动重装载值

    TIM6->DIER |= TIM_DIER_UIE; // 使能更新中断

    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 1);
    NVIC_EnableIRQ(TIM6_IRQn);

    TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM6_SetCallback(TIM6_Callback cb_TIM6, uint16_t p)
{
    callback = cb_TIM6;
    P = p;
}

void TIM6_IRQHandler(void)
{
    if (TIM6->SR & TIM_SR_UIF)
    {
        TIM6->SR &= ~TIM_SR_UIF;
        if (callback)
        {
            callback(P);
        }
    }
}
