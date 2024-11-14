#include "app_time.h"

clock_time_t systemm_tick_1ms = 0;

clock_time_t clock_time(void)
{
	return (systemm_tick_1ms);
}

void timer_update(void)
{
	systemm_tick_1ms++;
}

void timer_set(timer_t *t, clock_time_t interval)
{
	t->interval = interval;
	t->start = clock_time();
}

int timer_expired(timer_t *t)
{
	clock_time_t diff = (clock_time() - t->start) + 1;
	return (t->interval < diff);
}

void timer_reset(timer_t *t)
{
	t->start += t->interval;
}

void timer_restart(timer_t *t)
{
	t->start = clock_time();
}

clock_time_t timer_remaining(timer_t *t)
{
	return (t->start + t->interval - clock_time());
}
