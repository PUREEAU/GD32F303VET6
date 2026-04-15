/*头文件包含区*/
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "systick.h"
#include "ili9488.h"
#include "ili9488_font.h"
#include "exmc_lcd.h"
#include "picture.h"

/*============================================================================
 * 宏定义区
 *============================================================================*/

/*============================================================================
 * 外部变量声明区
 *============================================================================*/
 
typde_lcd_parameter_struct LCD_Parameter;
typde_lcd_printf_parameter_struct LCD_PritnfParameter;

/*============================================================================
 * 函数声明区
 *============================================================================*/

static uint16_t LCD_ReadData(void);                      /* 读取 LCD 数据（内部使用） */

/*============================================================================
 * 函数实体区
 *============================================================================*/

/**
 * @brief LCD 写数据（16位）
 * @param data 要写入的数据
 */
void LCD_WriteReg_Data(__IO uint16_t data)
{
    BANK0_LCD_D = data;   /* 通过 EXMC 直接写入 LCD 数据寄存器 */
}

/**
 * @brief LCD 写寄存器编号/地址（命令）
 * @param regno 寄存器编号/地址
 */
void LCD_WriteReg_Address(__IO uint16_t regno)
{
    BANK0_LCD_C = regno;   /* 写入寄存器地址（命令模式） */
}

/**
 * @brief LCD 写寄存器（先写寄存器地址，再写数据）
 * @param regno 寄存器编号/地址（命令）
 * @param data  要写入的数据
 */
void LCD_WriteReg(__IO uint16_t regno, __IO uint16_t data)
{
    BANK0_LCD_C = regno;   /* 写入寄存器地址（命令模式） */
    BANK0_LCD_D = data;    /* 写入数据（数据模式） */
}

/**
 * @brief LCD 读取数据（从数据寄存器读取）
 * @return 读取到的数据（16位）
 */
static uint16_t LCD_ReadData(void)
{
    return BANK0_LCD_D;    /* 从数据寄存器读取 */
}


/**
 * @brief       ILI9488 寄存器初始化序列
 * @note        该函数配置 ILI9488 驱动 IC 的基础参数，包括电源、帧率、伽马、接口模式等
 * @param       无
 * @retval      无
 */
void LCD_ILI9488_Reginit(void)
{
    /* ******************************************
     * 1. 调整控制 (Adjust Control)
     * ******************************************/
    LCD_WriteReg_Address(0xF7);
    LCD_WriteReg_Data(0xA9);
    LCD_WriteReg_Data(0x51);
    LCD_WriteReg_Data(0x2C);
    LCD_WriteReg_Data(0x82);      /* 内部调整参数 */

    /* ******************************************
     * 2. 电源与泵控制 (Power & Pump Control)
     * ******************************************/
    LCD_WriteReg_Address(0xEC);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x02);
    LCD_WriteReg_Data(0x03);
    LCD_WriteReg_Data(0x7A);      /* 电源相关配置 */

    /* ******************************************
     * 3. 振荡器/时钟控制 (Oscillator/Clock Control)
     * ******************************************/
    LCD_WriteReg_Address(0xC0);
    LCD_WriteReg_Data(0x13);
    LCD_WriteReg_Data(0x13);      /* 振荡器频率调整 */

    LCD_WriteReg_Address(0xC1);
    LCD_WriteReg_Data(0x41);      /* 电源控制 */

    /* ******************************************
     * 4. VCOM 控制 (VCOM Control)
     * ******************************************/
    LCD_WriteReg_Address(0xC5);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x28);
    LCD_WriteReg_Data(0x80);      /* VCOM 电压设定 */

    /* ******************************************
     * 5. 帧率控制 (Frame Rate Control)
     * ******************************************/
    LCD_WriteReg_Address(0xB1);
    LCD_WriteReg_Data(0xB0);
    LCD_WriteReg_Data(0x11);      /* 设置帧率约 70Hz */

    /* ******************************************
     * 6. 反转控制 (Inversion Control)
     * ******************************************/
    LCD_WriteReg_Address(0xB4);
    LCD_WriteReg_Data(0x02);      /* 列反转 */

    /* ******************************************
     * 7. 接口控制 (RGB/MCU Interface Control)
     * ******************************************/
    LCD_WriteReg_Address(0xB6);
    LCD_WriteReg_Data(0x02);      /* MCU 接口模式 */
    LCD_WriteReg_Data(0x22);

    LCD_WriteReg_Address(0xB7);
    LCD_WriteReg_Data(0xC6);      /* 源极驱动控制 */

    LCD_WriteReg_Address(0xBE);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x04);      /* 内部设置 */

    /* ******************************************
     * 8. 其他控制
     * ******************************************/
    LCD_WriteReg_Address(0xE9);
    LCD_WriteReg_Data(0x00);      /* 保留控制 */

    LCD_WriteReg_Address(0xF4);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x0F);

    /* ******************************************
     * 9. 伽马校正 (Gamma Correction)
     * ******************************************/
    LCD_WriteReg_Address(0xE0);   /* 正伽马 (Positive Gamma) */
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x04);
    LCD_WriteReg_Data(0x0E);
    LCD_WriteReg_Data(0x08);
    LCD_WriteReg_Data(0x17);
    LCD_WriteReg_Data(0x0A);
    LCD_WriteReg_Data(0x40);
    LCD_WriteReg_Data(0x79);
    LCD_WriteReg_Data(0x4D);
    LCD_WriteReg_Data(0x07);
    LCD_WriteReg_Data(0x0E);
    LCD_WriteReg_Data(0x0A);
    LCD_WriteReg_Data(0x1A);
    LCD_WriteReg_Data(0x1D);
    LCD_WriteReg_Data(0x0F);

    LCD_WriteReg_Address(0xE1);   /* 负伽马 (Negative Gamma) */
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x1B);
    LCD_WriteReg_Data(0x1F);
    LCD_WriteReg_Data(0x02);
    LCD_WriteReg_Data(0x10);
    LCD_WriteReg_Data(0x05);
    LCD_WriteReg_Data(0x32);
    LCD_WriteReg_Data(0x34);
    LCD_WriteReg_Data(0x43);
    LCD_WriteReg_Data(0x02);
    LCD_WriteReg_Data(0x0A);
    LCD_WriteReg_Data(0x09);
    LCD_WriteReg_Data(0x33);
    LCD_WriteReg_Data(0x37);
    LCD_WriteReg_Data(0x0F);

    LCD_WriteReg_Address(0xF4);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x00);
    LCD_WriteReg_Data(0x0F);

    /* ******************************************
     * 10. 内存访问控制 (Memory Access Control)
     * ******************************************/
    LCD_WriteReg_Address(0x36);
    LCD_WriteReg_Data(0x08);      /* RGB/BGR, 扫描方向等 */

    /* ******************************************
     * 11. 像素接口格式 (Pixel Interface Format)
     * ******************************************/
    LCD_WriteReg_Address(0x3A);
    LCD_WriteReg_Data(0x55);      /* 16 位 RGB565 (若 0x66 为 18 位) */

    /* ******************************************
     * 12. 睡眠唤醒并开启显示
     * ******************************************/
    LCD_WriteReg_Address(0x20);   /* 退出睡眠 */
    LCD_WriteReg_Address(0x11);   /* 退出睡眠模式 (Sleep Out) */
    delay_1ms(120);
    LCD_WriteReg_Address(0x29);   /* 开启显示 (Display On) */
    delay_1ms(50);
}

