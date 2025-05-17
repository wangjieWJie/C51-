#include "APAD.h"



sbit BB = P2^4;   //蜂鸣器
sbit LED = P2^5;	
sbit ctrl = P2^7;	//主持人总控键


uchar P1is; //记录一瞬间的按键
uchar disp_now = 0XFF;
uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）
uint BBS = 1;  // 表示蜂鸣器响多少秒，只要不为零就响，无需多余操作
uint gogogo = 0; //表示数码管倒计时多少秒，只要不为零就计时，无需多余操作
uint outwhile = 0;
uchar anxiazanting = 'N';

void main()
{
	int_timer0();	//闲着没事先初始化一个定时器，不用白不用
	BB = 0; //让蜂鸣器开局别响
	P3 = 0;//用于测试单片机功能
	
	LcdInit(); //初始化LCD
	
	while(1)
	{
		LcdShowInit("Ready to answer!", 1, 0);
		LcdShowInit("                ", 2, 0);
		if(!ctrl)  // 主持人按下按钮进入抢答
		{
			LcdShowInit("Time is going:  ", 1, 0);      //这些只显示一次后面就不动他了
			LcdShowInit("Who'll get it:  ", 2, 0);
			
			while(!ctrl);
			BBS = 1;
			BB = 1;  //可以不写这句话，但是不写的话蜂鸣器肯定会延时响应
			gogogo = 10; //十秒倒计时
			while(1)	
			{
				daojishi();
				if(outwhile == 1)
				{
					outwhile = 0;
					break;
				}
				
				if(P1 != 0XFF && disp_now == 0XFF) //某个按钮按下时进入，检测哪个按钮被按下,我的意思是一次循环那么快，就算两个按钮同时按下，也有先后顺序吧，也能检测到吧（Doge）
				{
					P1is = P1;
					easy_button();	//检测按下的按键，存入disp_now中，此行可保证这个条件语句只被执行一次
					LCDwAOne(disp_now, 2, 15); //检测完按键按下直接显示
					anxiazanting = 'Y';
					BBS = 1;	//蜂鸣器响两秒
					BB = 1;  //可以不写这句话，但是不写的话蜂鸣器肯定会延时响应
				}
				if(!ctrl)
				{
					while(!ctrl);
							BBS = 1;
							BB = 1;  //可以不写这句话，但是不写的话蜂鸣器肯定会延时响应
							disp_now = 0XFF;
							outwhile = 1;
							anxiazanting = 'N';
							
				}
			}
		}
	}
}
	

		
		
void daojishi()
{
						LCDwAOne(disp_now, 2, 15);

						if(gogogo > 0 && gogogo <= 10)
						{
							LCDwAOne('0' + gogogo-1, 1, 15);
						}
						else
						{
							anxiazanting = 'N';
							BBS = 1;
							BB = 1;  //可以不写这句话，但是不写的话蜂鸣器肯定会延时响应
							disp_now = 0XFF;
							outwhile = 1;
						}

	
}
		
		

void Timer0_ISR() interrupt 1   // 定时器0的中断 ,模式2无需手动重载TH0/TL0
{
    time_counter++;             // 累计中断次数
    
    if(time_counter >= 10000)      // 10000次×100μs=1000ms
    {
      time_counter = 0;
			if(BBS > 0)
			{
				BB = 1; //蜂鸣器BB响
				BBS--; //每秒减一
			}
			else
			{
				BB = 0; 
			}
				
			if(gogogo > 0 && anxiazanting=='N')  //倒计时
			{
				gogogo--;
			}
		}
}



void delay(uint ms)
{
	int i, j;
	for(; ms > 0; ms--)
		for (i = 2; i > 0; i--)
       for (j = 239; j > 0; j--);
}




void easy_button()	//检测按下的按键，存入disp_now中
{
	switch(P1is)   //检测按下的是个键
	{
					case 0XFE:
						disp_now = '1';   //记录下按下的按钮
						break;
					case 0XFD:
						disp_now = '2';
						break;
					case 0XFB:
						disp_now = '3';
						break;
					case 0XF7:
						disp_now = '4';
						break;
					case 0XEF:
						disp_now = '5';
						break;
					case 0XDF:
						disp_now = '6';
						break;
					case 0XBF:
						disp_now = '7';
						break;
					case 0X7F:
						disp_now = '8';
						break;
				}
}



void int_timer0()   // 初始化定时器0为模式2（自动重载）
{
    TMOD &= 0XF0;   // 清零定时器0的模式位（保留定时器1的配置）
    TMOD |= 0X02;   // 设置定时器0为模式2（8位自动重载）
    TH0 = 0xA4;     // 自动重载值（100μs定时初值，11.0592MHz）
    TL0 = 0xA4;     // 首次启动时的初值（必须与TH0相同）
    ET0 = 1;        // 允许定时器0中断
    TR0 = 1;        // 启动定时器0
    EA = 1;         // 开启全局中断
}

		


