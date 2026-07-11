#ifndef __IIC2_H__
#define __IIC2_H__

#include "stm32f10x.h"

// IIC2初始化
void IIC2_Init(void);
// IIC2启始
uint8_t IIC2_Start(void);
// IIC2停止
void IIC2_Stop(void);
// IIC2发送Addr
uint8_t IIC2_SendAddr(uint8_t);
// IIC2发送一个字节
uint8_t IIC2_SendByte(uint8_t);
// IIC2接收一个字节
uint8_t IIC2_RecvByte(void);
// 发送ACK
void IIC2_SendACK(uint8_t);

#endif /* __IIC2_H__ */