/**
 * @brief       设置 LCD 显示方向（竖屏/横屏）
 * @param       dir 方向参数，可选值：
 *               - LCD_DISPLAY_DIR_VER : 竖屏
 *               - 其他值（如 LCD_DISPLAY_DIR_HOR）: 横屏
 * @retval      无
 * @note        该函数会根据当前 LCD 控制器 ID（如 ILI9488）自动调整宽度、高度、
 *              命令码以及显示窗口，并配置寄存器 0x36 实现 270 度旋转。
 */
void LCD_DisplayDirSet(uint8_t dir)   /* LCD 方向设置 */
{
    uint16_t regval = 0;

    LCD_Parameter.dir = dir;   /* 保存方向标志（竖屏/横屏） */

    /* 根据方向设置宽高及命令码 */
    if (dir == LCD_DISPLAY_DIR_VER)   /* 竖屏模式 */
    {
        LCD_Parameter.width  = 320;
        LCD_Parameter.height = 480;

        if (LCD_Parameter.id == 0X9488)
        {
            LCD_Parameter.wramcmd = 0X2C;
            LCD_Parameter.setxcmd = 0X2A;
            LCD_Parameter.setycmd = 0X2B;
        }
        /* 若为 ILI9488，重新确认分辨率为 320×480 */
        if (LCD_Parameter.id == 0X9488)
        {
            LCD_Parameter.width  = LCD_WIDTH;    /* 宏定义宽度，通常为 320 */
            LCD_Parameter.height = LCD_LENGTH;   /* 宏定义高度，通常为 480 */
        }
    }
    else   /* 横屏模式 */
    {
        LCD_Parameter.width  = 480;   /* 默认宽度 */
        LCD_Parameter.height = 320;   /* 默认高度 */

        if (LCD_Parameter.id == 0X9488)
        {
            LCD_Parameter.wramcmd = 0X2C;
            LCD_Parameter.setxcmd = 0X2A;
            LCD_Parameter.setycmd = 0X2B;
        }
        if (LCD_Parameter.id == 0X9488)
        {
            LCD_Parameter.width  = LCD_LENGTH;   /* 交换宽高为 480 */
            LCD_Parameter.height = LCD_WIDTH;    /* 交换宽高为 320 */
        }
    }

    /* ========== 配置 0x36 寄存器，控制扫描方向 ========== */
    /* 常用方向配置（基于 ILI9488）：
        位 (Bit)	名称	功能说明 (0/1 效果)
        7	MY	行扫描顺序：0 = 从右到左；1 = 从左到右
        6	MX	列扫描顺序：0 = 从上到下；1 = 从下到上
        5	MV	行列交换：0 = 不交换；1 = 交换 (是横竖屏切换的关键)
        4	ML	垂直扫描顺序，通常设为0
        3	RGB/BGR	颜色顺序：0 = RGB；1 = BGR
        2	MH	水平刷新顺序，通常设为0
        1-0	-	保留位，通常设为0
    11001000
     */
    if (dir == LCD_DISPLAY_DIR_VER)   /* 竖屏 */
    {
        regval = 0xC8;   /* BGR=1, MX=0, MY=0, MV=0, 根据实际颜色顺序可改为 0x08 */
    }
    else                              /* 横屏 */
    {
        regval= 0xA8;
    }

    /* ILI9488 需要额外设置 bit3 为 1（BGR 像素顺序） */
    if (LCD_Parameter.id == 0X9488)
    {
        regval |= (1 << 3);
    }

    LCD_WriteReg(0x36, regval);

    /* 根据实际宽高重新设置显示窗口（开窗） */
    if (LCD_Parameter.id == 0X9488)
    {
        /* 设置 X 坐标范围 */
        LCD_WriteReg_Address(LCD_Parameter.setxcmd);
        LCD_WriteReg_Data(0);
        LCD_WriteReg_Data(0);
        LCD_WriteReg_Data((LCD_Parameter.width  - 1) >> 8);
        LCD_WriteReg_Data((LCD_Parameter.width  - 1) & 0xFF);

        /* 设置 Y 坐标范围 */
        LCD_WriteReg_Address(LCD_Parameter.setycmd);
        LCD_WriteReg_Data(0);
        LCD_WriteReg_Data(0);
        LCD_WriteReg_Data((LCD_Parameter.height - 1) >> 8);
        LCD_WriteReg_Data((LCD_Parameter.height - 1) & 0xFF);
    }
}

