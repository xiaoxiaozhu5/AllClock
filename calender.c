#include "calender.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd12864.h"

#define uchar unsigned char


void show_calender(short _year, short _month, short _day)
{
	short i = 0,day=0;
	uchar line = 17;
	uchar space_cnt = 0;
	uchar daysInMonth = 0;
	char tmp[10] = {0};
	short year,month;
	signed long int startingDay,dayOfWeek;

    year = _year;
    month = _month;

    // Calculate the number of days in the month
    if (month == 2) {
        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            daysInMonth = 29;
        else
            daysInMonth = 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    } else {
        daysInMonth = 31;
    }

    // Calculate the starting day of the month
    startingDay = ((signed long int)year - 1) * 365 + ((signed long int)year - 1) / 4 - ((signed long int)year - 1) / 100 + ((signed long int)year - 1) / 400;
    for (i = 1; i < month; ++i) {
        if (i == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                startingDay += 29;
            else
                startingDay += 28;
        } else if (i == 4 || i == 6 || i == 9 || i == 11) {
            startingDay += 30;
        } else {
            startingDay += 31;
        }
    }

    dayOfWeek = (startingDay + 1) % 7; // Incrementing the starting day by 1

    // Display the calendar
	sprintf(tmp, "%d-%02d-%02d", year, month, _day);
	printString3x5(tmp,128/3,0);
	drawHorizontalLine(8,0,7,0xff);
	printString3x5("Sun Mon Tue Wed Thu Fri Sat",11,10);

	/*
	sprintf(tmp, "%ld", dayOfWeek);
	printString3x5(tmp,20,40);
	sprintf(tmp, "%ld", startingDay);
	printString3x5(tmp,20,47);
	*/
	
	
	printString3x5("   ",0,17);
	space_cnt+=8;
    // Print spaces for the days before the starting day of the month
    for (i = 0; i < dayOfWeek; ++i) {
        printString3x5("    ",space_cnt,19);
		space_cnt+=15;
    }

	 
    // Print the days of the month
    for (day = 1; day <= daysInMonth; ++day) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%4d", day);
		printString3x5(tmp,space_cnt,line);
		space_cnt+=15;
        if ((day + dayOfWeek) % 7 == 0) {
			line+=8;
			space_cnt = 8;
        }
    }
	
}

