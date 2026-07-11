#include "Driver_spi.h"

void Driver_SPI_Init(void)
{
    /* 1. 打开时钟  SPI1时钟  GPIOA GPIOC */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 2. 配置引脚模式 */
    // SCK PA5 复用推挽输出1011
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5);
    GPIOA->CRL &= ~(GPIO_CRL_CNF5_0);
    // MISO PA6 浮空输入  0100
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= (GPIO_CRL_CNF6_0);
    // MOSI PA7 复用推挽输出1011
    GPIOA->CRL |= (GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7);
    GPIOA->CRL &= ~(GPIO_CRL_CNF7_0);
    // CS 片选线 PC13  通用推挽 0011
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    /* 3. 配置SPI1 */
    // 3.1 配置8位数据帧
    SPI1->CR1 &= ~SPI_CR1_DFF;
    // 3.2 配置软件从设备选择 => 手动拉高拉低片选线
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;
    SPI1->CR2 &= ~SPI_CR2_SSOE;
    // 3.3 高位优先
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // 3.4 设置波特率 选择8分频 010  9MHz
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_1;
    // 3.5 设置为主设备
    SPI1->CR1 |= SPI_CR1_MSTR;
    // 3.6 相位和极性都配置为0  => 选择SPI模式0
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    /* 4. 使能 */
    SPI1->CR1 |= SPI_CR1_SPE;
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
    // 对应SPI的时序  => 写缓存是交换数据之前写
    //          读缓存 => 交换数据完成之后再读的
    /* 1. 先写缓存  */
    while ((SPI1->SR & SPI_SR_TXE) == 0)
        ;
    SPI1->DR = ch;

    /* 2. 读缓存 */
    while ((SPI1->SR & SPI_SR_RXNE) == 0)
        ;
    return (uint8_t)SPI1->DR;
}
