#ifndef __LCD12864_H__
#define __LCD12864_H__

void lcm_w_word(unsigned char *str);
void lcm_w_test(bit start, unsigned char ddata);
void display_12864(unsigned char row, unsigned char col, unsigned char* str);
void lcm_init();
void PWM_init (void);
void PWM1_set (unsigned char a);
void lcm_clr(void);
void reverse_show(unsigned char addr_x ,unsigned char addr_y ,unsigned char num);
void White_Line_Init();

void LCD_EnableGraphics(void);
void LCD_DisableGraphics(void);
//void LCD_FillScreenGraphics(const unsigned char* graphic);
//void LCD_GraphicsDisplayString(unsigned char line, char* str);//font 5x7
void LCD_ClearGraphics(void);

void printString3x5(const char* string, unsigned char leftX, unsigned char topY);
void drawHorizontalLine(unsigned char posY, unsigned char fromByte, unsigned char toByte, unsigned char pattern);
void drawHorizontalLine2(unsigned char posY, unsigned char fromByte, unsigned char toByte, unsigned char pattern) reentrant;
void drawVerticalLine(unsigned char posX, unsigned char fromY, unsigned char toY, unsigned char style, unsigned char pattern) reentrant;
void drawRectangle(unsigned char fromX, unsigned char fromY, unsigned char toX, unsigned char toY, unsigned char fill, unsigned char style) reentrant;


#endif