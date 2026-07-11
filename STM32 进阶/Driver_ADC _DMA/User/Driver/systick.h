#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "LED.h"

void systick_init(void);
void systick_callback_register(void (*func)(uint16_t), uint16_t Led);

#endif /* __SYSTICK_H__ */
