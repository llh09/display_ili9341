#include "app_hmi.h"
#include "bsp_led.h"
#include "font.h"
#include "app_time.h"
#include "string.h"

#define TFT_USE_DMA

// lcd id
uint16_t lcd_ID = 0;
// scan mode
uint8_t LCD_SCAN_MODE = 6;

// lcd background cloor
uint16_t display_background_color = WHITE;

// uint16_t display_buffer[LCD_BUFFER_SIZE];
uint8_t frame_end_flag = 1;

display_info_t display_info;
page_info_t page_main;
page_info_t page_show_pic;
page_info_t page_sys_info;

timer_t display_show;

void display_delay(volatile unsigned long count)
{
    while(count--)
    {}
}


uint16_t display_read_data(void)
{
	return (*(__IO uint16_t *)(FSMC_Addr_ILI9341_DATA));
}


void display_write_data(uint16_t Data)
{
	*(__IO uint16_t *)(FSMC_Addr_ILI9341_DATA) = Data;
}


void display_write_cmd(uint16_t Cmd)
{
	*(__IO uint16_t *)(FSMC_Addr_ILI9341_CMD) = Cmd;
}


uint16_t display_read_ID(void)
{
    uint16_t id = 0;

    display_write_cmd(0x04);
    display_read_data();
    display_read_data();

    id = display_read_data();
    id <<= 8;
    id |= display_read_data();
	
    if(id == 0x8552)
    {
        return id;
    }
    else
    {
        display_write_cmd(0xD3);
        display_read_data();
        display_read_data();
        id = display_read_data();
        id <<= 8;
        id |= display_read_data();
        if(id == 0x9341)
        {
            return id;
        }
    }

    return 0;
}

static void display_reg_config(void)
{
    lcd_ID = display_read_ID();
    display_delay(1000000);
    /*  Power control B (CFh)  */
    DEBUG_DELAY();
    display_write_cmd(0xCF);
    display_write_data(0x00);
    display_write_data(0x81);
    display_write_data(0x30);

    /*  Power on sequence control (EDh) */
    DEBUG_DELAY();
    display_write_cmd(0xED);
    display_write_data(0x64);
    display_write_data(0x03);
    display_write_data(0x12);
    display_write_data(0x81);

    /*  Driver timing control A (E8h) */
    DEBUG_DELAY();
    display_write_cmd(0xE8);
    display_write_data(0x85);
    display_write_data(0x10);
    display_write_data(0x78);

    /*  Power control A (CBh) */
    DEBUG_DELAY();
    display_write_cmd(0xCB);
    display_write_data(0x39);
    display_write_data(0x2C);
    display_write_data(0x00);
    display_write_data(0x34);
    display_write_data(0x03);

    /* Pump ratio control (F7h) */
    DEBUG_DELAY();
    display_write_cmd(0xF7);
    display_write_data(0x20);

    /* Driver timing control B */
    DEBUG_DELAY();
    display_write_cmd(0xEA);
    display_write_data(0x00);
    display_write_data(0x00);

    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    DEBUG_DELAY();
    display_write_cmd(0xB1);
    display_write_data(0x00);
    display_write_data(0x1B);

    /*  Display Function Control (B6h) */
    DEBUG_DELAY();
    display_write_cmd(0xB6);
    display_write_data(0x0A);
    display_write_data(0xA2);

    /* Power Control 1 (C0h) */
    DEBUG_DELAY();
    display_write_cmd(0xC0);
    display_write_data(0x35);

    /* Power Control 2 (C1h) */
    DEBUG_DELAY();
    display_write_cmd(0xC1);
    display_write_data(0x11);

    /* VCOM Control 1 (C5h) */
    display_write_cmd(0xC5);
    display_write_data(0x45);
    display_write_data(0x45);

    /*  VCOM Control 2 (C7h)  */
    display_write_cmd(0xC7);
    display_write_data(0xA2);

    /* Enable 3G (F2h) */
    display_write_cmd(0xF2);
    display_write_data(0x00);

    /* Gamma Set (26h) */
    display_write_cmd(0x26);
    display_write_data(0x01);
    DEBUG_DELAY();

    /* Positive Gamma Correction */
    display_write_cmd(0xE0); //Set Gamma
    display_write_data(0x0F);
    display_write_data(0x26);
    display_write_data(0x24);
    display_write_data(0x0B);
    display_write_data(0x0E);
    display_write_data(0x09);
    display_write_data(0x54);
    display_write_data(0xA8);
    display_write_data(0x46);
    display_write_data(0x0C);
    display_write_data(0x17);
    display_write_data(0x09);
    display_write_data(0x0F);
    display_write_data(0x07);
    display_write_data(0x00);

    /* Negative Gamma Correction (E1h) */
    display_write_cmd(0XE1); //Set Gamma
    display_write_data(0x00);
    display_write_data(0x19);
    display_write_data(0x1B);
    display_write_data(0x04);
    display_write_data(0x10);
    display_write_data(0x07);
    display_write_data(0x2A);
    display_write_data(0x47);
    display_write_data(0x39);
    display_write_data(0x03);
    display_write_data(0x06);
    display_write_data(0x06);
    display_write_data(0x30);
    display_write_data(0x38);
    display_write_data(0x0F);

    /* memory access control set */
    DEBUG_DELAY();
    display_write_cmd(0x36); 	
    display_write_data(0xC8);
    DEBUG_DELAY();

    /* column address control set */
    display_write_cmd(CMD_SetCoordinateX);
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_data(0xEF);

    /* page address control set */
    DEBUG_DELAY();
    display_write_cmd(CMD_SetCoordinateY); 
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_data(0x01);
    display_write_data(0x3F);

    /*  Pixel Format Set (3Ah)  */
    DEBUG_DELAY();
    display_write_cmd(0x3a); 
    display_write_data(0x55);

    /* Sleep Out (11h)  */
    display_write_cmd(0x11);	
    display_delay(0xAFF << 2);
    DEBUG_DELAY();

    /* Display ON (29h) */
    display_write_cmd(0x29);
}

