#include "lcd12864.h"

#include <STC12C5A60S2.h>
#include <string.h>
#include <intrins.h>
#include "delay.h"

#define uchar unsigned char
#define uint unsigned int

sbit	SID   		=	P3 ^ 2;
sbit	SCLK  		=	P3 ^ 3;

void lcm_w_byte(unsigned char bbyte)
{
	unsigned char i;
    for(i=0;i<8;i++){
        SID=bbyte&0x80;
        SCLK=1;
        SCLK=0;
        bbyte<<=1;
    }
}

void lcm_w_test(bit start, unsigned char ddata)
{
	unsigned char start_data,Hdata,Ldata;
    if(start==0){
		start_data=0xf8;	 //0:command
    }else{
    	start_data=0xfa;  //1:data
	}
    Hdata=ddata&0xf0;
    Ldata=(ddata<<4)&0xf0;
    lcm_w_byte(start_data);
    DELAY_MS(1);
    lcm_w_byte(Hdata);
    DELAY_MS(1);
    lcm_w_byte(Ldata);
    DELAY_MS(1);
}

void lcm_clr(void)
{
	lcm_w_test(0,0x01);
	DELAY_MS(20);
}

void lcm_clr_up(void)
{
	unsigned char row = 0;
	unsigned char col = 0;
	lcm_w_test(0, 0x36);  
	for(row = 0; row < 64; row++)     
	{
		lcm_w_test(0, 0x80 + ((row / 32) ? (row - 32) : row));     
		lcm_w_test(0, (row / 32) ? 0x88 : 0x80);      	
																		
		for(col = 0; col < 16; col++)      				
		{           
			lcm_w_test(1, 0);
		} 
	} 
	lcm_w_test(0, 0x30);  
}

void lcm_w_word(unsigned char *str)
{
    while(*str != '\0'){
        lcm_w_test(1,*str++);
    }	*str = 0;
}

void lcm_init(void)
{
	DELAY_MS(200);
    lcm_w_test(0,0x30);  //8 bit£¬basic
    lcm_w_test(0,0x0c);  //turn on display
    lcm_w_test(0,0x01);
	DELAY_MS(200);
}


void display_12864(unsigned char row, unsigned char col, unsigned char* str)
{
	unsigned char x = 0;
	switch(row)
	{
		case 1:
        x = 0x80;
        break;
		case 2:
        x = 0x90;
        break;
		case 3:
        x = 0x88;
        break;
		case 4:
        x = 0x98;
        break;
	}
	x += col;
	lcm_w_test(0,x);
	lcm_w_word(str);
}

void PWM_init (void)
{
	CMOD=0x02;
    CL=0x00;
    CH=0x00;
	CCAPM1=0x42;
	CCAP1L=0x00;
    CCAP1H=0x00;
    CR=1;
}

void PWM1_set (unsigned char a)
{
	CCAP1L= a;
    CCAP1H= a;
}

uchar LCD_X = 0;
uchar LCD_Y = 0;
void wr_address(void)
{
	lcm_w_test(0,0x34) ;
	lcm_w_test(0,LCD_Y) ;
	lcm_w_test(0,LCD_X) ;
	//lcm_w_test(0,0x30) ;
}

void White_Line_Init();
void reverse_show(uchar addr_x ,uchar addr_y ,uchar num)
{
    uchar j , k ;
    LCD_X = addr_x ;
    LCD_Y = addr_y ;
    White_Line_Init();
    for ( j = 0 ; j < 16 ; j++ )
    {
        wr_address();
        for ( k = 0 ; k < 2 * num ; k++ )
        {
            lcm_w_test(1,0xff) ;
        }
        LCD_Y += 1 ;
    }
    lcm_w_test(0,0x36) ;
    lcm_w_test(0,0x30) ;
}

//clear line
void White_Line_Init()
{
    uchar i,j;
    lcm_w_test(0,0x34);                        //enter extend basic, turn off display
    for(i=0;i<32;i++)
    {
        lcm_w_test(0,0x80+i);
        lcm_w_test(0,0x80);
        for(j=0;j<16;j++)
        {
            lcm_w_test(1,0x00);
            lcm_w_test(1,0x00);
        }
    }
    for(i=0;i<32;i++)                //clear line 3 and line 4
    {
        lcm_w_test(0,0x80+i);
        lcm_w_test(0,0x88);
        for(j=0;j<16;j++)
        {
            lcm_w_test(1,0x00);
            lcm_w_test(1,0x00);
        }
    }
    lcm_w_test(0,0x36);                        //enter extend basic, turn on display
}

//x(0-3),y(0-7),width:num
void White_Line(uchar x,uchar y,uchar width)
{
    unsigned char i,j,flag=0x00;

    White_Line_Init();

    if(x>1)
    {
        flag=0x08;
        x=x-2;
    }
    lcm_w_test(0,0x34);
    for(i=0;i<16;i++)
    {
        lcm_w_test(0,0x80+(x<<4)+i);
        lcm_w_test(0,0x80+flag+y);
        for(j=0;j<width;j++)
        {
            lcm_w_test(1,0xff);
            delay(1);
            lcm_w_test(1,0xff);
            delay(1);
        }
    }
    lcm_w_test(0,0x36);
    lcm_w_test(0,0x30);                        //exit extend basic, enter basic
}

void flash(void)
{
	lcm_w_test(0,0x08) ; //turn off display
	DELAY_MS(250) ;
	lcm_w_test(0,0x0c) ; //turn on display, turn off cursor, no flashing
	DELAY_MS(250) ;
}

void flow(void)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		lcm_w_test(0,0x1c);//turn on display shift, left to right
		DELAY_MS(250);
	}
}