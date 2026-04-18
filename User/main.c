#include "FreeRtosTask.h"

void Diver_Init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

}

int main(void)
{
    systick_config();

    Diver_Init();

    vTaskSchedule();

    while (1);
}

