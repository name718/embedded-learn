#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "stdio.h"

// #define USART1_TX GPIO_ODR_ODR_9
// #define USART1_RX GPIO_IDR_IDR_10

typedef void (*USART1_RX_Callback)(uint8_t *, uint8_t);

// 初始化串口
void USART1_Init(void);
// 初始化串口中断
void USART1_Init_IT(void);
// 初始化串口DMA
void USART1_Init_DMA(void);
// 发送一个字节
void USART1_TransmitByte(uint8_t);
// 接收一个字节
uint8_t USART1_ReceiveByte(void);

// 发送多个字节
void USART1_TransmitBytes(uint8_t *, uint8_t);
// 接收多个字节
void USART1_ReceiveBytes(uint8_t *, uint8_t *);

// 注册接收回调函数
void USART1_Register_RX_Callback(USART1_RX_Callback);

#endif /* __USART_H__ */
