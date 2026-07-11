#include "Driver_DMA1_RX.h"

void Driver_DMA1_RX_Init(void)
{
    /* 1. 打开时钟  DMA1 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. 配置CCR寄存器 */
    // 2.1 配置不是存储器到存储器
    DMA1_Channel5->CCR &= ~DMA_CCR5_MEM2MEM;

    // 2.2 数据宽度  统一都使用8位
    DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;
    DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE;

    // 2.3 地址自增   存储器自增   外设不自增
    DMA1_Channel5->CCR |= DMA_CCR5_MINC;
    DMA1_Channel5->CCR &= ~DMA_CCR5_PINC;

    // 2.4 循环模式  =>  需要打开
    DMA1_Channel5->CCR |= DMA_CCR5_CIRC;

    // 2.5 数据传输的方向
    DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;

    // 2.6 每次配置DMA通道  =>  双向配置
    // 需要打开USART1_RX中的DMA使能
    USART1->CR3 |= USART_CR3_DMAR;

    // 使能DMA中断  代替之前的USART中断  给出判断  表示当前一条数据接收完成
    // 2.7 打开传输完成中断
    DMA1_Channel5->CCR |= DMA_CCR5_TCIE;
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel5_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

void Driver_DMA1_RX_StartRecv(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    // 数据填写去
    // 外设地址  =>  源数据
    // 存储器地址  =>  目标地址
    DMA1_Channel5->CPAR = srcAddr;
    DMA1_Channel5->CMAR = desAddr;
    // 之前的数据长度8 => 表示8个字节
    // 此处的数据长度不是这个意思  => 根据数据宽度有关  表示的几个数据
    DMA1_Channel5->CNDTR = dataLen;

    // 打开DMA完成传输
    DMA1_Channel5->CCR |= DMA_CCR5_EN;
}

uint8_t recvFinished = 0;
void DMA1_Channel5_IRQHandler(void)
{
    /* 1. 判断中断标志位 */
    if (DMA1->ISR & DMA_ISR_TCIF5)
    {
        // 2. 清空标志位 => CGIF5 一次性清空所有的标志位
        DMA1->IFCR |= DMA_IFCR_CTCIF5;
        // 完成一轮数据的接收
        recvFinished = 1;
    }
}
