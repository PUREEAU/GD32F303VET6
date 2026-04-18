#ifndef __FREERTOS_TASK_H__
#define __FREERTOS_TASK_H__ 

#include "gd32f30x.h"
#include "systick.h"
#include "stdio.h"
#include "main.h"

#include "ili9488.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

#include "FreeRTOS.h"
#include "task.h"


void vTaskSchedule(void);


#endif /* __FREERTOS_TASK_H__ */
