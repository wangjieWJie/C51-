//slot game 老虎机相关函数
#include <reg52.h>
#include <stdlib.h>
#include "all_x.h"


uint count1 = 0;   //定时器1计数器

uchar Lucky_alphabet[5] = "XXXXX";
uchar if_start = 'N';    //游戏是否 已经开始，是Y 没开始N
uchar i = 0;


void slotgame()  //老虎机游戏总控
{
	LcdShowInit("Top score:000000", 1, 0);
	
		while(now_mod == 'G')  //点击右键R开始游戏，左键L退出
		{			
			get_button(5);   //检测按键，以便于暂停或者退出，或者将时间pin在第一行
			if(now_button == 'l')  //退出当前模式
			{
				now_mod = 'g';		//将状态调整到上一个状态
				now_button = 'n';  	//恢复按键状态
				break;
			}
			
			 
			else if(now_button == 'r') //   暂停/开始游戏
			{
				switch(if_start)
				{
					case 'N':        //如果此时游戏未开始
						start_game();
						break;
					
					case 'Y':				 //如果此时游戏已经开始
						gaming();
						break;
				}
				
			}			
		}

}

void Timer1_ISR() interrupt 3      //定时器1中断
{
	TH0 = 0x4C;         // 手动重装初值
  TL0 = 0x00;
  if(++count1 >= 20)  // 50ms×20=1秒
	{
		for(i=0; i<3; i++)
		{
			Lucky_alphabet[i] = 'A' + wj_rand();
		}
    
		
		count1 = 0;
	}
	
}

void start_game()  //启动初始化游戏
{
	LcdShowInit("Your Lucky:XXXXX", 2, 0);
	while(1)
	{
		get_button(6);
		
		if(now_button == 'l')  //退出当前模式
		{
			now_mod = 'g';		
			now_button = 'n';  	//恢复按键状态
			break;
		}
		else if(now_button == 'r')  //正式启动游戏
		{
			if_start = 'Y';			//游戏状态切换到进行中
			now_button = 'n';  	//恢复按键状态
			break;
		}	
	}

}




void gaming()    //进行游戏
{
	TR1 = 1;            // 开启定时器1
	while(1)
	{
		get_button(6);
		if(now_button == 'l')  //退出当前模式
		{
			now_mod = 'g';		
			now_button = 'n';  	//恢复按键状态
			break;
		}
		else if(now_button == 'r')  //游戏缓缓结束，显示分数
		{
			
			
			
			
			now_button = 'n';  	//恢复按键状态
			break;
		}	
	}
	
}


void int_timer1()     //定时器1的初始化
{
    // 模式配置（不影响定时器0的设置）
    TMOD &= 0x0F;       // 清除T1配置位
    TMOD |= 0x10;       // 设置模式1
    
		 // 计算50ms定时初值（11.0592MHz）
    TH1 = 0x4C;         // 高位初值（19456/256=0x4C）
    TL1 = 0x00;         // 初始值（11.0592MHz晶振计算）[5](@ref)
    
    // 中断配置
    ET1 = 1;            // 开启定时器1中断
    TR1 = 0;            // 暂时关闭定时器1
	
	  //EA = 1;         // 开启全局中断,定时器0到是已经开启过了

}




uint wj_rand() 	//生成随机数
{
		uint random_nm;
		uchar low, high;
           
    high = TH1;  //利用定时器的不断变换的高低八位来随机生成随机数
    low = TL1;		//定时器低八位
		srand(low<<8 | high);    //创意修改，让高位成为低位，低位成为高位，使得没次调用时，srand相差更大
		random_nm = rand() % 3;  //生成0-2之间的随机数
 
		return random_nm;		//返回生成的随机数
}
	



