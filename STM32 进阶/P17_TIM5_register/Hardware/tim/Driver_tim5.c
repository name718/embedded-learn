#include "Driver_tim5.h"

void Driver_TIM5_Init(void)
{
    /* 1. 开时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. 配置引脚模式  PA1 复用推挽输出  1011 */
    GPIOA->CRL |= (GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1_0);

    /* 3. 配置定时器时基单元 */
    // 3.1 预分频和重装载  
    TIM5->PSC = 7199; // 100us
    TIM5->ARR = 99;  // 10ms

    // 3.2 填写占空比的初始值
    TIM5->CCR2 = 95;
    /* 4. 选择定时器中的通道2 */
    // 4.1 选择通道2为输出功能
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    // 4.2 选择输出模式  PWM1  OC1M  110
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR1 &= ~(TIM_CCMR1_OC2M_0 );
    // 4.3 选择有效极性  高电平，0为高电平
    TIM5->CCER &= ~TIM_CCER_CC2P;
    // 4.2 使能通道2,输出功能开启，置为1
    TIM5->CCER |= TIM_CCER_CC2E;

    /* 5. 配置控制*/
    // 5.1 计数方向
    TIM5->CR1 &= ~TIM_CR1_DIR;
    
    // 5.2 不使用单脉冲
    TIM5->CR1 &= ~TIM_CR1_OPM;
    // 5.3 使能
}

void Driver_TIM5_Start(void)
{
    //打开使能开关
    TIM5->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM5_Stop(void)
{
    //关闭使能开关
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void Driver_TIM5_SetDutyCycle(uint16_t dutyCycle)
{
    TIM5->CCR2 = dutyCycle;
}
