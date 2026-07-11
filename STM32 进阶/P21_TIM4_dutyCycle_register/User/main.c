#include "Driver_USART1.h"
#include "Driver_tim5.h"
#include "Delay.h"
#include "Driver_tim4.h"
int main(void)
{

	Driver_USART1_Init();
	/* 使用通用定时器TIM5实现PWM方波的输出  再连接TIM4的通道1捕获信号测量周期频率 */
	Driver_TIM5_Init();
	Driver_TIM5_Start();

	Driver_TIM4_Init();
	Driver_TIM4_Start();
	Delay_s(1);
	double cycle, freq, dutyCycle;
	while (1)
	{
		cycle = Driver_TIM4_GetCycle();
		freq = Driver_TIM4_GetFreq();
		dutyCycle = Driver_TIM4_GetDutycycle();
		printf("cycle = %.4f,freq = %.4f,dutyCycle:%.4f\n", cycle, freq, dutyCycle);
		Delay_s(1);
	}
}