/**
 * @brief       背光控制引脚初始化
 * @param       无
 * @retval      无
 * @note        将背光控制引脚配置为推挽输出，最高频率 50MHz
 */
void LCD_BK_Init(void)
{
    rcu_periph_clock_enable(LCD_BK_GPIO_RCUPORT);                     /* 使能背光引脚所在 GPIO 端口时钟 */
    gpio_init(LCD_BK_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LCD_BK_GPIO_PIN); /* 配置为推挽输出，50MHz */
}

/**
 * @brief       开启 LCD 背光
 * @param       无
 * @retval      无
 */
void LCD_BK_TurnOn(void)
{
    gpio_bit_set(LCD_BK_GPIO_PORT, LCD_BK_GPIO_PIN);                  /* 输出高电平，点亮背光 */
}

/**
 * @brief       关闭 LCD 背光
 * @param       无
 * @retval      无
 */
void LCD_BK_TurnOff(void)
{
    gpio_bit_reset(LCD_BK_GPIO_PORT, LCD_BK_GPIO_PIN);                /* 输出低电平，关闭背光 */
}

/**
 * @brief       设置显示窗口（固定为从 (x, y) 到屏幕右下角）
 * @note        该函数将窗口起始点设为 (x, y)，结束点固定为屏幕宽度和高度。
 *              因此只有当 x=0, y=0 时窗口才为全屏；若 x,y 非零，窗口大小将异常。
 *              如需自定义窗口，请使用 LCD_SetWindow() 函数。
 * @param       x 起始 X 坐标（通常应设为 0）
 * @param       y 起始 Y 坐标（通常应设为 0）
 * @retval      无
 */
void LCD_SetCursor(uint16_t x, uint16_t y)
{
    LCD_WriteReg_Address(LCD_Parameter.setxcmd);
    LCD_WriteReg_Data(x >> 8);
    LCD_WriteReg_Data(x & 0xFF);
    LCD_WriteReg_Data(LCD_Parameter.width >> 8);
    LCD_WriteReg_Data(LCD_Parameter.width & 0xFF);

    LCD_WriteReg_Address(LCD_Parameter.setycmd);
    LCD_WriteReg_Data(y >> 8);
    LCD_WriteReg_Data(y & 0xFF);
    LCD_WriteReg_Data(LCD_Parameter.height >> 8);
    LCD_WriteReg_Data(LCD_Parameter.height & 0xFF);
}

/**
 * @brief       清屏函数，用指定颜色填充整个屏幕
 * @param       color 填充颜色（RGB565 格式）
 * @retval      无
 */
