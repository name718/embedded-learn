#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_DMA1.h"
#include "string.h"
uint8_t src[4] = {'a','b','c','d'};
int main(void)
{

	// 一定要先初始化USART1
	Driver_USART1_Init();
	printf("开始测试DMA\n");
	/* 使用DMA传输存储器数据到USART1_TX  发送数据到电脑串口软件  1s发1次 */
	Driver_DMA1_Init();

	// 使用DMA发送
	// Driver_DMA_Transmit((uint32_t)&src[0],(uint32_t)&(USART1->DR),strlen((char *)src));

	while (1)
	{
		Driver_DMA_Transmit((uint32_t)&src[0],(uint32_t)&(USART1->DR),strlen((char *)src));
		Delay_s(1);
	}
}
