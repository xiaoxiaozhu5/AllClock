// 7-segment and outline digit font rendering library
// (c) 2020 by Pawel A. Hernik

#include "digifont.h"

#include "lcd12864.h"
#include "delay.h"

#include "base_types.h"

#include <stdio.h>

void customLineH(int x0,int x1, int y, int c)
{
    drawHorizontalLine2((U8)y,(U8)x0,(U8)x1,(U8)c);
}
void customLineV(int x, int y0,int y1, int c)
{
    drawVerticalLine((U8)x,(U8)y0,(U8)y1,1,(U8)c);
}
void customRect(int x, int y,int w,int h, int c)
{
	U8 xx = x;
	U8 yy = y;
	U8 ww = w;
	U8 hh = h;
	U8 cc = c;
	U8 xw = xx+ww;
	U8 yh = yy+hh;
    drawRectangle(xx,yy,xw,yh,1,cc);
}

static unsigned char idata digits[16] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

// Define the DigiFont struct
struct DigiFont
{
	int digWd, digHt;
	int segWd, segHt;
	int segThick, segSt;
	int colOn, colOn2, colOff;
	int spacing;
	int clearBg;
};

typedef struct DigiFont DigiFont;
static DigiFont idata df[1];

// ----------------------------------------------------------------
void DigiFont_init() {
    DigiFont_setSize1(10, 20, 5);
    DigiFont_setColors(1, 0);
    df->spacing = 0;
    df->clearBg = 0;
}

// ----------------------------------------------------------------
// linehFun, linevFun and rectFun callbacks are necessary to initialize the library
void DigiFont_setSize1(int wd, int ht, int th) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segWd = wd - 2;
    df->segHt = (ht - 3) / 2;
}

#if 0
void DigiFont_setSize2(int wd, int ht, int th) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segWd = wd - th / 2 - th / 2 - 2;
    df->segHt = (ht - th - 2) / 2;
}

void DigiFont_setSize3(int wd, int ht, int th) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segWd = wd - th * 2 - 2;
    df->segHt = (ht - th / 2 - th / 2) / 2 - 1;
}

void DigiFont_setSizeF(int wd, int ht, int th) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segWd = wd;
    df->segHt = (ht - th * 3) / 2;
}

void DigiFont_setSizeO(int wd, int ht, int th) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segWd = wd;
    df->segHt = (ht - th * 3) / 2;
}

// Style #7 can replace styles #1 and #2
// segThick value should be an odd value for the best look
// segSt should be from 0 to int(segThick/2) for the best results
void DigiFont_setSize7(int wd, int ht, int th, int st) {
    df->digWd = wd;
    df->digHt = ht;
    df->segThick = th;
    df->segSt = st;
    df->segWd = wd - (df->segSt + 1) * 2;
    df->segHt = (ht - df->segSt * 2 - 3) / 2;
}
#endif

void DigiFont_setSegment(int wd, int ht, int th) {
    df->segThick = th;
    df->segWd = wd;
    df->segHt = ht;
}

