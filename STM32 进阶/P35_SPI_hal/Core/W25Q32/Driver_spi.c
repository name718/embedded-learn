#include "Driver_spi.h"

// ÖÐ¼äñîºÏ²ã
void Driver_SPI_Init(void)
{
    MX_SPI1_Init();
}

void Driver_SPI_Start(void)
{
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
}

void Driver_SPI_Stop(void)
{
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
}

uint8_t Driver_SPI_SwapByte(uint8_t ch)
{
    uint8_t rbyte = 0;
    HAL_SPI_TransmitReceive(&hspi1,&ch,&rbyte,1,1000);
    return rbyte;
}
