#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include "stm32f10x.h"

#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

/**
 * @brief LED初始化
 */
void LED_Init(void);

/**
 * @brief 点亮指定的LED
 *
 * @param led 指定的LED
 */
void LED_On(uint16_t led);

/**
 * @brief 熄灭指定的LED
 *
 * @param led 指定的LED
 */
void LED_Off(uint16_t led);

/**
 * @brief 切换指定的LED
 *
 * @param led 指定的LED
 */
void LED_Toggle(uint16_t led);

/**
 * @brief 点亮多个LED
 *
 * @param leds led的数组
 * @param len 数组长度
 */
void LED_OnAll(uint16_t *leds, uint8_t len);

/**
 * @brief 熄灭多个LED
 *
 * @param leds led的数组
 * @param len 数组长度
 */
void LED_OffAll(uint16_t *leds, uint8_t len);

#endif /* __LED_H__ */
