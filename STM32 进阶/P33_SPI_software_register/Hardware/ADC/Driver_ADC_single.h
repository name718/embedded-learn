#ifndef __DRIVER_ADC_SINGLE_H
#define __DRIVER_ADC_SINGLE_H

#include "stm32f10x.h"

void Driver_ADC_Single_Init(void);

void Driver_ADC_Single_Start(void);

double Driver_ADC_Single_ReadV(void);

#endif
