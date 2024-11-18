#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"

#define LED_RED     0
#define LED_BLUE    1
#define LED_GREEN   2


void led_init(void);
uint8_t led_on(uint8_t color);
uint8_t led_off(uint8_t color);
void led_switch(uint8_t color, uint8_t onoff);
#endif
