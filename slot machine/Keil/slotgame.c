//slot game 老虎机相关函数
#include <reg52.h>
#include <stdlib.h>
#include "all_x.h"


uint count1 = 0;   //定时器1计数器

int speed0[5] = {100,150,200,250,300}; 	//在游戏开始时，每位的初始循环速度都应该不同，这里单独控制每一位的循环速度，达到丝滑效果，这个值是一直变化的
int speed1[6] = {150,160,170,180,190,-1};		//用于赋给speed0的新值，第6位是标志位，1表示速度减，-1表示速度加，第六位以后表示加速度(migntianzainong)

uchar Lucky_alphabet[5] = "XXXXX";
uchar i = 0;


void slotgame()  //老虎机游戏总控
{
	LcdShowInit("Top score:000000", 1, 0);
	
		while(now_mod == 'G')  //点击右键R开始游戏，左键L退出
		{			
			get_button(5);   //检测按键，以便于开始或者退出
			
			if(now_button == 'l')  //刚进入模式，单击左键退出当前模式
			{
				now_mod = 'g';		//将状态调整到上一个状态
				break;
			}
			
			else if(now_button == 'r') 								//刚进入模式，单击右键开始游戏
			{
				TR1 = 1;            										// 开启定时器1，让后台开始随机产生随机字母
				LcdShowInit("Your Lucky:XXXXX", 2, 0);  //奠定一下基础，先把不变的东西写上去
				gaming();  															//游戏死循环，直到所有字母都停止随机并记完分数之后退出
				break;
			}				
		}			
}



void Timer1_ISR() interrupt 3      //定时器1中断
{
	TH0 = 0x04; ;         // 手动重装初值
  TL0 = 0x00;
  if(++count1 >= 14)  // 每次70ms，count1最大值乘以70ms就是计时的毫秒数   70ms*14 = 0.98s
	{
		count1 = 0;
	}
	
}



void gaming()    //进行游戏
{
	while(1)		//算了，开始游戏之后就不能退出了，不然一直检测按钮太卡了
	{
				
		for(i=0; i<5; i++)   
		{
			Lucky_alphabet[i] = 'A' + wj_rand();
		}
		
		speed_disp();
		
		for(i=0; i<5; i++){}
	}
	
}


void speed_disp()    //可以调速的显示（只显示第二行最后五位）
{
	for(i=0;i<5;i++)     //每位的调速
	{
		if(!(speed0[i]--)) { 
			LCD_one_data(Lucky_alphabet[i], 2, i+11);
			
			speed0[i] = speed1[i];
						
			if(speed1[i] > 20)
			{
				speed1[i] = speed1[i] + (speed1[5]*20);  //速度越来越快
			}
		}
	}
}




void int_timer1()     //定时器1的初始化
{
    // 模式配置（不影响定时器0的设置）
    TMOD &= 0x0F;       // 清除T1配置位
    TMOD |= 0x10;       // 设置模式1
    
		 // 计算50ms定时初值（11.0592MHz）
    TH1 = 0x04;         // 高位初值  65536 - 1024 = 64512；  64512 * 1.085069444us = 69,999.999971us = 70ms
    TL1 = 0x00;         // 初始值								  12 / 11.0592MHZ = 1.08506944444us↑						
    
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
	



