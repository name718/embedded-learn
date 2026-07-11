#include "IIC2.h"
// 硬件IIC2初始化
void IIC2_Init()
{
    // 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    // 引脚配置,复用开漏输出
    // CNF10[1:0]=11,MODE10[1:0]=11
    // CNF11[1:0]=11,MODE11[1:0]=11
    GPIOB->CRH |= (GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
    // I2C2配置
    I2C2->CR1 &= ~I2C_CR1_SMBUS; // I2C模式
    I2C2->CR2 &= ~I2C_CR2_FREQ;  // APB1时钟36MHz
    I2C2->CR2 |= 36;
    I2C2->CCR &= ~I2C_CCR_FS; // 标准模式
    I2C2->CCR &= ~I2C_CCR_CCR;
    I2C2->CCR |= 180;        // 100KHz
    I2C2->TRISE = 37;        // 1000ns
    I2C2->CR1 |= I2C_CR1_PE; // 使能I2C2
}

// 启始
uint8_t IIC2_Start()
{
    I2C2->CR1 |= I2C_CR1_START;
    uint16_t timeout = 0xFFFF;
    while (!(I2C2->SR1 & I2C_SR1_SB) && timeout)
    {
        timeout--;
    }
    
    return timeout ? 0 : 1;
}

// 停止
void IIC2_Stop()
{
    I2C2->CR1 |= I2C_CR1_STOP;
    // while (I2C1->CR1 & I2C_CR1_STOP)
    //     ;
}

// IIC2发送Addr
uint8_t IIC2_SendAddr(uint8_t addr)
{
    I2C2->DR = addr;
    uint16_t timeout = 0xFFFF;
    while (!(I2C2->SR1 & I2C_SR1_ADDR) && timeout)
    {
        timeout--;
    }
   if(timeout)
    {
        I2C2->SR2;   // 清除ADDR标志
    }

    return timeout ? 0 : 1;
}

// 发送一个字节
uint8_t IIC2_SendByte(uint8_t byte)
{
    //等待数据寄存器为空
    while (!(I2C2->SR1 & I2C_SR1_TXE))
        ;
    I2C2->DR = byte;
    //等待数据发送完成
    uint16_t timeout = 0xFFFF;
    while (!(I2C2->SR1 & I2C_SR1_BTF) && timeout)
    {
        timeout--;
    }

    return timeout ? 0 : 1;
}

// 接收一个字节
uint8_t IIC2_RecvByte()
{
    //等待数据接收完成
    uint16_t timeout = 0xFFFF;
    while (!(I2C2->SR1 & I2C_SR1_RXNE) && timeout)
    {
        timeout--;
    }
    uint8_t data = timeout ? I2C2->DR : 0;

    return data;
}

// 发送ACK
void IIC2_SendACK(uint8_t ack)
{
    if (!ack)
    {
        I2C2->CR1 |= I2C_CR1_ACK;  // 产生ACK
    }
    else
    {
        I2C2->CR1 &= ~I2C_CR1_ACK; // 产生NACK
    }
}
