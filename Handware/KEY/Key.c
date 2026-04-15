#include "Key.h"
#include <stdbool.h>
/*============================================================================
 * 函数实体区
 *============================================================================*/
/**
 * @brief 按键 GPIO 初始化
 * 
 * 配置所有按键对应的 GPIO 引脚为：
 * - 输入模式 (GPIO_MODE_IPU)：内部上拉，默认高电平
 * - 速度参数：对于输入模式无效，保留默认值即可
 * 
 * @note PA11/PA12 默认复用为 USB 功能。如果项目中使能了 USB 外设，
 *       请确保 USB 时钟未使能，或通过 GPIO 重映射/引脚复用功能解决冲突。
 *       如果不需要 USB，可以保持当前配置。
 */
void key_Init(void)
{
    /* 使能 GPIO 时钟（每个端口只需使能一次） */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* 批量配置同一端口的按键引脚 */
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, ALL_KEY_PINS_GPIOA);
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, ALL_KEY_PINS_GPIOB);
    gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, ALL_KEY_PINS_GPIOC);

}

//获取按键的值，确定是那个按键按下
static bool B1_data,B2_data,B3_data,B4_data,B5_data,B6_data,B7_data,B8_data;
static bool B1_last = 0, B2_last = 0, B3_last = 0, B4_last = 0;
static bool B5_last = 0, B6_last = 0, B7_last = 0, B8_last = 0;
uint8_t key_GetId(void)
{
    // 读取所有按键当前状态（true=按下，false=释放）
    B1_data = gpio_input_bit_get(KEY1_PORT, KEY1_PIN);
    B2_data = gpio_input_bit_get(KEY2_PORT, KEY2_PIN);
    B3_data = gpio_input_bit_get(KEY3_PORT, KEY3_PIN);
    B4_data = gpio_input_bit_get(KEY4_PORT, KEY4_PIN);
    B5_data = gpio_input_bit_get(KEY5_PORT, KEY5_PIN);
    B6_data = gpio_input_bit_get(KEY6_PORT, KEY6_PIN);
    B7_data = gpio_input_bit_get(KEY7_PORT, KEY7_PIN);
    B8_data = gpio_input_bit_get(KEY8_PORT, KEY8_PIN);

    uint8_t ret = 0;

    // 检测下降沿（上次为高，本次为低）
    if (!B1_last && !B1_data) ret = 1;
    if (!B2_last && !B2_data) ret = 2;
    if (!B3_last && !B3_data) ret = 3;
    if (!B4_last && !B4_data) ret = 4;
    if (!B5_last && !B5_data) ret = 5;
    if (!B6_last && !B6_data) ret = 6;
    if (!B7_last && !B7_data) ret = 7;
    if (!B8_last && !B8_data) ret = 8;
    
    // 更新所有上次状态（为下次检测准备）
    B1_last = B1_data;
    B2_last = B2_data;
    B3_last = B3_data;
    B4_last = B4_data;
    B5_last = B5_data;
    B6_last = B6_data;
    B7_last = B7_data;
    B8_last = B8_data;

    return ret;
}