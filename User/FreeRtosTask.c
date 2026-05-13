/*============================================================================
 * 头文件包含区
 *============================================================================*/
#include "FreeRtosTask.h"

float battery_voltage_soc = 0;
uint32_t charge_voltage_mv = 0;
uint32_t battery_voltage_mv = 0;
uint32_t Temperture_voltage = 0;
typedef enum {
    CHG_STATUS_IDLE = 0,      // 未充电
    CHG_STATUS_OVP = 1,       // 过压
    CHG_STATUS_NORMAL = 2,    // 充电中
    CHG_STATUS_LOW = 3        // 电压低
} chg_status_t;
typedef enum {
    BAT_STATUS_IDLE = 0,      // 电压极低
    BAT_STATUS_OVP = 1,       // 过压
    BAT_STATUS_NORMAL = 2,    // 电压正常
    BAT_STATUS_LOW = 3        // 电压低
} bat_status_t;
chg_status_t charge_status;
bat_status_t battery_status;

/*============================================================================
 * 任务句柄声明区（静态全局变量）
 *============================================================================*/
TaskHandle_t Start_Task_Handler;
TaskHandle_t Lcd_Stick_Task_Handler;
TaskHandle_t Lcd_Debug_Task_Handler;
TaskHandle_t Led_Task_Handler;
TaskHandle_t Adc_Data_Manage_Task_Handler;
TaskHandle_t Lcd_Manage_Task_Handler;
/*============================================================================
 * 函数实体区
 *============================================================================*/

/**
 * @brief       启动 FreeRTOS 调度器，创建起始任务
 * @param       无
 * @retval      无
 * @note        该函数由主函数调用，创建起始任务后启动调度器，不会返回。
 */
void vTaskSchedule(void)
{
    xTaskCreate(prvStartTask, "prvStartTask", START_TASK_STACK_SIZE,
                NULL, START_TASK_PRIO, &Start_Task_Handler);

    vTaskStartScheduler();
    
    while(1);
}

/**
 * @brief       起始任务，负责创建所有其他应用任务
 * @param       pvParameters 未使用
 * @retval      无
 * @note        创建 LVGL 心跳任务、LED 任务和调试任务后，打印自身栈使用信息并删除自身。
 */
static void prvStartTask(void *pvParameters)
{
    xTaskCreate(prvLcdStickTask, "prvLcdStickTask", LCD_STICK_TASK_STACK_SIZE,
                NULL, LCD_STICK_TASK_PRIO, &Lcd_Stick_Task_Handler);

    xTaskCreate(prvLEDTask, "prvLEDTask", LED_TASK_STACK_SIZE,
                      NULL, LED_TASK_PRIO, &Led_Task_Handler);

    xTaskCreate(prvLcdDebugTask, "prvLcdDebugTask", LCD_DEBUG_TASK_STACK_SIZE,
               NULL, LCD_DEBUG_TASK_PRIO, &Lcd_Debug_Task_Handler);

    xTaskCreate(prvAdcDataManageTask,"prvAdcDataManageTask", ADC_DATA_MANAGE_TASK_STACK_SIZE,
                NULL, ADC_DATA_MANAGE_TASK_PRIO, &Adc_Data_Manage_Task_Handler);

    xTaskCreate(prvLcdManageTask,"prvLcdManageTask", LCD_MANAGE_TASK_STACK_SIZE,
                NULL, LCD_MANAGE_TASK_PRIO, &Lcd_Manage_Task_Handler);

    printf("---------------------------\r\n");
    UBaseType_t rem = uxTaskGetStackHighWaterMark(Start_Task_Handler);
    printf("prvStartTask free: %d size, used: %d size\r\n", rem, START_TASK_STACK_SIZE - rem);
    vTaskDelete(NULL);
    Start_Task_Handler = NULL;
}

/**
 * @brief       LVGL 心跳任务，定期调用 lv_timer_handler
 * @param       pvParameters 未使用
 * @retval      无
 * @note        每 5 毫秒执行一次，刷新 LVGL 内部定时器。
 */
