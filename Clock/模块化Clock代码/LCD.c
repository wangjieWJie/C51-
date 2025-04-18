//LCD相关函数
#include "clock.h"


sbit LcdRs_P  = P2^0;		//LCD数据1/命令0   
sbit LcdRw_P  = P2^1;		//LCD读/写
sbit LcdEn_P  = P2^2;		//使能端(上升沿有效)



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


void LCDwAOne(uchar dat, uchar Y, uchar X)  //每次只写一个字的函数
{
	LcdGotoXY(Y,X);	    							
	LcdWriteData(dat);
}




