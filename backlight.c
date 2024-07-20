#include "backlight.h"
#include <STC12C5A60S2.h>

#include <intrins.h>

#include "delay.h"

void Backlight_init(unsigned char CHA)
{
	unsigned char AD_FIN=0; //A/D flag
    CHA &= 0x07;            //select 1 of 8 interface
    ADC_CONTR = 0x40;		//speed of ADC
    _nop_();
    ADC_CONTR |= CHA;       //select channel
    _nop_();
    ADC_CONTR |= 0x80;      //turn on 
    DELAY_MS(1);            //stablize
}

unsigned char Backlight_read()
{
	unsigned char AD_FIN=0; //A/D flag
    ADC_CONTR |= 0x08;      //start 
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while (AD_FIN ==0){     //wait for finish
        AD_FIN = (ADC_CONTR & 0x10); //check if finish
    }
    ADC_CONTR &= 0xE7;      //turn off 
	return (ADC_RES);       //return result of ADC(8 bit)
}