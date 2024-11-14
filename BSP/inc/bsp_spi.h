#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"

void spi1_init(void);
uint8_t spi1_send_byte(uint8_t data);
void spi1_dma_send_buffer(uint8_t *buffer, uint16_t size);
#endif
