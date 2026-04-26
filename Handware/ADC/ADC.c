/*头文件包含区*/
#include "Adc.h"


static uint16_t ADC0_Value[ADC_CHANNEL_MAX]={0};
static uint8_t ADC0_Flag;
static uint16_t adv_type;
uint16_t adc_charge_raw=0;
uint16_t adc_battery_raw=0;
uint16_t adc_temp_raw=0;

// NTC 热敏电阻 ADC 值转温度查找表（-40℃ ~ 150℃，步进1℃）
// ADC值对应 12位分辨率，VREF=3.3V
uint16_t ntc_adc_table[] = {
    /* -40℃ ~ -31℃ */
    3956, 3947, 3937, 3928, 3918, 3908, 3897, 3886, 3874, 3863,
    /* -30℃ ~ -21℃ */
    3850, 3837, 3824, 3810, 3795, 3779, 3763, 3747, 3729, 3711,
    /* -20℃ ~ -11℃ */
    3693, 3673, 3653, 3632, 3610, 3587, 3563, 3538, 3513, 3486,
    /* -10℃ ~ -1℃ */
    3458, 3429, 3400, 3369, 3338, 3305, 3272, 3238, 3203, 3167,
    /* 0℃ ~ 9℃ */
    3131, 3093, 3055, 3016, 2977, 2936, 2895, 2853, 2811, 2768,
    /* 10℃ ~ 19℃ */
    2725, 2682, 2637, 2593, 2548, 2503, 2458, 2412, 2366, 2321,
    /* 20℃ ~ 29℃ */
    2275, 2229, 2184, 2138, 2093, 2048, 2003, 1959, 1914, 1871,
    /* 30℃ ~ 39℃ */
    1827, 1784, 1742, 1699, 1658, 1617, 1577, 1537, 1498, 1459,
    /* 40℃ ~ 49℃ */
    1421, 1384, 1348, 1312, 1277, 1243, 1209, 1176, 1144, 1112,
    /* 50℃ ~ 59℃ */
    1082, 1052, 1023, 994,  966,  939,  913,  887,  862,  837,
    /* 60℃ ~ 69℃ */
    813,  790,  768,  745,  724,  703,  683,  664,  644,  626,
    /* 70℃ ~ 79℃ */
    608,  590,  573,  557,  541,  525,  510,  495,  481,  468,
    /* 80℃ ~ 89℃ */
    454,  441,  429,  417,  405,  394,  382,  372,  361,  351,
    /* 90℃ ~ 99℃ */
    342,  332,  323,  314,  305,  297,  289,  281,  274,  266,
    /* 100℃ ~ 109℃ */
    259,  252,  246,  239,  233,  227,  221,  215,  209,  204,
    /* 110℃ ~ 119℃ */
    199,  194,  189,  184,  179,  175,  170,  166,  162,  158,
    /* 120℃ ~ 129℃ */
    154,  150,  146,  143,  139,  136,  133,  129,  126,  123,
    /* 130℃ ~ 139℃ */
    120,  117,  114,  112,  109,  106,  104,  101,  99,   97,
    /* 140℃ ~ 149℃ */
    94,   92,   90,   88,   86,   84,   82,   80,   78,   76,
    /* 150℃ */
    75
};


/*============================================================================
 * 函数实体区
 *============================================================================*/

/**
 * @brief       ADC 外设时钟配置
 * @param       无
 * @retval      无
 * @note        使能 GPIOA、ADC0、DMA0 时钟，并将 ADC 时钟设置为 CK_APB2 的 16 分频。
 */
void ADC_Rcu_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV16);
}

/**
 * @brief       ADC 模拟输入 GPIO 配置
 * @param       无
 * @retval      无
 * @note        配置 PA4、PA5、PA6 为模拟输入模式，分别用于充电检测、电池电压、温度采样。
 */
void ADC_Gpio_Config(void)
{
    // 配置 充电检测、电池电压、温度 引脚口初始化
    // RCU_APB2EN &= 0x2U;
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_6);
}

/**
 * @brief       ADC DMA 传输配置
 * @param       无
 * @retval      无
 * @note        配置 DMA0 通道0 用于将 ADC0 转换结果传输至内存变量 ADC0_Value。
 *              外设地址固定，内存地址递增，传输数量为 ADC_CHANNEL_MAX，优先级最高。
 *              关闭循环模式，开启传输完成中断。
 */
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

/**
 * @brief       ADC 工作模式配置
 * @param       无
 * @retval      无
 * @note        配置 ADC0 为连续转换+扫描模式，右对齐。
 *              根据全局变量 adv_type 选择当前转换通道（充电/电池/温度），采样时间 55.5 周期。
 *              无外部触发，使能 DMA 模式，启动 ADC 并执行校准。
 */
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

/**
 * @brief       ADC 初始化总函数
 * @param       无
 * @retval      无
 * @note        依次调用时钟配置、GPIO配置、DMA配置、ADC模式配置，最后通过软件触发启动转换。
 *              调用顺序不可随意更改，需保证各模块初始化完成后再使能触发。
 */
void ADC_Init(void)
{
    ADC_Rcu_Config();   // 开启时钟
    
    ADC_Gpio_Config();
    
    ADC_Dma_Config();
    
    ADC_Config();
    
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
}

/**
 * @brief       ADC 扫描轮换处理函数
 * @param       无
 * @retval      无
 * @note        由 DMA 传输完成中断触发（通过 ADC0_Flag 标志）。
 *              读取当前 adv_type 对应的 ADC 转换值（存于 ADC0_Value[0]），
 *              然后轮换到下一个通道类型（充电→电池→温度→循环），
 *              重新配置 ADC 通道、DMA 地址与传输数量，并启动新一轮转换。
 */
void ADC_Scan(void)
{
    if(ADC0_Flag==1)
    {
        ADC0_Flag=0;
        if(adv_type==ADC_CHG_RANK_NUM)
        {
            adc_charge_raw=0;
            adc_charge_raw=ADC0_Value[0];
            //printf("adc_charge_raw(PA4)=%4.0d,ADC_IN0=%1.4f\r\n",adc_charge_raw,adc_charge_raw*3.3f/4096);
        }
        else if(adv_type==ADC_BAT_RANK_NUM)
        {
            adc_battery_raw=0;
            adc_battery_raw=ADC0_Value[0];
            //printf("adc_battery_raw(PA5)=%4.0d,ADC_IN1=%1.4f\r\n",adc_battery_raw,adc_battery_raw*3.3f/4096);
        }
        else if(adv_type==ADC_TMP_RANK_NUM)
        {
            adc_temp_raw=0;
            adc_temp_raw=ADC0_Value[0];
            //printf("adc_temp_raw(PA6)=%4.0d,ADC_IN2=%1.4f\r\n",adc_temp_raw,adc_temp_raw*3.3f/4096);
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

/**
 * @brief       DMA0 通道0 中断服务函数
 * @param       无
 * @retval      无
 * @note        当 DMA 传输完成时触发，清除中断标志并禁用 DMA 通道，
 *              然后设置 ADC0_Flag = 1，通知 ADC_Scan 函数处理数据并启动下一轮转换。
 */
void DMA0_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH0, DMA_INT_FLAG_FTF)==SET)
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_FLAG_FTF);//清除DMA通道传输完成标志
        dma_channel_disable(DMA0, DMA_CH0);
        ADC0_Flag=1;
    }
}