void LCD_Clear(uint16_t color)
{
    uint32_t index;
    uint32_t totalpoint = (uint32_t)LCD_Parameter.width * LCD_Parameter.height;

    LCD_Parameter.g_back_color = color;          /* 更新背景色 */
    LCD_Parameter.g_point_color = ~color;        /* 画笔颜色取反（可选） */

    LCD_SetWindow(0, 0, LCD_Parameter.width, LCD_Parameter.height); /* 设置全屏窗口 */
    BANK0_LCD_C = LCD_Parameter.wramcmd;         /* 发送“开始写 GRAM”命令 */

    for (index = 0; index < totalpoint; index++)
    {
        BANK0_LCD_D = color;                     /* 连续写入颜色数据 */
    }
}

/**
 * @brief       设置显示窗口（矩形区域），后续写入 GRAM 的数据将自动限制在此窗口内
 * @param       sx, sy: 窗口左上角坐标（像素）
 * @param       width, height: 窗口宽度和高度（必须大于 0）
 * @retval      无
 * @note        窗口大小 = width * height。该函数会根据当前 LCD 驱动 IC 自动适配命令格式。
 *              对于 ILI9341/ILI9488/ST7789 等常见 IC，使用标准 0x2A/0x2B 命令。
 *              对于特殊 IC（如 0x1963 竖屏模式），做坐标镜像处理。
 */
void LCD_SetWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t ex = sx + width - 1;   // 结束 X 坐标
    uint16_t ey = sy + height - 1;  // 结束 Y 坐标

    // 常见驱动 IC 列表（使用标准窗口设置命令）
    if (LCD_Parameter.id == 0x9488   ||   // ILI9488
        LCD_Parameter.id == 0x9341   ||   // ILI9341
        LCD_Parameter.id == 0x7789   ||   // ST7789
        LCD_Parameter.id == 0x5310   ||   // 某些 5310 兼容
        LCD_Parameter.id == 0x9948)
    {
        LCD_WriteReg_Address(LCD_Parameter.setxcmd);
        LCD_WriteReg_Data(sx >> 8);
        LCD_WriteReg_Data(sx & 0xFF);
        LCD_WriteReg_Data(ex >> 8);
        LCD_WriteReg_Data(ex & 0xFF);

        LCD_WriteReg_Address(LCD_Parameter.setycmd);
        LCD_WriteReg_Data(sy >> 8);
        LCD_WriteReg_Data(sy & 0xFF);
        LCD_WriteReg_Data(ey >> 8);
        LCD_WriteReg_Data(ey & 0xFF);
    }
    // 特殊驱动 IC：0x1963 在竖屏模式下的坐标镜像处理
    else if (LCD_Parameter.id == 0x1963 && LCD_Parameter.dir != 1)   // 竖屏模式
    {
        uint16_t mirror_sx = LCD_Parameter.width - width - sx;
        uint16_t mirror_ex = mirror_sx + width - 1;
        uint16_t end_y = sy + height - 1;

        LCD_WriteReg_Address(LCD_Parameter.setxcmd);
        LCD_WriteReg_Data(mirror_sx >> 8);
        LCD_WriteReg_Data(mirror_sx & 0xFF);
        LCD_WriteReg_Data(mirror_ex >> 8);
        LCD_WriteReg_Data(mirror_ex & 0xFF);

        LCD_WriteReg_Address(LCD_Parameter.setycmd);
        LCD_WriteReg_Data(sy >> 8);
        LCD_WriteReg_Data(sy & 0xFF);
        LCD_WriteReg_Data(end_y >> 8);
        LCD_WriteReg_Data(end_y & 0xFF);
    }
}
/**
 * @brief       在指定矩形区域内填充单一颜色（硬件加速）
 * @param       sx, sy: 矩形左上角坐标
 * @param       ex, ey: 矩形右下角坐标（包含）
 * @param       color: 填充颜色（32位，低16位有效，兼容LTDC）
 * @retval      无
 * @note        根据 display_mode 决定写入 LCD 或仅更新显存。
 */
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t width = ex - sx + 1;
    uint16_t height = ey - sy + 1;
    uint32_t total = (uint32_t)width * height;
    uint16_t color16 = (uint16_t)color;   /* 只取低16位 RGB565 */

    if (LCD_Parameter.display_mode == LCD_MODE)
    {
        LCD_SetWindow(sx, sy, width, height);          /* 设置矩形窗口 */
        BANK0_LCD_C = LCD_Parameter.wramcmd;           /* 开始写 GRAM */
        for (uint32_t i = 0; i < total; i++)
        {
            BANK0_LCD_D = color16;                     /* 填充颜色 */
        }
    }
    else   /* SRAM_MODE：仅更新显存 */
    {
        uint16_t *ram = LCD_Parameter.display_ram;
        uint16_t scr_width = LCD_Parameter.width;      /* 屏幕宽度，用于行偏移 */
        for (uint16_t y = sy; y <= ey; y++)
        {
            uint32_t row_offset = (uint32_t)y * scr_width;
            for (uint16_t x = sx; x <= ex; x++)
            {
                ram[row_offset + x] = color16;
            }
        }
    }
}

