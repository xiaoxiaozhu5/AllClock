#include "lcd12864.h"

#include <STC12C5A60S2.h>
#include <string.h>
#include <intrins.h>
#include "delay.h"

#include "font.h"

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
	uchar i;
	for(i = 0; i < 8; i++)
	{
		lcm_w_test(0,0x1c);//turn on display shift, left to right
		DELAY_MS(250);
	}
}

void LCD_EnableGraphics(void)
{
	lcm_w_test(0,0x20);		
	DELAY_MS(1);
	lcm_w_test(0,0x24);			// Switch to extended instruction mode.	
	DELAY_MS(1);
	lcm_w_test(0,0x26);			// Enable graphics mode.
	DELAY_MS(1);
}

void LCD_DisableGraphics(void)
{
	lcm_w_test(0,0x20);			// Graphics and extended instruction mode turned off.
	DELAY_MS(1);
}

/*
void LCD_FillScreenGraphics(const unsigned char* graphic)
{
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			for(x = 0; x < 8; x++)							// Draws top half of the screen.
			{												// In extended instruction mode, vertical and horizontal coordinates must be specified before sending data in.
				lcm_w_test(0,0x80 | y);					// Vertical coordinate of the screen is specified first. (0-31)
    			lcm_w_test(0,0x80 | x);					// Then horizontal coordinate of the screen is specified. (0-8)
				lcm_w_test(1,graphic[2*x + 16*y]);			// Data to the upper byte is sent to the coordinate.
				lcm_w_test(1,graphic[2*x+1 + 16*y]);		// Data to the lower byte is sent to the coordinate.
			}
		}
		else
		{
			for(x = 0; x < 8; x++)							// Draws bottom half of the screen.
			{												// Actions performed as same as the upper half screen.
				lcm_w_test(0,0x80 | (y-32));			// Vertical coordinate must be scaled back to 0-31 as it is dealing with another half of the screen.
    			lcm_w_test(0,0x88 | x);
				lcm_w_test(1,graphic[2*x + 16*y]);
				lcm_w_test(1,graphic[2*x+1 + 16*y]);
			}
		}
		
	}
}
*/

/*
void LCD_GraphicsDisplayString(unsigned char line, char* str)
{
	// Output text to the LCD in graphics mode using a 5x7 font
	// Each char occupies 6 cols, 8 lines, including of space to next char
	unsigned char row;
	unsigned char colInd;
	unsigned char shouldContinue = 1;
	unsigned char count = 0;
	unsigned char letterA = 0;
	unsigned char letterB = 0;
	unsigned char indA = 0;
	unsigned char indB = 0;
	unsigned char colListA[5];
	unsigned char colListB[5];
	unsigned char dataA = 0;
	unsigned char dataB = 0;

	while(*str && shouldContinue)
	{
		letterA = *str++;
		if (letterA == 0)
		{
			letterA = 32;
			shouldContinue = 0;
		}

		// if string length is odd, last letter does not come in pair, append space			
		letterB = *str++;
		if (letterB == 0) 
		{
			letterB = 32; // odd number of characters in a string, replace NULL with space		
			shouldContinue = 0;
		}

		indA = letterA < 0x52 ? letterA - 0x20 : letterA - 0x52;
		indB = letterB < 0x52 ? letterB - 0x20 : letterB - 0x52;
		
	
	    if(letterA < 0x52){
			colListA[4] = Alpha1[(indA*5)];
			colListA[3] = Alpha1[(indA*5)+1];
			colListA[2] = Alpha1[(indA*5)+2];
			colListA[1] = Alpha1[(indA*5)+3];
			colListA[0] = Alpha1[(indA*5)+4];
		}
		else
		{
			colListA[4] = Alpha2[(indA*5)];
			colListA[3] = Alpha2[(indA*5)+1];
			colListA[2] = Alpha2[(indA*5)+2];
			colListA[1] = Alpha2[(indA*5)+3];
			colListA[0] = Alpha2[(indA*5)+4];
		}
	
	    if(letterB < 0x52){
			colListB[4] = Alpha1[(indB*5)];
			colListB[3] = Alpha1[(indB*5)+1];
			colListB[2] = Alpha1[(indB*5)+2];
			colListB[1] = Alpha1[(indB*5)+3];
			colListB[0] = Alpha1[(indB*5)+4];
		}
		else
		{
			colListB[4] = Alpha2[(indB*5)];
			colListB[3] = Alpha2[(indB*5)+1];
			colListB[2] = Alpha2[(indB*5)+2];
			colListB[1] = Alpha2[(indB*5)+3];
			colListB[0] = Alpha2[(indB*5)+4];
		}

		for (row=0;row<8;row++)
		{
			if (line < 4)	// first half
			{
				lcm_w_test(0,0x80 | (line * 8 + row));
				lcm_w_test(0,0x80 | count);
			}
			else
			{
				lcm_w_test(0,0x80 | ( (line-4) * 8 + row));
				lcm_w_test(0,0x88 | count);
			}
					
			dataA = 0x00;
			for (colInd=0;colInd<5;colInd++)
			{
				if (colListA[colInd] & (1 << row))
				{
					dataA = dataA | (1 << (colInd+3));
				}
			}

			dataB = 0x00;
			for (colInd=0;colInd<5;colInd++)
			{
				if (colListB[colInd] & (1 << row))
				{
					dataB = dataB | (1 << (colInd+3));
				}
			}
	
			lcm_w_test(1,dataA);
			lcm_w_test(1,dataB);
		}

		count++;
	}
}
*/

