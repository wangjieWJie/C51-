#include "all_x.h"
#include <reg52.h>
#include <intrins.h>

sbit up = P2^3;			//上一个
sbit down = P2^4;		//下一个
sbit left = P2^5;		//后退
sbit right = P2^6;	//前进

sbit LED = P1^0;

uchar now_button = 'n';	//n表示当前未按下； u、d、l、r 分别表示上、下、前进、后退
uchar now_mod = 't';    //当前功能选择：t计时器的选择界面 ，或者g老虎机的选择界面，T计时器运行界面 ，或者G老虎机的运行界面

uchar* select_t = "select:stopwatch";  //启动设备显示模式选择：秒表或者老虎机游戏
uchar* select_g = "select:slot game";



void main()
{ 
	uchar* str2 =    " back:<    ok:> ";
	
	while(1)  // 准备开始游戏
	{
		LcdInit();   //初始化液晶屏
		LcdShowInit(select_t, 1, 0);	//显示初始设定的文本
		LcdShowInit(str2, 2, 0);
		int_timer0(); 	//初始化定时器0为模式2
		int_timer1();   //初始化定时器1为模式1


		while(1)
		{
			if(now_mod=='t'|| now_mod=='g')  //如果在加载界面就显示模式，伺机切换或者进入模式
			{
				change_mod();    //模式切换
			}
			else if(now_mod=='T'|| now_mod=='G') //确认进入界面之后的东西，其实这条选择语句没必要写，但是可以起保护程序的作用
			{
				switch(now_mod)
				{
					case 'T':  //进入秒表
						TR0 = 1;        // 开启定时器0
						ET0 = 1;        // 允许定时器0中断
						timing();				//秒表模式
					
						ET0 = 0;        // 禁止定时器0中断，节约资源
						LcdShowInit(select_t, 1, 0);	//退出秒表模式时重置起始界面
						LcdShowInit(str2, 2, 0);
						break;
					
					case 'G':  //进入游戏
						slotgame();
					
						LcdShowInit(select_t, 1, 0);	//退出游戏模式时重置起始界面
						LcdShowInit(str2, 2, 0);					
						break;
				}
			}
			
		}

	}
}




void get_button(uint dead)	//每次调用检测哪个按键被按下,dead参数用于选择是否要死循环查找按键，1死循环，0不执行 ，其他整数不死循环，此函数用完一定要记得恢复按键状态，否则将会导致不可描述的错误
{
	now_button = 'n';  //清除上一个按键
	while(dead)
	{
		if(!(up&&down&&left&&right))  
		{
			_nop_();_nop_();_nop_();  // 简单按键消抖
			
			if(up == 0 && down == 1 && left == 1 && right == 1)
			{
				now_button = 'u';
				LED = 0; //只作为测试使用
				break;
			}		
			else if(up == 1 && down == 0 && left == 1 && right == 1)
			{
				now_button = 'd';
				LED = 0; //只作为测试使用
				break;
			}
			else if(up == 1 && down == 1 && left == 0 && right == 1)
			{
				now_button = 'l';
				LED = 0; //只作为测试使用
				break;
			}
			else if(up == 1 && down == 1 && left == 1 && right == 0)
			{
				now_button = 'r';
				LED = 0; //只作为测试使用
				break;
			}
		}
		if(dead != 1){break;}  //只有dead等于一才能死循环
	}
	while(!(up && down  && left && right)){};   // 当每个按键都抬起时才能出去进行下一步
	LED = 1;
	
}
	



void change_mod()   //用于在开始界面更换模式,死循环，直到按下一个按键（每次响应一个按键，回退键当做前进键用）
{
	get_button(1); //开始界面死循环就好
	switch(now_button)
	{
		case 'u':                     //点击上下键切换模式：秒表或者老虎机
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
			
			
		case 'l':                   //点击左右键进入模式
		case 'r':
			switch(now_mod)
			{
				case 'g':
					LcdShowInit("Strat:>   Back:<", 2, 0);
					now_mod -= 32;   //小写字母加32变成大写字母G
					break;
				case 't':
					LcdShowInit("Pause:U  Clear:D", 1, 0);
					now_mod -= 32;  
					break;
			}
			break;
	}
}



/*


*/





