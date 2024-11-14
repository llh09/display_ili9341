#include "app_hmi.h"
#include "task_pc.h"
#include "bsp_led.h"
#include "task_hmi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "string.h"

// #define TASK_RUN

char str[200];
char *str1 = "LH & PJY";
char *str2 = "Hello world";;
static char count = 0x30;

static TimerHandle_t timer_1s_handler = NULL;

static void timer_callback(void* parameter)
{
    count++;
    
    if( count == 0x30 + 0x0A )
    {
        count = 0x30;
    }
}

void task_hmi(void *pvParameters)
{
    timer_1s_handler = xTimerCreate("timer", (TickType_t)1000, pdTRUE, (void *const)1, timer_callback);
    xTimerStart(timer_1s_handler, 0);
    
    // display_clear(0, 0, 50, 50, RED);
    // vTaskDelay(2000);
    
    // memcpy(str, "Hello world", sizeof("Hello world"));
    // display_show_str(0, "Hello world");
    while(1)
    {
#ifdef TASK_RUN
        memset(str, 0x20, 200);
        if( count > 0x34 )
        {
            memcpy(str, "LH & PJY", sizeof("LH & PJY") - 1);
            memset(&str[199], 0x00, 1);
        }
        else
        {
            memcpy(str, "Hello world", sizeof("Hello world") - 1);
            memcpy(&str[63], "asdsad", sizeof("asdsad") - 1);
            memset(&str[199], 0x00, 1);
        }
        
        // memcpy(&str[42], &count, 1);
        display_show_str(16, str);

        // display_show_char(((sizeof("Now count : ") - 1) * 8), 16, count, BLACK);
#else
        display_clear(88, 96, 64, 128, WHITE);
        vTaskDelay(20);
        display_clear(88, 96, 64, 128, BLACK);
        vTaskDelay(20);
        display_clear(88, 96, 64, 128, RED);
        vTaskDelay(20);
        display_clear(88, 96, 64, 128, GREEN);
        vTaskDelay(20);
        display_clear(88, 96, 64, 128, BLUE);

        // display_clear(0, 0, 240, 320/2, GREEN);
        // display_clear(0, 0, 240, 320/2, BLUE);
#endif
        vTaskDelay(50);
    }
}
