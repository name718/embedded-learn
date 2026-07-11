#ifndef __DRIVER_ADC_DOUBLE_H
#define __DRIVER_ADC_DOUBLE_H

#include "stm32f10x.h"

void Driver_ADC_double_Init(void);

void Driver_ADC_DMA_Init(void);

// DMA数据传输的方向:  从外设读  写到存储器
void Driver_ADC_DMA_Start(uint32_t desAddr,uint16_t dataLen);

#endif
