#ifndef __LCD12864_H__
#define __LCD12864_H__

void lcm_w_word(unsigned char *str);
void lcm_w_test(bit start, unsigned char ddata);
void display_12864(unsigned char row, unsigned char col, unsigned char* str);
void lcm_init();
void PWM_init (void);
void PWM1_set (unsigned char a);
void lcm_clr(void);
void lcm_clr_up(void);
void reverse_show(unsigned char addr_x ,unsigned char addr_y ,unsigned char num);
void White_Line_Init();
void White_Line(unsigned char x,unsigned char y,unsigned char width);
void flash(void);
void flow(void);

void LCD_EnableGraphics(void);
void LCD_DisableGraphics(void);
//void LCD_FillScreenGraphics(const unsigned char* graphic);
//void LCD_GraphicsDisplayString(unsigned char line, char* str);//font 5x7
void LCD_ClearGraphics(void);

void printString3x5(const char* string, unsigned char leftX, unsigned char topY);
void drawHorizontalLine(unsigned char posY, unsigned char fromByte, unsigned char toByte, unsigned char pattern);

#endif