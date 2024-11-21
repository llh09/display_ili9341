// #include official and system head file
#include "stm32f10x.h"
#include <string.h>

// include bsp file
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "bsp_spi.h"
#include "bsp_fsmc.h"

// include app file
#include "app_flash.h"
#include "app_time.h"
#include "app_hmi.h"
#include "font.h"

#define RESET_TIME  (1000 * 60 * 60 * 24 * 10)

// soft delay, used for debug
void delay(volatile unsigned long count);

char c = 'A';
int main(void)
{    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    uart1_init();
    led_init();
    timer6_init();
    spi1_init();
    W25Q64BV_init();
    display_init();
    
    display_init_windows();

    while(1)
    {
        led_task();
        message_loop();
        display_loop();
    }
}

void delay(volatile unsigned long count)
{
    while(count--)
    {}
}
