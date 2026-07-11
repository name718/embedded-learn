#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "stm32f10x.h"
#include "Delay.h"
// ºê¶¨Òå
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)



void Driver_SPI_Init(void);

void Driver_SPI_Start(void);

void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t ch);

#endif
