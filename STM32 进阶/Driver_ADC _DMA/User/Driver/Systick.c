#include "systick.h"

typedef void (*pFunction)(uint16_t);
typedef uint16_t LED;

pFunction call_back = NULL;
LED P;

void systick_init(void)
{
    SysTick->LOAD = 72000 - 1;               // 1ms
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE; // 总线时钟
    SysTick->CTRL |= SysTick_CTRL_TICKINT;   // 开启中断
    SysTick->CTRL |= SysTick_CTRL_ENABLE;    // 使能
}

void SysTick_Handler(void)
{
    static uint32_t ticks = 0;
    ticks++;
    if (ticks == 1000)
    {
        if (call_back != NULL)
        {
            call_back(P);
        }
        ticks = 0;
    }
}

void systick_callback_register(pFunction func, LED Led)
{
    call_back = func;
    P = Led;
}
