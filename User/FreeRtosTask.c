#include "FreeRtosTask.h"

#define LCD_STICK_TASK_PRIO     9
#define LCD_STICK_TASK_STACK_SIZE 512
TaskHandle_t Lcd_Stick_Task_Handler;
static void prvLcdStickTask(void *pvParameters);

#define START_TASK_PRIO         10
#define START_TASK_STACK_SIZE   128
TaskHandle_t Start_Task_Handler;
static void prvStartTask(void *pvParameters);

#define LED_TASK_PRIO           5
#define LED_TASK_STACK_SIZE     128
TaskHandle_t Led_Task_Handler;
static void prvLEDTask(void *pvParameters);

#define LCD_DEBUG_TASK_PRIO     5
#define LCD_DEBUG_TASK_STACK_SIZE 2 * 1024
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
    
    vTaskDelete(NULL);
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
        vTaskDelay(pdMS_TO_TICKS(500)); // 延迟 500ms
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
    while (1) {
        sprintf(buf, "%d", count++);
        lv_label_set_text(label, buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

