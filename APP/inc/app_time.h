#ifndef __APP_TIME_H__
#define __APP_TIME_H__

#include "stdint.h"

typedef uint64_t clock_time_t;

#pragma pack(push, 2)

typedef struct
{
    clock_time_t start;
    clock_time_t interval;
}timer_t;

#pragma pack(pop)

clock_time_t clock_time(void);
void timer_update(void);
void timer_set(timer_t *t, clock_time_t interval);
int timer_expired(timer_t *t);
void timer_reset(timer_t *t);
void timer_restart(timer_t *t);
clock_time_t timer_remaining(timer_t *t);

#endif
