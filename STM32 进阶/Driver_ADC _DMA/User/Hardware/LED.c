#include "LED.h"

/**
 * @brief LED初始化
 */
void LED_Init()
{
    // 开启时钟，第2位置1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 配置PA0引脚的输出方式为推挽，最大输出速率50MHz,
    GPIOA->CRL &= ~GPIO_CRL_CNF0_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0;
    GPIOA->CRL |= GPIO_CRL_MODE0_1;
    GPIOA->CRL |= GPIO_CRL_MODE0_0;
    // 配置PA1引脚的输出方式为推挽，最大输出速率50MHz,
    GPIOA->CRL &= ~GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;
    GPIOA->CRL |= GPIO_CRL_MODE1_1;
    GPIOA->CRL |= GPIO_CRL_MODE1_0;
    // 配置PA8引脚的输出方式为推挽，最大输出速率50MHz,
    GPIOA->CRH &= ~GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
    GPIOA->CRH |= GPIO_CRH_MODE8_1;
    GPIOA->CRH |= GPIO_CRH_MODE8_0;

    // 默认三个LED全灭
    LED_Off(LED1);
    LED_Off(LED2);
    LED_Off(LED3);
}

/**
 * @brief 点亮指定的LED
 *
 * @param led 指定的LED
 */
void LED_On(uint16_t led)
{
    // 点亮LED
    GPIOA->ODR &= ~led;
}

/**
 * @brief 熄灭指定的LED
 *
 * @param led 指定的LED
 */
void LED_Off(uint16_t led)
{
    // 熄灭LED
    GPIOA->ODR |= led;
}

/**
 * @brief 切换指定的LED
 *
 * @param led 指定的LED
 */
void LED_Toggle(uint16_t led)
{
    // 切换LED
    if ((GPIOA->ODR & led) == 0)
    {
        LED_Off(led);
    }
    else
    {
        LED_On(led);
    }
}

/**
 * @brief 点亮多个LED
 *
 * @param leds led的数组
 * @param len 数组长度
 */
void LED_OnAll(uint16_t *leds, uint8_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        LED_On(leds[i]);
    }
}

/**
 * @brief 熄灭多个LED
 *
 * @param leds led的数组
 * @param len 数组长度
 */
void LED_OffAll(uint16_t *leds, uint8_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        LED_Off(leds[i]);
    }
}
