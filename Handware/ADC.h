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


void ADC_Init(void);
void ADC_Scan(void);

extern uint16_t adc_chg_vtg;
extern uint16_t adc_bat_vtg;
extern uint16_t adc_tmp_vtg;
extern uint16_t ntc_adc_table[];
#endif
