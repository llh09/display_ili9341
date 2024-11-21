#include "bsp_led.h"

led_t led_info;

static void led_switch(void)
{
    if( led_info.status == LED_STATUS_ON )
    {
        TIM3->CCR2 = led_info.R; // r;
        TIM3->CCR3 = led_info.G; // g;
        TIM3->CCR4 = led_info.B; // b;
    }
    else if( led_info.status == LED_STATUS_OFF )
    {
        TIM3->CCR2 = 0; // r;
        TIM3->CCR3 = 0; // g;
        TIM3->CCR4 = 0; // b;
    }
}

static void led_set_color(void)
{
	TIM3->CCR2 = led_info.R; // r;
	TIM3->CCR3 = led_info.G; // g;
	TIM3->CCR4 = led_info.B; // b;
}

void led_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;	
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // open clock
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO), ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    
    // red PB5
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // green PB0
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // blue PB1
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    TIM_TimeBaseStructure.TIM_Period        = 255;
    TIM_TimeBaseStructure.TIM_Prescaler     = 4000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse           = 0;
    TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_Low;
    
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    TIM3->CCR2 = 0; // r;
    TIM3->CCR3 = 0; // g;
    TIM3->CCR4 = 0; // b;
    
    TIM_Cmd(TIM3, ENABLE);
    
    led_info.fun.led_on_off = led_switch;
    led_info.fun.led_change_color = led_set_color;
    led_info.mode = LED_MODE_KEEP_ON;

    timer_set(&led_info.led_task_timer, 10);
}

void led_task(void)
{
    static uint8_t blink_sta = 0;
    static uint8_t led_flag = 0;
    
    if( timer_expired(&led_info.led_task_timer) && (led_info.status != LED_STATUS_OFF) )
    {
        timer_restart(&led_info.led_task_timer);
        
        if( led_info.mode == LED_MODE_KEEP_ON )
        {
            TIM3->CCR2 = led_info.R; // r;
            TIM3->CCR3 = led_info.G; // g;
            TIM3->CCR4 = led_info.B; // b;
        }
        else if( led_info.mode == LED_MODE_KEEP_OFF )
        {
            TIM3->CCR2 = 0; // r;
            TIM3->CCR3 = 0; // g;
            TIM3->CCR4 = 0; // b;
        }
        else if( led_info.mode == LED_MODE_BLINK )
        {
            if( timer_expired(&led_info.led_blink_timer) )
            {
                timer_restart(&led_info.led_blink_timer);
                if( blink_sta == 0 )
                {
                    blink_sta = 1;
                    
                    TIM3->CCR2 = 0; // r;
                    TIM3->CCR3 = 0; // g;
                    TIM3->CCR4 = 0; // b;
                }
                else
                {
                    blink_sta = 0;
                    
                    TIM3->CCR2 = led_info.R; // r;
                    TIM3->CCR3 = led_info.G; // g;
                    TIM3->CCR4 = led_info.B; // b;
                }
            }
        }
    }
}
