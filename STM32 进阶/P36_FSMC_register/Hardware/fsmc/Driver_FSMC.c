#include "Driver_FSMC.h"

void Driver_FSMC_GPIO_Config(void)
{
    // 复用推挽输出 1011
    // 地址线  A0-A18
    // PD11-PD13 A16-A18
    GPIOD->CRH |= (GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13);
    GPIOD->CRH &= ~(GPIO_CRH_CNF11_0 | GPIO_CRH_CNF12_0 | GPIO_CRH_CNF13_0);

    // PG0-PG5 A10-A15
    GPIOG->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5);
    GPIOG->CRL &= ~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF1_0 | GPIO_CRL_CNF2_0 | GPIO_CRL_CNF3_0 | GPIO_CRL_CNF4_0 | GPIO_CRL_CNF5_0);

    // PF12-PF15 A6-A9
    GPIOF->CRH |= (GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
    GPIOF->CRH &= ~(GPIO_CRH_CNF12_0 | GPIO_CRH_CNF13_0 | GPIO_CRH_CNF14_0 | GPIO_CRH_CNF15_0);

    // PF0-PF5  A0-A5
    GPIOF->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5);
    GPIOF->CRL &= ~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF1_0 | GPIO_CRL_CNF2_0 | GPIO_CRL_CNF3_0 | GPIO_CRL_CNF4_0 | GPIO_CRL_CNF5_0);

    /*
        2 数据端口 复用推挽输出
            在实际应用中，即使数据线被配置为输出模式，FSMC控制器仍然能够管理数据线的方向，使其在需要时成为输入线。
            这种自动切换是由FSMC控制器硬件管理的，不需要软件干预。
            因此，即使GPIO配置为复用推挽输出，FSMC依然可以实现读取操作。
    */
    /* =============MODE=============== */
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

    /* 3 其他控制端口  复用推挽输出 */
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

    GPIOG->CRH |= (GPIO_CRH_MODE10);
    GPIOG->CRH |= (GPIO_CRH_CNF10_1);
    GPIOG->CRH &= ~(GPIO_CRH_CNF10_0);
}

void Driver_FSMC_Init(void)
{
    /* 1. 打开时钟 DEFG */
    RCC->AHBENR |= RCC_AHBENR_FSMCEN;
    // AFIO主要功能 => 引脚重定向
    // 开启外部中断EXTI设置
    // RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    /* 2. 配置引脚模式 */
    Driver_FSMC_GPIO_Config();

    /* 3. 设置FSMC_BCR3 */
    // 将FSMC需要使用的BCR和BTR寄存器存放到一个数组中
    // NE1:BCR1[0] BTR1[1] 
    // NE2:BCR2[2] BTR2[3] 
    // NE3:BCR3[4] BTR3[5] 
    // NE4:BCR4[6] BTR4[7] 
    // 3.1 存储块使能
    FSMC_Bank1->BTCR[4] |= FSMC_BCR3_MBKEN;

    // 3.2 不使用数据线和地址线的复用
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MUXEN;

    // 3.3 存储器类型 sram 00
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MTYP;

    // 3.4 数据宽度8位 MWID 00 
    FSMC_Bank1->BTCR[4] &= ~FSMC_BCR3_MWID;

    // 3.5 打开写使能位
    FSMC_Bank1->BTCR[4] |= FSMC_BCR3_WREN;

    /* 4. 配置BTR3  */
    // 4.1 地址设置时间  0+1个周期
    FSMC_Bank1->BTCR[5] &= ~FSMC_BTR3_ADDSET;
    // 4.2 地址保持时间  0+1个周期
    FSMC_Bank1->BTCR[5] &= ~FSMC_BTR3_ADDHLD;
    // 4.3 数据保持时间  合计高于55ns   71+1 = 1us
    FSMC_Bank1->BTCR[5] &= ~FSMC_BTR3_DATAST;
    FSMC_Bank1->BTCR[5] |= 71 << 8;
    
}
