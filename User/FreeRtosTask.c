/*============================================================================
 * 头文件包含区
 *============================================================================*/
#include "FreeRtosTask.h"

/*============================================================================
 * 宏定义区
 *============================================================================*/
/* 设置 lvgl 的心跳函数 */
#define LCD_STICK_TASK_PRIO             9
#define LCD_STICK_TASK_STACK_SIZE       400

/* FreeRTOS 起始任务优先级和栈大小 */
#define START_TASK_PRIO                 10
#define START_TASK_STACK_SIZE           128

/* lcd显示任务（实际为LED闪烁任务） */
#define LED_TASK_PRIO                   5
#define LED_TASK_STACK_SIZE             128

/* 系统调试任务 */
#define LCD_DEBUG_TASK_PRIO             8
#define LCD_DEBUG_TASK_STACK_SIZE       512

/* ADC 数据管理任务 */
#define ADC_DATA_MANAGE_TASK_PRIO       5
#define ADC_DATA_MANAGE_TASK_STACK_SIZE 256

/*============================================================================
 * 任务句柄声明区（静态全局变量）
 *============================================================================*/
TaskHandle_t Lcd_Stick_Task_Handler;
TaskHandle_t Start_Task_Handler;
TaskHandle_t Led_Task_Handler;
TaskHandle_t Lcd_Debug_Task_Handler;
TaskHandle_t Adc_Data_Manage_Task_Handler;

/*============================================================================
 * 函数声明区
 *============================================================================*/
static void prvLcdStickTask(void *pvParameters);
static void prvStartTask(void *pvParameters);
static void prvLEDTask(void *pvParameters);
static void prvLcdDebugTask(void *pvParameters);
static void prvAdcDataManageTask(void *pvParameters);

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
    /* 创建标签一次 */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_obj_center(label);
    lv_label_set_text(label, "0");
    uint16_t count = 0;
    char buf[20];
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);

    UBaseType_t rem;

    HeapStats_t xHeapStats;

    while (1) {
        // rem = uxTaskGetStackHighWaterMark(Lcd_Stick_Task_Handler);
        // printf("LcdStick 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, LCD_STICK_TASK_STACK_SIZE - rem);

        // rem = uxTaskGetStackHighWaterMark(Led_Task_Handler);
        // printf("Led 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, LED_TASK_STACK_SIZE - rem);
        printf("-------------------------------\r\n");
        rem = uxTaskGetStackHighWaterMark(Lcd_Debug_Task_Handler);
        printf("LcdDebug free: %d size, used: %d size\r\n", rem, LCD_DEBUG_TASK_STACK_SIZE - rem);
        printf("-------------------------------\r\n");
        rem = uxTaskGetStackHighWaterMark(Adc_Data_Manage_Task_Handler);
        printf("AdcDataManage free: %d size, used: %d size\r\n", rem, ADC_DATA_MANAGE_TASK_STACK_SIZE - rem);

        // printf("------- LVGL Heap Stats -------\r\n");
        // uint32_t used_bytes = mon.total_size - mon.free_size;
        // printf("Total: %u bytes, Used: %u bytes (%.1f%%), Free: %u bytes, Max used: %u bytes, Frag: %u%%\r\n",
        //     (unsigned int)mon.total_size,
        //     (unsigned int)used_bytes,
        //     (float)(used_bytes * 100.0f / mon.total_size),
        //     (unsigned int)mon.free_size,
        //     (unsigned int)mon.max_used,
        //     (unsigned int)mon.frag_pct);
        
        /* 获取堆统计信息 */
        vPortGetHeapStats(&xHeapStats);
        printf("----- FreeRTOS Heap Stats -----\r\n");
        printf("Total Free: %lu bytes\r\n", xHeapStats.xAvailableHeapSpaceInBytes);
        printf("Largest Free Block: %lu bytes\r\n", xHeapStats.xSizeOfLargestFreeBlockInBytes);
        printf("Min Ever Free: %lu bytes\r\n", xHeapStats.xMinimumEverFreeBytesRemaining);
        printf("Num Free Blocks: %lu\r\n", xHeapStats.xNumberOfFreeBlocks);
        


        
        sprintf(buf, "%d", count++);
        lv_label_set_text(label, buf);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

uint16_t charge_voltage_mv = 0;
static void prvAdcDataManageTask(void *pvParameters){

    while (1)
    {
        ADC_Scan();
        charge_voltage_mv = adc_charge_raw * 1000*(3.3f/4095) * (10+91)/10;


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