static void prvLcdStickTask(void *pvParameters)
{
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/**
 * @brief       LED 闪烁任务（示例）
 * @param       pvParameters 未使用
 * @retval      无
 * @note        使能 GPIOC 时钟并初始化 PA8（原代码为 GPIOC 时钟+PA8引脚，此处按原始逻辑保留未修改）。
 */
static void prvLEDTask(void *pvParameters)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    
    while (1) {
        gpio_bit_write(GPIOA, GPIO_PIN_8, (bit_status)(1 - gpio_input_bit_get(GPIOA, GPIO_PIN_8)));
        // printf("Led Work\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief       系统调试任务，打印栈和堆信息，更新 LVGL 标签
 * @param       pvParameters 未使用
 * @retval      无
 * @note        每 5 秒输出一次各个任务的栈使用情况、LVGL 内存监控和 FreeRTOS 堆状态。
 */
static void prvLcdDebugTask(void *pvParameters)
{
    UBaseType_t rem;
    while (1) {
        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);   // 每次循环获取最新内存信息

        // printf("-----------------------------------------\r\n");
        // rem = uxTaskGetStackHighWaterMark(Lcd_Debug_Task_Handler);
        // printf("LcdDebug free: %d size, used: %d size\r\n", rem, LCD_DEBUG_TASK_STACK_SIZE - rem);
        
        // printf("-----------------------------------------\r\n");
        // rem = uxTaskGetStackHighWaterMark(Adc_Data_Manage_Task_Handler);
        // printf("AdcDataManage free: %d size, used: %d size\r\n", rem, ADC_DATA_MANAGE_TASK_STACK_SIZE - rem);
        
        printf("-----------------------------------------\r\n");
        rem = uxTaskGetStackHighWaterMark(Lcd_Manage_Task_Handler);
        printf("Lcd_Manage_Task_Handler free: %d size, used: %d size\r\n", rem, LCD_MANAGE_TASK_STACK_SIZE - rem);

        // printf("------- LVGL Heap Stats -------\r\n");
        // printf("Total: %lu bytes, Used: %lu bytes (%.1f%%), Free: %lu bytes, Max used: %lu bytes, Frag: %u%%\r\n",
        //        mon.total_size,
        //        mon.total_size - mon.free_size,   // 已用字节 = 总大小 - 空闲大小
        //        (float)(mon.total_size - mon.free_size) * 100.0f / mon.total_size,
        //        mon.free_size,
        //        mon.max_used,
        //        mon.frag_pct);

        HeapStats_t xHeapStats;
        vPortGetHeapStats(&xHeapStats);
        printf("---------- FreeRTOS Heap Stats ----------\r\n");
        printf("Total Free: %lu bytes\r\n", xHeapStats.xAvailableHeapSpaceInBytes);
        printf("Largest Free Block: %lu bytes\r\n", xHeapStats.xSizeOfLargestFreeBlockInBytes);
        printf("Min Ever Free: %lu bytes\r\n", xHeapStats.xMinimumEverFreeBytesRemaining);
        printf("Num Free Blocks: %lu\r\n", xHeapStats.xNumberOfFreeBlocks);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

static void prvAdcDataManageTask(void *pvParameters)
{
    ADC_Scan();
    vTaskDelay(pdMS_TO_TICKS(500));
    while (1) {
        
        taskENTER_CRITICAL();
        ADC_Scan();
        
        // 充电电压处理（不变）
        charge_voltage_mv = adc_charge_raw * 1000 * (3.3f / 4095) * (10 + 91) / 10;
        if (charge_voltage_mv >= 5500) {
            charge_status = CHG_STATUS_OVP;
        } else if (charge_voltage_mv >= 4450) {
            charge_status = CHG_STATUS_NORMAL;
        } else if (charge_voltage_mv >= 500) {
            charge_status = CHG_STATUS_LOW;
        } else {
            charge_status = CHG_STATUS_IDLE;
        }

        // 获取原始电池电压
        uint32_t raw_voltage_mv = adc_battery_raw * 1000 * (3.3f / 4095) * (10 + 91) / 10;
        // 滑动平均滤波（使用全局静态变量）
        voltage_sum = voltage_sum - voltage_buffer[voltage_buffer_index] + raw_voltage_mv;
        voltage_buffer[voltage_buffer_index] = raw_voltage_mv;
        voltage_buffer_index = (voltage_buffer_index + 1) % VOLTAGE_WINDOW_SIZE;
        battery_voltage_mv = voltage_sum / VOLTAGE_WINDOW_SIZE;
        // 电池状态判断
        if (battery_voltage_mv > 4200) {
            battery_status = BAT_STATUS_OVP;
        } else if (battery_voltage_mv >= 3700) {
            battery_status = BAT_STATUS_NORMAL;
        } else if (battery_voltage_mv >= 3500) {
            battery_status = BAT_STATUS_LOW;
        } else {
            battery_status = BAT_STATUS_IDLE;
        }
        battery_voltage_soc = battery_voltage_mv / 1000.0f / 4.0f * 100.0f;

        temperture_sum = temperture_sum - temperture_buffer[temperture_buffer_index] + adc_temperture_raw;
        temperture_buffer[temperture_buffer_index] = adc_temperture_raw;
        temperture_buffer_index = (temperture_buffer_index + 1) % TEMPERTURE_WINDOW_SIZE;
        uint32_t temperture_avg = temperture_sum / TEMPERTURE_WINDOW_SIZE;
        uint8_t idx = 0;
        if (temperture_avg <= ntc_adc_table[0]) {
            idx = 0;
        } else if (temperture_avg >= ntc_adc_table[NTC_ADC_SIZE - 1]) {
            idx = NTC_ADC_SIZE - 1;
        } else {
            uint8_t lo = 0, hi = NTC_ADC_SIZE - 1;
            while (lo <= hi) {
                uint8_t mid = (lo + hi) / 2;
                if (temperture_avg == ntc_adc_table[mid]) {
                    idx = mid;
                    break;
                } else if (temperture_avg < ntc_adc_table[mid]) {
                    hi = mid - 1;
                } else {
                    lo = mid + 1;
                }
            }
            if (temperture_avg != ntc_adc_table[idx]) {
                idx = hi;   
            }
        }
        Temperture_voltage = idx - 44;

        taskEXIT_CRITICAL();
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void prvLcdManageTask(void *pvParameters){

    createMainInterface();
    
    while (1)
    {
        taskENTER_CRITICAL();
        mainmenukeyboardNavPointer = 0;
        mainMenuButtonChoice();
        taskEXIT_CRITICAL();
        while (!mainmenukeyboardNavPointer)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            if(shutdownDevice){
                vTaskDelay(pdMS_TO_TICKS(100));
                mainmenukeyboardNavPointer = 0xFF;
                break;
            }
        }

        switch (mainmenukeyboardNavPointer)
        {
            case 1:
                settingsModeInterface();
            break;

            default:
            break;
        }

        while(mainmenukeyboardNavPointer)
        {
            vTaskDelay(50);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}



