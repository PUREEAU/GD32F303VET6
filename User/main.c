#include "FreeRtosTask.h"

void Diver_Init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ,GPIO_PIN_9);
    gpio_init(GPIOA,GPIO_MODE_IPU,GPIO_OSPEED_10MHZ,GPIO_PIN_10);
    
    rcu_periph_clock_enable(RCU_USART0);
    usart_deinit(USART0);
    usart_baudrate_set(USART0,115200);
    usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    
}

int main(void)
{
    systick_config();

    Diver_Init();

    vTaskSchedule();

    while (1);
}


