#ifndef __DMA1_H__
#define __DMA1_H__

#include "stm32f10x.h"
#include <stdio.h>

typedef void (*DMA1_Callback_TX)(void);
typedef void (*DMA1_Callback_RX)(uint32_t *Addr, uint32_t length);

void DMA1_TX_Init(void);
void DMA1_RX_Init(void);
void DMA1_ADC_Init(void);
void DMA1_Transmit(uint32_t *srcAddr, uint32_t *dstAddr, uint32_t length);
void DMA1_Receive(uint32_t *srcAddr, uint32_t *dstAddr, uint32_t length);
void DMA1_ADC_Receive(uint32_t *dstAddr, uint32_t length);
void DMA1_Register_TX_AddCallback(DMA1_Callback_TX callback);
void DMA1_Register_RX_AddCallback(DMA1_Callback_RX callback, uint32_t *Addr, uint32_t length);

#endif /* __DMA1_H__ */
