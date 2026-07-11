#include "Delay.h"

// 指定延时多少微秒
void Delay_us(uint16_t us)
{
	// 设置倒计数的起始值
	SysTick->LOAD = 72 * us;
	SysTick->VAL = 0;

	// 配置滴答定时器，选择系统时钟置1、关闭中断置0、开始计数置1
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT;
	SysTick->CTRL |= SysTick_CTRL_ENABLE;

	// 循环，计数计到0停下来, 如果 COUNTFLAT标志位是0就继续循环，直到是1
	while (((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0) && (SysTick->CTRL & SysTick_CTRL_ENABLE))
		;

	// 关闭定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

// 指定延时多少毫秒
void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}

// 指定延时多少秒
void Delay_s(uint16_t s)
{
	while (s--)
	{
		Delay_ms(1000);
	}
}
