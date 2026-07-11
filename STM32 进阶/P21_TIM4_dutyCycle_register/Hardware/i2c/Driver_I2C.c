#include "Driver_I2C.h"

void Driver_I2C_Init(void)
{
    /* 1. 打开引脚和片上外设的时钟 I2C2 PB10 PB11 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    /* 2. 配置引脚模式 PB10 复用开漏输出 1111   PB11  复用开漏输出 */
    GPIOB->CRH |= (GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11);

    /* 3. 配置I2C的寄存器 */
    // 3.1 选择使用I2C模式 0   选择使用SMBUS模式 1
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    // 3.2 配置接入时钟频率
    I2C2->CR2 &= ~I2C_CR2_FREQ;
    I2C2->CR2 |= 36;

    // 3.3 选择I2C的速度模式 标准模式100K/S 0   快速模式 400K/S 1
    I2C2->CCR &= ~I2C_CCR_FS;
    I2C2->CCR &= ~I2C_CCR_CCR;
    // 3.4 选择分频系数  -> 计算方法    CCR *  1 / 设置的时钟频率(36) = 5   CCR = 180
    I2C2->CCR |= 180;

    // 3.5 设置上升沿时间周期  ->  等于标准模式中最大允许SCL上升时间为1us 对应的周期+1 = 36+1 = 37
    I2C2->TRISE &= ~I2C_TRISE_TRISE;
    I2C2->TRISE |= 37;

    // 3.2 打开I2C2总开关  ->  配置的最后打开
    I2C2->CR1 |= I2C_CR1_PE;
}

/* I2C的启动停止信号 使用寄存器标记不断发送  ->  SR1中SB被硬件置位为1 表示发送完成 */
uint8_t Driver_I2C_Start(void)
{
    // 1. 置位CR1 产生启动信号
    I2C2->CR1 |= I2C_CR1_START;

    // 2. 设置超时次数 0xffff
    uint16_t timeout = 0xffff;
    // 每次判断是否起始信号发送成功都记录一次
    // 超过timeout次数  认为发送起始信号失败
    while (timeout && ((I2C2->SR1 & I2C_SR1_SB) == 0))
    {
        timeout--;
    }

    // if (timeout)
    // {
    //     // 正常响应
    //     return OK;
    // }
    // else
    // {
    //     // 超时响应
    //     return FAIL;
    // }

    return timeout > 0 ? OK : FAIL;
}
// 调用的时候注意  在当前收发完成之后 会自动发停止位
void Driver_I2C_Stop(void)
{
    // 置位CR1 stop
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Driver_I2C_ACK(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}
void Driver_I2C_NACK(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/* 发送设备地址 */
uint8_t Driver_I2C_SendADDR(uint8_t dev_addr)
{
    // 发送设备地址的流程 在 启动信号之后紧接的位置  不会在次之前发数据
    I2C2->DR = dev_addr;

    // 等待发送完成  SR1 ADDR = 1
    uint16_t timeout = 0xffff;
    while (timeout && (I2C2->SR1 & I2C_SR1_ADDR) == 0)
    {
        timeout--;
    }

    // 重新置位ADDR地址发送成功位   给下一次发送地址做准备
    if (timeout > 0)
    {
        I2C2->SR2;
    }

    return timeout > 0 ? OK : FAIL;
}

/* 发送一个字节 向DR赋值*/
uint8_t Driver_I2C_SendChar(uint8_t ch)
{
    uint16_t timeout = 0xffff;
    // 1. 判断状态  当前是否空闲能发信的数据   TXE = 1
    while (timeout && (I2C2->SR1 & I2C_SR1_TXE) == 0)
    {
        timeout--;
    }
    if (timeout > 0)
    {
        // 2. 发送数据
        I2C2->DR = ch;
        timeout = 0xffff;
        // 3. 判断状态  第二步发送的数据完成啦    BTF = 1
        while (timeout && ((I2C2->SR1 & I2C_SR1_BTF) == 0))
        {
            timeout--;
        }
    }
    return timeout > 0 ? OK : FAIL;
}

/* 读取一个字节 从DR读值*/
uint8_t Driver_I2C_ReceiveChar(void)
{
    // return 返回接收的数据
    uint16_t timeout = 0xffff;
    // 1. 判断状态RXNE = 1 说明有数据可以读
    while (timeout && ((I2C2->SR1 & I2C_SR1_RXNE) == 0))
    {
        timeout--;
    }

    return timeout > 0 ? I2C2->DR : 0;
}
