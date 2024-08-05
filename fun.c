#include "fun.h"

#include "ds1302.h"
#include "lcd12864.h"
#include "beep.h"
#include "backlight.h"
#include "calender.h"
#include "bigclock.h"

#include "delay.h"

unsigned char g_sec = 0;
unsigned char v = 0;
bit BEEP_bit = 0;

extern unsigned char yy,mo,dd,xq,hh,mm,ss;

unsigned char DSS (void);

void update(void)
{
	unsigned char i = 0,m = 0;
	read_clockS();
	lcm_w_test(0,0x88);
	lcm_w_word("20");
	lcm_w_test(1,yy/16+0x30);
	lcm_w_test(1,yy%16+0x30);
	lcm_w_word("-");
	lcm_w_test(1,mo/16+0x30);
	lcm_w_test(1,mo%16+0x30);
	lcm_w_word("-");
	lcm_w_test(1,dd/16+0x30);
	lcm_w_test(1,dd%16+0x30);
	lcm_w_word(" ");
	lcm_w_test(1,hh/16+0x30);
	lcm_w_test(1,hh%16+0x30);
	m = read_clock(0x81)&0x01;//:
	if(m == 1){
		lcm_w_word(":");
	}else{
		lcm_w_word(" ");
	}
	lcm_w_test(1,mm/16+0x30);
	lcm_w_test(1,mm%16+0x30);

	lcm_w_test(0,0x98);
	lcm_w_test(1,0xd0);lcm_w_test(1,0xc7);//week
	lcm_w_test(1,0xc6);lcm_w_test(1,0xda);
	if(xq==7)  {lcm_w_test(1,0xc8);lcm_w_test(1,0xd5);}//sunday
    if(xq==6)  {lcm_w_test(1,0xc1);lcm_w_test(1,0xf9);}//sartday
    if(xq==5)  {lcm_w_test(1,0xce);lcm_w_test(1,0xe5);}//friday
    if(xq==4)  {lcm_w_test(1,0xcb);lcm_w_test(1,0xc4);}//thursday
    if(xq==3)  {lcm_w_test(1,0xc8);lcm_w_test(1,0xfd);}//wedsday
    if(xq==2)  {lcm_w_test(1,0xb6);lcm_w_test(1,0xfe);}//tuesday
    if(xq==1)  {lcm_w_test(1,0xd2);lcm_w_test(1,0xbb);}//monday

	i = DSS();
	i-=2;
	if(i < 60)
	{
		lcm_w_test(0,0x9C);
		lcm_w_test(1,0xce);lcm_w_test(1,0xc2);//degrees centigrade
		lcm_w_test(1,0xb6);lcm_w_test(1,0xc8);
		lcm_w_test(1,i/10+0x30);
		lcm_w_test(1,i%10+0x30);
		lcm_w_test(1,0xa1);lcm_w_test(1,0xe6);//degrees centigrade symbol
	}
}

void Set_time(unsigned char sel,unsigned char a,unsigned char up,unsigned char down)
{
    signed char address = 0,item = 0;
    signed char max = 0,mini = 0;
    lcm_w_test(0,0x83);
    lcm_w_word("Adjust");
    lcm_w_test(0,0x92);
    lcm_w_word("         ");
    lcm_w_test(0,0x92);
    //if(sel==6)  {lcm_w_word("sec");address=0x80; max=0;mini=0;}     //sec 7
    if(sel==5)  {lcm_w_word("Min");address=0x82; max=59;mini=0;}    //min 6
    if(sel==4)  {lcm_w_word("Hour");address=0x84; max=23;mini=0;}    //hour 5
    if(sel==3)  {lcm_w_word("Week");address=0x8a; max=7;mini=1;}    //week 4
    if(sel==2)  {lcm_w_word("Day");address=0x86; max=31;mini=1;}    //day 3
    if(sel==1)  {lcm_w_word("Month");address=0x88; max=12;mini=1;}    //month 2
    if(sel==0)  {lcm_w_word("Year");address=0x8c; max=99; mini=0;}    //year 1
    if(sel==200)  {lcm_w_word("Mode");address=0xc2+a*6; max=6;mini=0;}    //MODE
    if(sel==201)  {lcm_w_word("Hour");address=0xc2+a*6+2; max=23;mini=0;}    //hour
    if(sel==202)  {lcm_w_word("Min");address=0xc2+a*6+4; max=59;mini=0;}    //min
    if(sel==40)  {lcm_w_word("   Beep");address=0xc2; max=9;mini=0;}
    if(sel==50)  {lcm_w_word("Backlight");address=0xc4; max=9;mini=0;}
    if(sel==230)  {lcm_w_word("Callbrate");address=0xFC; max=20;mini=0;}

    item=((read_clock(address+1))/16)*10 + (read_clock(address+1))%16;
    if(up == 1)
    {
        item++;
    }
    if(down == 1)
    {
        item--;
    }
    if(item>max) item=mini;
    if(item<mini) item=max;
    write_clock(0x8e,0x00);//enable write
    write_clock(address,(item/10)*16+item%10);//translate to hex
    if(sel==5)
    {
        write_clock(0x80,0x00);//sec
    }
    write_clock(0x8e,0x80);//disable write protect
}

