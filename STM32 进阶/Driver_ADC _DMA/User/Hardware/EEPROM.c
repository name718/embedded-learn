#include "EEPROM.h"

static int EEPROM_WriteToOnePage(uint8_t addr, uint8_t *data, uint16_t len)
{
    int ret = 0;
    ret |= IIC2_Start();
    ret |= IIC2_SendAddr(EEPROM_ADDR_w);
    ret |= IIC2_SendByte(addr);
    for (uint16_t i = 0; i < len; i++)
    {

        ret |= IIC2_SendByte(data[i]);
    }
    IIC2_Stop();
    return ret;
}

int EEPROM_WritePage(uint8_t addr, uint8_t *data, uint16_t len)
{
    int ret = 0;
    uint16_t page_remain;

    while (len > 0)
    {
        page_remain = EEPROM_PAGE_SIZE - addr % EEPROM_PAGE_SIZE;
        if (len <= page_remain)
        {
            ret |= EEPROM_WriteToOnePage(addr, data, len);
            len = 0;
        }
        else
        {
            ret |= EEPROM_WriteToOnePage(addr, data, page_remain);
            len -= page_remain;
            data += page_remain;
            addr += page_remain;
            page_remain = EEPROM_PAGE_SIZE;
            Delay_ms(5); // 等待写入完成
        }
    }
    return ret;
}

int EEPROM_Read(uint8_t addr, uint8_t *data, uint16_t len)
{
    int ret = 0;
    ret |= IIC2_Start();
    ret |= IIC2_SendAddr(EEPROM_ADDR_w);
    ret |= IIC2_SendByte(addr);

    ret |= IIC2_Start();
    ret |= IIC2_SendAddr(EEPROM_ADDR_R);

    for (uint16_t i = 0; i < len; i++)
    {

        if (i == len - 1)
        {
            IIC2_SendACK(1);
            IIC2_Stop();
        }
        else
        {
            IIC2_SendACK(0);
        }
        data[i] = IIC2_RecvByte();
    }

    return ret;
}
