#include "bsp_spi.h"

static void spi1_mode_config(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruct.SPI_Direction            = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode                 = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize             = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL                 = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA                 = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS                  = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler    = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_FirstBit             = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial        = 7;

    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);
}

static void spi1_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // SPI1 CS pin: C.0
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // SPI1 CLK pin: A.5
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // SPI1 MOSI pin: A.7
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // SPI1 MISO pin: A.6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

void spi1_init(void)
{
    spi1_mode_config();
    spi1_gpio_config();
}

uint8_t spi1_send_byte(uint8_t data)
{
#if 0
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        //
    }

    SPI_I2S_SendData(SPI1, data);


    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        // 
    }

    return SPI_I2S_ReceiveData(SPI1);
#else
    while( (SPI1->SR & 0x0002) == 0 );
    SPI1->DR = data;
    while( (SPI1->SR & 0x0001) == 0);
    return SPI1->DR;
#endif
}

void spi1_dma_send_buffer(uint8_t *buffer, uint16_t size)
{
    //
}
