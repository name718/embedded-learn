#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_DMA1_RX.h"
#include "string.h"
uint8_t DMA1_rx_buff[8] = {0};
int main(void)
{

	// 一定要先初始化USART1
	Driver_USART1_Init();
	printf("开始测试DMA\n");
	/* 使用DMA接收USART1_RX串口收到的数据  存储到数组中 */
	Driver_DMA1_RX_Init();
	// DMA的普通模式 => 根据写入数据长度的大小  =>  总共就传输对应的数据个数  之后就不工作了
	// DMA的循环  =>  根据写入数据长度的大小 => 决定存储到ARRAY中的下标位置  => 会在固定的几个位置不断循环 
	Driver_DMA1_RX_StartRecv((uint32_t) & (USART1->DR), (uint32_t)DMA1_rx_buff, 3);
	while (1)
	{
		if (recvFinished == 1)
		{
			recvFinished = 0;
			// 表示接收一轮数据  =>  可以使用数据
			printf("%s\n", DMA1_rx_buff);
		}

		Delay_ms(500);
	}
}
