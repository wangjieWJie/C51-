//LCD液晶屏驱动
#include <reg52.h>
#include "all_x.h"

sbit LcdRs_P  = P2^0;		//LCD数据1/命令0   
sbit LcdRw_P  = P2^1;		//LCD读/写
sbit LcdEn_P  = P2^2;		//使能端(上升沿有效)


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
	if(line==1)        
		LcdWriteCmd(0x80+column);   //第一行的地址从0X80开始
	if(line==2)        
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

void LCD_one_data(uchar a_data, uchar Y, uchar X)    //一次只写一个字
{	
	LcdGotoXY(Y,X);	    		//定位光标		
	LcdWriteData(a_data);		//写入一个字
}
	





