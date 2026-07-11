#include "TIM1.h"

void TIM1_Init(void)
{
    // 使能时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 配置引脚PA8复用推挽输出1011
    GPIOA->CRH &= ~(GPIO_CRH_CNF8);
    GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1;

    // 配置定时器
    // 输出5个2HZ，占空比为50%的脉冲
    TIM1->PSC = 35999; // 72MHz / 36000 = 2KHz
    TIM1->ARR = 999;   // 2KHz / 1000 = 2Hz
    TIM1->RCR = 4;     // 5个脉冲
    TIM1->CCR1 = 500;  // 50%占空比

    // 配置PWM模式1
    // 通道1输出
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // PWM模式1：OC1M = 110
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    // 高电平有效
    TIM1->CCER &= ~TIM_CCER_CC1P;
    // 向上计数
    TIM1->CR1 &= ~TIM_CR1_DIR;
    // 关闭单脉冲模式
    TIM1->CR1 &= ~TIM_CR1_OPM;

    // 手动更新
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;

    // 使能输出
    TIM1->CCER |= TIM_CCER_CC1E;
    // 输出使能
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 使能更新中断
    TIM1->DIER |= TIM_DIER_UIE;

    // 使能定时器中断
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 2);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void TIM1_Start(void)
{

    // 使能定时器
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM1_Stop(void)
{
    // 关闭定时器
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

void TIM1_SetDutyCycle(uint16_t dutyCycle)
{
    TIM1->CCR1 = dutyCycle;
}

void TIM1_UP_IRQHandler(void)
{
    // 清除中断标志
    TIM1->SR &= ~TIM_SR_UIF;

    TIM1_Stop();
}
