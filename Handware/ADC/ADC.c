#include "ADC.h"
static uint16_t ADC0_Value[ADC_CHANNEL_MAX]={0};
static uint8_t ADC0_Flag;
static uint16_t adv_type;
uint16_t adc_chg_vtg=0;
uint16_t adc_bat_vtg=0;
uint16_t adc_tmp_vtg=0;

uint16_t ntc_adc_table[]=
{ //adc数值   温度      序号
    3956,   //-40       0
    3947,   //-39       1
    3937,   //-38       2
    3928,   //-37       3
    3918,   //-36       4
    3908,   //-35       5
    3897,   //-34       6
    3886,   //-33       7
    3874,   //-32       8
    3863,   //-31       9
    3850,   //-30       10
    3837,   //-29       11
    3824,   //-28       12
    3810,   //-27       13
    3795,   //-26       14
    3779,   //-25       15
    3763,   //-24       16
    3747,   //-23       17
    3729,   //-22       18
    3711,   //-21       19
    3693,   //-20       20
    3673,   //-19       21
    3653,   //-18       22
    3632,   //-17       23
    3610,   //-16       24
    3587,   //-15       25
    3563,   //-14       26
    3538,   //-13       27
    3513,   //-12       28
    3486,   //-11       29
    3458,   //-10       30
    3429,   //-9        31
    3400,   //-8        32
    3369,   //-7        33
    3338,   //-6        34
    3305,   //-5        35
    3272,   //-4        36
    3238,   //-3        37
    3203,   //-2        38
    3167,   //-1        39
    3131,   //0         40
    3093,   //1         41
    3055,   //2         42
    3016,   //3         43
    2977,   //4         44
    2936,   //5         45
    2895,   //6         46
    2853,   //7         47
    2811,   //8         48
    2768,   //9         49
    2725,   //10        50
    2682,   //11        51
    2637,   //12        52
    2593,   //13        53
    2548,   //14        54
    2503,   //15        55
    2458,   //16        56
    2412,   //17        57
    2366,   //18        58
    2321,   //19        59
    2275,   //20        60
    2229,   //21        61
    2184,   //22        62
    2138,   //23        63
    2093,   //24        64
    2048,   //25        65
    2003,   //26        66
    1959,   //27        67
    1914,   //28        68
    1871,   //29        69
    1827,   //30        70
    1784,   //31        71
    1742,   //32        72
    1699,   //33        73
    1658,   //34        74
    1617,   //35        75
    1577,   //36        76
    1537,   //37        77
    1498,   //38        78
    1459,   //39        79
    1421,   //40        80
    1384,   //41        81
    1348,   //42        82
    1312,   //43        83
    1277,   //44        84
    1243,   //45        85
    1209,   //46        86
    1176,   //47        87
    1144,   //48        88
    1112,   //49        89
    1082,   //50        90
    1052,   //51        91
    1023,   //52        92
    994 ,   //53        93
    966 ,   //54        94
    939 ,   //55        95
    913 ,   //56        96
    887 ,   //57        97
    862 ,   //58        98
    837 ,   //59        99
    813 ,   //60        100
    790 ,   //61        101
    768 ,   //62        102
    745 ,   //63        103
    724 ,   //64        104
    703 ,   //65        105
    683 ,   //66        106
    664 ,   //67        107
    644 ,   //68        108
    626 ,   //69        109
    608 ,   //70        110
    590 ,   //71        111
    573 ,   //72        112
    557 ,   //73        113
    541 ,   //74        114
    525 ,   //75        115
    510 ,   //76        116
    495 ,   //77        117
    481 ,   //78        118
    468 ,   //79        119
    454 ,   //80        120
    441 ,   //81        121
    429 ,   //82        122
    417 ,   //83        123
    405 ,   //84        124
    394 ,   //85        125
    382 ,   //86        126
    372 ,   //87        127
    361 ,   //88        128
    351 ,   //89        129
    342 ,   //90        130
    332 ,   //91        131
    323 ,   //92        132
    314 ,   //93        133
    305 ,   //94        134
    297 ,   //95        135
    289 ,   //96        136
    281 ,   //97        137
    274 ,   //98        138
    266 ,   //99        139
    259 ,   //100       140
    252 ,   //101       141
    246 ,   //102       142
    239 ,   //103       143
    233 ,   //104       144
    227 ,   //105       145
    221 ,   //106       146
    215 ,   //107       147
    209 ,   //108       148
    204 ,   //109       149
    199 ,   //110       150
    194 ,   //111       151
    189 ,   //112       152
    184 ,   //113       153
    179 ,   //114       154
    175 ,   //115       155
    170 ,   //116       156
    166 ,   //117       157
    162 ,   //118       158
    158 ,   //119       159
    154 ,   //120       160
    150 ,   //121       161
    146 ,   //122       162
    143 ,   //123       163
    139 ,   //124       164
    136 ,   //125       165
    133 ,   //126       166
    129 ,   //127       167
    126 ,   //128       168
    123 ,   //129       169
    120 ,   //130       170
    117 ,   //131       171
    114 ,   //132       172
    112 ,   //133       173
    109 ,   //134       174
    106 ,   //135       175
    104 ,   //136       176
    101 ,   //137       177
    99  ,   //138       178
    97  ,   //139       179
    94  ,   //140       180
    92  ,   //141       181
    90  ,   //142       182
    88  ,   //143       183
    86  ,   //144       184
    84  ,   //145       185
    82  ,   //146       186
    80  ,   //147       187
    78  ,   //148       188
    76  ,   //149       189
    75  ,   //150       190
};
void ADC_Rcu_Config(void){
    
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV16);
}

