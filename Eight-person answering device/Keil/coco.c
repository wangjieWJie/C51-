#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int

sbit BB = P2^4;   //蜂鸣器
sbit LED = P2^5;	
sbit ctrl = P2^6;	//主持人总控键
/* sfr button = P3;		选手抢答键
	 sfr display = P0;	八段数码管 */
sbit select1 = P2^0;	//数码管选段1,用于显示选中的选手编号
sbit select2 = P2^1;	//数码管选段2,
sbit select3 = P2^2;	//数码管选段3,
sbit select4 = P2^3;	//数码管选段4,

uchar num_seg[10] = {0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F}  //数码管共阴极字段码

void main()
{
	while(1)
	{
		while(!ctrl)  // 主持人按下按钮进入抢答
		{
			while(P3 != 0XFF) //某个按钮按下时进入循环，检测哪个按钮被按下,我的意思是一次循环那么快，就算两个按钮同时按下，也有先后顺序吧，也能检测到吧（Doge）
			{
				switch(P3)
				{
					case 0XFE:
						break;
					case 0XFD:
						break;
					case 0XFB:
						break;
					case 0XF7:
						break;
					case 0XEF:
						break;
					case 0XDF:
						break;
					case 0XBF:
						break;
					case 0X7F:
						break;
				}
			}	
		}
	}
}