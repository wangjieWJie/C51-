//全局变量、函数定义、储存在这
/*



*/




#define uchar unsigned char	
#define uint  unsigned int	
	

//main
void get_button(uint dead);	//每次调用检测哪个按键被按下,dead参数用于选择是否要死循环查找按键，0否1是
void change_mod();   //用于在开始界面更换模式

extern uchar now_mod;    //当前功能选择：t计时器的选择界面 ，或者g老虎机的选择界面，T计时器运行界面 ，或者G老虎机的运行界面
extern uchar now_button;	//n表示当前未按下； u、d、l、r 分别表示上、下、前进、后退



//LCD 
void LcdWriteCmd(uchar cmd);	//LCD写命令
void LcdWriteData(uchar dat);	//LCD写数据
void LcdGotoXY(uchar line,uchar column);	//LCD定位光标
void LcdPrintStr(uchar *str);	//LCD写入&显示字符串
void LcdOff();
void LcdOn();
void LcdInit();   //初始化液晶屏
void LcdShowInit(uchar *str_1, uchar Y, uchar X);	//LCD显示总控，输入位置、字符串，显示在LCD规定的位置
void LCD_one_data(uchar a_data, uchar Y, uchar X);    //一次只写一个字
	

//StopWatch
void int_timer0();	//定时器0初始化
void DelayMs(uint time);	//延时
uint update_second();  // 更新秒，返回一个进位标志，感觉虽然str_num是uchar类型的，但是加一确实可以让他加一[doge]
uint update_minute();  // 更新分钟
void update_hour();  // 更新小时，为了省事直接偷了之前写过的代码，只能计数到24小时，就这样吧，懒得改
void timing();   //秒表的总控函数
void update_time(); //每次执行都会让秒加一，连带着分钟和小时变化

extern uchar str_num[16];  //总时间显示，coco.c和stopwatch.c都要用


//Slotgame
uint wj_rand(); 	//生成随机数
void slotgame();  //老虎机游戏总控
void gaming();  //进行游戏
void start_game();  //启动初始化游戏
void int_timer1();     //定时器1的初始化
void speed_disp();   //可以调速的显示（只显示第二行最后五位）







