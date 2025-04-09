#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int

void easy_button();
void display(uchar dispN, uint S);
void int_timer0();
void delay(uint ms);


sbit BB = P2^4;   //蜂鸣器
sbit LED = P2^5;	
sbit ctrl = P2^7;	//主持人总控键
/* sfr button = P3;		选手抢答键
	 sfr display = P0;	八段数码管 */
sbit select1 = P2^0;	//数码管选段1,用于显示选中的选手编号
sbit select2 = P2^1;	//数码管选段2,
sbit select3 = P2^2;	//数码管选段3,
sbit select4 = P2^3;	//数码管选段4,

uchar P3is; //记录一瞬间的按键
uchar num_seg[10] = {0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};  //数码管共阴极字段码
uchar disp_now = 0X88;
uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）
uint BBS = 1;  // 表示蜂鸣器响多少秒，只要不为零就响，无需多余操作
uint gogogo = 0; //表示数码管倒计时多少秒，只要不为零就计时，无需多余操作

void main()
{
	int_timer0();	//闲着没事先初始化一个定时器，不用白不用
	BB = 0; //让蜂鸣器开局别响
	while(1)
	{
		if(!ctrl)  // 主持人按下按钮进入抢答
		{
			BBS = 1;
			while(1)
			{
				if(P3 != 0XFF && disp_now == 0X88) //某个按钮按下时进入循环，检测哪个按钮被按下,我的意思是一次循环那么快，就算两个按钮同时按下，也有先后顺序吧，也能检测到吧（Doge）
				{
					P3is = P3;
					easy_button();	//检测按下的案件，存入disp_now中，此行可保证这个条件语句只被执行一次
					display(disp_now, 1); //检测完按键按下直接显示
					BBS = 2;	//蜂鸣器响两秒
					gogogo = 10; //十秒倒计时
				}
				else if(disp_now != 0X88) // 表示按键按下并记录完成，进入下一步，开始十秒倒计时
				{
					//display(disp_now, 1);      
					if(gogogo > 0 && gogogo <= 10)
					{
						display(num_seg[gogogo-1],2);
					}
					else
					{
						BBS = 2;
						disp_now = 0X88;
						break;
					}
				}
				//delay(500);
			}
		}
	}
}





void delay(uint ms)
{
	uint i,j;
	for(i = 52;i>0;i--)
	{
		for(j=ms;j>0;j--)
		{
		}
	}
}


void display(uchar dispN, uint S)	//数码管显示，参数为显示数字和显示位置
{
	switch(S)
	{
		case 1:
			select1 = 0;
			select2 = 1;
			select3 = 1;
			select4 = 1;
			P0 = dispN;
			select1 = 1;
			select2 = 1;
			select3 = 1;
			select4 = 1;
			break;
		case 2:
			select1 = 1;
			select2 = 0;
			select3 = 1;
			select4 = 1;
			P0 = dispN;
			select1 = 1;
			select2 = 1;
			select3 = 1;
			select4 = 1;
			break;
		case 3:
			select1 = 1;
			select2 = 1;
			select3 = 0;
			select4 = 1;
			P0 = dispN;
			select1 = 1;
			select2 = 1;
			select3 = 1;
			select4 = 1;
			break;
		case 4:
			select1 = 1;
			select2 = 1;
			select3 = 1;
			select4 = 0;
			P0 = dispN;
			select1 = 1;
			select2 = 1;
			select3 = 1;
			select4 = 1;
			break;
		delay(1);
	}
}


void easy_button()	//检测按下的按键，存入disp_now中
{
	switch(P3is)   //检测按下的是个键
	{
					case 0XFE:
						disp_now = num_seg[1];   //记录下按下的按钮
						break;
					case 0XFD:
						disp_now = num_seg[2];
						break;
					case 0XFB:
						disp_now = num_seg[3];
						break;
					case 0XF7:
						disp_now = num_seg[4];
						break;
					case 0XEF:
						disp_now = num_seg[5];
						break;
					case 0XDF:
						disp_now = num_seg[6];
						break;
					case 0XBF:
						disp_now = num_seg[7];
						break;
					case 0X7F:
						disp_now = num_seg[8];
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
				
			if(gogogo > 0 )  //倒计时
			{
				gogogo--;
			}
		}
}
		


