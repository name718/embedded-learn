#include "Driver_ADC_single.h"

void Driver_ADC_Single_Init(void)
{
    /* 1. 打开时钟 ADC1  PC0 */
    RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;
    // ADC的时钟频率不能高于14M  => 直接打开72MHz
    // 使用ADC的6分频
    RCC->CFGR |=RCC_CFGR_ADCPRE_1;
    RCC->CFGR &=~RCC_CFGR_ADCPRE_0;
    RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
    

    /* 2. 配置引脚模式  PC0  ->  模拟输入 0000 */
   GPIOC->CRL &=~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);


 

    /* 3. 配置基础CR1 CR2的模式功能 */
    // 3.1 单通道关闭扫描模式 0
    ADC1->CR1 &= ~ADC_CR1_SCAN;



    // 3.2 数据对齐 右对齐0
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.3 连续转换
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.4 采样时间  选择010   13.5周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= 2;

    // 3.5 配置序列寄存器
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;

    // 3.6 配置转换通道的个数
    ADC1->SQR1 &= ~ADC_SQR1_L;

    /* 4. 配置外部触发转换功能 */   
    // 4.1 打开外部触发功能
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // 4.2 配置使用的外部触发  =>  111 软件SWSTART触发
    ADC1->CR2 |= ADC_CR2_EXTSEL;
}

void Driver_ADC_Single_Start(void)
{
    /* 1. 唤醒上电 */
    ADC1->CR2 |= ADC_CR2_ADON;
    /* 2. 进行AD校准 */
    ADC1->CR2 |= ADC_CR2_CAL;
    // 校准完成之后  会自动修改为0
    while ((ADC1->CR2 & ADC_CR2_CAL))
        ;

    /* 3. 开始转换 */
    // 内部触发
    // 间隔一段时间之后  再次进行上电  等于开启转换
    // ADC1->CR2 |= ADC_CR2_ADON;//上电
    
    // 外部触发功能替代内部上电触发
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while ((ADC1->SR & ADC_SR_EOC) == 0)
        ;
}

double Driver_ADC_Single_ReadV(void)
{
    uint16_t nums = (ADC1->DR) ;
    return nums * 3.3 / 4095;
}