void DigiFont_drawSeg1(int seg, int x, int y, int c) {
    int i, ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: // top
        for (i = 0; i < df->segThick; i++)
            //(*df->linehFun)(x + i, x + df->segWd - 1 - i, y + i, c ? df->colOn : df->colOff);
            customLineH(x + i, x + df->segWd - 1 - i, y + i, c ? df->colOn : df->colOff);
        break;
        case 3: // bottom
        for (i = 0; i < df->segThick; i++)
            //(*df->linehFun)(x + i, x + df->segWd - 1 - i, y + df->segThick - 1 - i, c ? df->colOn : df->colOff);
            customLineH(x + i, x + df->segWd - 1 - i, y + df->segThick - 1 - i, c ? df->colOn : df->colOff);
        break;
        case 6: // middle
        for (i = 0; i < 1 + ofs; i++)
            //(*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs - i, c ? df->colOn : df->colOff);
            customLineH(x + i, x + df->segWd - 1 - i, y + ofs - i, c ? df->colOn : df->colOff);
        for (i = 1; i < 1 + ofs; i++)
            //(*df->linehFun)(x + i, x + df->segWd - 1 - i, y + ofs + i, c ? df->colOn : df->colOff);
            customLineH(x + i, x + df->segWd - 1 - i, y + ofs + i, c ? df->colOn : df->colOff);
        break;
        case 4: // left
        case 5: // left
        for (i = 0; i < df->segThick; i++)
            //(*df->linevFun)(x + i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
            customLineV(x + i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
        break;
        case 1: // right
        case 2: // right
        for (i = 0; i < df->segThick; i++)
            //(*df->linevFun)(x + df->segThick - 1 - i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
            customLineV(x + df->segThick - 1 - i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
        break;
    }
}

#if 0
void DigiFont_drawSeg2(int seg, int x, int y, int c) {
    int i,ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: // top
        case 3: // bottom
        case 6: // middle
        for (i = 0; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y + ofs - i, c ? df->colOn : df->colOff);
        for (i = 1; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y + ofs + i, c ? df->colOn : df->colOff);
        break;
        case 1: // right
        case 2: // right
        case 4: // left
        case 5: // left
        for (i = 0; i < ofs + 1; i++) df->linevFun(x + ofs - i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
        for (i = 1; i < ofs + 1; i++) df->linevFun(x + ofs + i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
        break;
    }
}

void DigiFont_drawSeg2c(int seg, int x, int y, int c) {
    int i,ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: // top
        case 3: // bottom
        case 6: // middle
        for (i = 0; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y + ofs - i, c ? df->colOn : df->colOff);
        for (i = 1; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y + ofs + i, c ? df->colOn2 : df->colOff);
        break;
        case 1: // right
        case 2: // right
        case 4: // left
        case 5: // left
        for (i = 0; i < ofs + 1; i++) df->linevFun(x + ofs - i, y + i, y + df->segHt - 1 - i, c ? df->colOn : df->colOff);
        for (i = 1; i < ofs + 1; i++) df->linevFun(x + ofs + i, y + i, y + df->segHt - 1 - i, c ? df->colOn2 : df->colOff);
        break;
    }
}

void DigiFont_drawSeg3(int seg, int x, int y, int c) {
    int ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: // top
        case 3: // bottom
        case 6: // middle
        df->rectFun(x, y, df->segWd, df->segThick, c ? df->colOn : df->colOff);
        break;
        case 1: // right
        case 2: // right
        case 4: // left
        case 5: // left
        df->rectFun(x, y, df->segThick, df->segHt, c ? df->colOn : df->colOff);
        break;
    }
}

void DigiFont_drawSeg7(int seg, int x, int y, int c) {
    int i,ofs;
    if (!c && !df->clearBg) return;
    ofs = df->segThick / 2;
    switch (seg) {
        case 0: // top
        for (i = 0; i < df->segThick; i++) df->linehFun(x + (i < df->segSt ? df->segSt - i : i - df->segSt), x + df->segWd - 1 - (i < df->segSt ? df->segSt - i : i - df->segSt), y + i, c ? (i <= df->segSt ? df->colOn : df->colOn2) : df->colOff);
        break;
        case 3: // bottom
        for (i = 0; i < df->segThick; i++) df->linehFun(x + (i < df->segSt ? df->segSt - i : i - df->segSt), x + df->segWd - 1 - (i < df->segSt ? df->segSt - i : i - df->segSt), y + df->segThick - 1 - i, c ? (i < df->segSt ? df->colOn2 : df->colOn) : df->colOff);
        break;
        case 6: // middle
        for (i = 0; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y - i, c ? df->colOn : df->colOff);
        for (i = 1; i < ofs + 1; i++) df->linehFun(x + i, x + df->segWd - 1 - i, y + i, c ? df->colOn2 : df->colOff);
        break;
        case 1: // right
        case 2: // right
        for (i = 0; i < df->segThick; i++) df->linevFun(x + df->segThick - 1 - i, y + (i < df->segSt ? df->segSt - i : i - df->segSt), y + df->segHt - 1 - (i < df->segSt ? df->segSt - i : i - df->segSt), c ? (i < df->segSt ? df->colOn2 : df->colOn) : df->colOff);
        break;
        case 4: // left
        case 5: // left
        for (i = 0; i < df->segThick; i++) df->linevFun(x + i, y + (i < df->segSt ? df->segSt - i : i - df->segSt), y + df->segHt - 1 - (i < df->segSt ? df->segSt - i : i - df->segSt), c ? (i <= df->segSt ? df->colOn : df->colOn2) : df->colOff);
        break;
    }
}
#endif

