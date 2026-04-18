#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

#include "ili9488.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "FreeRtosTask.h"
//#define LED_GPIO_RCUPORT		RCU_GPIOA
//#define LED_GPIO_PORT				GPIOA
//#define LED_GPIO_PIN				GPIO_PIN_1
//#define LED_GPIO_PINNUM			1
//void LED_Init(void)
//{
//	rcu_periph_clock_enable(LED_GPIO_RCUPORT);
//	gpio_init(LED_GPIO_PORT,GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN); //将PA8配置为推挽输出，最高支持50MHz
//}
//void LED_TurnOn(void)
//{
//	//GPIO_BOP(LED_GPIO_PORT) = BIT(LED_GPIO_PINNUM);
//	gpio_bit_set(LED_GPIO_PORT,LED_GPIO_PIN);
//}
//void LED_BlinkOnOff(void)
//{
//	if(gpio_input_bit_get(LED_GPIO_PORT,LED_GPIO_PIN) == 1)
//		gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
//	else
//		gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
//}
//void LED_Task(void *pvParameters)
//{
//    while(1)
//    {
//      LED_BlinkOnOff();
//      vTaskDelay(500);
//    }
//} 

//void LED_TurnOff(void)
//{
//	//GPIO_BC(LED_GPIO_PORT) = BIT(LED_GPIO_PINNUM);
//	gpio_bit_reset(LED_GPIO_PORT,LED_GPIO_PIN);
//}

//#define LED_TASK_PRIO				(tskIDLE_PRIORITY + 1)
////任务堆栈大小	
//#define LED_STK_SIZE 		50  
////任务句柄
//TaskHandle_t LEDTask_Handler;

//void RTOS_Init(void)
//{
//	taskENTER_CRITICAL();           //进入临界区
//	//创建LED任务
//	xTaskCreate((TaskFunction_t )LED_Task,     	
//							(const char*    )"LED_task",   	
//							(uint16_t       )LED_STK_SIZE, 
//							(void*          )NULL,				
//							(UBaseType_t    )LED_TASK_PRIO,	
//							(TaskHandle_t*  )&LEDTask_Handler); 
//					
//							
//	taskEXIT_CRITICAL();            //退出临界区	
//	vTaskStartScheduler();          //开启任务调度
//}

//void System_Init(void)
//{
//	
//	LED_Init();		//初始化LED
//	LED_TurnOff();//默认关闭LED
//}

//int main(void)
//{
//    /* configure systick */
//    systick_config();
//		System_Init();
//		RTOS_Init();
//}
































// int main(void)
// {
    
//     rcu_periph_clock_enable(RCU_GPIOA);
//     gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
//     gpio_bit_reset(GPIOA, GPIO_PIN_8);
    
//     systick_config();
    
//     rcu_periph_clock_enable(RCU_TIMER2);
//     timer_parameter_struct tim2_init;
//     tim2_init.prescaler         = 1200 - 1;          // 预分频值
//     tim2_init.alignedmode       = TIMER_COUNTER_EDGE;       // 边沿对齐
//     tim2_init.counterdirection  = TIMER_COUNTER_UP;        // 向上计数
//     tim2_init.period            = 100000 - 1;         // 自动重载值
//     tim2_init.clockdivision     = TIMER_CKDIV_DIV1;        // 时钟分频
//     tim2_init.repetitioncounter = 0;                 // 重复计数器（通用定时器用0）
//     timer_init(TIMER2,&tim2_init);
//     timer_interrupt_enable(TIMER2,TIMER_INT_UP);
//     nvic_irq_enable(TIMER2_IRQn,0,1);
//     timer_enable(TIMER2);
    
//     lv_init();
//     lv_port_disp_init();
//     lv_port_indev_init();

//     lv_obj_t * scr = lv_disp_get_scr_act(NULL);
//     lv_obj_t * label = lv_label_create(scr);
//     lv_label_set_text(label, "Hello, World!");
//     lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
//     while (1)
//     {

//         lv_timer_handler();
//         delay_1ms(1);
//     }
// } 

// uint8_t lag;
// void TIMER2_IRQHandler(void){
//     if(timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP) == SET){
//         timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
//         lag = !lag;
//         if(lag)gpio_bit_set(GPIOA, GPIO_PIN_8);
//         else gpio_bit_reset(GPIOA, GPIO_PIN_8);
//     }
// }

#define START_TASK_PRIO 6
#define START_TASK_STACK_SIZE 512
TaskHandle_t Start_Task_Handler;
void Start_Task( void * pvParameters );


int main(void)
{
    systick_config();
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_bit_reset(GPIOA, GPIO_PIN_8);
    xTaskCreate( Start_Task, "Start_Task", START_TASK_STACK_SIZE, NULL, START_TASK_PRIO, &Start_Task_Handler );
    vTaskStartScheduler();
}
uint8_t lag;
void Start_Task( void * pvParameters )
{
    while (1)
    {
        lag = !lag;
        if(lag)gpio_bit_set(GPIOA, GPIO_PIN_8);
        else gpio_bit_reset(GPIOA, GPIO_PIN_8);
        vTaskDelay(1000);
    }
    
}
