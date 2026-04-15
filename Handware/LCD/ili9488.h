#ifndef ILI9488_H
#define ILI9488_H

/*头文件包含区*/
#include "gd32f30x.h"

/*============================================================================
 * LCD 背光控制引脚配置
 *============================================================================*/
#define LCD_BK_GPIO_RCUPORT    RCU_GPIOA   /* 背光引脚所在端口的时钟 */
#define LCD_BK_GPIO_PORT       GPIOA       /* 背光引脚端口 */
#define LCD_BK_GPIO_PIN        GPIO_PIN_1  /* 背光引脚号 */
#define LCD_BK_GPIO_PINNUM     1           /* 背光引脚号（数值） */

/*============================================================================
 * EXMC 总线地址映射（用于 8080 并行接口 LCD）
 *============================================================================*/
#define BANK0_LCD_C    REG16(((uint32_t)0x60000000))   /* 命令寄存器地址 (RS=0) */
#define BANK0_LCD_D    REG16(((uint32_t)0x61000000))   /* 数据寄存器地址 (RS=1) */

/*============================================================================
 * 显示方向选择
 *============================================================================*/
#define LCD_DISPLAY_DIR_VER    0   /* 垂直方向（竖屏） */
#define LCD_DISPLAY_DIR_HOR    1   /* 水平方向（横屏） */

/*============================================================================
 * 屏幕物理分辨率（单位：像素）
 *============================================================================*/
#define LCD_WIDTH              320 /* 宽度（横屏时为 X 轴像素数） */
#define LCD_LENGTH             480 /* 长度（横屏时为 Y 轴像素数） */

/*============================================================================
 * 常用颜色（RGB565 格式，16位）
 *============================================================================*/
#define WHITE       0xFFFF   /* 白色     R=31 G=63 B=31 */
#define BLACK       0x0000   /* 黑色     R=0  G=0  B=0  */
#define RED         0xF800   /* 红色     R=31 G=0  B=0  */
#define GREEN       0x07E0   /* 绿色     R=0  G=63 B=0  */
#define BLUE        0x001F   /* 蓝色     R=0  G=0  B=31 */
#define MAGENTA     0xF81F   /* 品红色   R=31 G=0  B=31 (红+蓝) */
#define YELLOW      0xFFE0   /* 黄色     R=31 G=63 B=0  (红+绿) */
#define CYAN        0x07FF   /* 青色     R=0  G=63 B=31 (绿+蓝) */

/*============================================================================
 * 非常用颜色（RGB565 格式，16位）
 *============================================================================*/
#define BROWN       0xBC40   /* 棕色     R=23 G=41 B=8   (橙/棕调) */
#define BRRED       0xFC07   /* 棕红色   R=31 G=24 B=7   (红+少量绿) */
#define GRAY        0x8430   /* 灰色     R=16 G=33 B=16 (中等灰度) */
#define DARKBLUE    0x01CF   /* 深蓝色   R=0  G=7  B=31 (偏深蓝) */
#define LIGHTBLUE   0x7D7C   /* 浅蓝色   R=15 G=58 B=28 (亮蓝绿调) */
#define GRAYBLUE    0x5458   /* 灰蓝色   R=10 G=34 B=22 (蓝灰) */
#define LIGHTGREEN  0x841F   /* 浅绿色   R=16 G=33 B=31 (偏青的浅绿) */
#define LGRAY       0xC618   /* 浅灰色   R=24 G=49 B=24 (窗口背景常用) */
#define LGRAYBLUE   0xA651   /* 浅灰蓝色 R=20 G=38 B=17 (中间层颜色) */
#define LBBLUE      0x2B12   /* 浅棕蓝色 R=5  G=17 B=18 (选择条反色) */

/*============================================================================
 * 外部变量声明区
 *============================================================================*/

/**
 * @brief 显示模式枚举
 * @note  用于选择 LCD 或 SRAM 作为显示输出目标
 */
typedef enum {
    LCD_MODE  = 0,   /*!< LCD 模式，直接驱动显示屏 */
    SRAM_MODE = 1    /*!< SRAM 模式，使用显示缓冲区 */
} DISPLAY_MODE;

/**
 * @brief ASCII 字体大小枚举
 * @note  值为对应字体的像素宽度（等宽字体，宽=高）
 */
typedef enum {
    FONT_ASCII_12_6  = 12,   /*!< 6x12 点阵字体 */
    FONT_ASCII_16_8  = 16,   /*!< 8x16 点阵字体 */
    FONT_ASCII_24_12 = 24,   /*!< 12x24 点阵字体 */
    FONT_ASCII_32_16 = 32    /*!< 16x32 点阵字体 */
} FONT_ASCII;

/**
 * @brief LCD 参数结构体
 * @note  集中管理 LCD 的基本属性和当前状态
 */
