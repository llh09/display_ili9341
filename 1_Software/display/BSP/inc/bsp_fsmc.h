#ifndef __BSP_FSMC_H
#define __BSP_FSMC_H

#include "stm32f10x.h"

void fsmc_mode_config(void);
void fsmc_gpio_config(void);
void fsmc_dma_config(uint32_t Paddr);
void fsmc_dma_send(uint16_t len);

#endif
