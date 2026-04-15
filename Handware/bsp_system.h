#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#define GPIOA_BASE  0x40010800U
#define GPIOB_BASE  0x40010C00U
#define GPIOC_BASE  0x40011000U
#define GPIOD_BASE  0x40011400U
#define GPIOE_BASE  0x40011800U
#define GPIOF_BASE  0x40011C00U
#define GPIOG_BASE  0x40012000U
#define GPIOA_CTL0  (*(volatile uint32_t *)(GPIOA_BASE + 0x00U))
#define GPIOA_CTL1  (*(volatile uint32_t *)(GPIOA_BASE + 0x04U))
    
#define GPIOB_CTL0  (*(volatile uint32_t *)(GPIOB_BASE + 0x00U))
#define GPIOB_CTL1  (*(volatile uint32_t *)(GPIOB_BASE + 0x04U))
    
#define GPIOC_CTL0  (*(volatile uint32_t *)(GPIOC_BASE + 0x00U))
#define GPIOC_CTL1  (*(volatile uint32_t *)(GPIOC_BASE + 0x04U))
    
#define GPIOD_CTL0  (*(volatile uint32_t *)(GPIOD_BASE + 0x00U))
#define GPIOD_CTL1  (*(volatile uint32_t *)(GPIOD_BASE + 0x04U))
    
#define GPIOE_CTL0  (*(volatile uint32_t *)(GPIOE_BASE + 0x00U))
#define GPIOE_CTL1  (*(volatile uint32_t *)(GPIOE_BASE + 0x04U))
    
#define GPIOF_CTL0  (*(volatile uint32_t *)(GPIOF_BASE + 0x00U))
#define GPIOF_CTL1  (*(volatile uint32_t *)(GPIOF_BASE + 0x04U))

#define GPIOA_OCTL  (*(volatile uint32_t *)(GPIOA_BASE + 0x0CU))

#endif
