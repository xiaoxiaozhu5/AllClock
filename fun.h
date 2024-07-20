#ifndef __FUN_H__
#define __FUN_H__

unsigned char MainScreen(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char MainMenuSetTime(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char MainMenuSetAlarm(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char MainMenuSetHourBeep(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char MainMenuSetBacklight(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char MenuSetTime(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuSetAlarmSwitch(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuSetAlarmDetail(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuDoSetAlarmSwitch(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuDoSetAlarmDetail(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuDoSetHourBeep(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuSetBacklightAuto(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuSetBacklightManual(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuDoSetBacklightAuto(unsigned char key_up, unsigned char key_down, unsigned char key_enter);
unsigned char SubMenuDoSetBacklightManual(unsigned char key_up, unsigned char key_down, unsigned char key_enter);

#endif