#include "FreeRtosTask.h"

void Diver_Init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    
    ADC_Init();
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ,GPIO_PIN_2);
    gpio_init(GPIOA,GPIO_MODE_IPU,GPIO_OSPEED_10MHZ,GPIO_PIN_3);
    
    rcu_periph_clock_enable(RCU_USART1);
    usart_deinit(USART1);
    usart_baudrate_set(USART1,115200);
    usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
    
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_bit_set(GPIOC,GPIO_PIN_1);
}

int main(void)
{
    systick_config();

    Diver_Init();

    vTaskSchedule();

    while (1);
}

int fputc(int ch, FILE *p){
    usart_data_transmit(USART1,(uint8_t)ch);
    while (RESET == usart_flag_get(USART1,USART_FLAG_TBE));
    return ch;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack overflow in task: %s\r\n", pcTaskName);
    for(;;);
}


