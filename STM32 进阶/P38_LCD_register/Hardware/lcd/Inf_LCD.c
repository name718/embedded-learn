#include "Inf_LCD.h"
#include "Font.h"

// LCD的数据宽度是16位
void Inf_LCD_WriteCmd(uint16_t cmd)
{
    *LCD_CMD = cmd;
}

void Inf_LCD_WriteData(uint16_t data)
{
    *LCD_DATA = data;
}

/* 初始化寄存器的值 */
void Inf_LCD_RegConfig(void)
{
    /* 1. 设置灰阶电压以调整TFT面板的伽马特性， 正校准。一般出厂就设置好了 */
    Inf_LCD_WriteCmd(0xE0);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x07);
    Inf_LCD_WriteData(0x10);
    Inf_LCD_WriteData(0x09);
    Inf_LCD_WriteData(0x17);
    Inf_LCD_WriteData(0x0B);
    Inf_LCD_WriteData(0x41);
    Inf_LCD_WriteData(0x89);
    Inf_LCD_WriteData(0x4B);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x0C);
    Inf_LCD_WriteData(0x0E);
    Inf_LCD_WriteData(0x18);
    Inf_LCD_WriteData(0x1B);
    Inf_LCD_WriteData(0x0F);

    /* 2. 设置灰阶电压以调整TFT面板的伽马特性，负校准 */
    Inf_LCD_WriteCmd(0XE1);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x17);
    Inf_LCD_WriteData(0x1A);
    Inf_LCD_WriteData(0x04);
    Inf_LCD_WriteData(0x0E);
    Inf_LCD_WriteData(0x06);
    Inf_LCD_WriteData(0x2F);
    Inf_LCD_WriteData(0x45);
    Inf_LCD_WriteData(0x43);
    Inf_LCD_WriteData(0x02);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x09);
    Inf_LCD_WriteData(0x32);
    Inf_LCD_WriteData(0x36);
    Inf_LCD_WriteData(0x0F);

    /* 3.  Adjust Control 3 (F7h)  */
    /*LCD_WriteCmd(0XF7);
   Inf_LCD_WriteData(0xA9);
   Inf_LCD_WriteData(0x51);
   Inf_LCD_WriteData(0x2C);
   Inf_LCD_WriteData(0x82);*/
    /* DSI write DCS command, use loose packet RGB 666 */

    /* 4. 电源控制1*/
    Inf_LCD_WriteCmd(0xC0);
    Inf_LCD_WriteData(0x11); /* 正伽马电压 */
    Inf_LCD_WriteData(0x09); /* 负伽马电压 */

    /* 5. 电源控制2 */
    Inf_LCD_WriteCmd(0xC1);
    Inf_LCD_WriteData(0x02);
    Inf_LCD_WriteData(0x03);

    /* 6. VCOM控制 */
    Inf_LCD_WriteCmd(0XC5);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    Inf_LCD_WriteCmd(0xB1);
    Inf_LCD_WriteData(0xB0);
    Inf_LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) （正负电压反转，减少电磁干扰）*/
    Inf_LCD_WriteCmd(0xB4);
    Inf_LCD_WriteData(0x02);

    /* 9.  Display Function Control (B6h)  */
    Inf_LCD_WriteCmd(0xB6);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0xA2);

    /* 10. Entry Mode Set (B7h)  */
    Inf_LCD_WriteCmd(0xB7);
    Inf_LCD_WriteData(0xc6);

    /* 11. HS Lanes Control (BEh) */
    Inf_LCD_WriteCmd(0xBE);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x04);

    /* 12.  Interface Pixel Format (3Ah) */
    Inf_LCD_WriteCmd(0x3A);
    Inf_LCD_WriteData(0x55); /* 0x55 : 16 bits/pixel  */

    /* 13. Sleep Out (11h) 关闭休眠模式 */
    Inf_LCD_WriteCmd(0x11);

    /* 14. 设置屏幕方向和RGB */
    Inf_LCD_WriteCmd(0x36);
    Inf_LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    Inf_LCD_WriteCmd(0x29);
}

void Inf_LCD_Reset(void)
{
    // PG15 RST引脚低电平复位  => 选择低电平持续一段时间
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(100);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}

void Inf_LCD_BkOpen(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}

