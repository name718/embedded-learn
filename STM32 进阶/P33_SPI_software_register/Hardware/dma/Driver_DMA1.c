#include "Driver_DMA1.h"

void Driver_DMA1_Init(void)
{
    /* 1. 开时钟 DMA1_channel4*/
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. 配置DMA通道 */
    // 2.1 不使用存储器到存储器
    DMA1_Channel4->CCR &= ~DMA_CCR4_MEM2MEM;

    // 2.2 数据宽度  =>  传输的是字符串  字节数值 都是8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;

    // 2.3 设置地址自增  =>  存储器都是自增的  外设都是不自增的
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;

    // 2.4 设置循环模式  =>  DMA单独执行 不受到CPU的影响  CPU延迟跟DMA发送没关系
    // 一旦开启循环  不会停  一直发
    DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;

    // 2.5 设置数据传输方向为 从存储器读1
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 特别注意 => 需要单独打开USART的DMA使能
    // 配置USART1的CR3寄存器  需要建立在打开USART1时钟的基础上

    USART1->CR3 |= USART_CR3_DMAT;

    // 打开传输完成的中断  来实现 1s发1次
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    // 不能在初始化的时候传输数据  =>  需要等待数据确认下来地址才能打开
}

// srcAddr => 存储器地址
// desAddr => 外设地址
void Driver_DMA_Transmit(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    // 2.6 填写数据地址
    DMA1_Channel4->CMAR = srcAddr;
    DMA1_Channel4->CPAR = desAddr;
    DMA1_Channel4->CNDTR = dataLen;

    // 2.7 才能打开DMA
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

void DMA1_Channel4_IRQHandler(void)
{
    // 每次传输完成进入中断
    /* 1. 清除中断标志位 */
    if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        // 清除中断标志位
        DMA1->IFCR |= DMA_IFCR_CGIF4;

        /* 2. 关闭DMA */
        DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    }
}