void alarm_update(unsigned char index)
{
	lcm_w_test(0,0x8B);//mode
	lcm_w_test(1,index+0x30);//alarm index
	lcm_w_test(0,0x8D);//hour
	lcm_w_test(1,read_clock(0xc3+index*6+2)/16+0x30);//hour
	lcm_w_test(1,read_clock(0xc3+index*6+2)%16+0x30);
	lcm_w_word(":");
	lcm_w_test(1,read_clock(0xc3+index*6+4)/16+0x30);//min
	lcm_w_test(1,read_clock(0xc3+index*6+4)%16+0x30);
	lcm_w_test(0,0x98);
	switch (read_clock(0xc3+index*6))
	{
		case 0:
        lcm_w_word("Always turn off");
		break;
		case 1:
        lcm_w_word("Alarm once");
		break;
		case 2:
        lcm_w_word("Always turn on");
		break;
		case 3:
        lcm_w_word("Mon to Fri on");
		break;
		case 4:
        lcm_w_word("Mon to Sart on");
		break;
		case 5:
        lcm_w_word("Sart and Sun on");
		break;
		case 6:
        lcm_w_word("Only Sun on");
		break;
		default:
		break;
	}
}

void alarm_check(void)
{
	unsigned char i = 0;
	write_clock(0x8e,0x00);//enable write
	for(i=1;i<7;i++)
	{
		if(read_clock(0xc3+i*6) != 0 &&	//is set
           read_clock(0xc3+i*6+2) == read_clock(0x85) && //hour compare
           read_clock(0xc3+i*6+4) == read_clock(0x83)&& //min compare
           read_clock(0x81) < 0x09 && //sec < 8s
           v != read_clock(0x83))
        {
            switch (read_clock(0xc3+i*6))
            {
                case 1://once
                BEEP_bit = 1;
                write_clock(0xc2+i*6,0);
                break;
                case 2://always on
                BEEP_bit = 1;
                break;
                case 3://except sartday and sunday
                if(read_clock(0x8b) < 6)
                {
                    BEEP_bit = 1;
                }
                break;
                case 4://except sunday
                if(read_clock(0x8b) < 7)
                {
                    BEEP_bit = 1;
                }
                break;
                case 5://sartday and sunday
                if(read_clock(0x8b) > 5)
                {
                    BEEP_bit = 1;
                }
                break;
                case 6://sunday
                if(read_clock(0x8b) == 7)
                {
                    BEEP_bit = 1;
                }
                break;
			}
			v = read_clock(0x83);
		}
	}
	write_clock(0x8e,0x80);//disable write
}

