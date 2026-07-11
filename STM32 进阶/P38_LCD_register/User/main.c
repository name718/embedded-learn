#include "Driver_USART1.h"
#include "Delay.h"
#include "Inf_LCD.h"

int main(void)
{

	/* 使用FSMC扩展LCD 展示不同的图像 */
	Driver_USART1_Init();

	// 完成初始化 打开背光
	Inf_LCD_Init();

	// 读取id
	printf("ID:0x%x",Inf_LCD_ReadId());
	
	// RGB 5:6:5  RED F800
	// RGB 5:6:5  GREEN 07E0
	Inf_LCD_ClearAll(WHITE);

	// Inf_LCD_DrawPoint(50,50,1,RED);
	// Inf_LCD_DrawPoint(50,80,2,RED);
	// Inf_LCD_DrawPoint(50,100,5,RED);
	// Inf_LCD_DrawPoint(50,150,8,RED);
	// Inf_LCD_DrawPoint(50,200,10,RED);


	// Inf_LCD_DrawLine(50,40,10,20,5,RED);

	// Inf_LCD_DrawRange(50,50,150,50,150,150,50,150,5,BLUE);

	// Inf_LCD_DrawCircle(100,100,50,5,RED);

	// Inf_LCD_DrawCircleFill(100,100,50,5,BLUE);

	Inf_LCD_WriteASCIIChar(100,100,'h',24,RED,BLUE);
	Inf_LCD_WriteASCIIChar(112,100,'i',24,RED,BLUE);

	while (1)
	{
	}
}
