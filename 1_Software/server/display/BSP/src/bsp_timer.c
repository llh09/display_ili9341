#include "bsp_timer.h"
#include "app_time.h"

void timer6_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef        NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period                       = 1000;    // enter into interrupt every 1ms 
    TIM_TimeBaseInitStruct.TIM_Prescaler                    = (72 - 1); // (72MHz / 72 = 1MHz), (1 / 1MHz = 1us)
    // TIM_TimeBaseInitStruct.TIM_ClockDivision             = ;         // TIM6 and TIM7 not used
    // TIM_TimeBaseInitStruct.TIM_CounterMode               = ;         // TIM6 and TIM7 not used
    // TIM_TimeBaseInitStruct.TIM_RepetitionCounter         = ;         // TIM6 and TIM7 not used
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

    // if not clear TIM_FLAG_Update, it will enter interrupt immediately when enable TIM
    // because the function TIM_TimeBaseInit generate an update event by software by set
    // register TIMx->EGR bit UG 1
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM6, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                      = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM6_IRQHandler(void)
{
    if( TIM_GetITStatus(TIM6, TIM_FLAG_Update) == SET )
    {
        timer_update();
        TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
    }
}