void hour_check(void)
{
	unsigned char i = 0;
	if(read_clock(0x81)==0 && read_clock(0x83)==0 && read_clock(0xc7)==0)
	{
		i = read_clock(0x85);
		switch (read_clock(0xc3))
		{
			case 1:
            if(i >= 0x06 && i <= 0x20)
            {
                Beep_bs();
            }
            break;
			case 2:
            if(i >= 0x06 && i <= 0x23)
            {
                Beep_bs();
            }
            break;
			case 3:
            if(i >= 0x07 && i <= 0x20)
            {
                Beep_bs();
            }
            break;
			case 4:
            if(i >= 0x07 && i <= 0x23)
            {
                Beep_bs();
            }
            break;
			case 5:
            if(i >= 0x08 && i <= 0x20)
            {
                Beep_bs();
            }
            break;
			case 6:
            if(i >= 0x08 && i <= 0x23)
            {
                Beep_bs();
            }
            break;
			case 7:
            if(i >= 0x09 && i <= 0x20)
            {
                Beep_bs();
            }
            break;
			case 8:
            if(i >= 0x09 && i <= 0x23)
            {
                Beep_bs();
            }
            break;
			case 9:
            if(i >= 0x00 && i <= 0x23)
            {
                Beep_bs();
            }
            break;
			default:
            break;
		}
		update();
	}
}

void Backlight_auto_ctrl()
{
	unsigned char d = 0;
	d = Backlight_read();
	if(read_clock(0xc1) == 1)
    {
		if(d > 0xD0)
        {
			PWM1_set(1);
		}
        else
        {
			PWM1_set(read_clock(0xc5)*0x1C);
		}
	}
}

