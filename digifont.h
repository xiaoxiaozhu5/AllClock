// 7-segment and outline digit font rendering library
// (c) 2020 by Pawel A. Hernik

#ifndef _DIGI_FONT_H
#define _DIGI_FONT_H

// Function prototypes
void DigiFont_init();
void DigiFont_setSize1(int wd, int ht, int th);
#if 0
void DigiFont_setSize2(int wd, int ht, int th);
void DigiFont_setSize3(int wd, int ht, int th);
void DigiFont_setSizeF(int wd, int ht, int th);
void DigiFont_setSizeO(int wd, int ht, int th);
void DigiFont_setSize7(int wd, int ht, int th, int st);
#endif
void DigiFont_setSegment(int wd, int ht, int th);
void DigiFont_drawSeg1(int seg, int x, int y, int c);
#if 0
void DigiFont_drawSeg2(int seg, int x, int y, int c);
void DigiFont_drawSeg2c(int seg, int x, int y, int c);
void DigiFont_drawSeg3(int seg, int x, int y, int c);
void DigiFont_drawSeg7(int seg, int x, int y, int c);
#endif
int DigiFont_drawDigit1(int ch, int x, int y);
#if 0
int DigiFont_drawDigit2(int ch, int x, int y);
int DigiFont_drawDigit2c(int ch, int x, int y);
int DigiFont_drawDigit3(int ch, int x, int y);
int DigiFont_drawDigitF(int ch, int x, int y);
int DigiFont_drawDigitO(int ch, int x, int y);
int DigiFont_drawDigit4(int ch, int x, int y);
int DigiFont_drawDigit5(int ch, int x, int y);
int DigiFont_drawDigit7(int ch, int x, int y);
#endif
int DigiFont_printNumber1(char *txt, int x, int y);
#if 0
int DigiFont_printNumber2(char *txt, int x, int y);
int DigiFont_printNumber2c(char *txt, int x, int y);
int DigiFont_printNumber3(char *txt, int x, int y);
int DigiFont_printNumberF(char *txt, int x, int y);
int DigiFont_printNumberO(char *txt, int x, int y);
int DigiFont_printNumber4(char *txt, int x, int y);
int DigiFont_printNumber5(char *txt, int x, int y);
int DigiFont_printNumber7(char *txt, int x, int y);
void DigiFont_clear45(int ch, int x, int y);
#endif
void DigiFont_setSpacing(int sp);
void DigiFont_setClearBg(int cl);
void DigiFont_setColors(int c1, int c0);
void DigiFont_setColors2(int c1, int c2, int c0);
int DigiFont_getWidth();
int DigiFont_getHeight();
int DigiFont_numberWidth(char *txt);
void DigiFont_rect(int x, int y, int w, int h, int col);


#endif
