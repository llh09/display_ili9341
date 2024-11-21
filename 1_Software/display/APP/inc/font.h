#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"

typedef struct _tFont
{
    const uint8_t *table;
    uint16_t Width;
    uint16_t Height;
}sFONT;

extern sFONT Font8x16;

#endif
