#include "Delay.h"

void Delay_us(uint16_t us)
{
	/* 1. 控制1us -> 重装载的值  -> 72 */
	SysTick->LOAD = 72 * us;
	// 补充 -> 手动清空value值
	SysTick->VAL = 0;

	/* 2. 修改ctrl 使能系统滴答定时器 */
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE; // 72MHz
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT;	 // 关闭中断
	SysTick->CTRL |= SysTick_CTRL_ENABLE;

	/* 3. 等待1us -> 判断标志 -> countflag */
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
		;

	/* 4. 关闭滴答定时器 */
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}
void Delay_s(uint16_t s)
{
	while (s--)
	{
		Delay_ms(1000);
	}
}
