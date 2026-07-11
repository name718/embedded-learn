#ifndef __DRIVER_DMA1_RX_H
#define __DRIVER_DMA1_RX_H

#include "stm32f10x.h"

extern uint8_t recvFinished;
void Driver_DMA1_RX_Init(void);
void Driver_DMA1_RX_StartRecv(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

#endif
