#include "Driver_USART1.h"
#include "Driver_tim5.h"
#include "Delay.h"
int main(void)
{

	/* 使用通用定时器TIM5实现PWM方波的输出  控制LED2实现呼吸灯 */
	Driver_TIM5_Init();
	Driver_TIM5_Start();

	// 随着时间 动态修改占空比  实现呼吸灯的效果
	// uint16_t dutyCycle = 10;
	// uint8_t dir = 0; // 0表示占空比增长   1表示占空比下降

	// while (1)
	// {
	// 	if (dir == 0)
	// 	{
	// 		// 现在是增长
	// 		dutyCycle++;
	// 		// 初始的方向增长
	// 		if (dutyCycle == 98)
	// 		{
	// 			dir = 1;
	// 		}
	// 	}
	// 	else if (dir == 1)
	// 	{
	// 		// 占空比下降
	// 		dutyCycle--;
	// 		if (dutyCycle == 1)
	// 		{
	// 			dir = 0;
	// 		}
	// 	}
	// 	Driver_TIM5_SetDutyCycle(dutyCycle);
	// 	Delay_ms(10);
	// }

	// 第二种方法实现呼吸灯
	uint16_t dutyCycle = 10;
	int8_t step = 1; // 1表示每次增加1   -1表示每次减少1
	while (1)
	{
		if (dutyCycle == 99 || dutyCycle == 1)
		{
			step = -step;
		}
		dutyCycle += step;
		Driver_TIM5_SetDutyCycle(dutyCycle);
		Delay_ms(10);
	}
}
