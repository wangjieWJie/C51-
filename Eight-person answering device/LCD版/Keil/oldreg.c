#include "APAD.h"

sbit select1 = P2^0;	//数码管选段1,用于显示选中的选手编号
sbit select2 = P2^1;	//数码管选段2,
sbit select3 = P2^2;	//数码管选段3,
sbit select4 = P2^3;	//数码管选段4,


uchar num_seg[12] = {0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0X88,0X83};  //数码管共阳极字段码


void display(uchar dispN, uint S)	//数码管显示，参数为显示数字和显示位置
{
	switch(S)
	{
		case 1:
			select1 = 1;
			select2 = 0;
			select3 = 0;
			select4 = 0;
		case 2:
			select1 = 0;
			select2 = 1;
			select3 = 0;
			select4 = 0;
			break;
		case 3:
			select1 = 0;
			select2 = 0;
			select3 = 1;
			select4 = 0;
			break;
		case 4:
			select1 = 0;
			select2 = 0;
			select3 = 0;
			select4 = 1;	
			break;
	}
	P0 = dispN;
		
	delay(1);
				
	select1 = 0;
	select2 = 0;
	select3 = 0;
	select4 = 0;
}
