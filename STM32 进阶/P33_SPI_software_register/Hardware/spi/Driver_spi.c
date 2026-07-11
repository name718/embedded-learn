#include "Driver_spi.h"

void Driver_SPI_Init(void)
{
    /* 1. 打开GPIO引脚时钟 PA PC*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 2. 配置引脚模式 */
    // 0011  PA5 SCK 输出  PA7 MOSI 输出   PC13 CS 输出
    GPIOA->CRL &= ~(GPIO_CRL_CNF5);
    GPIOA->CRL |= (GPIO_CRL_MODE5);

    GPIOA->CRL &= ~(GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE7);

    GPIOC->CRH &= ~(GPIO_CRH_CNF13);
    GPIOC->CRH |= (GPIO_CRH_MODE13);
    // 0100 PA6 MISO 浮空输入
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= (GPIO_CRL_CNF6_0);

    /* 3. 初始化  默认不片选的   默认时钟空闲 */
    CS_HIGH;
    SCK_LOW;
}

void Driver_SPI_Start(void)
{
    CS_LOW;
}

void Driver_SPI_Stop(void)
{
    CS_HIGH;
}

uint8_t Driver_SPI_SwapByte(uint8_t ch)
{
    uint8_t rbyte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        // 上升沿之前  => 准备需要发送的数据
        if (ch & 0x80)
        {
            // 最到位为1
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }

        // 写数据的移位在写之后
        ch <<= 1;

        // 上升沿触发发送
        SCK_HIGH;

        rbyte <<= 1;
        // 高电平时机  =>  读取别人给你发送的数据
        if (MISO_READ)
        {
            // 读取到高电平  => 存放到rbyte
            rbyte |= 1;
        }
        SCK_LOW;
    }

    return rbyte;
}
