#ifndef __INF_M24C02
#define __INF_M24C02

#include "Driver_I2C.h"
#include "Delay.h"
#define WRITE_ADDR 0xA0
#define READ_ADDR 0xA1


void Inf_M24C02_Init(void);

/**
 * 写入一个字节
 * uint8_t byte_addr: EEPROM内部字节地址  0-255
 * uint8_t ch: 要写入的字节数据
 */
void Inf_M24C02_WriteChar(uint8_t byte_addr ,uint8_t ch);

/**
 * 读取一个字节
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * return : 返回读取的字节
 */
uint8_t Inf_M24C02_ReadChar(uint8_t byte_addr);

/**
 * 一次写入多个字节  -> 按照底层16字节1page划分  每次写数据  只能在当前页写数据 -> 如果当前页写满  继续写入会从这一页的开头重新写数据
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * uint8_t *str: 需要写入的字符串
 * uint8_t len: 写入字符串的长度
 */
void Inf_M24C02_PageWrite(uint8_t byte_addr,uint8_t *str,uint8_t len);

/**
 * 一次读取多个字节 -> 跨页读取数据的
 * uint8_t byte_addr:  EEPROM内部字节地址  0-255
 * uint8_t buff[]: 存放读取的数据
 * uint8_t len: 读取多少字节
 */
void Inf_M24C02_ReadString(uint8_t byte_addr,uint8_t buff[],uint8_t len);

#endif
