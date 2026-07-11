#include "Driver_tim4.h"

void Driver_TIM4_Init(void)
{
    /* 1. 打开时钟  TIM4  PB */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    /* 2. 配置引脚模式 PB6 浮空输入 0100 */
    GPIOB->CRL |= GPIO_CRL_CNF6_0;
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    /* 3. 配置时基单元 */
    // 输入捕获的预分频尽量小 1us 精度高
    TIM4->PSC = 72 - 1;
    // 重装载要尽量的大  影响最大的测量范围
    TIM4->ARR = 65535;
    /* 4. 配置通道1 */
    // 接线选择
    // 4.1 通道1的接线  不使用异或功能
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 4.2 IC1的接线模式选择TI1直连 CCMR1_CC1S 01
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    // 功能选择
    // 4.3 不使用滤波
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 4.4 不使用分频
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 通道使能
    // 4.5 不反向
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 4.6 通道使能
    TIM4->CCER |= TIM_CCER_CC1E;

    // 配置通道2
    // 接线选择
    // 4.7 使用IC2的非直连 10  连接到TI1
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    // 功能选择
    // 4.8 不使用滤波
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2F;
    // 4.9 不使用分频
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    // 4.10 选择极性  需要下降沿
    TIM4->CCER |= TIM_CCER_CC2P;
    // 4.11 使能通道2
    TIM4->CCER |= TIM_CCER_CC2E;

    /* 5. 定时器CR1 */
    // 5.1计数方向
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // 5.2 不使用单脉冲模式
    TIM4->CR1 &= ~TIM_CR1_OPM;

    // 开启从模式的复位功能  => 自动完成CNT的清空   和   CNT的捕获功能
    // 6.1 从模式触发信号选择  TI1FP1  TS=101
    TIM4->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0);
    TIM4->SMCR &= ~(TIM_SMCR_TS_1);
    // 6.2 从模式触发之后完成CNT的复位
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~(TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1);
}

void Driver_TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

// CCR1中存的值: 第一次上升沿清0  => 第二次上升沿捕获  = 周期
// CCR2中存的值: 第一次上升沿清0  => 第一次下降沿捕获  = 高电平的时间
// 换算单位为ms
double Driver_TIM4_GetCycle(void)
{
    return (TIM4->CCR1 + 1) / 1000.0;
}

// 使用s单位表示
double Driver_TIM4_GetFreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}

double Driver_TIM4_GetDutycycle(void)
{
    return (TIM4->CCR2 + 1) * 1.0 / (TIM4->CCR1 + 1);
}
