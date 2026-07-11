#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stm32f10x.h"
#include "Delay.h"
#include "IIC2.h"

#define EEPROM_ADDR_w 0xA0
#define EEPROM_ADDR_R 0xA1
#define EEPROM_PAGE_SIZE 8

int EEPROM_WritePage(uint8_t addr, uint8_t *data, uint16_t len);
int EEPROM_Read(uint8_t addr, uint8_t *data, uint16_t len);

#endif // __EEPROM_H__
