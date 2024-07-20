#ifndef __DS_1302__
#define __DS_1302__

unsigned char read_clock(unsigned char ord);
void write_clock(unsigned char ord, unsigned char dd);
void read_clockS(void);
void Init_1302(void);

#endif