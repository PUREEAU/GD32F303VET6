/*!
    \file    exmc_lcd.c
    \brief   EXMC LCD driver

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "exmc_lcd.h"
#include "bsp_system.h"
/*!
    \brief      LCD 外设初始化
    \param[in]  none 
    \param[out] none
    \retval     none
*/
void exmc_lcd_init(void)
{
    
    exmc_norsram_parameter_struct lcd_init_struct;
    exmc_norsram_timing_parameter_struct lcd_timing_init_struct;
    
    /* 1. 使能 EXMC 时钟 */
//    RCU_AHBEN |= (1 << 8);               // 直接寄存器操作（等效，已注释）
    rcu_periph_clock_enable(RCU_EXMC);

    /* 2. 使能 GPIO 时钟 */
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
//    RCU_APB2EN |= (1 << 5);              // 对应 GPIOE 时钟
//    RCU_APB2EN |= (1 << 6);              // 对应 GPIOD 时钟
//    RCU_APB2EN |= (1 << 8);              // 对应 GPIOF 时钟

    /* 3. 配置 EXMC 数据总线 (EXMC_D[0~15]) */
    /* PD14(EXMC_D0), PD15(EXMC_D1), PD0(EXMC_D2), PD1(EXMC_D3), 
       PD8(EXMC_D13), PD9(EXMC_D14), PD10(EXMC_D15) */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
              GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
              GPIO_PIN_14 | GPIO_PIN_15);

    /* PE7(EXMC_D4), PE8(EXMC_D5), PE9(EXMC_D6), PE10(EXMC_D7), 
       PE11(EXMC_D8), PE12(EXMC_D9), PE13(EXMC_D10), PE14(EXMC_D11), PE15(EXMC_D12) */
    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* 4. 配置地址线 A23（用作命令/数据选择） */ // 选择高位，处理地址线错位问题 : HADDR[25:0] 	EXMC_A[24:0]
    /* PE2(EXMC_A23) */ 
    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    
    /* 5. 配置控制信号：NOE（读使能）和 NWE（写使能） */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);

    /* 6. 配置片选 NE0（Bank0 Region0） */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    /* 7. 填充 EXMC 时序参数（根据 LCD 数据手册调整） */ //80MHz.配置EXMC时序后一个周期为12.5ns
    lcd_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;
    lcd_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;
    lcd_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;
    lcd_timing_init_struct.bus_latency = 1;
    lcd_timing_init_struct.asyn_data_setuptime = 5;
    lcd_timing_init_struct.asyn_address_holdtime = 2;
    lcd_timing_init_struct.asyn_address_setuptime = 3;
    

    // 配置 EXMC 基本工作参数
    lcd_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION0;   // 使用片选 NE0，地址基址 0x60000000
    lcd_init_struct.write_mode = EXMC_ASYN_WRITE;                  // 异步写模式（无需同步时钟）
    lcd_init_struct.extended_mode = DISABLE;                       // 关闭扩展模式，读写时序相同
    lcd_init_struct.asyn_wait = DISABLE;                           // 禁用异步等待（不插入额外等待周期）
    lcd_init_struct.nwait_signal = DISABLE;                        // 禁用 NWAIT 信号（LCD 不支持）
    lcd_init_struct.memory_write = ENABLE;                         // 允许写操作（向 LCD 发命令/数据）
    lcd_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;       // NWAIT 采样点（同步模式用，此处无效）
    lcd_init_struct.wrap_burst_mode = DISABLE;                     // 禁用地址绕回突发（LCD 不支持）
    lcd_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;      // NWAIT 低电平有效（同步模式用）
    lcd_init_struct.burst_mode = DISABLE;                          // 禁用突发模式（LCD 为异步访问）
    lcd_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;    // 16 位数据总线（LCD 的 DB[15:0]）
    lcd_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;           // 存储器类型设为 SRAM（产生 8080 时序）
    lcd_init_struct.address_data_mux = DISABLE;                    // 地址/数据线不复用（RS 接地址线，数据线独立）
    lcd_init_struct.read_write_timing = &lcd_timing_init_struct;   // 读写共用的时序参数（因 extended_mode = DISABLE）
    lcd_init_struct.write_timing = &lcd_timing_init_struct;        // 写时序指针（扩展模式未用，但赋值一致）

    /* 9. 根据配置初始化 EXMC */
    exmc_norsram_init(&lcd_init_struct);

    /* 10. 使能 EXMC Bank0 Region0 */
    exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION0);
    
}