/**
 * @brief       画一个点（16位颜色）
 * @param       x, y: 坐标
 * @param       color: 颜色（32位，低16位有效）
 * @retval      无
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{
    uint16_t color16 = (uint16_t)color;

    if (LCD_Parameter.display_mode == LCD_MODE)
    {
        /* 使用单点窗口，替代可能有问题的 LCD_SetCursor */
        LCD_SetWindow(x, y, 1, 1);
        BANK0_LCD_C = LCD_Parameter.wramcmd;
        BANK0_LCD_D = color16;
    }
    else
    {
        uint16_t *ram = LCD_Parameter.display_ram;
        ram[(uint32_t)y * LCD_Parameter.width + x] = color16;
    }
}

/**
 * @brief       在指定矩形区域内填充颜色数组（图片显示）
 * @param       sx, sy: 矩形左上角坐标
 * @param       ex, ey: 矩形右下角坐标（包含）
 * @param       color: 颜色数组首地址（RGB565，每个元素16位）
 * @retval      无
 * @note        数组大小必须等于矩形面积 (ex-sx+1)*(ey-sy+1)。
 */
void LCD_ColorFill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t width = ex - sx + 1;
    uint16_t height = ey - sy + 1;
    uint32_t total = (uint32_t)width * height;

    if (LCD_Parameter.display_mode == LCD_MODE)
    {
        LCD_SetWindow(sx, sy, width, height);
        BANK0_LCD_C = LCD_Parameter.wramcmd;
        for (uint32_t i = 0; i < total; i++)
        {
            BANK0_LCD_D = color[i];
        }
    }
    else   /* SRAM_MODE：更新显存 */
    {
        uint16_t *ram = LCD_Parameter.display_ram;
        uint16_t scr_width = LCD_Parameter.width;
        for (uint16_t y = sy; y <= ey; y++)
        {
            uint32_t row_offset = (uint32_t)y * scr_width;
            uint32_t src_offset = (uint32_t)(y - sy) * width;   /* 源数组行偏移 */
            for (uint16_t x = sx; x <= ex; x++)
            {
                ram[row_offset + x] = color[src_offset + (x - sx)];
            }
        }
    }
}

/**
 * @brief       将显存（SRAM模式）的内容刷新到 LCD 屏幕
 * @retval      无
 * @note        仅在 display_mode == SRAM_MODE 时有效，将 LCD_Parameter.display_ram 全屏写入 LCD。
 */
void LCD_Sram_Refresh(void)
{
    if (LCD_Parameter.display_mode != SRAM_MODE)
        return;

    uint16_t width = LCD_Parameter.width;
    uint16_t height = LCD_Parameter.height;
    uint32_t total = (uint32_t)width * height;
    uint16_t *ram = LCD_Parameter.display_ram;

    /* 设置全屏窗口 */
    LCD_SetWindow(0, 0, width, height);
    BANK0_LCD_C = LCD_Parameter.wramcmd;   /* 开始写 GRAM */

    /* 批量写入显存数据 */
    for (uint32_t i = 0; i < total; i++)
    {
        BANK0_LCD_D = ram[i];
    }
}

/*----------------------------------------------------------------------------
 * 图形绘制函数
 *----------------------------------------------------------------------------*/

/**
 * @brief       画线（Bresenham算法）
 * @param       x1, y1: 起点坐标
 * @param       x2, y2: 终点坐标
 * @param       color: 线条颜色（RGB565）
 * @retval      无
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int8_t incx = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
    int8_t incy = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
    uint16_t distance;
    int16_t xerr = 0, yerr = 0;
    int16_t row = x1, col = y1;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    distance = (dx > dy) ? dx : dy;

    for (uint16_t t = 0; t <= distance; t++)
    {
        LCD_DrawPoint(row, col, color);
        xerr += dx;
        yerr += dy;
        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief       画水平线
 * @param       x, y: 起点坐标
 * @param       len: 长度（像素）
 * @param       color: 颜色
 * @retval      无
 */
void LCD_DrawHline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if (len == 0 || x >= LCD_Parameter.width || y >= LCD_Parameter.height)
        return;
    LCD_Fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       画矩形边框
 * @param       x1, y1: 左上角坐标
 * @param       x2, y2: 右下角坐标
 * @param       color: 边框颜色
 * @retval      无
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
 * @brief       画空心圆（Bresenham算法）
 * @param       x0, y0: 圆心坐标
 * @param       r: 半径
 * @param       color: 颜色
 * @retval      无
 */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a = 0, b = r;
    int di = 3 - (r << 1);  // 决策变量

    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b, color);
        LCD_DrawPoint(x0 + b, y0 - a, color);
        LCD_DrawPoint(x0 + b, y0 + a, color);
        LCD_DrawPoint(x0 + a, y0 + b, color);
        LCD_DrawPoint(x0 - a, y0 + b, color);
        LCD_DrawPoint(x0 - b, y0 + a, color);
        LCD_DrawPoint(x0 - a, y0 - b, color);
        LCD_DrawPoint(x0 - b, y0 - a, color);
        a++;
        if (di < 0)
            di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

