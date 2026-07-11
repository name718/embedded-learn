#ifndef __INF_W25Q32_H
#define __INF_W25Q32_H

// (1)直接在Inf层代码中修改 =>  耦合HAL库生成的驱动层   =>  需要大量的修改接口层代码  不方便的
// (2)设计一个耦合层  =>  函数声明完全使用移植前的代码  => 函数定义的时候等效替代为HAL的实现

#include "Driver_spi.h"

void Inf_W25Q32_Init(void);

void Inf_W25Q32_ReadId(uint8_t *did,uint16_t *mid);

// 块 64K一块  一共64块   2位16进制数字 
// 扇区 4k  一共16个扇区  1位16进制数字
// 页  256字节  一共16个页  1位16进制数字
void Inf_W25Q32_WritePage(uint8_t block,uint8_t sector,uint8_t page,uint8_t data[],uint8_t dataLen);

// 最小单位擦除  =>  一个扇区
void Inf_W25Q32_SectorErase(uint8_t block,uint8_t secotr);


// 读取数据  => 可以一次性读取多个扇区的数据
void Inf_W25Q32_ReadData(uint8_t block,uint8_t sector,uint8_t page,uint8_t buff[],uint16_t dataLen);

#endif
