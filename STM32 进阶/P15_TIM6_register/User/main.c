#include "Driver_tim6.h"
#include "Driver_USART1.h"
int main(void)
{

	/* 使用基本定时器TIM6  来控制LED2 1s亮 1s灭 */
	// 初始化
	Driver_TIM6_Init();
	Driver_LED_Init();
	Driver_USART1_Init();
	Driver_LED_Off(LED1);
	Driver_LED_Off(LED2);
	Driver_LED_Off(LED3);

	while (1)
	{
	}
}