typedef struct __typde_lcd_parameter_struct
{
    uint16_t width;                 /*!< LCD 宽度（像素） */
    uint16_t height;                /*!< LCD 高度（像素） */
    uint16_t id;                    /*!< LCD 控制器 ID */
    uint8_t  dir;                   /*!< 显示方向：0=竖屏，1=横屏 */

    uint16_t wramcmd;               /*!< 开始写 GRAM 的命令码 */
    uint16_t setxcmd;               /*!< 设置 X 坐标的命令码 */
    uint16_t setycmd;               /*!< 设置 Y 坐标的命令码 */

    __IO uint32_t g_point_color;    /*!< 当前画笔颜色（volatile） */
    __IO uint32_t g_back_color;     /*!< 当前背景色（volatile） */

    DISPLAY_MODE display_mode;      /*!< 显示模式（枚举） */
    uint16_t* display_ram;          /*!< 指向显示缓冲区的指针 */

} typde_lcd_parameter_struct;

/*============================================================================
 * LCD 打印参数结构体
 *============================================================================*/

/**
 * @brief LCD 打印参数结构体
 * @note  用于管理 LCD 上 printf 类函数的显示区域、当前位置及字体大小
 */
typedef struct __typde_lcd_printf_parameter_struct
{
    uint16_t x_start;               /*!< 打印区域起始 X 坐标 */
    uint16_t y_start;               /*!< 打印区域起始 Y 坐标 */
    uint16_t x_end;                 /*!< 打印区域结束 X 坐标 */
    uint16_t y_end;                 /*!< 打印区域结束 Y 坐标 */
    uint16_t x_now;                 /*!< 当前打印光标 X 坐标 */
    uint16_t y_now;                 /*!< 当前打印光标 Y 坐标 */

    FONT_ASCII size;                /*!< 字体大小（枚举类型，如 12/16/24/32） */

    /* LCD 的画笔颜色和背景色 */
    __IO uint32_t point_color;      /*!< 当前画笔颜色（volatile） */
    __IO uint32_t back_color;       /*!< 当前背景色（volatile） */

} typde_lcd_printf_parameter_struct;

/*============================================================================
 * 外部变量声明区
 *============================================================================*/

/**
 * @brief LCD 全局参数实例
 * @note  管理 LCD 的基本属性（宽、高、ID、方向、命令码、颜色等）
 */
extern typde_lcd_parameter_struct LCD_Parameter;

/**
 * @brief LCD 打印全局参数实例
 * @note  管理 printf 类函数的显示区域、光标位置及字体
 */
extern typde_lcd_printf_parameter_struct LCD_PritnfParameter;

/*============================================================================
 * 函数声明区
 *============================================================================*/

uint8_t LCD_init(void);                                         /* LCD 初始化 */
void LCD_ILI9488_Reginit(void);                                 /* ILI9488 寄存器初始化（内部调用） */
void LCD_DisplayOn(void);                                       /* 开启 LCD 显示 */
void LCD_DisplayDirSet(uint8_t dir);                            /* 设置显示方向（0=竖屏,1=横屏） */
void LCD_Clear(uint16_t color);                                 /* 清屏（指定颜色） */
void LCD_SetCursor(uint16_t x, uint16_t y);                     /* 设置光标位置（写 GRAM 起始点） */
void LCD_SetWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height); /* 设置显示窗口 */
void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);      /* 填充实心圆 */
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color); /* 纯色填充矩形（32位色，兼容 LTDC） */
void LCD_ColorFill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color); /* 彩色填充矩形（颜色数组） */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color);     /* 画点（32位色，兼容 LTDC） */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color); /* 画空心圆 */
void LCD_DrawHline(uint16_t x, uint16_t y, uint16_t len, uint16_t color); /* 画水平线 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color); /* 画直线（Bresenham） */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color); /* 画空心矩形 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, FONT_ASCII size, uint8_t mode, uint16_t color, uint16_t back_color); /* 显示单个 ASCII 字符 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, FONT_ASCII size, uint16_t color); /* 显示十进制数字 */
void LCD_ShowXNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, FONT_ASCII size, uint8_t mode, uint16_t color); /* 显示十六进制数字 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, FONT_ASCII size, char *p, uint16_t color); /* 显示字符串 */
void LCD_PrintfInit(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, FONT_ASCII char_size, uint16_t back_color, uint16_t point_color); /* 初始化 LCD 打印功能 */
void LCD_Printf(const char * sFormat, ...);                     /* 格式化打印（类似 printf） */
void LCD_ShowLog(void);                                         /* 显示日志信息（调试用） */
void Gui_Drawbmp16(uint16_t x, uint16_t y, const unsigned char *p); /* 显示 40x40 BMP 图片 */

#endif
