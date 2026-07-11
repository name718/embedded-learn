#ifndef __DRIVER_LED
#define __DRIVER_LED

#include "stm32f10x.h"

#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8


/* 初始化硬件LED灯  ->  才能调用下列方法操作led灯
*/
void Driver_LED_Init(void);

void Driver_LED_On(uint16_t led);
void Driver_LED_Off(uint16_t led);
void Driver_LED_Toggle(uint16_t led);

void Driver_LED_OnAll(uint16_t leds[], uint8_t size);
void Driver_LED_OffAll(uint16_t leds[], uint8_t size);



#endif