/**
 * @brief       填充实心圆（水平线扫描法）
 * @param       x, y: 圆心坐标
 * @param       r: 半径
 * @param       color: 填充颜色
 * @retval      无
 */
void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;  // r/sqrt(2) 近似
    uint32_t sqmax = (uint32_t)r * r + (r >> 1);
    int32_t xr = r;

    LCD_DrawHline(x - r, y, 2 * r, color);  // 中间水平线

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            if (xr > (int32_t)imax)
            {
                LCD_DrawHline(x - i + 1, y + xr, 2 * (i - 1), color);
                LCD_DrawHline(x - i + 1, y - xr, 2 * (i - 1), color);
            }
            xr--;
        }
        LCD_DrawHline(x - xr, y + i, 2 * xr, color);
        LCD_DrawHline(x - xr, y - i, 2 * xr, color);
    }
}

/*----------------------------------------------------------------------------
 * 字符/字符串显示函数
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * 字符/字符串显示函数
 *----------------------------------------------------------------------------*/

/**
 * @brief       显示单个ASCII字符
 * @param       x, y: 左上角坐标
 * @param       chr: 要显示的字符（如 'A'）
 * @param       size: 字体大小（枚举FONT_ASCII）
 * @param       mode: 0-非叠加（背景色覆盖），1-叠加（只画前景，背景不变）
 * @param       color: 字体颜色
 * @param       back_color: 背景色（mode=0时使用）
 * @retval      无
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, FONT_ASCII size, uint8_t mode, uint16_t color, uint16_t back_color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);  // 点阵字节数
    uint8_t *pfont = NULL;
    uint8_t height = size;      // 字符高度
    uint8_t char_index;         // 用于数组下标的无符号值

    // 将char转换为无符号值，避免下标为负的警告
    char_index = (uint8_t)(chr - ' ');
    if (char_index > 95) return;   // 仅支持ASCII 32~127

    // 选择字体数据（假设外部定义了 ascii_12_6, ascii_16_8, ascii_24_12, ascii_32_16）
    switch (size)
    {
        case FONT_ASCII_12_6:  pfont = (uint8_t*)ascii_12_6[char_index]; break;
        case FONT_ASCII_16_8:  pfont = (uint8_t*)ascii_16_8[char_index]; break;
        case FONT_ASCII_24_12: pfont = (uint8_t*)ascii_24_12[char_index]; break;
        case FONT_ASCII_32_16: pfont = (uint8_t*)ascii_32_16[char_index]; break;
        default: return;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)    // 该位为1，画前景色
            {
                LCD_DrawPoint(x, y, color);
            }
            else if (mode == 0) // 非叠加模式，画背景色
            {
                LCD_DrawPoint(x, y, back_color);
            }
            // 叠加模式且为0：不画任何点，保留原内容

            temp <<= 1;
            y++;
            if (y - y0 == height)   // 一列绘制完成
            {
                y = y0;
                x++;
                break;              // 退出当前字节的位循环，继续下一个字节
            }
            if (y >= LCD_Parameter.height) return;
            if (x >= LCD_Parameter.width) return;
        }
    }
}

/**
 * @brief       计算m的n次方（内部函数）
 * @param       m: 底数
 * @param       n: 指数（0~9）
 * @return      结果
 */
static uint32_t LCD_Math_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

/**
 * @brief       显示十进制数字（高位不显示0，用空格占位）
 * @param       x, y: 起始坐标
 * @param       num: 数值
 * @param       len: 总显示位数
 * @param       size: 字体大小
 * @param       color: 字体颜色
 * @retval      无
 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, FONT_ASCII size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t char_width = size / 2;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Math_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < len - 1)
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + char_width * t, y, ' ', size, 0, color, LCD_Parameter.g_back_color);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        LCD_ShowChar(x + char_width * t, y, temp + '0', size, 0, color, LCD_Parameter.g_back_color);
    }
}

/**
 * @brief       扩展显示十进制数字（高位可补0或空格）
 * @param       x, y: 起始坐标
 * @param       num: 数值
 * @param       len: 总显示位数
 * @param       size: 字体大小
 * @param       mode: bit7=1 高位补0，bit0=1 叠加模式
 * @param       color: 字体颜色
 * @retval      无
 */
void LCD_ShowXNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, FONT_ASCII size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t char_width = size / 2;
    uint8_t fill_zero = (mode & 0x80) ? 1 : 0;
    uint8_t overlay = mode & 0x01;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Math_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < len - 1)
        {
            if (temp == 0)
            {
                if (fill_zero)
                    LCD_ShowChar(x + char_width * t, y, '0', size, overlay, color, LCD_Parameter.g_back_color);
                else
                    LCD_ShowChar(x + char_width * t, y, ' ', size, overlay, color, LCD_Parameter.g_back_color);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        LCD_ShowChar(x + char_width * t, y, temp + '0', size, overlay, color, LCD_Parameter.g_back_color);
    }
}

