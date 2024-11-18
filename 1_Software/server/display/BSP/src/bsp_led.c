#include "bsp_led.h"

void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // open clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // red PB5
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // green PB0
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // blue PB1
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // close all led
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
    GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

uint8_t led_on(uint8_t color)
{
    if( color > LED_GREEN )
    {
        return 0;
    }
    
    switch(color)
    {
        case LED_RED:
            GPIO_ResetBits(GPIOB, GPIO_Pin_5);
            break;
        case LED_GREEN:
            GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            break;
        case LED_BLUE:
            GPIO_ResetBits(GPIOB, GPIO_Pin_1);
            break;
        default:
            break;
    }
    
    return 1;
}

uint8_t led_off(uint8_t color)
{
    if( color > LED_GREEN )
    {
        return 0;
    }
    
    switch(color)
    {
        case LED_RED:
            GPIO_SetBits(GPIOB, GPIO_Pin_5);
            break;
        case LED_GREEN:
            GPIO_SetBits(GPIOB, GPIO_Pin_0);
            break;
        case LED_BLUE:
            GPIO_SetBits(GPIOB, GPIO_Pin_1);
            break;
        default:
            break;
    }
    
    return 1;
}

void led_switch(uint8_t color, uint8_t onoff)
{
    if( color == LED_RED )
    {
        if( onoff == 0 )
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_5);
        }
        else
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        }
    }
    
    if( color == LED_GREEN )
    {
        if( onoff == 0 )
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_0);
        }
        else
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        }
    }

    if( color == LED_BLUE )
    {
        if( onoff == 0 )
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_1);
        }
        else
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_1);
        }
    }
}

void led_toogle(uint8_t color)
{
    if( color == LED_RED )
    {
    }
}