void Inf_LCD_BkClose(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

void Inf_LCD_Init(void)
{
    /* 1. 初始化FSMC */
    Driver_LCD_FSMC_Init();
    /* 2. 复位LCD */
    Inf_LCD_Reset();
    /* 3. 打开背光灯 */
    Inf_LCD_BkOpen();
    /* 4. 初始化寄存器的值  恢复出厂设置 */
    Inf_LCD_RegConfig();
}

uint16_t Inf_LCD_ReadData(void)
{
    return *LCD_DATA;
}

uint32_t Inf_LCD_ReadId(void)
{
    uint32_t id = 0;
    // 1. 传入读取ID的命令
    Inf_LCD_WriteCmd(0x04);

    // 2. 读取空数据
    Inf_LCD_ReadData();

    // 3. 读取id进行拼接
    id |= (Inf_LCD_ReadData() & 0xff) << 16;
    id |= (Inf_LCD_ReadData() & 0xff) << 8;
    id |= (Inf_LCD_ReadData() & 0xff) << 0;

    return id;
}

void Inf_LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. 设置x的范围
    Inf_LCD_WriteCmd(0X2A);
    // 设置X的起始位置
    Inf_LCD_WriteData(x >> 8);
    Inf_LCD_WriteData(x & 0XFF);
    // 设置X的结束位置
    Inf_LCD_WriteData((x + w - 1) >> 8);
    Inf_LCD_WriteData((x + w - 1) & 0XFF);

    // 2. 设置Y的位置
    Inf_LCD_WriteCmd(0X2B);
    // 设置Y的起始位置
    Inf_LCD_WriteData(y >> 8);
    Inf_LCD_WriteData(y & 0XFF);
    // 设置Y的结束位置
    Inf_LCD_WriteData((y + h - 1) >> 8);
    Inf_LCD_WriteData((y + h - 1) & 0XFF);
}

// LCD屏幕展示的逻辑:
// (1) 选择接下来要涂色的区域  分辨率320*480
// (2) 写入像素点 => 每一个像素点使用1个16位的数字 => 320*480
// (3) 会自动换行

void Inf_LCD_ClearAll(uint16_t color)
{
    // 1. 选择要涂色的区域
    Inf_LCD_SetArea(0, 0, 320, 480);

    // 2. 写入每一个像素点的颜色
    Inf_LCD_WriteCmd(0X2C);
    for (uint32_t i = 0; i < 320 * 480; i++)
    {
        Inf_LCD_WriteData(color);
    }
}

// 重复涂色会直接覆盖之前的颜色  不会叠加
void Inf_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    // 1. 选择要涂色的区域
    Inf_LCD_SetArea(x, y, w, w);
    // 2. 涂色 表示一个点
    Inf_LCD_WriteCmd(0x2c);
    for (uint32_t i = 0; i < w * w; i++)
    {
        Inf_LCD_WriteData(color);
    }
}

void Inf_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color)
{
    if (x2 == x1)
    {
        if (y1 < y2)
        {
            for (uint16_t y = y1; y < y2; y++)
            {
                Inf_LCD_DrawPoint(x1, y, w, color);
            }
        }
        else
        {
            for (uint16_t y = y2; y < y1; y++)
            {
                Inf_LCD_DrawPoint(x1, y, w, color);
            }
        }
    }
    else
    {
        // y = kx + b
        double k = 1.0 * (y2 - y1) / (x2 - x1);
        double b = y1 - k * x1;

        // 沿着函数解析方程  =>  画点
        if (x1 < x2)
        {
            for (uint16_t x = x1; x < x2; x++)
            {
                uint16_t y = x * k + b;
                Inf_LCD_DrawPoint(x, y, w, color);
            }
        }
        else
        {
            for (uint16_t x = x2; x < x1; x++)
            {
                uint16_t y = x * k + b;
                Inf_LCD_DrawPoint(x, y, w, color);
            }
        }
    }
}

void Inf_LCD_DrawRange(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t x4, uint16_t y4, uint16_t w, uint16_t color)
{
    Inf_LCD_DrawLine(x1, y1, x2, y2, w, color);
    Inf_LCD_DrawLine(x2, y2, x3, y3, w, color);
    Inf_LCD_DrawLine(x3, y3, x4, y4, w, color);
    Inf_LCD_DrawLine(x4, y4, x1, y1, w, color);
}

#include "math.h"

void Inf_LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    for (uint16_t angle = 0; angle < 360; angle++)
    {
        uint16_t x = xCenter + r * cos(angle * 3.14 / 180);
        uint16_t y = yCenter + r * sin(angle * 3.14 / 180);
        Inf_LCD_DrawPoint(x, y, w, color);
    }
}

