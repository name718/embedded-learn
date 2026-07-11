#include "TIM4.h"

void TIM4_Init(void)
{
    // 使能时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 配置引脚PB6浮空输入0100
    GPIOB->CRL |= GPIO_CRL_CNF6_0;
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);

    // 配置定时器
    TIM4->PSC = 71;    // 72MHz / 72 = 1MHz
    TIM4->ARR = 65535; // 1MHz / 65535 = 15.25Hz

    // 配置通道1
    // 不使用异或
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 通道1输入映射到TI1
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    // 不滤波
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 不分频
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 上升沿捕获
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 通道1使能
    TIM4->CCER |= TIM_CCER_CC1E;

    // 通道2配置
    // 通道2输入映射到TI1
    // 10：CC2通道被配置为输入，IC2映射在TI1上；
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    // 不滤波
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2F;
    // 不分频
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    // 下降沿捕获
    TIM4->CCER |= TIM_CCER_CC2P;
    // 通道2使能
    TIM4->CCER |= TIM_CCER_CC2E;

    // 定时器
    // 计数方向
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // 不使用单脉冲
    TIM4->CR1 &= ~TIM_CR1_OPM;

    // // 使能比较中断
    // TIM4->DIER |= TIM_DIER_CC1IE;
    // // 使能更新中断
    // TIM4->DIER |= TIM_DIER_UIE;

    // 从模式,101
    // 触发源：滤波后的定时器输入1(TI1FP1)
    TIM4->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0);
    TIM4->SMCR &= ~TIM_SMCR_TS_1;

    // 从模式触发后复位,100：复位模式
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~(TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1);
}

void TIM4_Start(void)
{
    // 使能定时器
    TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_Stop(void)
{
    // 关闭定时器
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

double TIM4_GetCycle_ms(void)
{
    return (TIM4->CCR1 + 1) / 1000.0;
}

double TIM4_GetFreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}

double TIM4_GetDuty(void)
{
    return (TIM4->CCR2 + 1.0) / (TIM4->CCR1 + 1.0);
}
