#ifndef LCD12864_H_
#define LCD12864_H_

extern void LCD12864_wrCmd(char cmd);
extern void LCD12864_wrDat(char cmd);
extern void DisplayCgrom(char addr,char *hz);
extern void Display(void);
extern void LCD12864_init();
#endif