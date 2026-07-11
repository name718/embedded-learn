#ifndef __ADC1_H__
#define __ADC1_H__

#include "stm32f10x.h"

void ADC1_Init(void);
void ADC1_Start(void);
void ADC1_Init_Channel10(void);
void ADC1_Init_Channel12(void);

double ADC1_GetVoltage(void);

#endif /* __ADC1_H__ */
