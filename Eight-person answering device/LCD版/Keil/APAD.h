#include <reg52.h>

#define uchar unsigned char	
#define uint  unsigned int	


void easy_button();
void display(uchar dispN, uint S);
void int_timer0();
void delay(uint ms);
void daojishi();



//LCD
void LcdWriteCmd(uchar cmd);
void LcdWriteData(uchar dat);
void LcdGotoXY(uchar line,uchar column);
void LcdPrintStr(uchar *str);
void LcdOff();
void LcdOn();
void LcdInit();
void LcdShowInit(uchar *str_1, uchar Y, uchar X);
void LCDwAOne(uchar dat, uchar Y, uchar X);  //每次只写一个字的函数




void DelayMs(uint time);


void display(uchar dispN, uint S);	//数码管显示，参数为显示数字和显示位置
