#include "Inf_M24C02.h"

void Inf_M24C02_Init(void)
{
    Driver_I2C_Init();
}

/**
 * 写入一个字节
 * uint8_t byte_addr: EEPROM内部字节地址  0-255
 * uint8_t ch: 要写入的字节数据
 */
void Inf_M24C02_WriteChar(uint8_t byte_addr, uint8_t ch)
{
    // 1. 启动信号
    Driver_I2C_Start();
    // 2. 写设备地址
    Driver_I2C_SendADDR(WRITE_ADDR);
    // 3. (不再需要接收ACK) -> 写字节地址
    Driver_I2C_SendChar(byte_addr);
    // 4. 写字节数据
    Driver_I2C_SendChar(ch);
    // 5. 发送停止信号
    Driver_I2C_Stop();
    // 在函数中加延时  外部调用就不需要加了
    Delay_ms(5);
}

/**
 * 读取一个字节
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * return : 返回读取的字节
 */
uint8_t Inf_M24C02_ReadChar(uint8_t byte_addr)
{
    // 1. 发启动信号 假写
    Driver_I2C_Start();
    // 2. 发送写设备地址
    Driver_I2C_SendADDR(WRITE_ADDR);
    // 3. 写字节地址
    Driver_I2C_SendChar(byte_addr);
    // 4. 发启动信号 真读
    Driver_I2C_Start();
    // 5. 发送读设备地址
    Driver_I2C_SendADDR(READ_ADDR);
    // 6. 设置回复NACK  -> 先设置需要回复的是NACK
    Driver_I2C_NACK();
    // 7. 设置发送停止信号 -> 先设置读完这个数据之后就停止信号
    Driver_I2C_Stop();
    // 8. 读数据  ->  推迟到最后
    return Driver_I2C_ReceiveChar();
}

/**
 * 一次写入多个字节  -> 按照底层16字节1page划分  每次写数据  只能在当前页写数据 -> 如果当前页写满  继续写入会从这一页的开头重新写数据
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * uint8_t *str: 需要写入的字符串
 * uint8_t len: 写入字符串的长度
 */
void Inf_M24C02_PageWrite(uint8_t byte_addr, uint8_t *str, uint8_t len)
{
    // 1. 发启动信号
    Driver_I2C_Start();
    // 2. 发写设备地址
    Driver_I2C_SendADDR(WRITE_ADDR);
    // 3. 发字节地址
    Driver_I2C_SendChar(byte_addr);
    // 4. 循环 -> 一直写数据
    for (uint8_t i = 0; i < len; i++)
    {
        Driver_I2C_SendChar(str[i]);
    }
    // 5. 发停止信号
    Driver_I2C_Stop();

    // 在函数中加延时  外部调用就不需要加了
    Delay_ms(5);
}

/**
 * 一次读取多个字节 -> 跨页读取数据的
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * uint8_t buff[]: 存放读取的数据
 * uint8_t len: 读取多少字节
 */
void Inf_M24C02_ReadString(uint8_t byte_addr, uint8_t buff[], uint8_t len)
{
    // 1. 发启动信号 假写
    Driver_I2C_Start();
    // 2. 发写设备地址
    Driver_I2C_SendADDR(WRITE_ADDR);
    // 3. 发字节地址
    Driver_I2C_SendChar(byte_addr);
    // 4. 发启动信号 真读
    Driver_I2C_Start();
    // 5. 发读设备地址
    Driver_I2C_SendADDR(READ_ADDR);
    // 6. 设置接下来读数据之后都回复ACK
    Driver_I2C_ACK();
    // 7. 循环 len-1次
    for (uint8_t i = 0; i < len - 1; i++)
    {
        buff[i] = Driver_I2C_ReceiveChar();
    }
    // 8. 先设置接下来读数据回复NACK
    Driver_I2C_NACK();
    // 9. 读完数据发停止信号
    Driver_I2C_Stop();
    // 10. 读最后一个字节
    buff[len - 1] = Driver_I2C_ReceiveChar();
}
