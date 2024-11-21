#include "bsp_fsmc.h"
#include <stddef.h>
#include "app_hmi.h"

void fsmc_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOD, &GPIO_InitStructure );




    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_Init(GPIOD, &GPIO_InitStructure );



    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOD, &GPIO_InitStructure );
}

void fsmc_mode_config(void)
{
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    FSMC_ReadWriteTimingStruct.FSMC_AccessMode              = FSMC_AccessMode_B;
    FSMC_ReadWriteTimingStruct.FSMC_AddressHoldTime         = 0x00;
    FSMC_ReadWriteTimingStruct.FSMC_AddressSetupTime        = 0x01;
    FSMC_ReadWriteTimingStruct.FSMC_BusTurnAroundDuration   = 0x00;
    FSMC_ReadWriteTimingStruct.FSMC_CLKDivision             = 0x00;
    FSMC_ReadWriteTimingStruct.FSMC_DataLatency             = 0x00;
    FSMC_ReadWriteTimingStruct.FSMC_DataSetupTime           = 0x04;

    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait         = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_Bank                     = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode          = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux           = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode             = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth          = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType               = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct    = &FSMC_ReadWriteTimingStruct;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal               = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive         = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity       = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode                 = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst               = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation           = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct        = &FSMC_ReadWriteTimingStruct;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void fsmc_dma_config(uint32_t Paddr)
{
    DMA_InitTypeDef DMA_InitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStruct.DMA_BufferSize                       = 100;
    DMA_InitStruct.DMA_DIR                              = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_M2M                              = DMA_M2M_Enable;
    DMA_InitStruct.DMA_MemoryBaseAddr                   = ( ( uint32_t ) 0x60020000 );
    DMA_InitStruct.DMA_MemoryDataSize                   = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryInc                        = DMA_MemoryInc_Disable;
    DMA_InitStruct.DMA_Mode                             = DMA_Mode_Normal;
    DMA_InitStruct.DMA_PeripheralBaseAddr               = Paddr;
    DMA_InitStruct.DMA_PeripheralDataSize               = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc                    = DMA_PeripheralInc_Enable;
    DMA_InitStruct.DMA_Priority                         = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA2_Channel4, ENABLE);
}


void fsmc_dma_send(uint16_t len)
{
    // while(DMA_GetFlagStatus(DMA1_IT_TC4) == SET);
    DMA_Cmd(DMA1_Channel4, DISABLE );
    DMA_SetCurrDataCounter(DMA1_Channel4, len);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}


void DMA1_Channel4_IRQHandler(void)
{
    if( DMA_GetITStatus(DMA1_IT_TC4) == SET )
    {
        frame_end_flag = 1;
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }
}

