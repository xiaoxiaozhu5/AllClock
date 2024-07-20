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

#endif