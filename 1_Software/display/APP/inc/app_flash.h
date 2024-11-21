#ifndef __APP_W25Q64_H
#define __APP_W25Q64_H

#include "stm32f10x.h"
#include "bsp_spi.h"

#define W25Q64VB_cs_low()      GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define W25Q64VB_cs_high()     GPIO_SetBits(GPIOC, GPIO_Pin_0)

// data address
#define SYSTEM_INFO_ADDR            0

// status register 1
#define BUSY_FLAG                   (0x01 << 0) // bit[0]   : is set to a 1 when the device is executing a Page Program, Sector Erase, Block Erase,
                                                //            Chip Erase or Write Status Register instruction.
#define WEL_FLAG                    (0x01 << 1) // bit[1]   : Write enable latch
#define BP_FLAG                     (0x07 << 2) // bit[4:2] : Block protect
#define TP_FLAG                     (0x01 << 5) // bit[5]   : Top/Bottom write protect
#define SEC_FLAH                    (0x01 << 6) // bit[6]   : Sector protect
#define SRP0_FLAG                   (0x01 << 7) // bit[7]   : Status register protect 0

// status register 2
// #define SRP0_FLAG                   (0x01 << 0) // bit[0]   : Status register protect 1
#define QE_FLAG                     (0x01 << 1) // bit[1]   : Quad enable


#define W25Q64BV_MaxPageSize        256

#define WriteEnable                 0x06
#define WriteDIsable                0x04
#define ReadStatusRegister1         0x05
#define ReadStatusRegister2         0x35
#define WriteStatusRegister         0x01
#define ReadDate                    0x03
#define FastRead                    0x0B
#define PageProgram                 0x02
#define SectorErase                 0x20
#define BlockErase32KB              0x52
#define BlockErase64KB              0xD8
#define ChipErase                   0xC7  // or 0x60
#define EraseSuspend                0x75
#define EraseResume                 0x74
#define PowerDown                   0xB9
#define ReadManufacturerOrDeviceID  0x90
#define ReadUniqueIDNumber          0x4B
#define ReadJedecID                 0x9F
#define ContinuousReadModeReset     0xFF  //  or 0xFFFF
#define DummyByte                   0xFF
#define WakeUp                      0xAB

typedef struct 
{
    uint8_t Status_OnBoardLed;
}DevInfo_t;

extern DevInfo_t DevInfo;

uint8_t W25Q64BV_init(void);
void W25Q64BV_wait_write_end(void);
void W25Q64BV_write_enable(void);
void W25Q64BV_sector_erase(uint32_t SectorAddr);
void W25Q64BV_chip_erase(void);
void W25Q64BV_page_write(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData);
uint32_t W25Q64BV_read_jedce_ID(void);
void W25Q64BV_buffer_write(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData);
void W25Q64BV_buffer_read(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData);
void W25Q64BV_power_down(void);
void W25Q64BV_wake_up(void);

// uint8_t save_sysinfo_to_flash(uint32_t addr, const SystemInfo_t * buff);

#endif
