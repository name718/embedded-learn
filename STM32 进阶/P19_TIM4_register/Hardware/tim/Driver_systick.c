#include "Driver_systick.h"

/**
 * (1) 凑出1s的概念
 * (2) 具体的条件控制led灯
 */
void Driver_systick_Init(void)
{
    // 自动时钟  ->  72MHz  ->  1/72 us
    // 72000000  -> 数值过大 不方便操作
    // 拆分计算的方法  ->  让滴答定时器的中断设置1ms  -> 72000

    /* 1. 填写重装载寄存器  循环触发  N-1 */
    SysTick->LOAD = 72000 - 1;

    /* 2. 控制状态寄存器  配置时钟  开启使能  开始时钟 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

uint16_t count = 0;

/* 实现中断服务函数  ->  控制LED灯的亮灭 */
void SysTick_Handler(void)
{
    // 1ms触发一次  ->  1000次为1s
    count++;
    if (count >= 1000)
    {
        Driver_LED_Toggle(LED1);
        count = 0;
        printf("1\n");
    }
}
