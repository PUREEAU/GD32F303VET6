#include "Gui.h"
void Gui_Init(void){
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *th = lv_theme_basic_init(disp);
    lv_disp_set_theme(disp, th);
}

static void btn_event_cb(lv_event_t * e){
    lv_obj_t * btn = lv_event_get_target(e);
    int page_id = (int)lv_obj_get_user_data(btn);
    printf("%d\r\n",page_id);
}

void ZHUjiemanshezhi(void){
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, LCD_LENGTH, LCD_WIDTH);   // 480 x 320
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_scr_load(scr);

    lv_obj_t * top_status_bar = lv_obj_create(scr);
    lv_obj_set_style_radius(top_status_bar, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(top_status_bar, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(top_status_bar, 0, LV_STATE_DEFAULT);
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
    char buf[20];
    lv_obj_set_style_text_color(icon_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);
    
    lv_obj_t * bottom_status_bar = lv_obj_create(scr);
    lv_obj_set_style_radius(bottom_status_bar,0,LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(bottom_status_bar,30,LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(bottom_status_bar,65,LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(bottom_status_bar,0,LV_STATE_DEFAULT);
    lv_obj_set_size(bottom_status_bar,LCD_LENGTH,280);
    lv_obj_align(bottom_status_bar,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_set_style_bg_color(bottom_status_bar,lv_color_hex(0x1E2B24),LV_STATE_DEFAULT);

    lv_obj_set_layout(bottom_status_bar,LV_LAYOUT_GRID);
    #define BTN_COUNT  9
    #define ROW_HEIGHT 50
    #define COL_NUM    5
    static lv_coord_t col_dsc[] = {
        LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
        LV_GRID_TEMPLATE_LAST
    };
    static lv_coord_t row_dsc[] = {ROW_HEIGHT, ROW_HEIGHT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(bottom_status_bar, col_dsc, row_dsc);
    lv_obj_set_style_pad_row(bottom_status_bar,30, 0);
    lv_obj_set_style_pad_column(bottom_status_bar,30,0);

    lv_group_t *group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);

    for (uint8_t i = 0; i < BTN_COUNT; i++) {

        lv_obj_t * btn = lv_btn_create(bottom_status_bar);
        lv_obj_set_style_radius(btn, 10, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xD2E0CF), LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn, 0, 0);
        lv_obj_set_style_border_width(btn, 4, LV_STATE_FOCUSED);
        lv_obj_set_style_border_color(btn, lv_color_hex(0x3A6B47), LV_STATE_FOCUSED);
        lv_obj_set_style_outline_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_outline_width(btn, 5, LV_STATE_PRESSED);
        lv_obj_set_style_outline_color(btn, lv_color_hex(0x3A6B47), LV_STATE_FOCUSED);
        lv_obj_set_style_text_font(btn,&lv_font_montserrat_14,LV_STATE_DEFAULT);
        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(btn,btn_event_cb,LV_EVENT_CLICKED,NULL);

        lv_obj_t * label = lv_label_create(btn);
        char text[10];
        sprintf(text,"%d",i);
        lv_label_set_text(label,text);
        lv_obj_center(label);

        lv_group_add_obj(group, btn);

        lv_obj_set_grid_cell(btn, 
            LV_GRID_ALIGN_STRETCH, i % COL_NUM, 1,
            LV_GRID_ALIGN_STRETCH, i / COL_NUM, 1);

    }


    while (1)
    {
        sprintf(buf, "%.1f%%", battery_voltage_soc);
        lv_label_set_text(icon_label, buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
