#ifndef __FREERTOS_TASK_H__
#define __FREERTOS_TASK_H__ 

#include "gd32f30x.h"
#include "systick.h"
#include "main.h"

#include "ili9488.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Adc.h"
#include "Uart.h"
#include "Gui.h"

void vTaskSchedule(void);

/*============================================================================
 * 宏定义区
 *============================================================================*/
/* FreeRTOS 起始任务优先级和栈大小 */
#define START_TASK_PRIO                 10
#define START_TASK_STACK_SIZE           128

 /* 设置 lvgl 的心跳函数 */
#define LCD_STICK_TASK_PRIO             9
#define LCD_STICK_TASK_STACK_SIZE       400

/* 系统调试任务 */
#define LCD_DEBUG_TASK_PRIO             8
#define LCD_DEBUG_TASK_STACK_SIZE       512

/* lcd显示任务（实际为LED闪烁任务） */
#define LED_TASK_PRIO                   7
#define LED_TASK_STACK_SIZE             128

/* ADC 数据管理任务 */
#define ADC_DATA_MANAGE_TASK_PRIO       5
#define ADC_DATA_MANAGE_TASK_STACK_SIZE 128

/* LCD 管理任务 */
#define LCD_MANAGE_TASK_PRIO            5
#define LCD_MANAGE_TASK_STACK_SIZE      512

/*============================================================================
 * 函数声明区
 *============================================================================*/
static void prvStartTask(void *pvParameters);
static void prvLcdStickTask(void *pvParameters);
static void prvLcdDebugTask(void *pvParameters);

static void prvLEDTask(void *pvParameters);
static void prvAdcDataManageTask(void *pvParameters);
static void prvLcdManageTask(void *pvParameters);



extern float battery_voltage_soc;





#endif /* __FREERTOS_TASK_H__ */
