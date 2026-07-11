#include "DMA1.h"

DMA1_Callback_TX DMA1_TX_Callback = NULL;
DMA1_Callback_RX DMA1_RX_Callback = NULL;
uint32_t *DMA1_RX_Addr = NULL;
uint32_t DMA1_RX_Length = 0;

void DMA1_TX_Init(void)
{
    // 使能DMA1时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 设置DMA1通道1配置
    DMA1_Channel4->CCR &= ~DMA_CCR4_MEM2MEM; // 关闭内存到内存传输
    // 数据宽度为8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE; // 外设数据宽度为8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE; // 存储器数据宽度为8位
    // 存储器地址自增，外设地址不变
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC; // 外设地址不自增
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;  // 存储器地址自增
    // 循环模式关闭
    DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;
    // 传输方向：从存储器到外设
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 中断
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE; // 打开传输完成中断

    // 优先级
    NVIC_SetPriorityGrouping(3); // 设置优先级分组
    NVIC_SetPriority(DMA1_Channel4_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void DMA1_Transmit(uint32_t *srcAddr, uint32_t *dstAddr, uint32_t length)
{
    // 设置DMA1通道1存储器地址
    DMA1_Channel4->CMAR = (uint32_t)srcAddr;
    // 设置DMA1通道1外设地址
    DMA1_Channel4->CPAR = (uint32_t)dstAddr;
    // 设置DMA1通道1传输数据长度
    DMA1_Channel4->CNDTR = length;
    // 使能DMA1通道1
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

void DMA1_Channel4_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF4;

        if (DMA1_TX_Callback != NULL)
        {
            DMA1_TX_Callback();
        }
        // 关闭DMA1通道1
        DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    }
}

void DMA1_RX_Init(void)
{
    // 使能DMA1时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 设置DMA1通道1配置
    DMA1_Channel5->CCR &= ~DMA_CCR5_MEM2MEM; // 关闭内存到内存传输
    // 数据宽度为8位
    DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE; // 外设数据宽度为8位
    DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE; // 存储器数据宽度为8位
    // 存储器地址自增，外设地址不变
    DMA1_Channel5->CCR |= DMA_CCR5_MINC;  // 存储器地址自增
    DMA1_Channel5->CCR &= ~DMA_CCR5_PINC; // 外设地址不自增
    // 循环模式开启
    DMA1_Channel5->CCR |= DMA_CCR5_CIRC;
    // 传输方向：从外设到存储器
    DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;

    // 中断
    DMA1_Channel5->CCR |= DMA_CCR5_TCIE; // 打开传输完成中断

    // 优先级
    NVIC_SetPriorityGrouping(3); // 设置优先级分组
    NVIC_SetPriority(DMA1_Channel5_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

void DMA1_Receive(uint32_t *srcAddr, uint32_t *dstAddr, uint32_t length)
{
    // 设置DMA1通道5外设地址
    DMA1_Channel5->CPAR = (uint32_t)srcAddr;
    // 设置DMA1通道5存储器地址
    DMA1_Channel5->CMAR = (uint32_t)dstAddr;
    // 设置DMA1通道5传输数据长度
    DMA1_Channel5->CNDTR = length;
    // 使能DMA1通道5
    DMA1_Channel5->CCR |= DMA_CCR5_EN;
}

void DMA1_Channel5_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF5)
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF5;
        DMA1_RX_Callback(DMA1_RX_Addr, DMA1_RX_Length);
    }
}

void DMA1_Register_TX_AddCallback(DMA1_Callback_TX callback)
{
    DMA1_TX_Callback = callback;
}

void DMA1_Register_RX_AddCallback(DMA1_Callback_RX callback, uint32_t *Addr, uint32_t length)
{
    DMA1_RX_Callback = callback;
    DMA1_RX_Addr = Addr;
    DMA1_RX_Length = length;
}

void DMA1_ADC_Init()
{
    // 使能DMA1时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 设置DMA1通道1配置
    DMA1_Channel1->CCR &= ~DMA_CCR1_MEM2MEM; // 关闭内存到内存传输
    // 数据宽度为16位 01
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0; // 外设数据宽度为16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0; // 存储器数据宽度为16位
    // 存储器地址自增，外设地址不变
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC; // 外设地址不自增
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;  // 存储器地址自增
    // 循环模式开启
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
    // 传输方向：从外设到存储器
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // // 中断
    // DMA1_Channel1->CCR |= DMA_CCR1_TCIE; // 打开传输完成中断

    // // 优先级
    // NVIC_SetPriorityGrouping(3); // 设置优先级分组
    // NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
    // NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void DMA1_ADC_Receive(uint32_t *dstAddr, uint32_t length)
{
    // 设置DMA1通道1外设地址
    DMA1_Channel1->CPAR = (uint32_t) & (ADC1->DR);
    // 设置DMA1通道1存储器地址
    DMA1_Channel1->CMAR = (uint32_t)dstAddr;
    // 设置DMA1通道1传输数据长度
    DMA1_Channel1->CNDTR = length;
    // 使能DMA1通道1
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}