void display_backled_control(FunctionalState enumState)
{
    if( enumState )
    {
        GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    }
    else
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_12);
    }
}


void display_reset(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    display_delay(500000);
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
    display_delay(500000);
}


void display_gram_scan(uint8_t Option)
{
    uint16_t x_length;
    uint16_t y_length;

    if( Option > 7 )
    {
        return;
    }

    LCD_SCAN_MODE = Option;

    if( Option % 2 == 0 )
    {
        x_length = 240;
        y_length = 320;
    }
    else
    {
        x_length = 320;
        y_length = 240;
    }

    display_write_cmd(0x36);

    display_write_data(0x08 | (Option << 5));

    display_write_cmd(CMD_SetCoordinateX); 
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_data(((x_length - 1) >> 8) & 0xFF);
    display_write_data((x_length - 1) & 0xFF);

    display_write_cmd(CMD_SetCoordinateY);
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_data(((y_length - 1) >> 8) & 0xFF);
    display_write_data((y_length - 1) & 0xFF);

    display_write_cmd(CMD_SetPixel);
}


void display_init(void)
{
    fsmc_gpio_config();
    fsmc_mode_config();
    fsmc_dma_config((uint32_t)display_info.display_buffer);
    
    display_backled_control( ENABLE );
    display_reset();
    display_reg_config();

    display_gram_scan(LCD_SCAN_MODE);
}


void display_open_window(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
    display_write_cmd(CMD_SetCoordinateX);
    display_write_data((X & 0xFF00) >> 8);
    display_write_data(X & 0xff);
    display_write_data((X + Width - 1) >> 8);
    display_write_data((X + Width - 1) & 0xff);

    display_write_cmd(CMD_SetCoordinateY);
    display_write_data((Y & 0xFF00) >> 8);
    display_write_data(Y & 0xff);
    display_write_data((Y + Height - 1) >> 8);
    display_write_data((Y + Height - 1) & 0xff);
}

#if 0
static void display_refresh1(uint32_t NumberOfPorint, uint16_t Color)
{
    display_write_cmd(CMD_SetPixel);

#ifndef TFT_USE_DMA
    for(uint32_t i = 0; i < NumberOfPorint; i ++)
    {
        display_write_data(Color);
    }
#else
    for(uint16_t i = 0; i < LCD_BUFFER_SIZE; i++)
    {
        display_info.display_buffer[i] = Color;
    }
    fsmc_dma_send((uint16_t)LCD_BUFFER_SIZE);
#endif
}
#endif

