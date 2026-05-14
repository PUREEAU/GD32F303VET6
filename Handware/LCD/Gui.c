#include "Gui.h"
void Gui_Init(void){
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *th = lv_theme_basic_init(disp);
    lv_disp_set_theme(disp, th);
}

lv_obj_t * backgroundScreen = NULL;
lv_obj_t * top_status_bar = NULL;
lv_obj_t * bottom_status_bar = NULL;
lv_obj_t * icon_label = NULL;
uint16_t mainmenukeyboardNavPointer = 0;
uint8_t shutdownDevice = 0;

static void mainMenuButtonChonice_event_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);
    mainmenukeyboardNavPointer = (uint16_t)(uintptr_t)lv_obj_get_user_data(btn) + 1;
    // printf("%d",mainmenukeyboardNavPointer);
}

void createMainInterface(void){
    backgroundScreen = lv_obj_create(NULL);
    lv_obj_set_size(backgroundScreen, LCD_LENGTH, LCD_WIDTH);
    lv_obj_set_style_bg_color(backgroundScreen, lv_color_hex(0x000000), 0);
    lv_scr_load(backgroundScreen);

    top_status_bar = lv_obj_create(backgroundScreen);
    // lv_obj_set_style_radius(top_status_bar, 0, LV_STATE_DEFAULT);    // 设置圆角
    // lv_obj_set_style_pad_all(top_status_bar, 0, LV_STATE_DEFAULT);  // 清除所有内边距
    // lv_obj_set_style_border_width(top_status_bar, 0, LV_STATE_DEFAULT);// 清除边框宽度
    lv_obj_set_size(top_status_bar, LCD_LENGTH, 40);
    lv_obj_align(top_status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(top_status_bar, lv_color_hex(0x5E8B6F), LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(top_status_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_status_bar, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t * name_label = lv_label_create(top_status_bar);
    lv_label_set_text(name_label, "EAU");
    lv_obj_set_style_text_color(name_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);
    lv_obj_t * battery_label = lv_label_create(top_status_bar);
    lv_label_set_text(battery_label, "2026.5");
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);
    lv_obj_t * icon_label = lv_label_create(top_status_bar);
    lv_obj_set_style_text_color(icon_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);

    bottom_status_bar = lv_obj_create(backgroundScreen);
    lv_obj_set_size(bottom_status_bar,LCD_LENGTH,280);
    lv_obj_align(bottom_status_bar,LV_ALIGN_BOTTOM_MID,0,0);
    // lv_obj_set_style_radius(bottom_status_bar,0,LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(bottom_status_bar,0,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(bottom_status_bar,lv_color_hex(0x1E2B24),LV_STATE_DEFAULT);

}
char buf[16];
void mainMenuButtonChoice(void){

    if(bottom_status_bar != NULL){
        lv_obj_clean(bottom_status_bar);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    lv_obj_set_style_pad_hor(bottom_status_bar,30,LV_STATE_DEFAULT);// 设置水平内边距
    lv_obj_set_style_pad_ver(bottom_status_bar,65,LV_STATE_DEFAULT);// 设置垂直内边距

    lv_obj_set_layout(bottom_status_bar,LV_LAYOUT_GRID);
    static uint8_t btn_count = 9;
    static uint8_t col_num = 5;
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {50, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(bottom_status_bar, col_dsc, row_dsc);
    lv_obj_set_style_pad_row(bottom_status_bar,30, LV_STATE_DEFAULT);// 设置网格行间距
    lv_obj_set_style_pad_column(bottom_status_bar,30,LV_STATE_DEFAULT);// 设置网格列间距

    lv_group_t *group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
    for (uint8_t i = 0; i < btn_count; i++) {

        lv_obj_t * btn = lv_btn_create(bottom_status_bar);
        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        // lv_obj_set_style_shadow_width(btn, 0, 0);//设置阴影
        lv_obj_set_style_radius(btn, 10, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xD2E0CF), LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 4, LV_STATE_FOCUSED);
        lv_obj_set_style_border_color(btn, lv_color_hex(0x3A6B47), LV_STATE_FOCUSED);
        lv_obj_set_style_outline_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_outline_width(btn, 5, LV_STATE_PRESSED);
        lv_obj_set_style_outline_color(btn, lv_color_hex(0x3A6B47), LV_STATE_FOCUSED);
        // lv_obj_set_style_text_font(btn,&lv_font_montserrat_14,LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn,mainMenuButtonChonice_event_cb,LV_EVENT_CLICKED,NULL);
        
        lv_obj_t * label = lv_label_create(btn);
        char text[10];
        sprintf(text,"%d",i);
        lv_label_set_text(label,text);
        lv_obj_center(label);

        lv_group_add_obj(group, btn);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % col_num, 1,LV_GRID_ALIGN_STRETCH, i / col_num, 1);

    }


}

//    while (1)
//    {
//        sprintf(buf, "%.1f%%", battery_voltage_soc);
//        lv_label_set_text(icon_label, buf);
//        vTaskDelay(pdMS_TO_TICKS(1000));
//    }
lv_obj_t * Screen_BrightnessSlider = NULL;
lv_obj_t * ScreenSlider_ContentLabel = NULL;
static void settingMode_TabViewanim_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
        lv_anim_t * a = lv_event_get_param(e);
        if(a)  a->time = 0;
    }
}
static void button_event_setting_tab1(lv_event_t* e)
{
    // lv_obj_t* current_btn = lv_event_get_current_target(event);
    // uint32_t current_btn_index = lv_obj_get_index(current_btn);
    if(lv_event_get_code(e) == LV_EVENT_KEY) {
    uint8_t key=lv_event_get_key(e);
        if(key == LV_KEY_ESC)
        {
            mainmenukeyboardNavPointer = 0;
        }
    }

    // if(event->code==LV_EVENT_RELEASED)
    // {  
	// 		if(current_btn_index==9)
	// 		{
	// 			mainmenukeyboardNavPointer=0;	
	// 			// comm_mode=comm_debug;
	// 			// m_funiot_setting_refresh=0;
	// 		}
	// 		else if(current_btn_index==1)//hello.txt按钮
	// 		{
    //             mainmenukeyboardNavPointer=0;
	// 		}
	// 		else if(current_btn_index==2)//bin按钮
	// 		{
	// 			mainmenukeyboardNavPointer=0;
	// 		}
    // }
}

