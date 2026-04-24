#include "FreeRtosTask.h"

// 设置lvgl的心跳函数
#define LCD_STICK_TASK_PRIO     9
#define LCD_STICK_TASK_STACK_SIZE 400
TaskHandle_t Lcd_Stick_Task_Handler;
static void prvLcdStickTask(void *pvParameters);

// RreeTtos 起始任务优先级和栈大小
#define START_TASK_PRIO         10
#define START_TASK_STACK_SIZE   128
TaskHandle_t Start_Task_Handler;
static void prvStartTask(void *pvParameters);

//lcd显示任务
#define LED_TASK_PRIO           5
#define LED_TASK_STACK_SIZE     128
TaskHandle_t Led_Task_Handler;
static void prvLEDTask(void *pvParameters);

//系统调试任务
#define LCD_DEBUG_TASK_PRIO     5
#define LCD_DEBUG_TASK_STACK_SIZE 512
TaskHandle_t Lcd_Debug_Task_Handler;
static void prvLcdDebugTask(void *pvParameters);

void vTaskSchedule(void){

    xTaskCreate(prvStartTask, "prvStartTask", START_TASK_STACK_SIZE,
                NULL, START_TASK_PRIO, &Start_Task_Handler);

    vTaskStartScheduler();
    
    while(1);
}

static void prvStartTask(void *pvParameters)
{

    xTaskCreate(prvLcdStickTask, "prvLcdStickTask", LCD_STICK_TASK_STACK_SIZE,
                NULL, LCD_STICK_TASK_PRIO, &Lcd_Stick_Task_Handler);

    xTaskCreate(prvLEDTask, "prvLEDTask", LED_TASK_STACK_SIZE,
                      NULL, LED_TASK_PRIO, &Led_Task_Handler);

    xTaskCreate(prvLcdDebugTask, "prvLcdDebugTask", LCD_DEBUG_TASK_STACK_SIZE,
               NULL, LCD_DEBUG_TASK_PRIO, &Lcd_Debug_Task_Handler);
    
    UBaseType_t rem;
    rem = uxTaskGetStackHighWaterMark(Start_Task_Handler);
    printf("prvStartTask 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, START_TASK_STACK_SIZE - rem);
    printf("---------------------------\r\n");
    vTaskDelete(NULL);
    Start_Task_Handler = NULL;
}

static void prvLcdStickTask(void *pvParameters)
{
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

static void prvLEDTask(void *pvParameters)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    
    while (1) {
        gpio_bit_write(GPIOA, GPIO_PIN_8, (bit_status)(1 - gpio_input_bit_get(GPIOA, GPIO_PIN_8)));
        // printf("Led Work\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延迟 500ms
    }

}
static void prvLcdDebugTask(void *pvParameters)
{
    // 创建标签一次
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
        rem = uxTaskGetStackHighWaterMark(Lcd_Stick_Task_Handler);
        printf("LcdStick 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, LCD_STICK_TASK_STACK_SIZE - rem);

        rem = uxTaskGetStackHighWaterMark(Led_Task_Handler);
        printf("Led 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, LED_TASK_STACK_SIZE - rem);

        rem = uxTaskGetStackHighWaterMark(Lcd_Debug_Task_Handler);
        printf("LcdDebug 剩余栈: %d 字, 已用栈: %d 字\r\n", rem, LCD_DEBUG_TASK_STACK_SIZE - rem);

        uint32_t used_bytes = mon.total_size - mon.free_size;  // 当前已使用字节数
        printf("Total: %u bytes, Used: %u bytes (%.1f%%), Free: %u bytes, Max used: %u bytes, Frag: %u%%\r\n",
            (unsigned int)mon.total_size,
            (unsigned int)used_bytes,
            (float)(used_bytes * 100.0f / mon.total_size),
            (unsigned int)mon.free_size,
            (unsigned int)mon.max_used,
            (unsigned int)mon.frag_pct);

        /* 获取堆统计信息 */
        vPortGetHeapStats(&xHeapStats);
        /* 打印关键信息，用于调试 */
        printf("--- FreeRTOS Heap Stats ---\r\n");
        printf("Total Free: %lu bytes\r\n", xHeapStats.xAvailableHeapSpaceInBytes);
        printf("Largest Free Block: %lu bytes\r\n", xHeapStats.xSizeOfLargestFreeBlockInBytes);
        printf("Min Ever Free: %lu bytes\r\n", xHeapStats.xMinimumEverFreeBytesRemaining);
        printf("Num Free Blocks: %lu\r\n", xHeapStats.xNumberOfFreeBlocks);
        printf("---------------------------\r\n");

        sprintf(buf, "%d", count++);
        lv_label_set_text(label, buf);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int fputc(int ch, FILE *p){
    usart_data_transmit(USART1,(uint8_t)ch);
    while (RESET == usart_flag_get(USART1,USART_FLAG_TBE));
    return ch;
}
