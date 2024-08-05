#include <STC12C5A60S2.h>

#include "fun.h"
#include "lcd12864.h"
#include "ds1302.h"
#include "beep.h"
#include "backlight.h"
#include "bigclock.h"

#include "delay.h"
#define uchar unsigned char

//key define
sbit keyback = P2^2;	 //D
sbit KEY_1_GND = P0^2;	 //GND
sbit keyenter = P2^7;	 //C
sbit KEY_2_GND = P0^7;	 //GND
sbit keyup = P0^5;	     //B
sbit KEY_3_GND = P2^5;	 //GND
sbit keydown = P0^0;	 //A
sbit KEY_4_GND = P2^0;	 //GND

uchar func_index=0;
//typedef uchar (*func_operation)(uchar key_up, uchar key_down, uchar key_enter);
//func_operation g_current_func = 0;
uchar (*func_operation)(uchar key_up, uchar key_down, uchar key_enter) = 0;
typedef struct
{
	uchar current;
	uchar up;//up index
	uchar down;//down index
	uchar enter;//confirm index
	uchar back;//return index
	//func_operation current_operation;
    uchar (*current_operation)(uchar key_up, uchar key_down, uchar key_enter);
} key_table;
key_table code table[]=
{
    {0,16,15,1,0,(*MainScreen)},//layer 1: show main screen
	{1,4,2,5,0,(*MainMenuSetTime)},//layer 2: main menu
	{2,1,3,6,0,(*MainMenuSetAlarm)},//layer 2: main menu
	{3,2,4,10,0,(*MainMenuSetHourBeep)},//layer 2: main menu
	{4,3,1,11,0,(*MainMenuSetBacklight)},//layer 2: main menu
	{5,5,5,5,1,(*MenuSetTime)},//layer 3: set time
	{6,7,7,8,2,(*SubMenuSetAlarmSwitch)},//layer 3: set alarm
    {7,6,6,9,2,(*SubMenuSetAlarmDetail)},//layer 3: set alarm
    {8,8,8,8,6,(*SubMenuDoSetAlarmSwitch)},//layer 4: alarm detail
	{9,9,9,9,7,(*SubMenuDoSetAlarmDetail)},//layer 4: alarm detail
	{10,10,10,10,3,(*SubMenuDoSetHourBeep)},//layer 5: hour beep
	{11,12,12,13,4,(*SubMenuSetBacklightAuto)},//layer 6: backlight
	{12,11,11,14,4,(*SubMenuSetBacklightManual)},//layer 6: backlight
	{13,13,13,13,11,(*SubMenuDoSetBacklightAuto)},//layer 6: backlight
	{14,14,14,14,12,(*SubMenuDoSetBacklightManual)},//layer 6: backlight
	{15,0,16,1,0,(*MainScreenCalender)},//layer 1: show main screen 2
	{16,15,0,1,0,(*MainScreenBigClock)},//layer 1: show main screen 3
};

void main()
{
    uchar ret = 0;
    uchar last_func_index = 0;
    uchar key_up=0,key_down=0,key_enter=0;
    P3M0 = 0xFF;
	P3M1 = 0x00;
	P1M0 = 0xF7;
	P1M1 = 0x00;
	P0 = 0xFF;
	P2 = 0xFF;
	KEY_1_GND = 0;
	KEY_2_GND = 0;
	KEY_3_GND = 0;
	KEY_4_GND = 0;
	Backlight_init(3);
	PWM_init();
	Init_1302();
	lcm_init();
	PWM1_set(180);
	big_clock_init();

    while(1)
    {
		if((keyup==0)||(keydown==0)||(keyenter==0)||(keyback==0))
		{
			delay(10);
			Beep_key();
			if(keyup==0)
			{
                key_up = 1;
				func_index=table[func_index].up;    //up
				while(!keyup);
			}
			if(keydown==0)
			{
                key_down = 1;
				func_index=table[func_index].down;    //down
				while(!keydown);
			}
			if(keyenter==0)
			{
                key_enter = 1;
				func_index=table[func_index].enter;    //confirm
				while(!keyenter);
			}
			if(keyback==0)
			{
				func_index=table[func_index].back;    //back
				while(!keyback);
			}
		}

		if(func_index == 0)
		{
			func_operation=table[func_index].current_operation;
			ret = (*func_operation)(0,0,0);
			last_func_index = func_index;
		}
		else
		{
			if(last_func_index != func_index)
			{
                lcm_clr();
				func_operation=table[func_index].current_operation;
				ret = (*func_operation)(0, 0, 0);
				last_func_index = func_index;
                key_up = 0;
                key_down = 0;
                key_enter = 0;
			}
            else
            {
                if(ret != 0)
                {
                    func_operation=table[func_index].current_operation;
                    ret = (*func_operation)(key_up, key_down, key_enter);
                    key_up = 0;
                    key_down = 0;
                    key_enter = 0;
                }
            }
        }
    }
}