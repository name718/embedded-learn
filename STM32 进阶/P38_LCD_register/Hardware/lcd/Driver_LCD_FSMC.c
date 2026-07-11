#include "Driver_LCD_FSMC.h"

void Driver_LCD_FSMC_GPIO_Config(void)
{
    // 数据线D0-D15
    GPIOD->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1);
    GPIOD->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    GPIOE->CRL |= (GPIO_CRL_MODE7);
    GPIOE->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE11 |
                   GPIO_CRH_MODE12 |
                   GPIO_CRH_MODE13 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    /* =============CNF=============== */
    GPIOD->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF0_0 |
                    GPIO_CRL_CNF1_0);

    GPIOD->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOD->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);

    GPIOE->CRL |= (GPIO_CRL_CNF7_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF7_0);

    GPIOE->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF11_1 |
                   GPIO_CRH_CNF12_1 |
                   GPIO_CRH_CNF13_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOE->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF11_0 |
                    GPIO_CRH_CNF12_0 |
                    GPIO_CRH_CNF13_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);

    // A10  PG0 复用推挽
    GPIOG->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0);
    GPIOG->CRL &= ~(GPIO_CRL_CNF0_0);

    // 其他控制引脚
    //
    GPIOD->CRL |= (GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);
    GPIOD->CRL |= (GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF4_0 |
                    GPIO_CRL_CNF5_0);

    GPIOE->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1);
    GPIOE->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF0_0 |
                    GPIO_CRL_CNF1_0);
    // 片选引脚  PG10 = NE3   PG12 = NE4
    GPIOG->CRH |= (GPIO_CRH_MODE12);
    GPIOG->CRH |= (GPIO_CRH_CNF12_1);
    GPIOG->CRH &= ~(GPIO_CRH_CNF12_0);

    // LCD独有的控制引脚  不属于FSMC 通用推挽输出 0011
    // BkOpen = PB0   RST = PG15
    GPIOB->CRL &= ~(GPIO_CRL_CNF0);
    GPIOB->CRL |= (GPIO_CRL_MODE0);

    GPIOG->CRH &= ~(GPIO_CRH_CNF15);
    GPIOG->CRH |= (GPIO_CRH_MODE15);
}

void Driver_LCD_FSMC_Init(void)
{
    /* 1. 打开时钟 FSMC PB PD PE PF PG */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    RCC->AHBENR |= RCC_AHBENR_FSMCEN;

    /* 2. 配置引脚模式 */
    Driver_LCD_FSMC_GPIO_Config();

    // NE4
    /* 3. 配置FSMC 两个寄存器 BCR4 BTR4  */
    // 3.1 存储器块使能
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MBKEN;
    // 3.2 地址数据线复用
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MUXEN;
    // 3.3 存储器类型
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MTYP;
    // 3.4 总线宽度  数据宽度  01 16位
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MWID;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MWID_0;
    // 3.5 写使能位
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_WREN;

    /* 4. BTR4  时序 */
    // 4.1 地址保持时间 稳定一个周期
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ADDHLD;
    // 4.2 地址建立时间 稳定一个周期
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ADDSET;
    // 4.3 数据保持时间
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_DATAST;
    FSMC_Bank1->BTCR[7] |= 71 << 8;
}
