#ifndef __APP_HMI_H
#define __APP_HMI_H

#include "stm32f10x.h"
#include "bsp_fsmc.h"

#define     FSMC_Addr_ILI9341_CMD       ( ( uint32_t ) 0x60000000 ) // FSMC_A16 = 1
#define     FSMC_Addr_ILI9341_DATA      ( ( uint32_t ) 0x60020000 ) // FSMC_A16 = 0

#define     DEBUG_DELAY()

#define     BACKGROUND                  BLACK   //默认背景颜色

#define     WHITE                       0xFFFF  // 白色
#define     BLACK                       0x0000  // 黑色
#define     RED                         0xF800  // 红色
#define     GREEN                       0x07E0  // 绿色
#define     BLUE                        0x001F  // 蓝色
#define     GREY                        0xF7DE  // 灰色
#define     BABY_BLUE                   0x051F  // 浅蓝色
#define     MAGENTA                     0xF81F  // 红紫色，洋红色
#define     CYAN                        0x7FFF  // 蓝绿色，青色
#define     YELLOW                      0xFFE0  // 黄色
#define     BRED                        0xF81F
#define     GRED                        0xFFE0
#define     GBLUE                       0x07FF

#define     CMD_SetCoordinateX          0x2A    // 设置X坐标
#define     CMD_SetCoordinateY          0x2B    // 设置Y坐标
#define     CMD_SetPixel                0x2C    // 填充像素

// lcd size
#define     LCD_X_LENGTH                (3 * 40) // 240
#define     LCD_Y_LENGTH                (4 * 40) // 320
#define     LCD_BUFFER_SIZE             (LCD_X_LENGTH * LCD_Y_LENGTH)

#define     WINDOWS_1                   0
#define     WINDOWS_2                   1
#define     WINDOWS_3                   2
#define     WINDOWS_4                   3

#define     AXIS_X                      0
#define     AXIS_Y                      1

typedef struct
{
    volatile uint8_t event;
    struct
    {
        uint8_t key_up      : 1;
        uint8_t key_down    : 1;
        uint8_t key_left    : 1;
        uint8_t key_right   : 1;
        uint8_t key_enter   : 1;
        uint8_t key_esc     : 1;
        uint8_t rcv         : 2;
    }bits;
}event_t;

struct page_info
{
    uint8_t page_index;             // 当前页索引
    uint8_t max_index;              // 当前页索引条目
    uint8_t *str[8];                // 页面条目
    uint8_t *head;                  // 页面头

    uint16_t font_color;            // 字体颜色
    
    struct page_info * pNextPage;   // 下一页
    struct page_info * pLastPage;   // 上一页
};
typedef struct page_info page_info_t;

typedef struct
{
    uint8_t ActiveWindow;           // 当前活跃窗口
    uint8_t coordinate[4][2];       // 窗口起始坐标
    uint16_t color[4];              // 窗口背景色
    page_info_t * pCurrentPage;     // 当前处于哪个页面，main? sys_info? show-pic?
    event_t event;                  // 按键事件
    uint16_t display_buffer[LCD_BUFFER_SIZE];
}display_info_t;


extern uint8_t frame_end_flag;
extern uint16_t display_buffer[LCD_BUFFER_SIZE];
extern display_info_t display_info;

uint16_t display_read_data(void);
void display_write_data(uint16_t Data);
void display_write_cmd(uint16_t Cmd);

void display_init(void);

void display_clear(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color);
void display_show_char(uint16_t X, uint16_t Y, const char Char, uint16_t Color);
void display_show_str(uint16_t Line, char *pStr, uint8_t len);
void display_refresh(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color);
void display_show_index(uint16_t Line);
void display_loop(void);
void display_init_windows(void);

#endif
