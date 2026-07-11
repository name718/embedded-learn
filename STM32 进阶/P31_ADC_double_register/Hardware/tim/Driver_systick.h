#ifndef __DRIVER_SYSTICK_H
#define __DRIVER_SYSTICK_H


#include "Driver_LED.h"
#include "Driver_USART1.h"
/* 初始化系统滴答定时器  设置1s触发一次修改led1的亮灭 */
void Driver_systick_Init(void);

#endif
