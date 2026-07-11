#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_tim1.h"
int main(void)
{

	Driver_USART1_Init();
	/* 使用TIM1输出固定5个PWM方波  => 控制LED3闪烁5次 */
	printf("TIM1初始化开始\n");
	Driver_TIM1_Init();
	printf("TIM1初始化完成\n");
	Driver_TIM1_Start();
	printf("TIM1启动完成完成\n");

	while (1)
	{
	}
}
