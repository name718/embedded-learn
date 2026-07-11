#include "Inf_W25Q32.h"

void Inf_W25Q32_Init(void)
{
    Driver_SPI_Init();
}

void Inf_W25Q32_ReadId(uint8_t *did, uint16_t *mid)
{
    // 走读写流程
    /* 1. 片选启动  */
    Driver_SPI_Start();
    /* 2. 交换数据   SPI 虽然是交换数据  实际上 多数情况下都是只有一方发的数据有用 */
    // 2.1 发送9F指令
    Driver_SPI_SwapByte(0x9f);
    // 2.2 接收数据的时候   填写的交换发送数据是没有意义的
    *did = Driver_SPI_SwapByte(0XFF);
    *mid = 0;
    *mid |= Driver_SPI_SwapByte(0XFF) << 8;
    *mid |= Driver_SPI_SwapByte(0XFF) << 0;

    /* 3. 片选关闭 */
    Driver_SPI_Stop();
}

void Inf_W25Q32_WaitBusy(void)
{
    uint8_t status = 0;
    /* 1. 片选开启 */
    Driver_SPI_Start();
    /* 2. 发送读取状态指令 */
    Driver_SPI_SwapByte(0x05);
    /* 3. 接收状态寄存器的值 */
    status = Driver_SPI_SwapByte(0xff);
    // 如果最低位为1  表示忙
    while (status & 0x01)
    {
        status = Driver_SPI_SwapByte(0xff);
    }

    /* 4. 片选关闭 */
    Driver_SPI_Stop();
}

void Inf_W25Q32_WriteEnable(void)
{
    /* 1. 片选开启 */
    Driver_SPI_Start();

    /* 2. 开启写使能 */
    Driver_SPI_SwapByte(0x06);

    /* 3. 片选关闭 */
    Driver_SPI_Stop();
}

void Inf_W25Q32_WriteDisenable(void)
{
    /* 1. 片选开启 */
    Driver_SPI_Start();

    /* 2. 开启写使能 */
    Driver_SPI_SwapByte(0x04);

    /* 3. 片选关闭 */
    Driver_SPI_Stop();
}



// 每次上电写入数据的时候  需要先对对应的内存区域进行擦除
void Inf_W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t data[], uint8_t dataLen)
{
    /* 1. 判断忙状态 */
    Inf_W25Q32_WaitBusy();
    /* 2. 开启写使能 */
    Inf_W25Q32_WriteEnable();
    /* 3. 写入数据 */
    // 3.1 片选使能
    Driver_SPI_Start();

    // 3.2 发送写数据的指令
    Driver_SPI_SwapByte(0x02);

    // 3.3 写入数据的起始地址
    // W25Q32的地址是22位   表示的时候使用24位  =  6个16进制表示
    uint32_t addr = block * 0x010000 + sector * 0x001000 + page * 0x000100;

    Driver_SPI_SwapByte(addr >> 16);
    Driver_SPI_SwapByte((addr >> 8) & 0xff);
    Driver_SPI_SwapByte((addr >> 0) & 0xff);

    // 3.4 写入数据
    for (uint8_t i = 0; i < dataLen; i++)
    {
        Driver_SPI_SwapByte(data[i]);
    }
    // 3.5 关闭片选 =>  如果不关 是没有办法再次写入命令的
    Driver_SPI_Stop();

    /* 4. 关闭写使能  */
    Inf_W25Q32_WriteDisenable();
}


void Inf_W25Q32_SectorErase(uint8_t block,uint8_t secotr)
{
    /* 1. 等待忙状态 */
    Inf_W25Q32_WaitBusy();
    /* 2. 开启写使能 */
    Inf_W25Q32_WriteEnable();
    /* 3. 执行扇区擦除 */
    // 3.1 开启片选
    Driver_SPI_Start();

    // 3.2 发送擦除命令
    Driver_SPI_SwapByte(0x20);

    // 3.3 发送擦除地址
    uint32_t addr = block * 0x010000 + secotr * 0x001000;
    Driver_SPI_SwapByte((addr >> 16));
    Driver_SPI_SwapByte((addr >> 8) & 0xff);
    Driver_SPI_SwapByte((addr >> 0) & 0xff);

    // 3.4 关闭片选
    Driver_SPI_Stop();

    /* 4. 关闭写使能 */
    Inf_W25Q32_WriteDisenable();
}


void Inf_W25Q32_ReadData(uint8_t block,uint8_t sector,uint8_t page,uint8_t buff[],uint16_t dataLen)
{
    /* 1. 等待忙状态 */
    Inf_W25Q32_WaitBusy();

    /* 2. 开始读数据 */
    // 2.1 开启片选
    Driver_SPI_Start();

    // 2.2 发送读数据指令
    Driver_SPI_SwapByte(0x03);

    // 2.3 读取数据的起始地址
    uint32_t addr = block * 0x010000 + sector * 0x001000 + page * 0x000100;
    Driver_SPI_SwapByte((addr >> 16));
    Driver_SPI_SwapByte((addr >> 8) & 0xff);
    Driver_SPI_SwapByte((addr >> 0) & 0xff);

    // 2.4 读取数据存储到buff
    for (uint16_t i = 0; i < dataLen; i++)
    {
        buff[i] = Driver_SPI_SwapByte(0xff);
    }
    // 2.5 需要手动调用停止  =>  停止片选
    Driver_SPI_Stop();
}
