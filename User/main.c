#include "FreeRtosTask.h"

void Diver_Init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    
    ADC_Init();
    adc_filter_prefill();
    
    Uart_Init();
    
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_set(GPIOC, GPIO_PIN_1);
    
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


