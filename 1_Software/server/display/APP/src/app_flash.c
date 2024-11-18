#include "app_flash.h"
#include <string.h>

DevInfo_t DevInfo;

// spi flash operation function
uint8_t W25Q64BV_init(void)
{
    uint8_t err_count = 0;

    while(1)
    {
        if( W25Q64BV_read_jedce_ID() != 0xEF4017 )
        {
            err_count++;
            if( err_count > 5 )
            {
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
}

void W25Q64BV_wait_write_end(void)
{
    uint8_t W25Q64BV_Status = 0;
    W25Q64VB_cs_low();

    spi1_send_byte(ReadStatusRegister1);

    do
    {
        W25Q64BV_Status = spi1_send_byte(DummyByte);
    }
    while((W25Q64BV_Status & BUSY_FLAG) == SET);

    W25Q64VB_cs_high();
}

void W25Q64BV_write_enable(void)
{
    W25Q64VB_cs_low();
    spi1_send_byte(WriteEnable);
    W25Q64VB_cs_high();
}

/*
 * 扇区擦除：擦除 4096 个 byte
 * Created by LH，2024.4.2
 */
void W25Q64BV_sector_erase(uint32_t SectorAddr)
{
    W25Q64BV_write_enable();

    W25Q64VB_cs_low();
    spi1_send_byte(SectorErase);
    spi1_send_byte((SectorAddr & 0xFF0000) >> 16);
    spi1_send_byte((SectorAddr & 0xFF00) >> 8);
    spi1_send_byte(SectorAddr & 0xFF);
    W25Q64VB_cs_high();

    W25Q64BV_wait_write_end();
}

void W25Q64BV_chip_erase(void)
{
    W25Q64BV_write_enable();
    W25Q64BV_wait_write_end();

    W25Q64VB_cs_low();
    spi1_send_byte(ChipErase);
    W25Q64VB_cs_high();

    W25Q64BV_wait_write_end();
}

void W25Q64BV_page_write(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData)
{
	W25Q64BV_write_enable();
	
	W25Q64VB_cs_low();
	spi1_send_byte(PageProgram);
	spi1_send_byte((WriteAddr & 0xFF0000) >> 16);
	spi1_send_byte((WriteAddr & 0xFF00) >> 8);
	spi1_send_byte(WriteAddr & 0xFF);

	if( LengthOfData > W25Q64BV_MaxPageSize )
	{
		return;
	}

	while( LengthOfData-- )
	{
		spi1_send_byte(*data);
		data++;
	}

	W25Q64VB_cs_high();

	W25Q64BV_wait_write_end();
}

uint32_t W25Q64BV_read_jedce_ID(void)
{
	uint8_t temp1, temp2, temp3;
	uint32_t temp;

	W25Q64VB_cs_low();

	spi1_send_byte(ReadJedecID);

	temp1 = spi1_send_byte(DummyByte);
	temp2 = spi1_send_byte(DummyByte);
	temp3 = spi1_send_byte(DummyByte);

	W25Q64VB_cs_high();

	temp = (temp1 << 16) | (temp2 << 8) | temp3;

	return temp;
}

void W25Q64BV_buffer_write(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData)
{
	uint8_t NumOfPage   = 0;
	uint8_t NumOfSingle = 0;
	uint8_t Addr        = 0;
	uint8_t count       = 0;
	uint8_t temp        = 0;

	Addr        = WriteAddr    % W25Q64BV_MaxPageSize;  // 检查要要写入的地址是否页对齐
	NumOfPage   = LengthOfData / W25Q64BV_MaxPageSize;  // 计算出这次一共要写入多少页
	NumOfSingle = LengthOfData % W25Q64BV_MaxPageSize;  // 计算出这次剩余不满一页的字节数
	count       = W25Q64BV_MaxPageSize - WriteAddr;     // 计算出刚好可以页对齐的字节数

	// 如果 Addr == 0 , 则 WriteAddr 刚好是页对齐的, 就从 WriteAddr 开始写入就可以了
	if( Addr == 0 )
	{
		if( NumOfPage == 0 )
		{
			// 如果要写入的数据不满一页, 直接写入
			W25Q64BV_page_write(WriteAddr, data, LengthOfData);
		}
		else
		{
			// 如果要写如的地址超过了一页, 就先把满一页的数据写完
			while( NumOfPage-- )
			{
				W25Q64BV_page_write(WriteAddr, data, W25Q64BV_MaxPageSize);
				WriteAddr += W25Q64BV_MaxPageSize;
				data += W25Q64BV_MaxPageSize;
			}
			// 整数页都写完后, 再把剩下的写入后面一页
			W25Q64BV_page_write(WriteAddr, data, NumOfSingle);
		}
	}
	else
	{
		if( NumOfPage == 0 )
		{
			if( NumOfSingle > count )
			{
				W25Q64BV_page_write(WriteAddr, data, count);

				temp = NumOfSingle - count;
				WriteAddr += count;
				data += count;

				W25Q64BV_page_write(WriteAddr, data, temp);
			}
			else
			{
				W25Q64BV_page_write(WriteAddr, data, LengthOfData);
			}
		}
		else
		{
			LengthOfData -= count;
			NumOfPage =  LengthOfData / W25Q64BV_MaxPageSize;
			NumOfSingle = LengthOfData % W25Q64BV_MaxPageSize;

			W25Q64BV_page_write(WriteAddr, data, count);

			WriteAddr +=  count;
			data += count;

			while( NumOfPage-- )
			{
				W25Q64BV_page_write(WriteAddr, data, W25Q64BV_MaxPageSize);
				WriteAddr +=  W25Q64BV_MaxPageSize;
				data += W25Q64BV_MaxPageSize;
			}
			if (NumOfSingle != 0)
			{
				W25Q64BV_page_write(WriteAddr, data, NumOfSingle);
			}
		}
	}
}

void W25Q64BV_buffer_read(uint32_t WriteAddr, uint8_t * data, uint16_t LengthOfData)
{
	W25Q64VB_cs_low();

	spi1_send_byte(ReadDate);

	spi1_send_byte((WriteAddr & 0xFF0000) >> 16);
	spi1_send_byte((WriteAddr& 0xFF00) >> 8);
	spi1_send_byte(WriteAddr & 0xFF);

	while( LengthOfData-- )
	{
		*data = spi1_send_byte(DummyByte);
		data++;
	}

	W25Q64VB_cs_high();
}

void W25Q64BV_power_down(void)
{ 
  W25Q64VB_cs_low();
  spi1_send_byte(PowerDown);
  W25Q64VB_cs_high();
}

void W25Q64BV_wake_up(void)
{
  W25Q64VB_cs_low();
  spi1_send_byte(WakeUp);
  W25Q64VB_cs_high();
}

#if 0
uint8_t save_sysinfo_to_flash(uint32_t addr, const SystemInfo_t * buff)
{
	uint8_t count = 0;
	uint8_t write[sizeof(SystemInfo_t)];
	uint8_t read[sizeof(SystemInfo_t)];

	memcpy(write, (uint8_t *)buff, sizeof(SystemInfo_t));

	do
	{
		W25Q64BV_buffer_write(addr, write, sizeof(SystemInfo_t));
		count++;
		W25Q64BV_buffer_read(addr, read, sizeof(SystemInfo_t));

		if( memcmp(write, read, sizeof(SystemInfo_t)) == 0 )
		{
			system_info.fault.bits.flash = 0;
			return 1;
		}
	}while( count < 4 );

	system_info.fault.bits.flash = 1;

	return 0;
}
#endif
