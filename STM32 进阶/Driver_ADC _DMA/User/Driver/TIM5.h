#ifndef __TIM5_H__
#define __TIM5_H__

#include "stm32f10x.h"
#include "Delay.h"

void TIM5_Init(void);
void TIM5_Start(void);
void TIM5_Stop(void);
void TIM5_SetDutyCycle(uint16_t dutyCycle);

#endif /* __TIM5_H__ */
