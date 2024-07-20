#include "ds1302.h"

#include <STC12C5A60S2.h>

sbit	clock_clk 	= 	P3 ^ 5;	
sbit	clock_dat 	= 	P3 ^ 6;	
sbit	clock_Rst 	= 	P3 ^ 7;

sbit a0   = ACC ^ 0;sbit a1   = ACC ^ 1;sbit a2   = ACC ^ 2;
sbit a3   = ACC ^ 3;sbit a4   = ACC ^ 4;sbit a5   = ACC ^ 5;
sbit a6   = ACC ^ 6;sbit a7   = ACC ^ 7;

unsigned char yy,mo,dd,xq,hh,mm,ss;

void clock_out(unsigned char dd)
{
	ACC=dd;
	clock_dat=a0; clock_clk=1; clock_clk=0;clock_dat=a1; clock_clk=1; clock_clk=0;
	clock_dat=a2; clock_clk=1; clock_clk=0;clock_dat=a3; clock_clk=1; clock_clk=0;
	clock_dat=a4; clock_clk=1; clock_clk=0;clock_dat=a5; clock_clk=1; clock_clk=0;
	clock_dat=a6; clock_clk=1; clock_clk=0;clock_dat=a7; clock_clk=1; clock_clk=0;
}

void write_clock(unsigned char ord, unsigned char dd)
{
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	clock_out(ord);
	clock_out(dd);
	clock_Rst=0;
	clock_clk=1;
}

unsigned char clock_in(void)
{
	clock_dat=1;
	a0=clock_dat;
	clock_clk=1; clock_clk=0; a1=clock_dat;
	clock_clk=1; clock_clk=0; a2=clock_dat;
	clock_clk=1; clock_clk=0; a3=clock_dat;
	clock_clk=1; clock_clk=0; a4=clock_dat;
	clock_clk=1; clock_clk=0; a5=clock_dat;
	clock_clk=1; clock_clk=0; a6=clock_dat;
	clock_clk=1; clock_clk=0; a7=clock_dat;
	return(ACC);
}

unsigned char read_clock(unsigned char ord)
{
	unsigned char dd=0;
	clock_clk=0;
	clock_Rst=0;
	clock_Rst=1;
	clock_out(ord);
	dd=clock_in();
	clock_Rst=0;
	clock_clk=1;
	return(dd);
}

void read_clockS(void)
{
	ss = read_clock(0x81);//sec
	mm = read_clock(0x83);//min
	hh = read_clock(0x85);//hour
	dd = read_clock(0x87);//day
	mo = read_clock(0x89);//month
	yy = read_clock(0x8d);//year
	xq = read_clock(0x8b);//week
	xq = xq%16; 
}

void Init_1302(void) //set initial datetime(2010.1.1 00:00:00 Mon)
{
	unsigned char f;
	if(read_clock(0x81) == 0x80)
	{
		write_clock(0x8e,0x00);//enable write
		write_clock(0x8c,0x10);//year
		write_clock(0x8a,0x01);//week
		write_clock(0x88,0x01);//month
		write_clock(0x86,0x01);//day
		write_clock(0x84,0x00);//hour
		write_clock(0x82,0x00);//min
		write_clock(0x80,0x50);//sec
		write_clock(0x90,0x05);//charge£¨0x05 no charging£¬0xa5 charging£©	
		for(f=0;f<60;f=f+2)//clear alarm bits
		{
			write_clock(0xc0+f,0x00);
		}
		write_clock(0xC4,0x09);//initial backlight 9
		write_clock(0x8e,0x80);//disable write
	}
}