#ifndef __TIM4_H__
#define __TIM4_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "USART.h"

void TIM4_Init(void);
void TIM4_Start(void);
void TIM4_Stop(void);
double TIM4_GetFreq(void);
double TIM4_GetCycle_ms(void);
double TIM4_GetDuty(void);
#endif /* __TIM4_H__ */
