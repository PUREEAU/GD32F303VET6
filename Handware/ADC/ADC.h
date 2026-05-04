#ifndef __ADC_H
#define __ADC_H

#include "gd32f30x.h"
#include "gd32f30x_adc.h"

#define ADC_GPIO_RCU_PORT RUC_GPIOA

//扫描ADC通道的Flag
#define ADC_CHG_RANK_NUM 0U
#define ADC_BAT_RANK_NUM 1U
#define ADC_TMP_RANK_NUM 2U
//ADC通道配置
#define ADC_CHG_CHANNEL ADC_CHANNEL_4 
#define ADC_BAT_CHANNEL ADC_CHANNEL_5
#define ADC_TMP_CHANNEL ADC_CHANNEL_6

#define ADC_CHANNEL_MAX 1
#define NTC_ADC_SIZE 191

void ADC_Init(void);
void ADC_Scan(void);
void ADC_Config(void);

extern uint16_t adc_charge_raw;
extern uint16_t adc_battery_raw;
extern uint16_t adc_temperture_raw;
extern uint16_t ntc_adc_table[];

#define VOLTAGE_WINDOW_SIZE  16
extern uint32_t voltage_buffer[VOLTAGE_WINDOW_SIZE];
extern uint8_t  voltage_buffer_index ;
extern uint32_t voltage_sum ;

#define TEMPERTURE_WINDOW_SIZE  4
extern uint32_t temperture_buffer[TEMPERTURE_WINDOW_SIZE];
extern uint8_t  temperture_buffer_index ;
extern uint32_t temperture_sum ;

void adc_filter_prefill(void);

#endif
