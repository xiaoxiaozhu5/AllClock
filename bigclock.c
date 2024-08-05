#include "bigclock.h"
#include "digifont.h"
#include "lcd12864.h"

#include <stdio.h>

void big_clock_init(void)
{
	DigiFont_init();
	DigiFont_setColors(0xff,0);
    DigiFont_setSpacing(2);
	DigiFont_setClearBg(0);
	DigiFont_setSize1(17/*wd*/,35/*ht*/,5/*th*/);//digWd, digHt, segThick, segWd=wd-2, segHt=(ht - 3) / 2
}

void show_big_block(unsigned char hour, unsigned char min)
{
#define SCR_WD  128
#define SCR_HT  64
	int t = 5;
    int w=29;
    int y = 0;
	DigiFont_setSize1(w-1,60,t);
	y=(SCR_HT-60)/2;
	DigiFont_drawDigit1(hour/16,0*w,y);
    DigiFont_drawDigit1(hour%16,1*w,y);
    DigiFont_drawDigit1(':',1+2*w,y);
    DigiFont_drawDigit1(min/16,t+3+2*w,y);
    DigiFont_drawDigit1(min%16,t+3+3*w,y);
}