/*
 Segment bits:
   00
  5  1
  5  1
   66
  4  2
  4  2
   33
*/

int DigiFont_drawDigit1(int ch, int x, int y) {
    int s = 0;
	//char tmp[15] = {0};
    switch (ch) {
        case '.':
        //(*df->rectFun)(x, y + df->segHt + df->segHt + 3 - df->segThick, df->segThick, df->segThick, df->colOn);
        customRect(x, y + df->segHt + df->segHt + 3 - df->segThick, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        //(*df->rectFun)(x, y + 2 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        //(*df->rectFun)(x, y + df->segHt + 1 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        customRect(x, y + 2 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        customRect(x, y + df->segHt + 1 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        s = 0;
        break;
        case '-':
        s = 0x40;
        break;
        default:
        s = digits[ch & 0xf];
    }
    DigiFont_drawSeg1(0, x + 1, y + 0, s & 0x01);
    DigiFont_drawSeg1(1, x + df->segWd + 2 - df->segThick, y + 1, s & 0x02);
    DigiFont_drawSeg1(2, x + df->segWd + 2 - df->segThick, y + df->segHt + 2, s & 0x04);
    DigiFont_drawSeg1(3, x + 1, y + df->segHt + 2 + df->segHt - df->segThick + 1, s & 0x08);
    DigiFont_drawSeg1(4, x + 0, y + df->segHt + 2, s & 0x10);
    DigiFont_drawSeg1(5, x + 0, y + 1, s & 0x20);
    DigiFont_drawSeg1(6, x + 1, y + df->segHt - df->segThick / 2 + 1, s & 0x40);
    return df->digWd;
}

#if 0
int DigiFont_drawDigit2(int ch, int x, int y) {
    int s, ofs = 1 + df->segThick / 2;
    switch (ch) {
        case '.':
        df->rectFun(x, y + df->segHt + df->segHt + 2, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        df->rectFun(x, y + 2 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        df->rectFun(x, y + df->segHt + 1 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        s = 0;
        break;
        case '-':
        s = 0x40;
        break;
        default:
        s = digits[ch & 0xf];
    }
    DigiFont_drawSeg2(0, x + ofs, y + 0, s & 0x01);
    DigiFont_drawSeg2(1, x + df->segWd + 1, y + ofs, s & 0x02);
    DigiFont_drawSeg2(2, x + df->segWd + 1, y + ofs + df->segHt + 1, s & 0x04);
    DigiFont_drawSeg2(3, x + ofs, y + df->segHt * 2 + 2, s & 0x08);
    DigiFont_drawSeg2(4, x + 0, y + ofs + df->segHt + 1, s & 0x10);
    DigiFont_drawSeg2(5, x + 0, y + ofs, s & 0x20);
    DigiFont_drawSeg2(6, x + ofs, y + df->segHt + 1, s & 0x40);
    return df->digWd;
}

int DigiFont_drawDigit2c(int ch, int x, int y)
{
    int s, ofs = 1 + df->segThick / 2;
    switch (ch) {
        case '.':
        df->rectFun(x, y + df->segHt + df->segHt + 2, df->segThick, df->segThick, df->colOn);
        df->rectFun(x, y + df->segHt + df->segHt + 2 + df->segThick / 2, df->segThick, df->segThick - df->segThick / 2, df->colOn2);
        return df->segThick;
        case ':':
        df->rectFun(x, y + ofs + (df->segHt - df->segThick) / 2, df->segThick, df->segThick / 2, df->colOn);
        df->rectFun(x, y + ofs + (df->segHt - df->segThick) / 2 + df->segThick / 2, df->segThick, df->segThick - df->segThick / 2, df->colOn2);
        df->rectFun(x, y + df->segHt + 1 + ofs + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        df->rectFun(x, y + df->segHt + 1 + ofs + (df->segHt - df->segThick) / 2 + df->segThick / 2, df->segThick, df->segThick - df->segThick / 2, df->colOn2);
        return df->segThick;
        case ' ':
        s = 0;
        break;
        case '-':
        s = 0x40;
        break;
        default:
        s = digits[ch & 0xf];
    }
    DigiFont_drawSeg2c(0, x + ofs, y + 0, s & 0x01);
    DigiFont_drawSeg2c(1, x + df->segWd + 1, y + ofs, s & 0x02);
    DigiFont_drawSeg2c(2, x + df->segWd + 1, y + ofs + df->segHt + 1, s & 0x04);
    DigiFont_drawSeg2c(3, x + ofs, y + df->segHt + df->segHt + 2, s & 0x08);
    DigiFont_drawSeg2c(4, x + 0, y + ofs + df->segHt + 1, s & 0x10);
    DigiFont_drawSeg2c(5, x + 0, y + ofs, s & 0x20);
    DigiFont_drawSeg2c(6, x + ofs, y + df->segHt + 1, s & 0x40);
    return df->digWd;
}

int DigiFont_drawDigit3(int ch, int x, int y)
{
    int s, ofs = df->segThick / 2;
    switch (ch) {
        case '.':
        (*df->rectFun)(x, y + df->digHt - 1 - df->segThick, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        (*df->rectFun)(x, y + (df->digHt / 2 - df->segThick) / 2 + 1, df->segThick, df->segThick, df->colOn);
        (*df->rectFun)(x, y + df->digHt - 1 - (df->digHt / 2 - df->segThick) / 2 - df->segThick - 1, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        s = 0;
        break;
        case '-':
        s = 0x40;
        break;
        default:
        s = digits[ch & 0xf];
    }
    DigiFont_drawSeg3(0, x + df->segThick + 1, y + 0, s & 0x01);
    DigiFont_drawSeg3(1, x + df->segThick + df->segWd + 2, y + ofs, s & 0x02);
    DigiFont_drawSeg3(2, x + df->segThick + df->segWd + 2, y + df->digHt - 1 - ofs - df->segHt, s & 0x04);
    DigiFont_drawSeg3(3, x + df->segThick + 1, y + df->digHt - 1 - df->segThick, s & 0x08);
    DigiFont_drawSeg3(4, x + 0, y + df->digHt - 1 - ofs - df->segHt, s & 0x10);
    DigiFont_drawSeg3(5, x + 0, y + ofs, s & 0x20);
    DigiFont_drawSeg3(6, x + df->segThick + 1, y + (df->digHt - df->segThick) / 2, s & 0x40);
    return df->digWd;
}

int DigiFont_drawDigit7(int ch, int x, int y)
{
    int s, offs = df->segSt + 1;
    switch (ch) {
        case '.':
        (*df->rectFun)(x, y + df->segHt + df->segHt + offs - df->segThick + df->segSt + 2, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        (*df->rectFun)(x, y + offs + 1 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        (*df->rectFun)(x, y + df->segHt + offs + 1 + (df->segHt - df->segThick) / 2, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        s = 0;
        break;
        case '-':
        s = 0x40;
        break;
        default:
        s = digits[ch & 0xf];
    }
    DigiFont_drawSeg7(0, x + offs, y, s & 0x01);
    DigiFont_drawSeg7(1, x + df->digWd - df->segThick, y + offs, s & 0x02);
    DigiFont_drawSeg7(2, x + df->digWd - df->segThick, y + offs + df->segHt + 1, s & 0x04);
    DigiFont_drawSeg7(3, x + offs, y + df->segHt + df->segHt + offs - df->segThick + df->segSt + 2, s & 0x08);
    DigiFont_drawSeg7(4, x, y + offs + df->segHt + 1, s & 0x10);
    DigiFont_drawSeg7(5, x, y + offs, s & 0x20);
    DigiFont_drawSeg7(6, x + offs, y + df->segHt + offs, s & 0x40);
    return df->digWd;
}

void DigiFont_clear45(int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    switch (ch) {
        case ' ':
        (*df->rectFun)(x, y, df->digWd, df->digHt, df->colOff);
        break;
        case '-':
        (*df->rectFun)(x, y, df->digWd, hc + df->segThick, df->colOff);
        (*df->rectFun)(x, y + hc + df->segThick * 2, df->digWd, df->digHt - hc - df->segThick * 2, df->colOff);
        break;
        case 0:
        (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        break;
        case 1:
        (*df->rectFun)(x, y, df->segThick, df->digHt, df->colOff);
        (*df->rectFun)(x + df->segThick, y, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        break;
        case 2:
        (*df->rectFun)(x, y + df->segThick, df->segThick, hc, df->colOff);
        (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
        break;
        case 3:
        (*df->rectFun)(x, y + df->segThick, df->segThick, df->digHt - df->segThick * 2, df->colOff);
        break;
        case 4:
        (*df->rectFun)(x, y + df->segThick * 2 + hc, df->segThick, df->digHt - df->segThick * 2 - hc, df->colOff);
        (*df->rectFun)(x + df->segThick, y, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        break;
        case 5:
        (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick, df->segThick, hc, df->colOff);
        (*df->rectFun)(x, y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOff);
        break;
        case 6:
        (*df->rectFun)(x + df->digWd - df->segThick, y + df->segThick, df->segThick, hc, df->colOff);
        break;
        case 7:
        (*df->rectFun)(x, y + df->segThick, df->segThick, df->digHt - df->segThick, df->colOff);
        (*df->rectFun)(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOff);
        (*df->rectFun)(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick*2, df->segThick, df->colOff);
        break;
        case 8:
        break;
        case 9:
        (*df->rectFun)(x, y + hc + df->segThick*2, df->segThick, df->digHt-hc-df->segThick*3, df->colOff);
        break;
    }
}

int DigiFont_drawDigitF(int ch, int x, int y) {
    int hc = (df->digHt - df->segThick * 3) / 2;
    ch = (ch >= '0' && ch <= '9') ? ch - '0' : ch;
    if (df->clearBg) DigiFont_clear45(ch, x, y);

    switch (ch) {
        case '.':
        df->rectFun(x, y + df->digHt - df->segThick, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        df->rectFun(x, y + (df->digHt / 2 - df->segThick) / 2 + 2, df->segThick, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - 1 - (df->digHt / 2 - df->segThick) / 2 - df->segThick - 1, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        break;
        case '-':
        df->rectFun(x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
        break;
        case 0:
        df->rectFun(x, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x + df->segThick, y, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        df->rectFun(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        break;
        case 1:
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        break;
        case 2:
        df->rectFun(x, y, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - df->segThick, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y + df->segThick, df->segThick, hc, df->colOn);
        break;
        case 3:
        df->rectFun(x, y, df->digWd - df->segThick, df->segThick, df->colOn);
        df->rectFun(x + df->digWd / 3, y + hc + df->segThick, df->digWd - df->segThick - df->digWd / 3, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        break;
        case 4:
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x, y, df->segThick, hc + df->segThick * 2, df->colOn);
        df->rectFun(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        break;
        case 5:
        df->rectFun(x, y, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - df->segThick, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + df->segThick, df->segThick, hc, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
        break;
        case 6:
        df->rectFun(x, y, df->digWd, df->segThick, df->colOn);
        df->rectFun(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - df->segThick, df->digWd, df->segThick, df->colOn);
        df->rectFun(x, y + df->segThick, df->segThick, df->digHt - df->segThick * 2, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y + hc + df->segThick * 2, df->segThick, df->digHt - hc - df->segThick * 3, df->colOn);
        break;
        case 7:
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x, y, df->digWd - df->segThick, df->segThick, df->colOn);
        break;
        case 8:
        df->rectFun(x, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x + df->segThick, y, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        df->rectFun(x + df->segThick, y + df->digHt - df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        df->rectFun(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        break;
        case 9:
        df->rectFun(x, y, df->segThick, hc + df->segThick * 2, df->colOn);
        df->rectFun(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        df->rectFun(x + df->segThick, y, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        df->rectFun(x + df->segThick, y + hc + df->segThick, df->digWd - df->segThick * 2, df->segThick, df->colOn);
        df->rectFun(x, y + df->digHt - df->segThick, df->digWd - df->segThick, df->segThick, df->colOn);
        break;
    }
    return df->digWd;
}
#endif

void DigiFont_rect(int x, int y, int w, int h, int col)
{
    customLineH(x, x+w-1, y+h-1,col);
    customLineH(x, x+w-1, y,    col);
    customLineV(x,    y+1, y+h-2,col);
    customLineV(x+w-1,y+1, y+h-2,col);
}

#if 0
int DigiFont_drawDigitO(int ch, int x, int y)
{
    int hc = (df->digHt - df->segThick * 3) / 2;
    ch = (ch >= '0' && ch <= '9') ? ch - '0' : ch;
    if (df->clearBg) {
        DigiFont_clear45(ch, x, y);
        // cleaning up leftover after previous digits
        (*(df->rectFun))(x + 1, y + 1, df->segThick - 1, df->segThick - 1, df->colOff);
        (*(df->rectFun))(x + df->digWd - df->segThick, y + 1, df->segThick - 1, df->segThick - 1, df->colOff);
        (*(df->rectFun))(x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->segThick - 1, df->segThick - 1, df->colOff);
        (*(df->rectFun))(x + df->digWd - df->segThick, y + hc + df->segThick, df->segThick - 1, df->segThick, df->colOff);
        (*(df->rectFun))(x + 1, y + hc + df->segThick, df->segThick - 1, df->segThick, df->colOff);
        (*(df->rectFun))(x + 1, y + df->digHt - df->segThick, df->segThick - 1, df->segThick - 1, df->colOff);
        (*(df->linevFun))(x + df->segThick, y + df->segThick + hc + 1, y + df->segThick + hc + 1 + df->segThick - 2, df->colOff); // le-mid-short '3'
        (*(df->linehFun))(x + 1, x + df->segThick - 1, y + df->digHt - df->segThick, df->colOff); // bot-left
    }
    switch (ch) {
        case '.':
        DigiFont_rect(x, y + df->digHt - df->segThick, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ':':
        DigiFont_rect(x, y + (df->digHt / 2 - df->segThick) / 2 + 2, df->segThick, df->segThick, df->colOn);
        DigiFont_rect(x, y + df->digHt - 1 - (df->digHt / 2 - df->segThick) / 2 - df->segThick - 1, df->segThick, df->segThick, df->colOn);
        return df->segThick;
        case ' ':
        break;
        case '-':
        DigiFont_rect(x, y + hc + df->segThick, df->digWd, df->segThick, df->colOn);
        break;
        case 0:
        DigiFont_rect(x, y, df->digWd, df->digHt, df->colOn);
        DigiFont_rect(x + df->segThick - 1, y + df->segThick - 1, df->digWd - df->segThick * 2 + 2, df->digHt - df->segThick * 2 + 2, df->colOn);
        break;
        case 1:
        DigiFont_rect(x + df->digWd - df->segThick, y, df->segThick, df->digHt, df->colOn);
        break;
        case 2:
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->segThick - 1, df->colOn); // top2
        (*(df->linehFun))(x, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->digHt - df->segThick, df->colOn); // bottom2
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->segThick + hc, df->colOn); // mid
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->segThick + hc + df->segThick - 1, df->colOn); // mid2
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + hc + df->segThick * 2 - 1, df->colOn); // rt
        (*(df->linevFun))(x + df->digWd - df->segThick, y + df->segThick - 1, y + hc + df->segThick, df->colOn); // rt le
        (*(df->linevFun))(x, y + hc + df->segThick + 1, y + df->digHt - 2, df->colOn); // le
        (*(df->linevFun))(x + df->segThick - 1, y + hc + df->segThick * 2 - 1, y + df->digHt - df->segThick, df->colOn); // le rt
        (*(df->linevFun))(x, y + 1, y + df->segThick - 2, df->colOn); // le short
        (*(df->linevFun))(x + df->digWd - 1, y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn); // rt short
        break;
        case 3:
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->segThick - 1, df->colOn); // top2
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->colOn); // bottom2
        (*(df->linehFun))(x + df->digWd / 3, x + df->digWd - df->segThick, y + df->segThick + hc, df->colOn); // mid1 2/3 width
        (*(df->linehFun))(x + df->digWd / 3, x + df->digWd - df->segThick, y + df->segThick * 2 + hc - 1, df->colOn); // mid2 2/3 width
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->digHt - 2, df->colOn); // rt-long
        (*(df->linevFun))(x, y + 1, y + df->segThick - 2, df->colOn); // le-top-short
        (*(df->linevFun))(x, y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn); // le-bot-short
        (*(df->linevFun))(x + df->digWd / 3 - 1, y + df->segThick + hc + 0, y + df->segThick + hc + 1 + df->segThick - 2, df->colOn); // le-mid-short 2/3 width
        (*(df->linevFun))(x + df->digWd - df->segThick, y + df->segThick, y + df->segThick + 1 + hc - 1, df->colOn); //le-top
        (*(df->linevFun))(x + df->digWd - df->segThick, y + df->segThick * 2 + hc, y + df->segThick * 2 + hc + 1 + hc - 1, df->colOn); //le-bot
        break;
        case 4:
        (*(df->linehFun))(x, x + df->segThick - 1, y, df->colOn); // top short1
        (*(df->linehFun))(x + df->digWd - df->segThick, x + df->digWd - 1, y, df->colOn); // top short2
        (*(df->linehFun))(x + df->digWd - df->segThick, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom short
        (*(df->linehFun))(x + df->segThick, x + df->digWd - df->segThick, y + df->segThick + hc, df->colOn); // mid1
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->segThick + hc + df->segThick - 1, df->colOn); // mid2
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->digHt - 2, df->colOn); // rt-long
        (*(df->linevFun))(x + df->digWd - df->segThick, y + 1, y + df->segThick + hc - 1, df->colOn); //rt-left-top
        (*(df->linevFun))(x + df->digWd - df->segThick, y + hc + df->segThick * 2, y + df->digHt - 2, df->colOn); //rt-left-bot
        (*(df->linevFun))(x, y + 1, y + df->segThick * 2 + hc - 1, df->colOn); //le-le
        (*(df->linevFun))(x + df->segThick - 1, y + 1, y + df->segThick + hc, df->colOn); //le-rt
        break;
        case 5:
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->segThick - 1, df->colOn); // top2
        (*(df->linehFun))(x, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->digHt - df->segThick, df->colOn); // bottom2
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->segThick + hc, df->colOn); // mid
        (*(df->linehFun))(x, x + df->digWd - df->segThick, y + df->segThick + hc + df->segThick - 1, df->colOn); // mid2
        (*(df->linevFun))(x, y + 1, y + hc + df->segThick * 2 - 1, df->colOn); // le
        (*(df->linevFun))(x + df->segThick - 1, y + df->segThick - 1, y + hc + df->segThick, df->colOn); // le rt
        (*(df->linevFun))(x + df->digWd - 1, y + hc + df->segThick + 1, y + df->digHt - 2, df->colOn); // rt
        (*(df->linevFun))(x + df->digWd - df->segThick, y + hc + df->segThick * 2, y + df->digHt - df->segThick - 1, df->colOn); // rt le
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->segThick - 2, df->colOn); // rt short
        (*(df->linevFun))(x, y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn); // le short
        break;
        case 6:
        DigiFont_rect(x + df->segThick - 1, y + hc + df->segThick * 2 - 1, df->digWd - df->segThick * 2 + 2, df->digHt - hc - df->segThick * 3 + 2, df->colOn);
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->segThick - 1, df->colOn); // top2
        (*(df->linehFun))(x, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom
        (*(df->linehFun))(x + df->segThick, x + df->digWd - 1, y + df->segThick + hc, df->colOn); // mid
        (*(df->linevFun))(x, y + 1, y + df->digHt - 2, df->colOn); // le long
        (*(df->linevFun))(x + df->segThick - 1, y + df->segThick - 1, y + hc + df->segThick, df->colOn); // le rt
        (*(df->linevFun))(x + df->digWd - 1, y + hc + df->segThick + 1, y + df->digHt - 2, df->colOn); // rt
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->segThick - 2, df->colOn); // rt short
        break;
        case 7:
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x + df->digWd - df->segThick, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bottom
        (*(df->linehFun))(x, x + df->digWd - 1 - df->segThick, y + df->segThick - 1, df->colOn); // bottom2
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->digHt - 2, df->colOn); // rt-long
        (*(df->linevFun))(x, y + 1, y + df->segThick - 2, df->colOn);  // le-short
        (*(df->linevFun))(x + df->digWd - df->segThick, y + df->segThick - 1, y + df->digHt - 1, df->colOn); // rt-le-long
        break;
        case 8:
        DigiFont_rect(x, y, df->digWd, df->digHt, df->colOn);
        DigiFont_rect(x + df->segThick - 1, y + df->segThick - 1, df->digWd - df->segThick * 2 + 2, hc + 2, df->colOn);
        DigiFont_rect(x + df->segThick - 1, y + hc + df->segThick * 2 - 1, df->digWd - df->segThick * 2 + 2, df->digHt - hc - df->segThick * 3 + 2, df->colOn);
        break;
        case 9:
        DigiFont_rect(x + df->segThick - 1, y + df->segThick - 1, df->digWd - df->segThick * 2 + 2, hc + 2, df->colOn);
        (*(df->linehFun))(x, x + df->digWd - 1, y, df->colOn); // top
        (*(df->linehFun))(x, x + df->digWd - 1, y + df->digHt - 1, df->colOn); // bot
        (*(df->linehFun))(x, x + df->digWd - 1 - df->segThick, y + df->segThick * 2 + hc - 1, df->colOn); // mid
        (*(df->linehFun))(x, x + df->digWd - 1 - df->segThick, y + df->digHt - df->segThick, df->colOn); // bot-higher
        (*(df->linevFun))(x + df->digWd - 1, y + 1, y + df->digHt - 2, df->colOn); // rt-long
        (*(df->linevFun))(x, y + 1, y + hc + df->segThick * 2 - 2, df->colOn);  // le-top
        (*(df->linevFun))(x, y + df->digHt - df->segThick + 1, y + df->digHt - 2, df->colOn); // le-bot-short
        (*(df->linevFun))(x + df->digWd - df->segThick, y + hc + df->segThick * 2 - 1, y + df->digHt - df->segThick, df->colOn); // rt-le-bot
        break;
    }
    return df->digWd;
}
#endif

int DigiFont_printNumber1(char *txt, int x, int y)
{
    while(*txt)
        x+=DigiFont_drawDigit1(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

#if 0
int DigiFont_printNumber2(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigit2(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

int DigiFont_printNumber2c(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigit2c(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

int DigiFont_printNumber3(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigit3(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

int DigiFont_printNumberF(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigitF(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

int DigiFont_printNumberO(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigitO(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}

int DigiFont_printNumber7(char *txt, int x, int y)
{
    while(*txt) x+=DigiFont_drawDigit7(*txt++,x,y)+df->spacing;
    return x-df->spacing;
}
#endif

void DigiFont_setSpacing(int sp)
{
    df->spacing = sp;
}
void DigiFont_setClearBg(int cl)
{
    df->clearBg = cl;
}
void DigiFont_setColors(int c1, int c0)
{
    df->colOn=c1;
	df->colOn2=c1;
    df->colOff=c0;
}
void DigiFont_setColors2(int c1, int c2, int c0)
{
    df->colOn=c1;
    df->colOn2=c2;
    df->colOff=c0;
}
int DigiFont_getWidth()
{
    return df->digWd;
}
int DigiFont_getHeight()
{
    return df->digHt;
}

int DigiFont_numberWidth(char *txt)
{
    int wd=0;
    while(*txt)
    {
        wd+=((*txt=='.' || *txt==':')?df->segThick:df->digWd)+df->spacing;
        txt++;
    }
    return wd-df->spacing;
}