#include "Driver_USART1.h"
#include "Delay.h"
#include "Driver_FSMC.h"


// 使用方式
// (1) 关键字声明到对应的地址
uint8_t v1 __attribute__((at(0x68000000)));
//  32位对齐 定义使用外部地址  地址的值必须要是4的倍数
uint8_t v2 __attribute__((at(0x68000004)));

// (2) 直接使用指针指向
#define V4 ((uint8_t *)0x68000002)


// 声明在全局变量
const uint8_t buff1[1024] = {0};

uint8_t *name = "hello";

int main(void)
{

	/* 使用FSMC外部拓展SRAM 能够实现在外部RAM的读写操作 */
	Driver_USART1_Init();
	Driver_FSMC_Init();

	// 初始化完成之后  就已经有了对应的内存
	uint8_t v = 10;
	printf("v= %p , %d\n",&v,v);

	v1 = 11;
	printf("v1= %p , %d\n",&v1,v1);

	v2 = 12;
	printf("v2= %p , %d\n",&v2,v2);

	// 局部变量中 使用关键字声明是不生效的  会自动优化到原先的内存
	uint8_t v3 __attribute__((at(0x68000008)));
	v3 = 13;
	printf("v3= %p , %d\n",&v3,v3);

	// 直接定义指针
	uint8_t *int8_addr = (uint8_t *)0x68000001;
	*int8_addr = 14;
	*(uint8_t *)0x68000001 = 15;
	printf("0x68000001:%d\n",*(uint8_t *)0x68000001);

	// 使用宏定义声明指针
	*V4 = 24;
	printf("v4:%p,%d\n",V4,*V4);

	// 函数中声明的内容存储到栈中  => SRAM
	uint8_t buff[1024] = {0};
	printf("buff:%p %s\n",buff,buff);

	printf("buff1:%p %s\n",buff1,buff1);

	printf("name:%p %s\n",name,name);

	while (1)
	{
	}
}
