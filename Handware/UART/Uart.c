#include "Uart.h"

void Uart_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
    
    rcu_periph_clock_enable(RCU_USART1);
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
    
}
















