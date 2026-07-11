#ifndef __INF_LCD_H
#define __INF_LCD_H

#include "Driver_LCD_FSMC.h"
#include "Delay.h"

// 推荐使用的FSMC拓展存储器的方法
// 使用宏定义  直接用指针指向
#define LCD_BASE 0X6C000000
#define LCD_CMD (uint16_t *)(LCD_BASE)
#define LCD_DATA (uint16_t *)(LCD_BASE + (1 << 11))

// 常用颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色

void Inf_LCD_Init(void);

uint32_t Inf_LCD_ReadId(void);

// 涂全部的lcd
void Inf_LCD_ClearAll(uint16_t color);

// 涂一个点
void Inf_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

// 画一条线
void Inf_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color);
// 画一个方形
void Inf_LCD_DrawRange(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t x4, uint16_t y4, uint16_t w, uint16_t color);

// 画一个圆
void Inf_LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);

// 画一个实心圆
void Inf_LCD_DrawCircleFill(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);

// 写一个字符
void Inf_LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint8_t ch,uint16_t h, uint16_t fColor, uint16_t bColor);
#endif
