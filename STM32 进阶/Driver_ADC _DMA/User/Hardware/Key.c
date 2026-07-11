#include "key.h"

void Key_SW5_Init(void)
{
    // 设置引脚时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // 设置引脚为上下拉模式
    GPIOF->CRH |= GPIO_CRH_CNF10_1; // 配置CNF10[1:0]=10
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    // 设置引脚为输入模式
    GPIOF->CRH &= ~GPIO_CRH_MODE10; // 配置MODE10[1:0]=00
    // 设置引脚默认输入0
    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    // 设置外部中断线
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;   // 配置EXTI10[3:0]=0000
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF; // 配置EXTI10[3:0]=0101

    // 设置EXTI
    EXTI->RTSR |= EXTI_RTSR_TR10; // 上升沿触发
    EXTI->IMR |= EXTI_IMR_MR10;   // 开放中断线10

    // 设置中断优先级(NVIC)
    NVIC_SetPriorityGrouping(3);         // 设置优先级分组
    NVIC_SetPriority(EXTI15_10_IRQn, 4); // 设置EXTI15_10优先级
    NVIC_EnableIRQ(EXTI15_10_IRQn);      // 使能EXTI15_10中断
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR10) // 判断是否是中断线10产生的中断
    {
        EXTI->PR = EXTI_PR_PR10; // 清除中断标志
        // 执行中断服务函数
        Delay_ms(100);
        if (GPIOF->IDR & GPIO_IDR_IDR10)
        {
            LED_Toggle(LED1);
        }
    }
}
