#include "TIM5.h"

void TIM5_Init(void)
{
    // 使能时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 配置引脚PA1复用推挽输出1011
    GPIOA->CRL |= (GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 配置定时器
    TIM5->PSC = 7199; // 72MHz / 7200 = 10KHz
    TIM5->ARR = 99;   // 10KHz / 100 = 100Hz

    // 占空比初始值
    TIM5->CCR2 = 50;

    // 配置PWM模式1
    // 通道2输出
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;
    // 极性：高电平有效
    TIM5->CCER &= ~TIM_CCER_CC2P;
    // 向上
    TIM5->CR1 &= ~TIM_CR1_DIR;
    // 关闭单脉冲
    TIM5->CR1 &= ~TIM_CR1_OPM;
    // 使能输出
    TIM5->CCER |= TIM_CCER_CC2E;
}

void TIM5_Start(void)
{
    // 使能定时器
    TIM5->CR1 |= TIM_CR1_CEN;
}

void TIM5_Stop(void)
{
    // 关闭定时器
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void TIM5_SetDutyCycle(uint16_t dutyCycle)
{
    TIM5->CCR2 = dutyCycle;
}

void TIM5_LED()
{
    static uint16_t ledDutyCycle = 10;
    static int8_t step = 1;
    if (ledDutyCycle == 1 || ledDutyCycle == 99)
    {
        step = -step;
    }
    ledDutyCycle += step;
    TIM5_SetDutyCycle(ledDutyCycle);
    Delay_ms(10);
}