void display_refresh(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color)
{
    uint16_t NumberOfPorint = 0;
    
    NumberOfPorint = (Width * Height);
    display_open_window(X, Y, Width, Height);
    display_write_cmd(CMD_SetPixel);

#ifndef TFT_USE_DMA
    for(uint32_t i = 0; i < NumberOfPorint; i ++)
    {
        display_write_data(Color);
    }
#else
    fsmc_dma_send(NumberOfPorint);
#endif
}

void display_clear(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color)
{
    if( frame_end_flag == 1 )
    {
        frame_end_flag = 0;
        display_open_window(X, Y, Width, Height);
        // display_refresh((Width * Height), Color);
    }
}


void display_show_char(uint16_t X, uint16_t Y, const char Char, uint16_t Color)
{
    uint8_t i, j, fontBytes;
    uint16_t RelativePositon;
    uint16_t index = 0;
    uint8_t *pChar;

    RelativePositon = Char - ' ';
    fontBytes = (Font8x16.Width * Font8x16.Height) / 8;
    pChar = (uint8_t *)&(Font8x16.table[RelativePositon * fontBytes]);
    
    index = X + ((Y + 1) * 120);
    for( i = 0; i < fontBytes; i ++ )
    {
        for( j = 0; j < 8; j++ )
        {
            if( (pChar[i] & (0x80 >> j)) )
            {
                display_info.display_buffer[index] = Color;
            }
            
            index++;
        }

        index += (120 - 8);
    }
}

void display_show_index(uint16_t Line)
{
    uint16_t index = 90;
    char *pStr = "<--";
    while(*pStr != '\0' )
    {
        if( (index - 0 + Font8x16.Width) > 240 )
        {
            index = 0;
            Line += Font8x16.Height;
        }

        if( (Line - 0 + Font8x16.Height) > 320 )
        {
            index = 0;
            Line = 0;
        }

        display_show_char(index, Line, *pStr, BLACK);

        pStr++;

        index += Font8x16.Width;
    }
}


void display_show_str(uint16_t Line, char *pStr, uint8_t len)
{
    uint16_t index = 0;
    // while(*pStr != '\0' )
    while( len-- )
    {
        if( (index - 0 + Font8x16.Width) > 120 )
        {
            index = 0;
            Line += Font8x16.Height;
        }

        if( (Line - 0 + Font8x16.Height) > 320 )
        {
            index = 0;
            Line = 0;
        }

        display_show_char(index, Line, *pStr, BLACK);

        pStr++;

        index += Font8x16.Width;
    }
}

void display_init_windows(void)
{
    // 85ms 刷新一次页面
    timer_set(&display_show, 85);
    
    // init main page
    page_main.page_index = 0;
    page_main.max_index = 2;
    page_main.font_color = BLACK;
    page_main.head = "ILI9341 V1.0.0";
    page_main.str[0] = "1.show-pic";
    page_main.str[1] = "2.sys-info";

    // init sys-info page
    page_sys_info.page_index = 0;
    page_sys_info.max_index = 1;
    page_sys_info.font_color = BLACK;
    page_sys_info.head = "1.sys-info";
    page_sys_info.str[0] = "    Tihs is a display demo";
    
    // init show-pic page
    page_show_pic.page_index = 0;
    page_show_pic.max_index = 6;
    page_show_pic.font_color = BLACK;
    page_show_pic.head = "2.show-pic";
    page_show_pic.str[0] = "1.1.pic-1";
    page_show_pic.str[1] = "1.2.pic-2";
    page_show_pic.str[2] = "1.3.pic-3";
    page_show_pic.str[3] = "1.4.pic-4";
    page_show_pic.str[4] = "1.5.pic-5";
    page_show_pic.str[5] = "1.6.pic-6";
    
    // init windows
    display_info.color[WINDOWS_1] = WHITE;
    display_info.color[WINDOWS_2] = GREEN;
    display_info.color[WINDOWS_3] = BABY_BLUE;
    display_info.color[WINDOWS_4] = YELLOW;
    
    display_info.coordinate[WINDOWS_1][AXIS_X] = 0;     // windows 1 
    display_info.coordinate[WINDOWS_1][AXIS_Y] = 0;
    display_info.coordinate[WINDOWS_2][AXIS_X] = 120;   // windows 2 
    display_info.coordinate[WINDOWS_2][AXIS_Y] = 0;
    display_info.coordinate[WINDOWS_3][AXIS_X] = 0;     // windows 3 
    display_info.coordinate[WINDOWS_3][AXIS_Y] = 160;
    display_info.coordinate[WINDOWS_4][AXIS_X] = 120;   // windows 4 
    display_info.coordinate[WINDOWS_4][AXIS_Y] = 160;
    
    display_info.ActiveWindow = WINDOWS_1;
    display_info.pCurrentPage = &page_main;
}

