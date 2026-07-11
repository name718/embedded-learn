#include "Driver_USART1.h"
#include "Delay.h"
#include "Inf_W25Q32.h"



int main(void)
{

	/* 使用SPI协议通信  实现W25Q32的读写操作 */
	Driver_USART1_Init();
	Inf_W25Q32_Init();
	uint8_t did = 0;
	uint16_t mid = 0;
	Inf_W25Q32_ReadId(&did, &mid);
	printf("did:%d,mid:%d\n", did, mid);

	// /* 1. 擦除扇区  块 0-63 扇区 0-15 */
	// Inf_W25Q32_SectorErase(0,1);

	// /* 2. 写入数据 page 0-15*/
	// Inf_W25Q32_WritePage(0,1,1,"hello mengge",12);
	
	/* 3. 读取数据 */
	uint8_t flash_buff[20] = {0};
	Inf_W25Q32_ReadData(0,1,1,flash_buff,12);

	printf("%s\n",flash_buff);

	while (1)
	{
	}
}
