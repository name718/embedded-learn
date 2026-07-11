#include "Driver_systick.h"
#include "Delay.h"
int main(void)
{

	/* 使用系统滴答定时器的中断  来控制LED1 1s亮 1s灭 */
	// 初始化
	Driver_systick_Init();
	Driver_LED_Init();
	Driver_USART1_Init();
	Driver_LED_Off(LED1);
	Driver_LED_Off(LED2);
	Driver_LED_Off(LED3);

	printf("2\r\n");
	Delay_s(5);
	printf("3\r\n");
	while (1)
	{
	}
}