void ADC_Gpio_Config(void){
    //配置 充电检测、电池电压、温度 引脚口初始化
    //RCU_APB2EN &= 0x2U;
    gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_4);
    gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_5);
    gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_6);
}

void ADC_Config(void){
    
    //开启ADC通道
    adc_deinit(ADC0);
    adc_mode_config(ADC_MODE_FREE);
    adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,ENABLE);//ADC开启连续转化模式
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);//adc开启扫描模式
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);//右对齐模式
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL,ADC_CHANNEL_MAX);
    if(adv_type == ADC_CHG_RANK_NUM)
        adc_routine_channel_config(ADC0, 0, ADC_CHG_CHANNEL, ADC_SAMPLETIME_55POINT5);
    else if(adv_type == ADC_BAT_RANK_NUM)
        adc_routine_channel_config(ADC0, 0, ADC_BAT_CHANNEL, ADC_SAMPLETIME_55POINT5);
    else if(adv_type == ADC_TMP_RANK_NUM)
        adc_routine_channel_config(ADC0, 0, ADC_TMP_CHANNEL, ADC_SAMPLETIME_55POINT5);
    adc_external_trigger_source_config(ADC0,ADC_ROUTINE_CHANNEL,ADC0_1_2_EXTTRIG_ROUTINE_NONE);//配置adc出发源
    adc_external_trigger_config(ADC0,ADC_ROUTINE_CHANNEL,ENABLE);
    adc_dma_mode_enable(ADC0);
    adc_enable(ADC0);
    adc_calibration_enable(ADC0);//启动ADC校准
    
}
void ADC_Dma_Config(void){
    //配置ADC的DMA
    dma_parameter_struct dam_parameter_structer;
    dma_deinit(DMA0,DMA_CH0);
    dam_parameter_structer.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0)); // 外设基地址
    dam_parameter_structer.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;   // 外设地址不递增
    dam_parameter_structer.memory_addr  = (uint32_t)(&ADC0_Value);       // 内存基地址
    dam_parameter_structer.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;    // 内存地址递增
    dam_parameter_structer.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;    // 外设数据宽度16位
    dam_parameter_structer.memory_width = DMA_MEMORY_WIDTH_16BIT;        // 内存数据宽度16位
    dam_parameter_structer.direction    = DMA_PERIPHERAL_TO_MEMORY;      // 传输方向：外设→内存
    dam_parameter_structer.number       = ADC_CHANNEL_MAX;               // 传输数量（1个半字）
    dam_parameter_structer.priority     = DMA_PRIORITY_ULTRA_HIGH;       // 最高优先级
    dma_init(DMA0,DMA_CH0,&dam_parameter_structer);
    dma_circulation_disable(DMA0,DMA_CH0);
    dma_memory_to_memory_disable(DMA0,DMA_CH0);
    dma_interrupt_enable(DMA0,DMA_CH0,DMA_INT_FTF);
    nvic_irq_enable(DMA0_Channel0_IRQn,0,0);
    dma_channel_enable(DMA0,DMA_CH0);
    
}
void ADC_Init(void){
    
    ADC_Rcu_Config();//开启时钟
    
    ADC_Gpio_Config();
    
    ADC_Dma_Config();
    
    ADC_Config();
    
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
}

void ADC_Scan(void)
{
    if(ADC0_Flag==1)
    {
        ADC0_Flag=0;
        if(adv_type==ADC_CHG_RANK_NUM)
        {
            adc_chg_vtg=0;
            adc_chg_vtg=ADC0_Value[0];
            //printf("adc_chg_vtg(PA4)=%4.0d,ADC_IN0=%1.4f\r\n",adc_chg_vtg,adc_chg_vtg*3.3f/4096);
        }
        else if(adv_type==ADC_BAT_RANK_NUM)
        {
            adc_bat_vtg=0;
            adc_bat_vtg=ADC0_Value[0];
            //printf("adc_bat_vtg(PA5)=%4.0d,ADC_IN1=%1.4f\r\n",adc_bat_vtg,adc_bat_vtg*3.3f/4096);
        }
        else if(adv_type==ADC_TMP_RANK_NUM)
        {
            adc_tmp_vtg=0;
            adc_tmp_vtg=ADC0_Value[0];
            //printf("adc_tmp_vtg(PA6)=%4.0d,ADC_IN2=%1.4f\r\n",adc_tmp_vtg,adc_tmp_vtg*3.3f/4096);
        }
        
        adv_type++;
        if(adv_type>ADC_TMP_RANK_NUM) adv_type=ADC_CHG_RANK_NUM;
        ADC_Config();
        dma_memory_address_config(DMA0,DMA_CH0,(uint32_t)(ADC0_Value));
        dma_transfer_number_config(DMA0,DMA_CH0,ADC_CHANNEL_MAX);
        dma_channel_enable(DMA0,DMA_CH0);
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
        
    }
}

void DMA0_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH0, DMA_INT_FLAG_FTF)==SET)
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_FLAG_FTF);//清除DMA通道传输完成标志
        dma_channel_disable(DMA0, DMA_CH0);
        ADC0_Flag=1;
    }
}