/**
 * @brief       显示字符串（自动换行）
 * @param       x, y: 起始坐标
 * @param       width, height: 显示区域大小（像素）
 * @param       size: 字体大小
 * @param       p: 字符串指针
 * @param       color: 字体颜色
 * @retval      无
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, FONT_ASCII size, char *p, uint16_t color)
{
    uint16_t x0 = x;
    uint16_t y0 = y;
    uint8_t char_width = size / 2;
    uint8_t char_height = size;

    while (*p && (y < y0 + height))
    {
        if (*p == '\r')
        {
            x = x0;
            p++;
            continue;
        }
        else if (*p == '\n')
        {
            y += char_height;
            x = x0;
            p++;
            continue;
        }
        if (x + char_width > x0 + width)
        {
            x = x0;
            y += char_height;
            if (y + char_height > y0 + height) break;
        }
        LCD_ShowChar(x, y, *p, size, 0, color, LCD_Parameter.g_back_color);
        x += char_width;
        p++;
    }
}

/*----------------------------------------------------------------------------
 * 简易printf打印功能
 *----------------------------------------------------------------------------*/

/**
 * @brief       初始化LCD打印窗口及参数
 * @param       x_start, y_start: 打印区域左上角
 * @param       x_end, y_end: 打印区域右下角（包含）
 * @param       char_size: 字体大小
 * @param       back_color: 背景色
 * @param       point_color: 字体颜色
 * @retval      无
 */
void LCD_PrintfInit(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,
                    FONT_ASCII char_size, uint16_t back_color, uint16_t point_color)
{
    LCD_PritnfParameter.x_start = x_start;
    LCD_PritnfParameter.y_start = y_start;
    LCD_PritnfParameter.x_end   = x_end;
    LCD_PritnfParameter.y_end   = y_end;
    LCD_PritnfParameter.x_now   = x_start;
    LCD_PritnfParameter.y_now   = y_start;
    LCD_PritnfParameter.size    = char_size;
    LCD_PritnfParameter.back_color = back_color;
    LCD_PritnfParameter.point_color = point_color;
    LCD_Fill(x_start, y_start, x_end, y_end, back_color);  // 清空打印区域
}

/**
 * @brief       在LCD上实现printf格式化打印（自动换行，支持区域滚动）
 * @param       sFormat: 格式化字符串
 * @param       ...: 可变参数
 * @retval      无
 * @note        需要包含 <stdio.h>、<string.h>、<stdarg.h>
 */
void LCD_Printf(const char *sFormat, ...)
{
    #define PRINTF_MAX_LEN 100
    char printf_buffer[PRINTF_MAX_LEN];
    char *p = printf_buffer;
    uint16_t len;
    va_list ParamList;

    va_start(ParamList, sFormat);
    vsprintf(printf_buffer, sFormat, ParamList);
    va_end(ParamList);

    len = strlen(printf_buffer);
    for (uint16_t count = 0; count < len; count++)
    {
        char c = *p++;
        if (c == '\r')
        {
            LCD_PritnfParameter.x_now = LCD_PritnfParameter.x_start;
        }
        else if (c == '\n')
        {
            LCD_PritnfParameter.y_now += LCD_PritnfParameter.size;
            LCD_PritnfParameter.x_now = LCD_PritnfParameter.x_start;
            // 如果超出底部，滚动（重置到顶部并清屏）
            if (LCD_PritnfParameter.y_now + LCD_PritnfParameter.size > LCD_PritnfParameter.y_end)
            {
                LCD_PritnfParameter.y_now = LCD_PritnfParameter.y_start;
                LCD_Fill(LCD_PritnfParameter.x_start, LCD_PritnfParameter.y_start,
                         LCD_PritnfParameter.x_end, LCD_PritnfParameter.y_end,
                         LCD_PritnfParameter.back_color);
            }
            else
            {
                // 填充新行背景
                LCD_Fill(LCD_PritnfParameter.x_start, LCD_PritnfParameter.y_now,
                         LCD_PritnfParameter.x_end, LCD_PritnfParameter.y_now + LCD_PritnfParameter.size - 1,
                         LCD_PritnfParameter.back_color);
            }
        }
        else
        {
            // 检查是否需要换行
            if (LCD_PritnfParameter.x_now + LCD_PritnfParameter.size/2 > LCD_PritnfParameter.x_end)
            {
                LCD_PritnfParameter.x_now = LCD_PritnfParameter.x_start;
                LCD_PritnfParameter.y_now += LCD_PritnfParameter.size;
                if (LCD_PritnfParameter.y_now + LCD_PritnfParameter.size > LCD_PritnfParameter.y_end)
                {
                    LCD_PritnfParameter.y_now = LCD_PritnfParameter.y_start;
                    LCD_Fill(LCD_PritnfParameter.x_start, LCD_PritnfParameter.y_start,
                             LCD_PritnfParameter.x_end, LCD_PritnfParameter.y_end,
                             LCD_PritnfParameter.back_color);
                }
                else
                {
                    LCD_Fill(LCD_PritnfParameter.x_start, LCD_PritnfParameter.y_now,
                             LCD_PritnfParameter.x_end, LCD_PritnfParameter.y_now + LCD_PritnfParameter.size - 1,
                             LCD_PritnfParameter.back_color);
                }
            }
            LCD_ShowChar(LCD_PritnfParameter.x_now, LCD_PritnfParameter.y_now, c,
                         LCD_PritnfParameter.size, 0,
                         LCD_PritnfParameter.point_color, LCD_PritnfParameter.back_color);
            LCD_PritnfParameter.x_now += LCD_PritnfParameter.size / 2;
        }
    }
}

