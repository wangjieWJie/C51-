//全局变量、函数定义、储存在这

#define uchar unsigned char	
#define uint  unsigned int	


void DelayMs(uint time);	//延时
void LcdWriteCmd(uchar cmd);	//LCD写命令
void LcdWriteData(uchar dat);	//LCD写数据
void LcdGotoXY(uchar line,uchar column);	//LCD定位光标
void LcdPrintStr(uchar *str);	//LCD写入&显示字符串
void LcdOff();
void LcdOn();
void LcdInit();	//LCD初始化
void LcdShowInit(uchar *str_1, uchar Y, uchar X);	//LCD显示总控，输入位置、字符串，显示在LCD规定的位置
void int_timer0();	//定时器0初始化
void get_button(uint dead);	//每次调用检测哪个按键被按下,dead参数用于选择是否要死循环查找按键，0否1是
void change_mod();   //用于在开始界面更换模式


//extern uint time_counte;