void deal_key_event(void)
{
    if( display_info.event.bits.key_down )
    {
        display_info.event.bits.key_down = 0;
        
        // 如果已经到顶部了，光标就不再往上
        if( display_info.pCurrentPage->page_index == (display_info.pCurrentPage->max_index - 1) )
        {
            display_info.pCurrentPage->page_index = (display_info.pCurrentPage->max_index - 1);
        }
        else
        {
            display_info.pCurrentPage->page_index++;
        }
    }
    else if( display_info.event.bits.key_up )
    {
        display_info.event.bits.key_up = 0;
        
        // 如果已经到底部了，光标就不再往下
        if( display_info.pCurrentPage->page_index == 0 )
        {
            display_info.pCurrentPage->page_index = 0;
        }
        else
        {
            display_info.pCurrentPage->page_index--;
        }
    }
    else if( display_info.event.bits.key_left )
    {
        display_info.event.bits.key_left = 0;
    }
    else if( display_info.event.bits.key_right )
    {
        display_info.event.bits.key_right = 0;
    }
    else if( display_info.event.bits.key_enter )
    {
        display_info.event.bits.key_enter = 0;
    }
    else if( display_info.event.bits.key_esc )
    {
        display_info.event.bits.key_esc = 0;
    }
}

void display_loop(void)
{
    if( timer_expired(&display_show) )
    {
        timer_restart(&display_show);
        
        deal_key_event();
        
        switch( display_info.ActiveWindow )
        {
            case WINDOWS_1:
            {
                for( uint16_t i = 0; i < LCD_BUFFER_SIZE; i++ )
                {
                    display_info.display_buffer[i] = display_info.color[display_info.ActiveWindow];
                }
                
                display_show_str(0, (char *)display_info.pCurrentPage->head, strlen((char *)display_info.pCurrentPage->head));
                
                for( uint16_t i = ((17 + 1) * 120); i < (((16 + 1) * 120) + 240); i++ )
                {
                    display_info.display_buffer[i] = BLACK;
                }
                
                for( uint8_t i = 0; i < display_info.pCurrentPage->max_index; i++ )
                {
                    display_show_str((20 + (20 * i)), (char *)display_info.pCurrentPage->str[i], strlen((char *)display_info.pCurrentPage->str[i]));
                }
                
                if( display_info.pCurrentPage != &page_sys_info )
                {
                    display_show_index(20 + (20 * display_info.pCurrentPage->page_index));
                }
                
                break;
            }
            case WINDOWS_2:
            {
                for( uint16_t i = 0; i < LCD_BUFFER_SIZE; i++ )
                {
                    display_info.display_buffer[i] = display_info.color[display_info.ActiveWindow];
                }
                break;
            }
            case WINDOWS_3:
            {
                for( uint16_t i = 0; i < LCD_BUFFER_SIZE; i++ )
                {
                    display_info.display_buffer[i] = display_info.color[display_info.ActiveWindow];
                }
                break;
            }
            case WINDOWS_4:
            {
                for( uint16_t i = 0; i < LCD_BUFFER_SIZE; i++ )
                {
                    display_info.display_buffer[i] = display_info.color[display_info.ActiveWindow];
                }
                break;
            }
            default:break;
        }
        
        display_open_window(display_info.coordinate[display_info.ActiveWindow][AXIS_X], display_info.coordinate[display_info.ActiveWindow][AXIS_Y], LCD_X_LENGTH, LCD_Y_LENGTH);
        display_write_cmd(CMD_SetPixel);
        fsmc_dma_send(LCD_BUFFER_SIZE);

        display_info.ActiveWindow = WINDOWS_1;
    }
}
