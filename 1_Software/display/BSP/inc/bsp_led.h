#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"
#include "app_time.h"

#define LED_STATUS_OFF      0
#define LED_STATUS_ON       1

#define LED_SUB_CMD_STATUS  0
#define LED_SUB_CMD_COLOR   1
#define LED_SUB_CMD_MODE    2

#define LED_MODE_KEEP_OFF   0
#define LED_MODE_KEEP_ON    1
#define LED_MODE_BLINK      2
#define LED_MODE_BREATH     3

typedef struct
{
    void (*led_on_off)(void);
    void (*led_change_color)(void);
}function_t;

typedef struct
{
    uint8_t status;             // 0:off; 1:on
    uint8_t mode;               // mode
    uint8_t R;                  // red
    uint8_t G;                  // green
    uint8_t B;                  // blue
    function_t fun;             // function
    timer_t led_task_timer;     // led task timer
    timer_t led_blink_timer;    // led blink timer
}led_t;

extern led_t led_info;

void led_init(void);
void led_task(void);

#endif
