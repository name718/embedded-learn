#include "USART.h"

// 接收缓冲区
uint8_t USART1_RX_BUF[64];

// 接收到的数据长度
uint8_t USART1_RX_LEN = 0;

// 回调函数
USART1_RX_Callback cb_USART = NULL;
// 初始化串口
void USART1_Init()
{
    // 使能串口1时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 配置PA9为复用推挽输出 CNF=10 MODE=11
    GPIOA->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 配置PA10为浮空输入 CNF=01 MODE=00
    GPIOA->CRH &= ~GPIO_CRH_MODE10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;

    // 串口配置
    // 串口使能，发送使能，接收使能
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
    // 数据位8位
    USART1->CR1 &= ~USART_CR1_M;
    // 无校验
    USART1->CR1 &= ~USART_CR1_PCE;
    // 1位停止位
    USART1->CR2 &= ~USART_CR2_STOP;

    // 波特率设置
    // 波特率115200
    // 72MHz / 115200 /16 = 39.0625
    USART1->BRR = 0x271; // 0x271 = 625
}

void USART1_Init_IT()
{
    USART1_Init();
    USART1->CR1 |= USART_CR1_RXNEIE; // 接收缓冲区非空中断使能
    USART1->CR1 |= USART_CR1_IDLEIE; // 空闲帧中断使能

    // NVIC配置
    NVIC_SetPriorityGrouping(3);      // 设置优先级分组
    NVIC_SetPriority(USART1_IRQn, 4); // 设置优先级
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_Init_DMA()
{
    USART1_Init();
    USART1->CR3 |= USART_CR3_DMAT; // DMA发送使能
    USART1->CR3 |= USART_CR3_DMAR; // DMA接收使能
}

void USART1_TransmitByte(uint8_t byte)
{
    // 等待发送缓冲区为空
    while ((USART1->SR & USART_SR_TXE) == 0)
        ;
    USART1->DR = byte;
}

uint8_t USART1_ReceiveByte(void)
{
    // 等待接收缓冲区非空
    while ((USART1->SR & USART_SR_RXNE) == 0)
        ;
    return USART1->DR;
}

void USART1_TransmitBytes(uint8_t *bytes, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        USART1_TransmitByte(bytes[i]);
    }
}

void USART1_ReceiveBytes(uint8_t *bytes, uint8_t *length)
{
    *length = 0;
    // 没有接收到空闲帧，就继续接收
    while (1)
    {
        while ((USART1->SR & USART_SR_RXNE) == 0)
        {
            if (USART1->SR & USART_SR_IDLE) // 接收到空闲帧
            {
                USART1->DR; // 清除空闲帧标志
                return;
            }
        }
        bytes[*length] = USART1->DR;
        (*length)++;
    }
}

void USART1_Register_RX_Callback(USART1_RX_Callback callback)
{
    cb_USART = callback;
}

// 串口1中断处理函数
void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE) // 接收缓冲区非空
    {
        USART1_RX_BUF[USART1_RX_LEN++] = USART1->DR;
    }
    else if (USART1->SR & USART_SR_IDLE) // 空闲帧
    {
        USART1->DR; // 清除空闲帧标志
        if (cb_USART != NULL)
        {
            cb_USART(USART1_RX_BUF, USART1_RX_LEN);
        }
        USART1_RX_LEN = 0;
    }
}

// 微型库的printf重定向
int fputc(int ch, FILE *f)
{
    USART1_TransmitByte((uint8_t)ch);
    return ch;
}
