#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "stm32f10x.h"
#include "Delay.h"
// ºê¶¨Òå
#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// ¶ÁMISOÏß
#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

#define SPI_DELAY Delay_us(5)

void Driver_SPI_Init(void);

void Driver_SPI_Start(void);

void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t ch);

#endif
