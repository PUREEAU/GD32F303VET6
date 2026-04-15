#ifndef __KEY_H
#define __KEY_H

#include "gd32f30x.h"

/*============================================================================
 * 宏定义区
 *============================================================================*/

/* 按键1 - PA0 */
#define KEY1_PIN         GPIO_PIN_0
#define KEY1_PORT        GPIOA

/* 按键2 - PA7 */
#define KEY2_PIN         GPIO_PIN_7
#define KEY2_PORT        GPIOA

/* 按键3 - PC4 */
#define KEY3_PIN         GPIO_PIN_4
#define KEY3_PORT        GPIOC

/* 按键4 - PC5 */
#define KEY4_PIN         GPIO_PIN_5
#define KEY4_PORT        GPIOC

/* 按键5 - PB0 */
#define KEY5_PIN         GPIO_PIN_0
#define KEY5_PORT        GPIOB

/* 按键6 - PB1 */
#define KEY6_PIN         GPIO_PIN_1
#define KEY6_PORT        GPIOB

/* 按键7 - PA11 */
#define KEY7_PIN         GPIO_PIN_11
#define KEY7_PORT        GPIOA

/* 按键8 - PA12 */
#define KEY8_PIN         GPIO_PIN_12
#define KEY8_PORT        GPIOA

#define ALL_KEY_PINS_GPIOA   (KEY1_PIN | KEY2_PIN | KEY7_PIN | KEY8_PIN)
#define ALL_KEY_PINS_GPIOB   (KEY5_PIN | KEY6_PIN)
#define ALL_KEY_PINS_GPIOC   (KEY3_PIN | KEY4_PIN)

/*============================================================================
 * 函数声明区
 *============================================================================*/
void key_Init(void);
uint8_t key_GetId(void);

#endif
