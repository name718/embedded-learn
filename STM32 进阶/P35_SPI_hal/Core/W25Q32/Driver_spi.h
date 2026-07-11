#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "spi.h"

// 将移植过来的代码的函数方法全部保留  =>  功能 等效替换
void Driver_SPI_Init(void);

void Driver_SPI_Start(void);

void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t ch);

#endif
