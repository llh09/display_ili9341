#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f10x.h"

typedef struct
{
    uint32_t id;
    uint8_t len;
    uint8_t * data[8];
}can_type;

typedef struct
{
    can_type can_frame[10];
    uint8_t head;
    uint8_t tail;
    uint8_t full;
}can_ring_type;

enum
{
    DIAG_CAN_ID_0x200 = 0,
    ID_MAX_NUMBER
};




#define DIAG_CAN_ID 0x200


extern uint8_t CAN1_send_status;
extern uint8_t CAN1_send_box;
#endif
