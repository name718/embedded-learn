#include "Driver_LED.h"

void Driver_LED_Init(void)
{
    // 1. 配置时钟和引脚模式
    // |= 写1   &= !() 写0
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 配置PA0 PA1 PA8 引脚模式  -> 通用推挽  0011
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH &= ~(GPIO_CRH_CNF8);

    // 2. 设置初始状态  -> 关闭所有灯
    // 关闭led
    GPIOA->ODR |= GPIO_ODR_ODR0;
    GPIOA->ODR |= GPIO_ODR_ODR1;
    GPIOA->ODR |= GPIO_ODR_ODR8;
}

void Driver_LED_On(uint16_t led)
{
    // 对应的odr值置为0
    GPIOA->ODR &= ~led;
}
void Driver_LED_Off(uint16_t led)
{
    // 对应的odr值置为1
    GPIOA->ODR |= led;
}

void Driver_LED_Toggle(uint16_t led)
{
    // 读取对应的状态  ->  读ODR的值和IDR的值都可以
    if (GPIOA->IDR & led)
    {
        // 大于0 -> 对应的LED值为1  ->  关的状态
        Driver_LED_On(led);
    }
    else
    {
        // 等于0 -> 对应的LED值为0  ->  开的状态
        Driver_LED_Off(led);
    }
}

void Driver_LED_OnAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_On(leds[i]);
    }
    
}
void Driver_LED_OffAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_Off(leds[i]);
    }
    
}