/**
 * @brief       显示Logo图片（示例）
 * @retval      无
 * @note        需要外部定义 picture 数组（160x87 RGB565）
 */
void LCD_ShowLog(void)
{
    // 假设 picture 是 160x87 的图片数据
    LCD_ColorFill(0, 0, 160 - 1, 87 - 1, (uint16_t*)picture);
}

/**
 * @brief       LCD 初始化
 * @param       无
 * @retval      1: 初始化成功; 0: 不支持的 LCD 控制器
 * @note        该函数执行 EXMC 接口初始化、背光引脚初始化、读取 LCD 控制器 ID，
 *              若 ID 为 0x9488 则执行 ILI9488 专用初始化序列，并设置为横屏、清白色。
 */
uint8_t LCD_init(void)
{
    exmc_lcd_init();                /* EXMC 硬件接口初始化 */
    delay_1ms(1);
    LCD_BK_Init();                  /* 背光控制引脚初始化 */

    /* 设置默认画笔与背景色 */
    LCD_Parameter.g_point_color = WHITE;    /* 画笔颜色 */
    LCD_Parameter.g_back_color  = BLACK;    /* 背景色 */

    /* 读取 LCD 控制器 ID (以 ILI9341/ILI9488 的 0xD3 寄存器为例) */
    LCD_WriteReg_Address(0xD3);
    LCD_Parameter.id = LCD_ReadData();      /* 空读 */
    LCD_Parameter.id = LCD_ReadData();      /* 读取 0x00 */
    LCD_Parameter.id = LCD_ReadData();      /* 读取高字节，如 0x93 */
    LCD_Parameter.id <<= 8;
    LCD_Parameter.id |= LCD_ReadData();     /* 读取低字节，如 0x41 -> 0x9341 或 0x9488 */
//    LCD_Printf("NOW_SHOW_ID=%x\r\n",LCD_Parameter.id);
    if (LCD_Parameter.id == 0X9488)
    {
        LCD_ILI9488_Reginit();              /* ILI9488 专用初始化序列 */
        LCD_BK_TurnOn();                    /* 点亮背光 */
    }
    else
    {
//        printf("ERROR\r\n");
        return 0;
    }

    LCD_DisplayDirSet(LCD_DISPLAY_DIR_HOR); /* 设置为横屏（可选 LCD_DISPLAY_DIR_VER）HO */
    LCD_Clear(0x0000);                       /* 清屏为白色 */
    return 1;
}

/**
 * @brief       显示一张 40×40 像素的 16 位 BMP 图片
 * @param       x 左上角 X 坐标
 * @param       y 左上角 Y 坐标
 * @param       p 图片数据指针（RGB565 格式，低位在前，高位在后）
 * @retval      无
 * @note        该函数会临时修改显示窗口为 40×40，写完后恢复全屏窗口。
 */
void Gui_Drawbmp16(uint16_t x, uint16_t y, const unsigned char *p)   /* 显示 40×40 图片 */
{
    int i;
    unsigned char picH, picL;

    LCD_SetWindow(x, y, 160, 87);                        /* 设置显示窗口为图片大小 */
    LCD_WriteReg_Address(LCD_Parameter.wramcmd);        /* 准备写入 GRAM （部分 LCD 需要此步骤） */

    for (i = 0; i < 160 * 87; i++)
    {
        picL = *(p + i * 2);                            /* 低字节在前 */
        picH = *(p + i * 2 + 1);                        /* 高字节在后 */
        LCD_WriteReg_Data(picH << 8 | picL);            /* 合并为 16 位 RGB565 数据写入 */
    }

    LCD_SetWindow(0, 0, LCD_Parameter.width, LCD_Parameter.height); /* 恢复全屏显示窗口 */
}

__attribute__((weak,noreturn))
void __aeabi_assert(const char *expr, const char *file, int line)
{
    while(1); // 进入死循环
}
