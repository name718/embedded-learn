#include "Driver_Key.h"

void Driver_Key_Init(void)
{
    // 按键生成中断信号  ->  EXTI配置  ->  NVIC配置优先级 使能中断
    // PF10下拉输入
    // 1. 打开时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // 2. 配置引脚模式
    // cnf10   mode00
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    // 下拉输入  配置 ODR寄存器 -> 0
    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    // 3. 让PF10连接EXTI
    // AFIO_EXTICR3 -> EXTI10 -> PF 0101
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI10);
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    // 4. EXTI配置触发方式和不屏蔽
    // 4.1 配置当前中断为上升沿触发
    EXTI->RTSR |= EXTI_RTSR_TR10;
    // 先设置功能  最后打开使能
    // 4.2 打开不屏蔽
    EXTI->IMR |= EXTI_IMR_MR10;

    // 5. NVIC配置优先级 使能
    // 5.1 配置NVIC优先级组类型
    NVIC_SetPriorityGrouping(3);
    // 5.2 配置EXTI10的优先级  (0-15)
    NVIC_SetPriority(EXTI15_10_IRQn, 3);
    // 5.3 配置EXTI10中断使能
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// 中断服务函数  中断处理程序
void EXTI15_10_IRQHandler(void)
{
    // 1. 第一步一定是清除中断标志位
    // 要清除EXTI10的状态标志位 PR10  ->  对应位写1
    EXTI->PR |= EXTI_PR_PR10;

    // 2. 消抖
    Delay_ms(10);

    // 3. 再次判断SW5传入的电平PF10  为1 表示按下
    if (GPIOF->IDR & GPIO_IDR_IDR10)
    {
        // 实现对应功能
        Driver_LED_Toggle(LED1);
    }
    
}