void Inf_LCD_DrawCircle1(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    for (uint16_t angle = 0; angle < 90; angle++)
    {
        double r_cos = r * cos(angle * 3.14 / 180);
        double r_sin = r * sin(angle * 3.14 / 180);

        uint16_t x = xCenter + r_cos;
        uint16_t y = yCenter + r_sin;
        Inf_LCD_DrawPoint(x, y, w, color);

        x = xCenter - r_cos;
        y = yCenter + r_sin;
        Inf_LCD_DrawPoint(x, y, w, color);

        x = xCenter + r_cos;
        y = yCenter - r_sin;
        Inf_LCD_DrawPoint(x, y, w, color);

        x = xCenter - r_cos;
        y = yCenter - r_sin;
        Inf_LCD_DrawPoint(x, y, w, color);
    }
}

void Inf_LCD_DrawCircleFill(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // 半径的步长 增加的快一些
    for (uint16_t i = 0; i < r; i += w)
    {
        Inf_LCD_DrawCircle1(xCenter, yCenter, i, w, color);
    }
}

// 展示字符
// (1) 前置原理
// 在一个w:h = 1:2的区域内 => 如果字符占位 => 则展示颜色 => 如果没有占位 => 不展示颜色
// LCD屏幕没办法只选择固定位置的像素点涂色 => 只能实现区域内每一个像素点挨个涂色
// 背景色 => 字符占位的地方  涂红色  没有占位的地方 => 涂背景色

// (2) 具体实现
// 取字模 得到对应的字符数组
// A的字模 8 * 16
// 一共是16个元素  每个元素表示1行
// 每个字节拆分为2进制数字  0x14 => 0001 0100
// 表示第7行  前三个像素点不亮  第4个亮  5,7,8不亮  6亮
uint8_t ASCII_A[16] = {0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00};

uint8_t SHI[16] = {0X18, 0X18, 0X18, 0X18, 0X18, 0X18, 0X18, 0XFF, 0XFF, 0X18, 0X18, 0X18, 0X18, 0X18, 0X18, 0X18};
// void Inf_LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint16_t h, uint16_t fColor, uint16_t bColor)
// {
//     /* 1. 取一个区域 */
//     Inf_LCD_SetArea(x, y, h / 2, h);
//     /* 2. 发一个写数据命令 */
//     Inf_LCD_WriteCmd(0x2c);
//     /* 3. 涂色像素点 */
//     for (uint16_t i = 0; i < 16; i++)
//     {
//         // 每次循环涂一行的像素点
//         uint8_t temp = SHI[i];
//         // 涂色 => 字节二进制的低位开始
//         // 判断当前的像素点亮不亮
//         for (uint8_t j = 0; j < 8; j++)
//         {
//             if (temp & 0x01)
//             {
//                 // 表名像素点亮
//                 Inf_LCD_WriteData(fColor);
//             }
//             else
//             {
//                 // 表名像素点不亮
//                 Inf_LCD_WriteData(bColor);
//             }
//             temp >>= 1;
//         }
//     }
// }

// 实现整个ASCII的字符展示
void Inf_LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint8_t ch, uint16_t h, uint16_t fColor, uint16_t bColor)
{
    /* 1. 取一个区域 */
    Inf_LCD_SetArea(x, y, h / 2, h);
    /* 2. 发一个写数据命令 */
    Inf_LCD_WriteCmd(0x2c);
    /* 3. 涂色像素点 */
    uint16_t index = ch - ' ';

    if (h == 16)
    {
        for (uint16_t i = 0; i < 16; i++)
        {
            // 每次循环涂一行的像素点
            uint8_t temp = ascii_1608[index][i];
            // 涂色 => 字节二进制的低位开始
            // 判断当前的像素点亮不亮
            for (uint8_t j = 0; j < 8; j++)
            {
                if (temp & 0x01)
                {
                    // 表名像素点亮
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    // 表名像素点不亮
                    Inf_LCD_WriteData(bColor);
                }
                temp >>= 1;
            }
        }
    }
    else if (h == 32)
    {
        // 每一行需要两个字节对应  前一个字节表示左边8个像素
        for (uint16_t i = 0; i < 64; i++)
        {
            uint8_t temp = ascii_3216[index][i];
            for (uint8_t j = 0; j < 8; j++)
            {
                if (temp & 0x01)
                {
                    // 亮
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    // 不亮
                    Inf_LCD_WriteData(bColor);
                }
                temp >>= 1;
            }
        }
    }
    else if (h == 24)
    {
        // 每一行需要1个半字节对应
        // 字模中 只使用每行第二个字节的低4位  高四位没有意义
        for (uint16_t i = 0; i < 48; i++)
        {
            uint8_t temp = ascii_2412[index][i];
            uint8_t count = i % 2 == 0 ? 8 : 4;
            for (uint8_t i = 0; i < count; i++)
            {
                if (temp & 0x01)
                {
                    // 亮
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    // 不亮
                    Inf_LCD_WriteData(bColor);
                }
                temp >>= 1;
            }
        }
    }
}
