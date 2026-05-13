#ifndef __GUI_H
#define __GUI_H

#include "gd32f30x.h"
#include "FreeRtosTask.h"

extern uint16_t mainmenukeyboardNavPointer;
extern uint8_t shutdownDevice;


void Gui_Init(void);
void createMainInterface(void);
void mainMenuButtonChoice(void);
void settingsModeInterface(void);


#endif
