#include "Adc.h"

static uint16_t ADC0_Value[ADC_CHANNEL_MAX]={0};
static uint8_t ADC0_Flag;
static uint16_t adv_type;

uint16_t adc_charge_raw=0;
uint16_t adc_battery_raw=0;
uint16_t adc_temperture_raw=0;

// ========== 滑动平均滤波器全局变量 ==========
uint32_t voltage_buffer[VOLTAGE_WINDOW_SIZE] = {0};
uint8_t  voltage_buffer_index = 0;
uint32_t voltage_sum = 0;

uint32_t temperture_buffer[TEMPERTURE_WINDOW_SIZE] = {0};
uint8_t  temperture_buffer_index = 0;
uint32_t temperture_sum = 0;


uint16_t ntc_adc_table[NTC_ADC_SIZE] = {
    /* -40℃ ~ -31℃ */
    75,   84,   94,   103,  113,  123,  134,  145,  157,  168,
    /* -30℃ ~ -21℃ */
    181,  194,  207,  221,  236,  252,  268,  284,  302,  320,
    /* -20℃ ~ -11℃ */
    338,  358,  378,  399,  421,  444,  468,  493,  518,  545,
    /* -10℃ ~ -1℃ */
    573,  602,  631,  662,  693,  726,  759,  793,  828,  864,
    /* 0℃ ~ 9℃ */
    900,  938,  976,  1015, 1054, 1095, 1136, 1178, 1220, 1263,
    /* 10℃ ~ 19℃ */
    1306, 1349, 1394, 1438, 1483, 1528, 1573, 1619, 1665, 1710,
    /* 20℃ ~ 29℃ */
    1756, 1802, 1847, 1893, 1938, 1983, 2028, 2072, 2117, 2160,
    /* 30℃ ~ 39℃ */
    2204, 2247, 2289, 2332, 2373, 2414, 2454, 2494, 2533, 2572,
    /* 40℃ ~ 49℃ */
    2610, 2647, 2683, 2719, 2754, 2788, 2822, 2855, 2887, 2919,
    /* 50℃ ~ 59℃ */
    2949, 2979, 3008, 3037, 3065, 3092, 3118, 3144, 3169, 3194,
    /* 60℃ ~ 69℃ */
    3218, 3241, 3263, 3286, 3307, 3328, 3348, 3367, 3387, 3405,
    /* 70℃ ~ 79℃ */
    3423, 3441, 3458, 3474, 3490, 3506, 3521, 3536, 3550, 3563,
    /* 80℃ ~ 89℃ */
    3577, 3590, 3602, 3614, 3626, 3637, 3649, 3659, 3670, 3680,
    /* 90℃ ~ 99℃ */
    3689, 3699, 3708, 3717, 3726, 3734, 3742, 3750, 3757, 3765,
    /* 100℃ ~ 109℃ */
    3772, 3779, 3785, 3792, 3798, 3804, 3810, 3816, 3822, 3827,
    /* 110℃ ~ 119℃ */
    3832, 3837, 3842, 3847, 3852, 3856, 3861, 3865, 3869, 3873,
    /* 120℃ ~ 129℃ */
    3877, 3881, 3885, 3888, 3892, 3895, 3898, 3902, 3905, 3908,
    /* 130℃ ~ 139℃ */
    3911, 3914, 3917, 3919, 3922, 3925, 3927, 3930, 3932, 3934,
    /* 140℃ ~ 149℃ */
    3937, 3939, 3941, 3943, 3945, 3947, 3949, 3951, 3953, 3955,
    /* 150℃ */
    3956
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
        adc_routine_channel_config(ADC0, 0, ADC_CHG_CHANNEL, ADC_SAMPLETIME_239POINT5);
    else if(adv_type == ADC_BAT_RANK_NUM)
        adc_routine_channel_config(ADC0, 0, ADC_BAT_CHANNEL, ADC_SAMPLETIME_239POINT5);
    else if(adv_type == ADC_TMP_RANK_NUM)
        adc_routine_channel_config(ADC0, 0, ADC_TMP_CHANNEL, ADC_SAMPLETIME_239POINT5);
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
            adc_temperture_raw=0;
            adc_temperture_raw=ADC0_Value[0];
            //printf("adc_temperture_raw(PA6)=%4.0d,ADC_IN2=%1.4f\r\n",adc_temperture_raw,adc_temperture_raw*3.3f/4096);
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

/**
 * @brief       预填充 ADC 滑动平均滤波器缓冲区（电压和温度）
 * @param       无
 * @retval      无
 * @note        本函数在系统启动阶段调用，预先采集 VOLTAGE_WINDOW_SIZE 次电池电压数据和
 *              TEMPERTURE_WINDOW_SIZE 次 NTC 温度数据，填满对应的滑动平均缓冲区，
 *              避免滤波初始阶段输出不平稳。
 *              
 *              执行流程：
 *              1. 关闭 ADC 连续/扫描模式，设置常规通道长度为 1，禁用 DMA 通道；
 *              2. 预填充电压缓冲区：
 *                 - 循环 VOLTAGE_WINDOW_SIZE 次，采集 ADC_BAT_CHANNEL 通道；
 *                 - 将原始数据转换为毫伏电压值（考虑分压电阻比例 10.1）；
 *                 - 更新滑动平均和电压缓冲区；
 *              3. 预填充温度缓冲区：
 *                 - 循环 TEMPERTURE_WINDOW_SIZE 次，采集 ADC_TMP_CHANNEL 通道；
 *                 - 直接存储原始 ADC 值（后续查表前再进行滑动平均）；
 *                 - 更新滑动平均和温度缓冲区；
 *              4. 恢复 ADC 和 DMA 原始配置（调用 ADC_Init() 和 ADC_Scan()）。
 *              
 *              电压计算公式：
 *              voltage_mv = raw * 1000 * (3.3f / 4095) * (10+91)/10 ≈ raw * 1000 * (3.3/4095) * 10.1
 *              
 *              全局变量依赖：
 *              - voltage_buffer, voltage_sum, voltage_buffer_index
 *              - temperture_buffer, temperture_sum, temperture_buffer_index
 */
void adc_filter_prefill(void)
{
    
    /* 1. 保存并关闭连续/扫描模式，关闭 DMA */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
    dma_channel_disable(DMA0, DMA_CH0);
    
    /* 2. 预填充电压缓冲区 */
    for (int i = 0; i < VOLTAGE_WINDOW_SIZE; i++) {
        adc_routine_channel_config(ADC0, 0, ADC_BAT_CHANNEL, ADC_SAMPLETIME_239POINT5);
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
        while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);
        uint16_t raw_voltage = adc_routine_data_read(ADC0);
        uint32_t raw_voltage_mv = raw_voltage * 1000 * (3.3f / 4095) * (10 + 91) / 10;
        voltage_sum = voltage_sum - voltage_buffer[voltage_buffer_index] + raw_voltage_mv;
        voltage_buffer[voltage_buffer_index] = raw_voltage_mv;
        voltage_buffer_index = (voltage_buffer_index + 1) % VOLTAGE_WINDOW_SIZE;
    }
    
    /* 3. 预填充温度缓冲区 */
    for (int i = 0; i < TEMPERTURE_WINDOW_SIZE; i++) {
        adc_routine_channel_config(ADC0, 0, ADC_TMP_CHANNEL, ADC_SAMPLETIME_239POINT5);
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
        while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);
        uint16_t raw_temperture = adc_routine_data_read(ADC0);
        temperture_sum = temperture_sum - temperture_buffer[temperture_buffer_index] + raw_temperture;
        temperture_buffer[temperture_buffer_index] = raw_temperture;
        temperture_buffer_index = (temperture_buffer_index + 1) % TEMPERTURE_WINDOW_SIZE;
    }
    
    
    ADC_Init();
    ADC_Scan();
}
