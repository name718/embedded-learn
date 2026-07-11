#include "stm32f10x.h"
#include <stdint.h>
#include <stdio.h>
#include "Delay.h"
#include "USART.h"
#include "TIM5.h"
#include "DMA1.h"
#include "ADC1.h"

int main()
{
	uint16_t ADC1_Value = 0;
	USART1_Init();
	TIM5_Init();
	DMA1_ADC_Init();

	// ADC1_Init_Channel10();
	ADC1_Init_Channel12();
	DMA1_ADC_Receive((uint32_t *)&ADC1_Value, 1);
	TIM5_Start();
	ADC1_Start();

	while (1)
	{
		double voltage = ADC1_Value * 3.3 / 4095;
		// double source = ADC1_Value[1] * 3.3 / 4095;

		printf("Volteage = %.2lf,\n", voltage);
		// printf("source = %.2lf,\n", source);
		Delay_ms(1000);
	}
}