void LCD_ClearGraphics(void)
{
	// This function performs similar to the LCD_FillScreenGraphics but
	// only zeros are sent into the screen instead of data from an array.
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			lcm_w_test(0,0x80 | y);
    		lcm_w_test(0,0x80);
		}
		else
		{
			lcm_w_test(0,0x80 | (y-32));
    		lcm_w_test(0,0x88);
		}
		for(x = 0; x < 8; x++)
		{
			lcm_w_test(1,0x00);
			lcm_w_test(1,0x00);
		}
	}
}

unsigned char xdata mapa[16][32][2] = {0};

//1-point display / deletion / inversion at posX coordinates (0 to 127) and posY (0 to 63)
void plot(uchar posX, uchar posY, uchar style) {
	uchar horiz = 0;
	uchar minibit = 0;
	uchar originalLeftByte = 0, originalRightByte = 0;
	uchar leftByte, rightByte;
    if (posX > 127) posX = 127;
    else if (posX < 0) posX = 0;
    if (posY > 63) posY = 63;
    else if (posY < 0) posY = 0;

    horiz = posX / 16;
    if (posY >= 32) {
        posY -= 32;
        horiz += 8;
    }

    minibit = posX & 15; 

	lcm_w_test(0, 0x80 + posY);
	lcm_w_test(0, 0x80 + horiz);

    originalLeftByte = mapa[horiz][posY][0];
    originalRightByte = mapa[horiz][posY][1];  
    
    if (minibit < 8) {
        if (style == 1) { 
            leftByte = (0x80 >> minibit) | originalLeftByte;
        } else if (style == 0) { 
            leftByte = ~(0x80 >> minibit) & originalLeftByte;
        } else { 
            leftByte = (0x80 >> minibit) ^ originalLeftByte;
        }
        mapa[horiz][posY][0] = leftByte;
        rightByte = originalRightByte;
    } else {
        if (style == 1) { 
            rightByte = (0x80 >> (minibit - 8)) | originalRightByte;
        } else if (style == 0) {  
            rightByte = ~(0x80 >> (minibit - 8)) & originalRightByte;
        } else { 
            rightByte = (0x80 >> (minibit - 8)) ^ originalRightByte;
        }
        mapa[horiz][posY][1] = rightByte;
        leftByte = originalLeftByte;
    }

	lcm_w_test(1, leftByte);
	lcm_w_test(1, rightByte);

}

void printString3x5(const char* string, uchar leftX, uchar topY) 
{    
    uchar actualX = leftX;
    uchar actualY = topY;
    uchar firstRun = 1;
	uchar row = 0, column = 0;

        while (*string) {
            unsigned char charCode = (unsigned char)*string++;
            
            if (charCode < 32 || charCode > 126) {
                charCode = 127;
            }
            
            charCode -= 32;

            if (!firstRun) {
                for (row = 0; row < 5; ++row) {
                    plot(actualX, actualY + row, 0);
                }
                actualX += 1;
            } else {
                firstRun = 0;
            }

            for (column = 0; column < CHAR_HEIGHT; ++column) {
                for (row = 0; row < CHAR_WIDTH; ++row) {
					plot(actualX, actualY + row, char3x5[charCode][column][row]);
                }
                actualX += 1;
            }
        }
}

void drawHorizontalLine(uchar posY, uchar fromByte, uchar toByte, uchar pattern) {
    uchar shift = fromByte;
	uchar r = 0;
    if (posY >= 32) {
        posY = posY - 32;
        shift = shift + 8;
    }

	lcm_w_test(0, 0x80 + posY);
	lcm_w_test(0, 0x80 + shift);

    for (r = 0; r <= toByte - fromByte; r++) {
		lcm_w_test(1, pattern);
		lcm_w_test(1, pattern);
        mapa[shift + r][posY][0] = pattern;
        mapa[shift + r][posY][1] = pattern;
    }
}