static void settingScreenslider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value=lv_slider_get_value(slider);
    lv_slider_set_value(slider,value,LV_ANIM_ON);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", value);
    lv_label_set_text(ScreenSlider_ContentLabel, buf);
    lcd_Screen_Set(value);

}
void settingsModeInterface(void){

    if(bottom_status_bar != NULL){
        lv_obj_clean(bottom_status_bar);
    }
    lv_obj_set_style_pad_hor(bottom_status_bar,0,LV_STATE_DEFAULT);// 设置水平内边距
    lv_obj_set_style_pad_ver(bottom_status_bar,0,LV_STATE_DEFAULT);// 设置垂直内边距

    lv_obj_t * settingMode_TabView = lv_tabview_create(bottom_status_bar,LV_DIR_LEFT,96);
    lv_obj_t * settingMode_TabView_TabBtn = lv_tabview_get_tab_btns(settingMode_TabView);
    lv_obj_add_event_cb(lv_tabview_get_content(settingMode_TabView), settingMode_TabViewanim_event_cb, LV_EVENT_SCROLL_BEGIN, NULL);
    lv_obj_add_event_cb(settingMode_TabView_TabBtn, button_event_setting_tab1, LV_EVENT_KEY, NULL);
    lv_group_t * tab_group = lv_group_create();
    lv_indev_set_group(indev_keypad,tab_group);
    lv_group_add_obj(tab_group,settingMode_TabView_TabBtn);
    

    lv_obj_set_style_bg_color(settingMode_TabView_TabBtn, lv_color_hex(0xD2E0CF), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(settingMode_TabView_TabBtn, lv_color_hex(0xD2E0CF), LV_PART_ITEMS | LV_STATE_PRESSED);
    
    lv_obj_set_style_bg_color(settingMode_TabView_TabBtn, lv_color_hex(0x81C784), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(settingMode_TabView_TabBtn,3,LV_PART_ITEMS | LV_STATE_CHECKED);
    
    lv_obj_set_style_border_side(settingMode_TabView_TabBtn,LV_BORDER_SIDE_RIGHT,LV_PART_ITEMS | LV_STATE_CHECKED);
    
    lv_obj_set_style_border_color(settingMode_TabView_TabBtn, lv_color_hex(0x2E8B57), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(settingMode_TabView_TabBtn,3,LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(settingMode_TabView_TabBtn,LV_BORDER_SIDE_RIGHT,LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(settingMode_TabView_TabBtn, lv_color_hex(0x2E8B57), LV_PART_ITEMS | LV_STATE_FOCUSED);

    lv_obj_t * About_tab = lv_tabview_add_tab(settingMode_TabView,"About");
    lv_obj_set_style_bg_color(About_tab, lv_color_hex(0xdee2dd), LV_STATE_DEFAULT);
    lv_obj_t * Status_tab = lv_tabview_add_tab(settingMode_TabView,"Status");
    lv_obj_set_style_bg_color(Status_tab, lv_color_hex(0xdee2dd), LV_STATE_DEFAULT);
    lv_obj_t * Update_tab = lv_tabview_add_tab(settingMode_TabView,"Update");
    lv_obj_set_style_bg_color(Update_tab, lv_color_hex(0xdee2dd), LV_STATE_DEFAULT);
    lv_obj_t * Screen_tab = lv_tabview_add_tab(settingMode_TabView,"Screen");
    lv_obj_set_style_bg_color(Screen_tab, lv_color_hex(0xdee2dd), LV_STATE_DEFAULT);
    char Setting_ContentLabel_text[256];

    lv_obj_t * About_Titlelabel = lv_label_create(About_tab);
    lv_label_set_recolor(About_Titlelabel, true);
    lv_label_set_text(About_Titlelabel,"#0000FF PureEau  # #007000 Controller  # #FF0000 OS#");
    lv_obj_align(About_Titlelabel, LV_ALIGN_TOP_MID,0,10);
    lv_obj_t * About_ContentLabel = lv_label_create(About_tab);
    lv_obj_set_pos(About_ContentLabel, 5, 40);
    snprintf(Setting_ContentLabel_text, sizeof(Setting_ContentLabel_text),
        "SoftWare: V1.0\t\t\t\tBootloader: V1.0\n"
        "HandWare: V1.0\n"
        "Device SN: 0000000001\n"
        "System Install Time: 2026/05/13 14:42\n"
        "System Info: FreeRTOS_V11.1+LVGL_V%d.%d\n"
        "MCU Info: GD32F303VET6, 512K Flash + 64K SRAM\n"
        "GitHub: https://github.com/PUREEAU",
        LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR);
    lv_label_set_text(About_ContentLabel, Setting_ContentLabel_text);

    lv_obj_t * Status_Titlelabel = lv_label_create(Status_tab);
    lv_label_set_recolor(Status_Titlelabel, true);
    lv_label_set_text(Status_Titlelabel,"#0000FF PureEau  # #007000 Controller  # #FF0000 Status#");
    lv_obj_align(Status_Titlelabel, LV_ALIGN_TOP_MID,0,10);
    lv_obj_t * Status_ContentLabel = lv_label_create(Status_tab);
    lv_obj_set_pos(Status_ContentLabel, 5, 40);
    snprintf(Setting_ContentLabel_text, sizeof(Setting_ContentLabel_text),
        "Battery_level: %.0f%%\t\t\t\tBattery_vtg: %d mV\n"
        "Charging Port Vtg: %d mV\n"
        "Temperature: %d °C\n"
        "Total Running time: 296 min\n"
        "Total startup times: 202\n",
        battery_voltage_soc, battery_voltage_mv,
        charge_voltage_mv, 
        Temperture_voltage);
    lv_label_set_text(Status_ContentLabel, Setting_ContentLabel_text);

    lv_obj_t * Screen_Titlelabel = lv_label_create(Screen_tab);
    lv_label_set_recolor(Screen_Titlelabel, true);
    lv_label_set_text(Screen_Titlelabel,"#0000FF PureEau  # #007000 Screen  # #FF0000 Setting#");
    lv_obj_align(Screen_Titlelabel, LV_ALIGN_TOP_MID,0,10);
    lv_obj_t * Screen_ContentLabel = lv_label_create(Screen_tab);
    lv_obj_set_pos(Screen_ContentLabel, 5, 40);
    snprintf(Setting_ContentLabel_text, sizeof(Setting_ContentLabel_text),
        "#FF0000 Set screen background brightness #");
    lv_label_set_recolor(Screen_ContentLabel, true);
    lv_label_set_text(Screen_ContentLabel, Setting_ContentLabel_text);

    static lv_style_t Screen_BrightnessSlider_main;
    static lv_style_t Screen_BrightnessSlider_indicator;
    static lv_style_t Screen_BrightnessSlider_knob;
    lv_style_init(&Screen_BrightnessSlider_main);
    lv_style_set_bg_color(&Screen_BrightnessSlider_main, lv_color_hex(0x6E8F79));
    lv_style_set_radius(&Screen_BrightnessSlider_main, LV_RADIUS_CIRCLE);
    lv_style_init(&Screen_BrightnessSlider_indicator);
    lv_style_set_bg_color(&Screen_BrightnessSlider_indicator, lv_color_hex(0x597E68));
    lv_style_set_radius(&Screen_BrightnessSlider_indicator, LV_RADIUS_CIRCLE);
    lv_style_init(&Screen_BrightnessSlider_knob);
    lv_style_set_bg_color(&Screen_BrightnessSlider_knob, lv_color_hex(0x4A6B5A));
    lv_style_set_radius(&Screen_BrightnessSlider_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&Screen_BrightnessSlider_knob, 4); 
    Screen_BrightnessSlider = lv_slider_create(Screen_tab);
    lv_obj_add_style(Screen_BrightnessSlider, &Screen_BrightnessSlider_main, LV_PART_MAIN);
    lv_obj_add_style(Screen_BrightnessSlider, &Screen_BrightnessSlider_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(Screen_BrightnessSlider, &Screen_BrightnessSlider_knob, LV_PART_KNOB);
    lv_obj_set_pos(Screen_BrightnessSlider, 22, 100);
    lv_obj_set_size(Screen_BrightnessSlider, 340, 20);
    lv_obj_set_style_outline_width(Screen_BrightnessSlider, 4, LV_PART_INDICATOR | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_color(Screen_BrightnessSlider, lv_color_hex(0xB5FFD0), LV_PART_INDICATOR | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(Screen_BrightnessSlider, LV_OPA_70, LV_PART_INDICATOR | LV_STATE_FOCUSED);
    lv_slider_set_range(Screen_BrightnessSlider, 0, 100);
    lv_slider_set_mode(Screen_BrightnessSlider, LV_SLIDER_MODE_NORMAL);
    lv_obj_add_flag(Screen_BrightnessSlider, LV_OBJ_FLAG_ADV_HITTEST);
    lv_slider_set_value(Screen_BrightnessSlider, 100, LV_ANIM_ON);
    ScreenSlider_ContentLabel = lv_label_create(Screen_tab);
    lv_obj_align(ScreenSlider_ContentLabel, LV_ALIGN_TOP_MID,0,130);
    lv_label_set_text(ScreenSlider_ContentLabel, "100 %");
    lv_obj_add_event_cb(Screen_BrightnessSlider, settingScreenslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_group_add_obj(tab_group, Screen_BrightnessSlider);

    static lv_style_t settingText_LineSpace;
    lv_style_init(&settingText_LineSpace);
    lv_style_set_text_line_space(&settingText_LineSpace, 15); 
    lv_obj_add_style(About_ContentLabel, &settingText_LineSpace, 0);
    lv_obj_add_style(Status_ContentLabel, &settingText_LineSpace, 0);
    lv_obj_add_style(Screen_ContentLabel, &settingText_LineSpace, 0);

}
