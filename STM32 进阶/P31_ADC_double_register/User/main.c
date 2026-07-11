#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_ADC_double.h"

uint16_t double_adc_buff[2];

int main(void)
{

	/* 使用ADC1的通道10测量PC0引脚的电压 使用ADC1的通道12测量电源电压 打印到控制台 */
	Driver_USART1_Init();

	// 初始化有先后顺序
	Driver_ADC_double_Init();
	Driver_ADC_DMA_Init();
	Driver_ADC_DMA_Start((uint32_t)double_adc_buff, 2);
	while (1)
	{
		printf("v1:%.4f,v2:%.4f\n", double_adc_buff[0] * 3.3 / 4095,
			   double_adc_buff[1] * 3.3 / 4095);
		Delay_s(1);
	}
}
