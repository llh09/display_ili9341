#include "bsp_uart.h"
#include "bsp_led.h"
#include "bsp_timer.h"

#include "app_hmi.h"

#include <string.h>

const uint8_t *SOFTWARE_VERSION = "V1.0.0";  // software version
const uint8_t *HARDWARE_VERSION = "Fire Compass";  // hardware version

UART_Info_t USART1_Info;

uint8_t boot_flag = 0;

static void uart1_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // open clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // USART1 TX PA9
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART1 RX PA10
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void uart1_mode_config(void)
{
    USART_InitTypeDef   USART_InitStruct;
    NVIC_InitTypeDef    NVIC_InitStruct;
    DMA_InitTypeDef     DMA_InitStruct;
    
    // open clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // USART1 mode config: buadrate 9600, 1 stop bit, 8 bit data, no parity, no hardware flow control
    USART_InitStruct.USART_BaudRate                     = 9600;
    USART_InitStruct.USART_HardwareFlowControl          = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode                         = (USART_Mode_Rx | USART_Mode_Tx);
    USART_InitStruct.USART_Parity                       = USART_Parity_No;
    USART_InitStruct.USART_StopBits                     = USART_StopBits_1;
    USART_InitStruct.USART_WordLength                   = USART_WordLength_8b;

    USART_Init(USART1, &USART_InitStruct);          // initialise USART1 mode
    USART_Cmd(USART1, ENABLE);                      // ENABLE USART1
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // ENABLE USART1 IDLE interrupt

    // USART1 NVIC config: ENABLE, main priority 1, sub priority 1
    NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 1;
    NVIC_Init(&NVIC_InitStruct);

    // USART1 Rx DMA config DMA1_Channel5
    DMA_InitStruct.DMA_BufferSize                       = USART1_RX_BUFF_LEN;
    DMA_InitStruct.DMA_DIR                              = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_M2M                              = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr                   = (uint32_t)(USART1_Info.Rx_Buff);
    DMA_InitStruct.DMA_MemoryDataSize                   = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc                        = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_Mode                             = DMA_Mode_Circular;
    DMA_InitStruct.DMA_PeripheralBaseAddr               = (uint32_t)&(USART1->DR);
    DMA_InitStruct.DMA_PeripheralDataSize               = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc                    = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority                         = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel5, &DMA_InitStruct);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // initlize ring buffer head and tail
    USART1_Info.ring_buff.head      = 0;
    USART1_Info.ring_buff.tail      = 0;
    // initlize ring buffer length
    USART1_Info.ring_buff.len       = USART1_RING_BUFF_LEN;
    USART1_Info.ring_buff.IDLELen   = USART1_RING_BUFF_LEN;
    USART1_Info.ring_buff.USEDLen   = 0;
    // ring buffer empty
    USART1_Info.ring_buff.Full_Flag = 0;
    
    memset(USART1_Info.ring_buff.buff, 0, USART1_RING_BUFF_LEN);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

void uart1_init(void)
{
    uart1_gpio_config();
    uart1_mode_config();
    
    timer_set(&USART1_Info.timer.timer_message_deal, 15);   // 15ms 处理一次串口消息
    timer_set(&USART1_Info.timer.timer_boot, 1000);         // 1000ms 发送一次 boot 信息
    timer_set(&USART1_Info.timer.timer_heart_beat, 1000);   // 1000ms 发送一次心跳 
}

