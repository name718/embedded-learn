#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>
#include "stm32f10x.h"

// 指定延时多少微秒
void Delay_us(uint16_t us);

// 指定延时多少毫秒
void Delay_ms(uint16_t ms);

// 指定延时多少秒
void Delay_s(uint16_t s);

#endif /* __DELAY_H__ */
