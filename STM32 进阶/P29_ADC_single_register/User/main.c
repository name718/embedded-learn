#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_ADC_single.h"

int main(void)
{

	/* 使用ADC1的通道10测量PC0引脚的电压  打印到控制台 */
	Driver_USART1_Init();
	
	Driver_ADC_Single_Init();
	Driver_ADC_Single_Start();
	double v;
	while (1)
	{
		v = Driver_ADC_Single_ReadV();
		printf("电压为%.4f\n",v);

		Delay_ms(500);
	}
}
