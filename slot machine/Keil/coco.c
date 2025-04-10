#include "all_x.h"
#include <reg52.h>

sbit up = P2^3;	//上一个
sbit down = P2^4;	//下一个
sbit left = P2^5;	//后退
sbit right = P2^6;	//前进

uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）

uchar now_button = 'n';	//n表示当前未按下； u、d、l、r 分别表示上、下、前进、后退
uchar now_mod = 't';    //当前功能选择：t计时器 ，或者g老虎机

uchar* select_t = "select:stopwatch";  //启动设备显示模式选择：秒表或者老虎机游戏
uchar* select_g = "select:slot game";



void main()
{ 
	uchar* str2 =    "  <back    >ok  ";
	
	
	while(1)  // 准备开始游戏
	{
		LcdInit();	
		LcdShowInit(select_t, 1, 0);	//显示初始设定的文本
		LcdShowInit(str2, 2, 0);
		int_timer0(); //初始化定时器0为模式二

		while(1)
		{
			change_mod();
		}

	}
}


void Timer0_ISR() interrupt 1   // 定时器0的中断 ,模式2无需手动重载TH0/TL0
{
    time_counter++;             // 累计中断次数
    
    if(time_counter >= 10000)      // 10000次×100μs=1000ms
    {
      time_counter = 0;
		}
}



void get_button(uint dead)	//每次调用检测哪个按键被按下,dead参数用于选择是否要死循环查找按键，0否1是
{
	while(dead)
	{
		if(up == 0 && down == 1 && left == 1 && right == 1)
		{
			now_button = 'u';
			break;
		}		
		else if(up == 1 && down == 0 && left == 1 && right == 1)
		{
			now_button = 'd';
			break;
		}
		else if(up == 1 && down == 1 && left == 0 && right == 1)
		{
			now_button = 'l';
			break;
		}
		else if(up == 1 && down == 1 && left == 1 && right == 0)
		{
			now_button = 'r';
			break;
		}
	}
	while(!(up && down  && left && right)){};   // 当每个按键都抬起时才能出去进行下一步
}
	

void change_mod()   //用于在开始界面更换模式,死循环，直到按下一个按键（每次响应一个按键，回退键当做前进键用）
{
	get_button(1); //开始界面死循环就好
	switch(now_button)
	{
		case 'u':
			if(now_mod == 't')
			{
				LcdShowInit(select_g, 1, 0);
				now_mod = 'g';
			}
			else
			{
				LcdShowInit(select_t, 1, 0);
				now_mod = 't';
			}
			break;
		case 'd':
			if(now_mod == 't')
			{
				LcdShowInit(select_g, 1, 0);
				now_mod = 'g';
			}
			else
			{
				LcdShowInit(select_t, 1, 0);
				now_mod = 't';
			}
			break;
		case 'l':
		case 'r':
			
			break;
	}
	now_button = 'n';  //清除当前按键
}



/*


*/





