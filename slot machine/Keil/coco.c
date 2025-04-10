#include <reg52.h>
#include <stdlib.h>

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
void int_timer0();

sbit LcdRs_P  = P2^0;		//LCD数据1/命令0   
sbit LcdRw_P  = P2^1;		//LCD读/写
sbit LcdEn_P  = P2^2;		//使能端(上升沿有效)

uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）




void main()
{ 
	uchar* str1 = " W J 2022121144 ";
	uchar* str2 = "  To My Heart   ";
	
	
	while(1)  // 准备开始游戏
	{
		LcdInit();	
		LcdShowInit(str2, 0, 0);
		LcdShowInit(str1, 1, 0);
		int_timer0(); //初始化定时器0为模式二

		while(1)
		{
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





void DelayMs(uint time)
{
	uint i,j;
	for(i=0;i<time;i++)
		for(j=0;j<112;j++);
}


void int_timer0()   // 初始化定时器0为模式2（自动重载）
{
    TMOD &= 0XF0;   // 清零定时器0的模式位（保留定时器1的配置）
    TMOD |= 0X02;   // 设置定时器0为模式2（8位自动重载）
    TH0 = 0xA4;     // 自动重载值（100μs定时初值，11.0592MHz）,每次TL0溢出时，自动重新装载为TH0
    TL0 = 0xA4;     // 首次启动时的初值（必须与TH0相同）
    ET0 = 1;        // 允许定时器0中断
    TR0 = 1;        // 启动定时器0
    EA = 1;         // 开启全局中断
}

void LcdWriteCmd(uchar cmd)	//写命令
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



void LcdWriteData(uchar dat)	//写数据
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



void LcdGotoXY(uchar line,uchar column)  //定位光标到XY处
{
	if(line==0)        
		LcdWriteCmd(0x80+column);   //第一行的地址从0X80开始
	if(line==1)        
		LcdWriteCmd(0x80+0x40+column); 	//第二行地址是第一个向后推40H
}



void LcdPrintStr(uchar *str)  // 输入字符串地址，向LCD写入数据，当前地址在哪就往哪写
{
	while(*str!='\0')   
			LcdWriteData(*str++);
}


void LcdInit()   //初始化液晶屏
{
	LcdWriteCmd(0x38);  // 16*2显示，5*7点阵，8位数据口      
	 LcdOn();       
	LcdWriteCmd(0x06);  // 地址加1，当写入数据后光标右移     
	LcdWriteCmd(0x01);  // 清屏
}


void LcdOn()   
{
	LcdWriteCmd(0x0c);  // 开显示，不显示光标
}
	
void LcdOff()  
{
	LcdWriteCmd(0x08);
}


void LcdShowInit(uchar *str_1, uchar Y, uchar X)  // 参数表示【输入字符串、输入行坐标（0或1）、输入列起始坐标（0~15）】
{
	LcdGotoXY(Y,X);	    		//定位光标					
	LcdPrintStr(str_1);			//写入字符串
}



uint wj_rand() 
{
		unsigned int random_nm;
		unsigned char high, low;
    unsigned int random;
           
    high = TH0;
    low = TL0;
		srand((high << 8) | low);
		random_nm = rand() % 5;
 
		return random_nm;
}
	


