#include "beep.h"
#include <STC12C5A60S2.h>

#include "delay.h"

sbit beep	=	P1 ^ 0;

void Beep(void)
{ 
	unsigned char a;
	for(a=60;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(600);
	}
	for(a=100;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(480);//
	}
	for(a=100;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(180);
	}
	beep = 1;
}

void Beep_set(void)
{
	unsigned char a;
	for(a=50;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(600);
	}
	for(a=100;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(300);
	}
	for(a=50;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(600);
	}
	beep = 1;
}

void Beep_bs(void)
{
	unsigned char a;
	for(a=150;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(400);
	}
	DELAY_MS(500);
	for(a=150;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(400);
	}
	DELAY_MS(500);
	for(a=150;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(400);
	}
	DELAY_MS(500);
	for(a=250;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(200);
	}
	beep = 1;
}

void Beep_key(void)
{
	unsigned char a;
	for(a=100;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(300);
	}
	beep = 1;
}		

void Beep_al(void)
{
	unsigned int a;
	for(a=2500;a>0;a--)
	{
		beep = ~beep;
		Delay_Bp(180);
	}
	beep = 1;
}