#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f10x.h"
#include "stdio.h"

/********************* define uart dma rx and tx buffer size *********************/
#define USART1_RX_BUFF_LEN      64
#define USART1_TX_BUFF_LEN      64
#define USART1_RING_BUFF_LEN    256

#define RING_BUFFER_EMPTY       1
#define RING_BUFFER_FULL        2

#define MESSAGE_LEN_MIN         6

#define MESSAGE_HEAD            0xAA
#define MESSAGE_TAIL            0xBB

#define MESSAGE_HEAD_ADDR       0
#define MESSAGE_CMD_ADDR        1
#define MESSAGE_LEN_ADDR        2
#define MESSAGE_DATA_ADDR       3

// cmd
#define CMD_BOOT                0x00
#define CMD_HEART_BEAT          0x01
#define CMD_LED_MODE_SET        0x10
#define CMD_LED_MODE_GET        0x11
#define CMD_DISPLAY_EVENT       0x20
#define CMD_DISPLAY_PIC         0x21

// len
#define CMD_HEART_BEAT_LEN      3
#define CMD_LED_GET_LEN         3

typedef struct
{
    uint16_t head;
    uint16_t tail;
    uint16_t len;
    uint16_t IDLELen;
    uint16_t USEDLen;
    uint8_t  Full_Flag; // 1 - full, 0 - not full
    uint8_t  buff[USART1_RING_BUFF_LEN];
}Ring_Bufff_t;

typedef struct
{
    uint8_t Rx_Buff[USART1_RX_BUFF_LEN];    // Rx buffer
    uint8_t Tx_Buff[USART1_TX_BUFF_LEN];    // Tx buffer

    uint8_t DMA_Tx_Flag;                        // DMA transmitting flag: 1 - transmitting, 0 - idle

    Ring_Bufff_t ring_buff;
}UART_Info_t;

extern UART_Info_t USART1_Info;
/********************* USART1 related function declaration *********************/
void uart1_init(void);
void uart1_send_buffer(uint8_t *data, uint8_t len);
void message_find_head(Ring_Bufff_t *pRingBuffer);
uint8_t ring_buffer_push_data(Ring_Bufff_t *pRingBuffer, uint8_t *data, uint16_t len);
uint16_t ring_buffer_peek_data(const Ring_Bufff_t RingBuffer, uint8_t *data, uint16_t len);
uint16_t ring_buffer_get_data(Ring_Bufff_t *RingBuffer, uint8_t *data, uint16_t len);
uint8_t ring_buffer_empty_or_full(const Ring_Bufff_t RingBuffer);

void uart1_dma_start_tx_test(void);

void message_loop(void);

#endif