void uart1_send_buffer(uint8_t cmd, uint8_t len)
{
    uint8_t index = (MESSAGE_LEN_MIN + len);

    USART1_Info.Tx_Buff[0] = MESSAGE_HEAD;
    USART1_Info.Tx_Buff[1] = cmd;
    USART1_Info.Tx_Buff[2] = len;
    USART1_Info.Tx_Buff[index-3] = 0; // crcH;
    USART1_Info.Tx_Buff[index-2] = 0; // crcL;
    USART1_Info.Tx_Buff[index-1] = MESSAGE_TAIL;
    
    for( uint8_t i = 0; i < index; i++ )
    {
        USART_SendData(USART1, USART1_Info.Tx_Buff[i]);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

void USART1_IRQHandler(void)
{
    uint16_t len = 0;
    if( USART_GetITStatus(USART1, USART_IT_IDLE) == SET )
    {
        // According to RM0008 <stm32f10x Reference manual> page 812:
        // IDLE bit is cleared by a software sequence (an read to the 
        // USART_SR register followed by a read to the USART_DR register)
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
        USART_ReceiveData(USART1);

        // copy DMA Rx buffer data to USART1 ring buffer
        DMA_Cmd(DMA1_Channel5, DISABLE);
        len = USART1_RX_BUFF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
        // add message to ring buffer
        ring_buffer_push_data(&USART1_Info.ring_buff, USART1_Info.Rx_Buff, len);
        DMA_SetCurrDataCounter(DMA1_Channel5, USART1_RX_BUFF_LEN); 
        DMA_Cmd(DMA1_Channel5, ENABLE);
    }
}


uint8_t ring_buffer_push_data(Ring_Bufff_t *pRingBuffer, uint8_t *data, uint16_t len)
{
    // ring buffer left size too small
    if( (pRingBuffer->IDLELen < len) || (data == NULL) )
    {
        return 0;
    }

    while(len--)
    {
        pRingBuffer->buff[pRingBuffer->tail] = *data++;
        pRingBuffer->tail = ((pRingBuffer->tail + 1) % pRingBuffer->len);
        pRingBuffer->IDLELen--;
        pRingBuffer->USEDLen++;
    }

    if( pRingBuffer->tail == pRingBuffer->head )
    {
        pRingBuffer->Full_Flag = 1;
    }
    else
    {
        pRingBuffer->Full_Flag = 0;
    }

    return 1;
}

uint8_t ring_buffer_empty_or_full(const Ring_Bufff_t RingBuffer)
{
    uint8_t ret = RING_BUFFER_FULL;

    if( (RingBuffer.head == RingBuffer.tail) && (RingBuffer.Full_Flag == 0) )
    {
        ret = RING_BUFFER_EMPTY;
    }
    else if( (RingBuffer.head == RingBuffer.tail) && (RingBuffer.Full_Flag == 1) )
    {
        ret = RING_BUFFER_FULL;
    }

    return ret;
}

uint16_t ring_buffer_peek_data(const Ring_Bufff_t RingBuffer, uint8_t *data, uint16_t len)
{
    uint16_t Index;

    // copy data to peek buffer
    Index = RingBuffer.head;
    for( uint16_t i = 0; i < len; i++, Index++ )
    {
        if( Index == RingBuffer.len )
        {
            Index = 0;
        }

        data[i] = RingBuffer.buff[Index];
    }

    // return length we have peeked
    return len;
}

uint16_t ring_buffer_get_data(Ring_Bufff_t *pRingBuffer, uint8_t *data, uint16_t len)
{
    uint16_t l;

    if( len > pRingBuffer->USEDLen )
    {
        len = pRingBuffer->USEDLen;
    }

    l = len;

    while(len--)
    {
        *data++ = pRingBuffer->buff[pRingBuffer->head];
        pRingBuffer->head = ((pRingBuffer->head + 1) % pRingBuffer->len);
        pRingBuffer->IDLELen++;
        pRingBuffer->USEDLen--;
    }

    return l;
}

void message_find_head(Ring_Bufff_t *pRingBuffer)
{
    // if first byte is not 0xAA(MESSAGE_HEAD) and ring buffer left size > 6,
    // move head to next byte to fine MESSAGE_HEAD
    do
    {
        pRingBuffer->head = ((pRingBuffer->head + 1) % pRingBuffer->len);
        pRingBuffer->IDLELen++;
        pRingBuffer->USEDLen--;
    }
    while( (pRingBuffer->buff[pRingBuffer->head] != MESSAGE_HEAD) && (pRingBuffer->IDLELen > (pRingBuffer->len - MESSAGE_LEN_MIN)) );
}


#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;
	return ch;
}
#endif

/************************************************ 分割线 **************************************************/
// 0x00 : boot
static void deal_cmd_boot(const uint8_t * data)
{
    if( data[0] == 0x01 )
    {
        boot_flag = 1;
    }
}

// 0x01 : heart beat
static void deal_cmd_heart_beat(const uint8_t * data)
{
    if( data[0] >= 1 && data[0] <= 255 )
    {
        //
    }
}

// 0x3 : led set
static void deal_led_set_cmd(const uint8_t *data)
{
    if( data[0] == LED_SUB_CMD_STATUS )
    {
        led_info.status = data[1];

        led_info.fun.led_on_off();
    }
    else if( data[0] == LED_SUB_CMD_COLOR )
    {
        led_info.R = data[1];
        led_info.G = data[2];
        led_info.B = data[3];

        // led_info.fun.led_change_color();
    }
    else if( data[0] == LED_SUB_CMD_MODE )
    {
        led_info.mode = data[1];

        if( led_info.mode == LED_MODE_BLINK )
        {
            timer_set(&led_info.led_blink_timer, (data[2] << 8 | data[3]));
        }
    }
}

// 0x20 : display key event
static void deal_cmd_display_event(const uint8_t * data)
{
    switch( data[0] )
    {
        case 0:
        {
            display_info.event.bits.key_up = 1;
            break;
        }
        case 1:
        {
            display_info.event.bits.key_down = 1;
            break;
        }
        case 2:
        {
            display_info.event.bits.key_left = 1;
            break;
        }
        case 3:
        {
            display_info.event.bits.key_right = 1;
            break;
        }
        case 4:
        {
            display_info.event.bits.key_enter = 1;
            break;
        }
        case 5:
        {
            display_info.event.bits.key_esc = 1;
            break;
        }
        default:break;
    }
}


// 0x21 : receive picture
static void deal_cmd_receive_pic(const uint8_t * data, const uint8_t len)
{
    uint8_t temp[2] = {0x00, 0x00};
    static uint16_t temp_index = 0;
    
    temp[0] = data[0];
    temp[1] = data[1];
    
    switch( temp[0] )
    {
        case 0x12:
            if( temp[1] == 0x34 )
            {
                display_info.Status = STA_RECVING;
                temp_index = 0;
                return;
            }
        case 0x34:
            if( temp[1] == 0x12 )
            {
                display_info.Status = STA_IDLE;
                temp_index = 0;
                display_info.ActiveWindow = WINDOWS_2;
                return;
            }
        default:break;
    }
    
    if( display_info.Status == STA_RECVING )
    {
        for( uint8_t i = 0; i < len; i += 2 )
        {
            temp[0] = data[i + 1];
            temp[1] = data[i];

            if( temp_index == LCD_BUFFER_SIZE )
            {
                return;
            }
            display_info.display_buffer[temp_index++] = ((temp[0] << 8) | temp[1]);
        }
    }
}

static void send_boot(void)
{
    uint8_t index = MESSAGE_DATA_ADDR;
    
    if( timer_expired(&USART1_Info.timer.timer_boot) && (boot_flag == 0) )
    {
        timer_restart(&USART1_Info.timer.timer_boot);
        
        memset(&USART1_Info.Tx_Buff, 0x00, USART1_TX_BUFF_LEN);

        USART1_Info.Tx_Buff[index++] = '1';                         // mi ma
        USART1_Info.Tx_Buff[index++] = '1';
        USART1_Info.Tx_Buff[index++] = '3';
        USART1_Info.Tx_Buff[index++] = '4';
        USART1_Info.Tx_Buff[index++] = 'V';                         // software version
        USART1_Info.Tx_Buff[index++] = '1';
        USART1_Info.Tx_Buff[index++] = '.';
        USART1_Info.Tx_Buff[index++] = '0';
        USART1_Info.Tx_Buff[index++] = '.';
        USART1_Info.Tx_Buff[index++] = '0';
        USART1_Info.Tx_Buff[index++] = 'F';                         // hardware version
        USART1_Info.Tx_Buff[index++] = 'i';
        USART1_Info.Tx_Buff[index++] = 'r';
        USART1_Info.Tx_Buff[index++] = 'e';
        USART1_Info.Tx_Buff[index++] = '-';
        USART1_Info.Tx_Buff[index++] = 'C';
        USART1_Info.Tx_Buff[index++] = 'o';
        USART1_Info.Tx_Buff[index++] = 'm';
        USART1_Info.Tx_Buff[index++] = 'p';
        USART1_Info.Tx_Buff[index++] = 'a';
        USART1_Info.Tx_Buff[index++] = 's';
        USART1_Info.Tx_Buff[index++] = 's';
        uart1_send_buffer(CMD_BOOT, CMD_BOOT_LEN);
    }
}

uint8_t aaa = 0;
uint8_t bbb = 0;
static void send_heart_beat(void)
{
    static uint8_t heatr_beat = 0;
    uint8_t index = MESSAGE_DATA_ADDR;
    
    if( timer_expired(&USART1_Info.timer.timer_heart_beat) )
    {
        timer_restart(&USART1_Info.timer.timer_heart_beat);

        USART1_Info.Tx_Buff[index++] = heatr_beat++;                // heart beat count
        USART1_Info.Tx_Buff[index++] = aaa;                        // fault level
        USART1_Info.Tx_Buff[index++] = bbb;                        // fault code
        USART1_Info.Tx_Buff[index++] = display_info.ActiveWindow;   // current windows
        
        if( heatr_beat >= 255 )
        {
            heatr_beat = 0;
        }

        uart1_send_buffer(CMD_HEART_BEAT, CMD_HEART_BEAT_LEN);
    }
}

static void message_deal(void)
{
    uint8_t dealBuffer[256];
    uint16_t len;
    uint8_t crcH;
    uint8_t crcL;

    if( ring_buffer_empty_or_full(USART1_Info.ring_buff) == RING_BUFFER_EMPTY )
    {
        return;
    }

    if( USART1_Info.ring_buff.buff[USART1_Info.ring_buff.head + MESSAGE_HEAD_ADDR] == MESSAGE_HEAD )
    {
        // peek minimum data frame
        ring_buffer_peek_data(USART1_Info.ring_buff, dealBuffer, MESSAGE_LEN_MIN);
        // get data frame length
        len = dealBuffer[MESSAGE_LEN_ADDR] + MESSAGE_LEN_MIN;
        // if the left data in buffer is not enough, return, wait next time
        if( ring_buffer_peek_data(USART1_Info.ring_buff, dealBuffer, len) != len )
        {
            return;
        }

        // check CRC
        crcH = 0x00;  // cal_crc(dealBuffer, len + MESSAGE_LEN_ADDR);
        crcL = 0x00;  // cal_crc(dealBuffer, len + MESSAGE_LEN_ADDR);
        if( crcH == dealBuffer[len - 3] && crcL == dealBuffer[len - 2] )
        {
            // if crc verification pass, get data frame
            ring_buffer_get_data(&USART1_Info.ring_buff, dealBuffer, len);
        }
        else
        {
            message_find_head(&USART1_Info.ring_buff);
            return;
        }

        switch(dealBuffer[MESSAGE_CMD_ADDR])
        {
            case CMD_BOOT:              // 0x00 boot cmd, not used yet
                deal_cmd_boot(&dealBuffer[MESSAGE_DATA_ADDR]);
                break;
            case CMD_HEART_BEAT:        // 0x01 heart beat cmd
                deal_cmd_heart_beat(&dealBuffer[MESSAGE_DATA_ADDR]);
                break;
            case CMD_LED_MODE_SET:      // 0x10 led mode set cmd
                deal_led_set_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                break;
            case CMD_LED_MODE_GET:      // 0x11 led mode get cmd
                // deal_led_get_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                break;
            case CMD_DISPLAY_EVENT:     // 0x20 display event cmd
                deal_cmd_display_event(&dealBuffer[MESSAGE_DATA_ADDR]);
                break;
            case CMD_DISPLAY_PIC:       // 0x21 display show picture cmd
                deal_cmd_receive_pic(&dealBuffer[MESSAGE_DATA_ADDR], dealBuffer[MESSAGE_LEN_ADDR]);
                break;
            default:
                break;
        }
    }
    else
    {
        // if the first byte is not 0xAA(MESSAGE_HEAD), find the head
        message_find_head(&USART1_Info.ring_buff);
        return;
    }
}

static void message_send(void)
{
    send_boot();
    send_heart_beat();
}

void message_loop(void)
{
    if( timer_expired(&USART1_Info.timer.timer_message_deal) )
    {
        timer_restart(&USART1_Info.timer.timer_message_deal);
        
        message_send();
        message_deal();
    }
}