/**************************************layer 1: main screen****************************************/
unsigned char MainScreen(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	alarm_check();
	hour_check();
	if(read_clock(0x81) != g_sec)
	{
        lcm_clr();
		g_sec = read_clock(0x81);
		update();
		Backlight_auto_ctrl();
	}
    return 0;
}
unsigned char MainScreenCalender(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	LCD_EnableGraphics();
	LCD_ClearGraphics();
	show_calender(2000+(yy/16)*10+(yy%16),mo,(dd/16)*10+dd%16);
	LCD_DisableGraphics();
	return 0;
}
unsigned char MainScreenBigClock(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	LCD_EnableGraphics();
	LCD_ClearGraphics();
	show_big_block(hh, mm);
	LCD_DisableGraphics();
	return 0;
}
/**************************************layer 2: main menu [Set time] "Set alarm" "Set hour" "Set backlight"***************************************/
unsigned char MainMenuSetTime(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Set time");
	display_12864(2,0,"Set alarm");
	display_12864(3,0,"Set hour");
	display_12864(4,0,"Set backlight");
	reverse_show(0x80,0x80,8);
    return 0;
}
/**************************************layer 2: main menu "Set time" [Set alarm] "Set hour" "Set backlight"***************************************/
unsigned char MainMenuSetAlarm(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Set time");
	display_12864(2,0,"Set alarm");
	display_12864(3,0,"Set hour");
	display_12864(4,0,"Set backlight");
	reverse_show(0x90,0x90,8);
    return 0;
}
/**************************************layer 2: main menu "Set time" "Set alarm" [Set hour] "Set backlight"***************************************/
unsigned char MainMenuSetHourBeep(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Set time");
	display_12864(2,0,"Set alarm");
	display_12864(3,0,"Set hour");
	display_12864(4,0,"Set backlight");
	reverse_show(0x88,0x80,8);
    return 0;
}
/**************************************layer 2: main menu "Set time" "Set alarm" "Set hour" [Set backlight]***************************************/
unsigned char MainMenuSetBacklight(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Set time");
	display_12864(2,0,"Set alarm");
	display_12864(3,0,"Set hour");
	display_12864(4,0,"Set backlight");
	reverse_show(0x98,0x90,8);
    return 0;
}
/**************************************layer 3: Set time ***************************************/
unsigned char MenuSetTime(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    static char set_item = 0;
    if(key_enter)
    {
        set_item++;
    }
    else
    {
        Set_time(set_item,0,key_up,key_down);
    }
    update();
    if(set_item==6)
    {
        set_item = 0;
    }
    return 1;
}
/**************************************layer 3: Set alarm ***************************************/
unsigned char SubMenuSetAlarmSwitch(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    display_12864(1,0,"Alarm switch");
    display_12864(2,0,"Alarm detail");
	reverse_show(0x80,0x80,8);
    return 0;
}
/**************************************layer 4: Alarm detail***************************************/
unsigned char SubMenuSetAlarmDetail(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    display_12864(1,0,"Alarm switch");
    display_12864(2,0,"Alarm detail");
	reverse_show(0x90,0x90,8);
    return 0;
}
/**************************************layer 4: Alarm switch***************************************/
unsigned char SubMenuDoSetAlarmSwitch(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    display_12864(2,1,"Main switch");
    if(key_enter)
    {
        write_clock(0x8e,0x00);//enable write
        if(read_clock(0xc7)==1)
        {
            write_clock(0xc6,0);
        }
        else
        {
            write_clock(0xc6,1);
        }
        write_clock(0x8e,0x80);//disable write
    }
    if(read_clock(0xc7)==1)
    {
        display_12864(3,3," ON ");
    }
    else
    {
        display_12864(3,3," OFF");
    }
    return 1;
}
/**************************************layer 4: Alarm detail***************************************/
unsigned char SubMenuDoSetAlarmDetail(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    static unsigned char alarm_index = 1;
    static unsigned char alarm_mode = 199;
    static unsigned char edit_mode = 0;
    display_12864(3,0,"Alarm ");
    if(key_enter)
    {
        edit_mode = 1;
        Set_time(alarm_mode++, alarm_index, key_up, key_down);
        if(alarm_mode == 203)
        {
            alarm_mode = 199;
            edit_mode = 0;
            display_12864(2,0,"        ");
        }
    }
    if(edit_mode)
    {
        Set_time(alarm_mode, alarm_index, key_up, key_down);
    }
    else
    {
        if(key_up)
        {
            alarm_index++;
            if(alarm_index > 6)
                alarm_index = 1;
        }
        if(key_down)
        {
            alarm_index--;
            if(alarm_index < 1)
                alarm_index = 6;
        }
    }
    alarm_update(alarm_index);
    return 1;
}
/**************************************layer 5: hour beep***************************************/
unsigned char SubMenuDoSetHourBeep(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    Set_time(40,0,key_up,key_down);
    switch(read_clock(0xc3))
    {
        case 0:
        display_12864(4,0,"      OFF       ");
        break;
        case 1:
        display_12864(4,0,"  06:00--20:00  ");
        break;
        case 2:
        display_12864(4,0,"  06:00--23:00  ");
        break;
        case 3:
        display_12864(4,0,"  07:00--20:00  ");
        break;
        case 4:
        display_12864(4,0,"  07:00--23:00  ");
        break;
        case 5:
        display_12864(4,0,"  08:00--20:00  ");
        break;
        case 6:
        display_12864(4,0,"  08:00--23:00  ");
        break;
        case 7:
        display_12864(4,0,"  09:00--20:00  ");
        break;
        case 8:
        display_12864(4,0,"  09:00--23:00  ");
        break;
        case 9:
        display_12864(4,0,"  00:00--23:00  ");
        break;
        default:
        break;
    }

    return 1;
}
/**************************************layer 5: backlight***************************************/
unsigned char SubMenuSetBacklightAuto(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Auto Ctrl");
	display_12864(2,0,"Manual Ctrl");
	reverse_show(0x80,0x80,8);
    return 0;
}
/**************************************layer 5: backlight***************************************/
unsigned char SubMenuSetBacklightManual(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
	display_12864(1,0,"Auto Ctrl");
	display_12864(2,0,"Manual Ctrl");
	reverse_show(0x90,0x90,8);
    return 0;
}
/**************************************layer 5: backlight***************************************/
unsigned char SubMenuDoSetBacklightAuto(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    display_12864(2,1,"Auto Ctrl");
    if(key_enter)
    {
        write_clock(0x8e,0x00);//enable write
        if(read_clock(0xc1)==1)
        {
            write_clock(0xc0,0);
        }
        else
        {
            write_clock(0xc0,1);
        }
        write_clock(0x8e,0x80);//disable write
    }
    if(read_clock(0xc1)==1)
    {
        display_12864(3,3," ON ");
    }
    else
    {
        display_12864(3,3," OFF");
    }
    return 1;
}
/**************************************layer 5: backlight***************************************/
unsigned char SubMenuDoSetBacklightManual(unsigned char key_up, unsigned char key_down, unsigned char key_enter)
{
    Set_time(50,0,key_up,key_down);
    lcm_w_test(0,0x9C);
    lcm_w_test(1,read_clock(0xc5)+0x30);
    PWM1_set(read_clock(0xc5)*0x1C);
    return 1;
}
