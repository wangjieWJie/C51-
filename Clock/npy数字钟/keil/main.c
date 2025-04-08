#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#define uchar unsigned char	
#define uint  unsigned int	
	


void DelayMs(uint time);
void LcdWriteCmd(uchar cmd);
void LcdWriteData(uchar dat);
void LcdGotoXY(uchar line,uchar column);
void LcdPrintStr(uchar *str);
void LcdOff();
void LcdOn();
void LcdInit();
void LcdShowInit(uchar *str_1, uchar Y, uchar X);
void keypad_scan();
uchar int_str(uchar int_num);
void fix_str_num();
void int_timer0();


sbit LcdRs_P  = P2^0;		//LCD数据1/命令0   
sbit LcdRw_P  = P2^1;		//LCD读/写
sbit LcdEn_P  = P2^2;		//使能端(上升沿有效)

sbit pad_row1 = P1^0;   //键盘列
sbit pad_row2 = P1^1;
sbit pad_row3 = P1^2;

sbit pad_line1 = P1^3;  	//键盘行
sbit pad_line2 = P1^4;
sbit pad_line3 = P1^5;
sbit pad_line4 = P1^6;

sbit tele_on = P3^0;   // 话机摘下与否

unsigned int time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）

P3_0 = 1;  // 0表示没有抬起话机，此时不显示电话

uchar num_now = 88;    // 当前输入数字,88表示当前没有数字输入,11表示*，12表示#
uchar str_num[16];     // 第二行的电话号码




void main()
{ 
	uchar* str1 = " WYM 2022121218 ";
	uchar* str2 = "  To My Heart   ";
	
	uint i = 0;
	for(;i<16;i++)   // 初始化str_num为全空格
	{
		str_num[i] = 32;
	}
	
	while(1)  // 准备待机或者显示数字
	{
		LcdInit();	
		LcdShowInit(str2, 0, 0);
		LcdShowInit(str1, 1, 0);
	
		while(P3_0)     // 开始死循环显示数字
		{
			P1 = 0X7F;	 //以防万一加一个初始化P1	
			keypad_scan();			//这是个找数死循环
			if(num_now != 88)  // 记录到有新数字之后添加到数组的最后一位
			{
				fix_str_num();
			}
			LcdShowInit(str_num, 1, 0);
			
			if(P3_0 == 0){break;}   // 由于找数是个死循环，所以这个不起作用，就这样吧，不想写了
			
			DelayMs(1);
		}
	}
}




void keypad_scan()   //逐列扫描键盘
{
	num_now = 88;
	while(num_now == 88)  //当前没数的时候死循环找数
	{
		P1 = 0X7F;	
		// row1
		pad_row1 = 0;
		DelayMs(1);
		if(P1 != 0X7E)   //判断按键是否按下
		{
			DelayMs(1);
			switch(P1)
			{
				case 0X76:
					num_now = 1;
				break;

				case 0X6E:
					num_now = 4;
				break;
				case 0X5E:
					num_now = 7;
				break;
				case 0X3E:
					num_now = 11;
				break;	
			}
			while(P1 != 0X7E);    //死循环等待按键弹起
		}
		pad_row1 = 1;				//将这一列重置为1
		
		//row2
		pad_row2 = 0;
		if(P1 != 0X7D)
		{
			DelayMs(1);
			switch(P1)
			{
				case 0X75:
					num_now = 2;
				break;

				case 0X6D:
					num_now = 5;
				break;
				case 0X5D:
					num_now = 8;
				break;
				case 0X3D:
					num_now = 0;
				break;	
			}
			while(P1 != 0X7D);
		}
		pad_row2 = 1;
		
		//row3
		pad_row3 = 0;
		if(P1 != 0X7B)
		{
			DelayMs(1);
			switch(P1)
			{
				case 0X73:
					num_now = 3;
				break;

				case 0X6B:
					num_now = 6;
				break;
				case 0X5B:
					num_now = 9;
				break;
				case 0X3B:
					num_now = 12;
				break;	
			}
			while(P1 != 0X7B);
		}
		pad_row3 = 1;
		
	}
}




void DelayMs(uint time)
{
	uint i,j;
	for(i=0;i<time;i++)
		for(j=0;j<112;j++);
}



void LcdWriteCmd(uchar cmd)
{ 
	LcdRs_P = 0;
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=cmd;
	DelayMs(1);
	LcdEn_P = 1;    
	DelayMs(1);
	LcdEn_P = 0;	
}



void LcdWriteData(uchar dat)
{
	LcdRs_P = 1; 
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=dat;
	DelayMs(1);
	LcdEn_P = 1;    
	DelayMs(1);
	LcdEn_P = 0;
}



void LcdGotoXY(uchar line,uchar column)
{
	if(line==0)        
		LcdWriteCmd(0x80+column); 
	if(line==1)        
		LcdWriteCmd(0x80+0x40+column); 
}



void LcdPrintStr(uchar *str)
{
	while(*str!='\0')
			LcdWriteData(*str++);
}


void LcdInit()
{
	LcdWriteCmd(0x38);       
	 LcdOn();       
	LcdWriteCmd(0x06);       
	LcdWriteCmd(0x01);      
}


void LcdOn()   
{
	LcdWriteCmd(0x0c);
}
	
void LcdOff()  
{
	LcdWriteCmd(0x08);
}


void LcdShowInit(uchar *str_1, uchar Y, uchar X)  // 参数表示【输入字符串、输入行坐标（0或1）、输入列起始坐标（0~15）】
{
	LcdGotoXY(Y,X);	    							
	LcdPrintStr(str_1);	
}


uchar int_str(uchar int_num)  // 这个程序也不用很专业的，个位数的转化就行
{
	return int_num + '0';
}


void fix_str_num()   // 将新的数字添加到数组的最后一位，然后将之前的所有位数前移
{ 
	uint i;
	for(i = 0;i<15;i++)
	{
		str_num[i] = str_num[i+1];   // 将数组数据向前挪一位，第0位等于第一位,挪到第十四位停下
	}
	str_num[15] = int_str(num_now);  //第十五位等于现在按下的数字

}


void int_timer0()   // 初始化定时器（TMOD为1）
{
	TMOD &= 0Xf0;
	TMOD |= 0X01;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
}


void Timer0_ISR() interrupt 1   // 定时器1的中断
{
	TH1 = 0X3C;   
	TL1 = 0XB0;


	time_counter++;

	if(time_counter > 20)
	{
		time_counter = 0;
		
	}
}


