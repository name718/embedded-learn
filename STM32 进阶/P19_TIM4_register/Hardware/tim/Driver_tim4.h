#ifndef __DRIVER_TIM4_H
#define __DRIVER_TIM4_H


#include "Driver_USART1.h"
void Driver_TIM4_Init(void);

void Driver_TIM4_Start(void);

void Driver_TIM4_Stop(void);

// 换算单位为ms
double Driver_TIM4_GetCycle(void);

// 使用s单位表示
double Driver_TIM4_GetFreq(void);

#endif
