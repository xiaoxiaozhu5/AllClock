#include "delay.h"

void delay(unsigned int t)
{
	unsigned int i = 0,j = 0;
	for(i = 0;i < t;i ++)
		for(j = 0;j < 20;j ++);
}

void DELAY_MS(unsigned int a)
{
		unsigned int i = 0;
		while( --a != 0)
		{		
			for(i = 0; i < 600; i++); 
		}   				  
}

void Delay_Bp(int num)
{
	while(num--) ;
}