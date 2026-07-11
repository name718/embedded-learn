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

    /* 5. 定时器CR1 */
    // 5.1计数方向
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // 5.2 不使用单脉冲模式
    TIM4->CR1 &= ~TIM_CR1_OPM;

    /* 6. 开启中断 */
    TIM4->DIER |= TIM_DIER_UIE;
    TIM4->DIER |= TIM_DIER_CC1IE;

    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn, 2);
    NVIC_EnableIRQ(TIM4_IRQn);
}

void Driver_TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

// 第一次上升沿触发 -> 清空CNT的值  重新计数
// 第二次上升沿触发 -> 自动捕获CNT的值存到CCR1 读出对应的值

uint8_t raiseCount = 0;
uint16_t t = 0;

void TIM4_IRQHandler(void)
{
    /* 1. 判断中断标志位 */
    if (TIM4->SR & TIM_SR_UIF)
    {
        TIM4->SR &= ~TIM_SR_UIF;
        // 定时器溢出  表明测量不了当前的周期  因为周期太长
        // printf("定时器溢出\n");
    }
    else if (TIM4->SR & TIM_SR_CC1IF)
    {
        // 清空标志位
        TIM4->SR &= ~TIM_SR_CC1IF;
        raiseCount++;
        /* 2. 判断第几次上升沿触发中断 */
        if (raiseCount == 1)
        {
            // 第一次触发
            TIM4->CNT = 0;
        }
        else if (raiseCount == 2)
        {
            raiseCount = 0;
            // CNT的值会自动写入到CCR1里面
            // 读取CCR1的值更准
            t = TIM4->CCR1;
        }
    }
}

// 换算单位为ms
double Driver_TIM4_GetCycle(void)
{
    return (t + 1) / 1000.0;
}

// 使用s单位表示
double Driver_TIM4_GetFreq(void)
{
    return 1000000.0 / (t + 1);
}
