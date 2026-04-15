#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

#include "ili9488.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"


int main(void)
{
    
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_bit_reset(GPIOA, GPIO_PIN_8);
    
    systick_config();
    
    rcu_periph_clock_enable(RCU_TIMER2);
    timer_parameter_struct tim2_init;
    tim2_init.prescaler         = 1200 - 1;          // 预分频值
    tim2_init.alignedmode       = TIMER_COUNTER_EDGE;       // 边沿对齐
    tim2_init.counterdirection  = TIMER_COUNTER_UP;        // 向上计数
    tim2_init.period            = 100000 - 1;         // 自动重载值
    tim2_init.clockdivision     = TIMER_CKDIV_DIV1;        // 时钟分频
    tim2_init.repetitioncounter = 0;                 // 重复计数器（通用定时器用0）
    timer_init(TIMER2,&tim2_init);
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);
    nvic_irq_enable(TIMER2_IRQn,0,1);
    timer_enable(TIMER2);
    
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    while (1)
    {

        lv_timer_handler();
        delay_1ms(1);
    }
}

uint8_t lag;
void TIMER2_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP) == SET){
        timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
        lag = !lag;
        if(lag)gpio_bit_set(GPIOA, GPIO_PIN_8);
        else gpio_bit_reset(GPIOA, GPIO_PIN_8);
    }
}

