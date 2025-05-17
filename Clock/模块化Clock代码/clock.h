//clock.h
#include <reg52.h>

#define uchar unsigned char	
#define uint  unsigned int	
	

//main
void DelayMs(uint time);
extern uchar num_now;
sbit button = P2^7;
//sbit EX0_key = P3^2;  		// P3.2是外部中断0的官方指定引脚 
sbit button_LED = P2^4;    //键盘灯，当按键按下后亮起，按键抬起熄灭
sbit speaker = P2^5;

sbit pad_row1 = P1^0;   //键盘列
sbit pad_row2 = P1^1;
sbit pad_row3 = P1^2;

sbit pad_line1 = P1^3;  	//键盘行
sbit pad_line2 = P1^4;
sbit pad_line3 = P1^5;
sbit pad_line4 = P1^6;



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


//扫描键盘
void keypad_scan();


//基本时间
void int_timer0();
uint update_second();  
uint update_minute();
void update_hour();
void init_time();
void update_time();

extern uchar str_num[16]; 


//设置时间
void set_time();
void set_twinkle();


//设置闹钟
void lesGotoAlarm();
void alarm();
void set_alarming();
void on_off();     //掌管闹钟的开关
void twinkle();
void if_alarming();
