#include "Driver_USART1.h"

/**
 * 初始化USART1  完成相关配置 能够调用下面收发数据的代码
 */
void Driver_USART1_Init(void)
{
    /* 1. 打开USART1相关时钟 AFIO主要控制事件和外部中断 -> 片上外设的复用引脚有专门的位控制 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    /* 2. 配置使用到的引脚模式  ->  PA9 TX 复用推挽输出 1011 ->  PA10 RX 浮空输入模式  0100 */
    GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9);
    GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);

    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_MODE10);

    /* 3. 配置USART1 */
    // 3.1 配置波特比率
    USART1->BRR = 0X0271;
    // 3.2 数据长度  8位
    USART1->CR1 &= ~(USART_CR1_M);
    // 3.3 校验位
    USART1->CR1 &= ~(USART_CR1_PCE);
    // 3.4 停止位
    USART1->CR2 &= ~(USART_CR2_STOP);

    // 3.5 打开使能开关
    // 3.5.1 发送开关
    USART1->CR1 |= USART_CR1_TE;
    // 3.5.2 接收开关
    USART1->CR1 |= USART_CR1_RE;

    /* 4. 打开中断 */
    // 4.1 打开RXNEIE中断 和 IDLEIE
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_IDLEIE;

    /* 5. NVIC设置 */
    // 整个USART1共用一个中断向量
    // 只使用抢占优先级
    NVIC_SetPriorityGrouping(3);
    // 设置对应中断的优先级
    NVIC_SetPriority(USART1_IRQn, 3);
    // 使能中断
    NVIC_EnableIRQ(USART1_IRQn);

    // 3.5.3 总开关  -> 应该放在配置的最后
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * 发送一个字节
 * uint8_t ch: 需要发送的一个字节
 */
void Driver_USART1_SendChar(uint8_t ch)
{
    // 往DR寄存器直接写 -> 发送数据
    // 需要等待上一个字节发送完成之后  ->  才能发送下一个字节
    // SR_TXE   如果数据正在发 ->  0
    //          如果数据发生完成 -> 1
    while ((USART1->SR & USART_SR_TXE) == 0)
        ;

    USART1->DR = ch;
}

/**
 * 接收一个字节
 * return: uint8_t 接收到的字节
 */
uint8_t Driver_USART1_ReceiveChar(void)
{
    // 接收数据也需要挂起 -> 等待有数据传入
    // SR_RXNE : 有数据进来 -> 1
    //           读取一次之后 -> 0
    while ((USART1->SR & USART_SR_RXNE) == 0)
        ;
    return USART1->DR;
}

/**
 * 发送多个字节
 * uint8_t *str: 一个字符串
 * uint8_t len: 字符串长度
 */
void Driver_USART1_SendString(uint8_t *str, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}

/**
 * 接收多个字节  ->  由于不能确定接收数据的长度 ->  推荐buff够大
 * 收到空闲位结束
 * uint8_t buff[]: 存接收到的数据
 * uint8_t * len: 实际接收数据的长度 -> 需要函数中赋值
 */
void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len)
{
    uint8_t i = 0;
    while (1)
    {
        // 1. 收到数据  存缓存
        if (USART1->SR & USART_SR_RXNE)
        {
            buff[i] = USART1->DR;
            i++;
        }

        // 2. 收到空闲 停止
        if (USART1->SR & USART_SR_IDLE)
        {

            *len = i;
            break;
        }
        // 3. 挂起等待
        // while ((USART1->SR & USART_SR_RXNE) == 0)
        //     ;
    }
}

// void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len)
// {
//     uint8_t i = 0;
//     while (1)
//     {
//         // 3. 需要挂起
//         while ((USART1->SR & USART_SR_RXNE) == 0)
//         {
//             // 2. 接收到空闲位 -> break
//             if (USART1->SR & USART_SR_IDLE)
//             {
//                 // 接收完成
//                 *len = i;
//                 return;
//             }
//         }
//         // 1. 接收到数据 -> 存buff len+1
//         buff[i] = USART1->DR;
//         i++;
//     }
// }

uint8_t buff[100] = {0};
uint8_t len = 0;
uint8_t isEnd = 0;
/**
 * 调用时机 -> 任意一个USART1的中断
 * 打开了RXNEIE和IDLEIE -> 会调用下面的函数
 * 使用电脑给STM32发消息 -> 收到一个字节 触发RXNEIE
 *                     ->  全部发送完成 触发IDLEIE
 */
void USART1_IRQHandler(void)
{
    // 1. 判断当前调用中断函数是哪种中断触发的
    // SR_RXNE是收到一个字节的中断标志位
    if (USART1->SR & USART_SR_RXNE)
    {
        // 对USART_DR的读操作可以将该位清零
        // USART1->DR;
        //  将数据存到BUFF
        buff[len] = USART1->DR;
        len++;
    }
    // SR_IDLE是全部发送完成触发的中断标志位
    else if (USART1->SR & USART_SR_IDLE)
    {
        // 清除中断标志位
        // 由软件序列清除该位(先读USART_SR，然后读USART_DR)
        USART1->SR;
        USART1->DR;
        // 接收完成 -> 将数据返回给电脑
        // Driver_USART1_SendString(buff, len);
        // len = 0;

        /* 特殊 */
        // 做一个收数据完成的标志位
        isEnd = 1;
    }
}


int fputc(int ch,FILE *file)
{
    Driver_USART1_SendChar((uint8_t )ch);
    return ch;
}

