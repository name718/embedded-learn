#ifndef __TIM6_H__
#define __TIM6_H__

#include "stm32f10x.h"
typedef void (*TIM6_Callback)(uint16_t);

void TIM6_Init(void);
void TIM6_SetCallback(TIM6_Callback, uint16_t p);

#endif /* __TIM6_H__ */
