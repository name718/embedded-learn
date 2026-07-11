#ifndef __DRIVER_I2C
#define __DRIVER_I2C

#include "stm32f10x.h"

#define OK 0
#define FAIL 1

void Driver_I2C_Init(void);

/* I2C的启动停止信号 使用寄存器标记不断发送  ->  SR1中SB被硬件置位为1 表示发送完成 */
uint8_t Driver_I2C_Start(void);
void Driver_I2C_Stop(void);

/* 只有在读数据 接收数据的时候 才会使用到  如果收到的字节是最后一个  发NACK  如果不是最后一个 发ACK */
void Driver_I2C_ACK(void);
void Driver_I2C_NACK(void);

/* 发送设备地址 */
uint8_t Driver_I2C_SendADDR(uint8_t dev_addr);

/* 发送一个字节 */
uint8_t Driver_I2C_SendChar(uint8_t ch);

/* 读取一个字节 */
uint8_t Driver_I2C_ReceiveChar(void);

#endif
