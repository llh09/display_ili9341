#include "bsp_can.h"
#include "stdlib.h"
#include "string.h"

uint8_t CAN1_send_status = 0;
uint8_t CAN1_send_box = 0;

can_ring_type can_rec;
uint8_t messaage[ID_MAX_NUMBER];

static void can_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void can_mode_config(void)
{
    CAN_InitTypeDef CAN_InitStruct;
    CAN_FilterInitTypeDef CAN_FilterInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    CAN_InitStruct.CAN_Mode                                 = CAN_OperatingMode_Normal;     // 工作在正常模式
    CAN_InitStruct.CAN_BS1                                  = CAN_BS1_8tq;                  // Buad Rate = 36M/((1 + (8 + 1) + (1 + 7))*(3 + 1))
    CAN_InitStruct.CAN_BS2                                  = CAN_BS2_7tq;                  //           = 36M / 72
    CAN_InitStruct.CAN_Prescaler                            = 3;                            //           = 500kbps
    CAN_InitStruct.CAN_SJW                                  = CAN_SJW_4tq;                  // 同步跳跃宽度 4Tq
    CAN_InitStruct.CAN_RFLM                                 = DISABLE;                      // 接收FIFO锁定关闭，如果接收FIFO已满，下一条消息会覆盖前一条
    CAN_InitStruct.CAN_TTCM                                 = DISABLE;                      // 时间触发通信模式关闭
    CAN_InitStruct.CAN_TXFP                                 = DISABLE;                      // 传输FIFO优先级，取决于消息ID
    CAN_InitStruct.CAN_ABOM                                 = DISABLE;                      // 自动总线离线关闭关闭
    CAN_InitStruct.CAN_AWUM                                 = DISABLE;                      // 自动唤醒关闭
    CAN_InitStruct.CAN_NART                                 = ENABLE;                       // 自动重传关闭

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    CAN_Init(CAN1, &CAN_InitStruct);

    CAN_FilterInitStruct.CAN_FilterActivation               = ENABLE;                       // 滤波器使能
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment           = CAN_Filter_FIFO0;             // 滤波器分配到FIFO0
    CAN_FilterInitStruct.CAN_FilterIdHigh                   = 0x0000;                       // 所有ID都可以通过
    CAN_FilterInitStruct.CAN_FilterIdLow                    = 0x0000;                       // 所有ID都可以通过
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh               = 0x0000;                       // 所有ID都可以通过
    CAN_FilterInitStruct.CAN_FilterMaskIdLow                = 0x0000;                       // 所有ID都可以通过
    CAN_FilterInitStruct.CAN_FilterMode                     = CAN_FilterMode_IdMask;        // 过滤器使用掩码模式
    CAN_FilterInitStruct.CAN_FilterNumber                   = 0;
    CAN_FilterInitStruct.CAN_FilterScale                    = CAN_FilterScale_32bit;

    CAN_FilterInit(&CAN_FilterInitStruct);

    NVIC_InitStructure.NVIC_IRQChannel                      = (USB_LP_CAN1_RX0_IRQn | USB_HP_CAN1_TX_IRQn);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void can_init(void)
{
    can_gpio_config();
    can_mode_config();
}

uint8_t can1_send_frame(uint8_t * data)
{
    if( CAN1_send_status == 0 )
    {
        CanTxMsg TxMessage;

        TxMessage.StdId = 0x12;
        TxMessage.ExtId = 0x12;
        TxMessage.IDE   = 0;
        TxMessage.RTR   = 0;
        TxMessage.DLC   = 8;

        memcpy(TxMessage.Data, data, 8);

        CAN1_send_status = 1;
        CAN1_send_box = CAN_Transmit(CAN1, &TxMessage);
        return 1;
    }
    else
    {
        return 0;
    }
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);

    // 记录帧ID
    if( RxMessage.IDE == 1 )
    {
        can_rec.can_frame[can_rec.tail].id  = RxMessage.ExtId;
    }
    else
    {
        can_rec.can_frame[can_rec.tail].id  = RxMessage.StdId;
    }
    // 记录帧长度
    can_rec.can_frame[can_rec.tail].len = RxMessage.DLC;
    // 保存帧数据
    memset(can_rec.can_frame[can_rec.tail].data, 0, 8);
    memcpy(can_rec.can_frame[can_rec.tail].data, RxMessage.Data, can_rec.can_frame[can_rec.tail].len);

    // 移动队列尾指针，并且判断buffer是否已满
    can_rec.tail = ((can_rec.tail + 1) % 10);
    if( can_rec.tail == can_rec.head )
    {
        can_rec.full = 1;
    }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
    CAN1_send_status = 0;
}
