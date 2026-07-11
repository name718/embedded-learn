#include "Driver_tim1.h"
#include "Driver_USART1.h"
void Driver_TIM1_Init(void)
{
    /* 1. 打开时钟  TIM1 PA8 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2. 配置引脚模式 PA8 复用推挽输出 1011 */
    GPIOA->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8);
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    /* 3. 配置TIM1的时基单元 */
    // 配置预分频和重装载  决定周期和频率 最终想要2hz 500ms
    // 72MHz => 1/72us  7199  => 100us => 0.1ms
    TIM1->PSC = 7199;
    TIM1->ARR = 5000 - 1;

    /* 4. 配置通道1为PWM1输出模式 */
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M_0);

    // 配置PWM极性和通道使能
    TIM1->CCER &= ~TIM_CCER_CC1P;
    TIM1->CCER |= TIM_CCER_CC1E;

    // 刹车输出总开关
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 设置一下初始占空比  (2500) / (4999+1)
    TIM1->CCR1 = 2500;

    /* 5. 配置重复计数器 控制5个周期  填写4 */
    TIM1->RCR = 4;

    // 手动进行更新  =>  不会启动之后自己更新
    // 设置EGR的UG为1  => 会正常产生一个更新标志位
    TIM1->EGR |= TIM_EGR_UG;
    // 清空手动产生更新事件的标志位
    TIM1->SR &= ~TIM_SR_UIF;

    /* 6. 配置更新中断 */
    TIM1->DIER |= TIM_DIER_UIE;
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 2);
    NVIC_EnableIRQ(TIM1_UP_IRQn);

    /* 7. 配置CR1 */
    TIM1->CR1 &= ~TIM_CR1_DIR;
    TIM1->CR1 &= ~TIM_CR1_OPM;
}

void Driver_TIM1_Start(void)
{
    // 定时器一旦启动  =>  会自动判断ARR的值和影子寄存器的值是否一致
    // 如果不一致  会自动产生一个更新事件(中断)
    TIM1->CR1 |= TIM_CR1_CEN;
}

// uint8_t flag = 0;
void TIM1_UP_IRQHandler(void)
{
    printf("进入更新中断\n");
    /* 1. 清空中断标志位 */
    TIM1->SR &= ~TIM_SR_UIF;

    /* 2. 关闭定时器 */
    TIM1->CR1 &= ~TIM_CR1_CEN;

    // if (flag == 0)
    // {
    //     flag++;
    // }
    // else
    // {
    //     /* 2. 关闭定时器 */
    //     TIM1->CR1 &= ~TIM_CR1_CEN;
    // }
    
}
