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
void int_timer0();
uint update_second();  
uint update_minute();
void update_hour();
void init_time();
void update_time();
void int_timer0();
void set_time();


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

sbit button = P2^7;

uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）
bit time_update_flag = 0; //时间更新标志，每分钟变为1之后分钟更新，然后变回0

uchar num_now = 88;    // 当前输入数字,88表示当前没有数字输入,11表示*，12表示#
uchar str_num[16];     // 第二行的所有的时间显示   18：24：18   总共需要8位  "Time is 18:24:18"
																																				//"     		8  11 14"  '0'的ascaii码是48，九是57  冒号是58
uint set_now = 1;  // 设置时间时确定现在正在设置哪一位



//P3_0 = 1;  // 0表示没有抬起话机，此时不显示电话
sbit LED = P2^6;


void main()
{ 
	uchar* str1 = " WYM 2022121218 ";
	uchar* str2 = "  To My Heart   ";
	
	
	while(1)  // 准备待机调整时间或者开始计时
	{
		LcdInit();	
		LcdShowInit(str2, 0, 0);
		LcdShowInit(str1, 1, 0);
	
		int_timer0(); //初始化定时器0为模式二
		init_time();  //将str_num初始化为"Time is 18:24:18" 的形式然后显示

		while(1)
		{
			LED = button;
			if(button == 0)     // 显示时间 
			{
				
				LcdShowInit(str_num, 1, 0);  //将第二行从头开始重写，有点耗时间
				update_time();							
			}
			else if(button == 1)     // 调整时间 
			{
				TR0 = 0;        // 关闭定时器0
				ET0 = 0;				// 关闭中断
				
				set_time(); 
				
				TR0 = 1;        // 重新启动定时器0
				ET0 = 1;				// 开启中断
			}
		}

	}
}




void keypad_scan()   //逐列扫描键盘
{
	num_now = 88;  //自动重设，自动死循环，太高级了
	while(num_now == 88 && button == 1)  //当前没数的时候死循环找数，但是此时必须是调整时间时
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
			if(update_second())
			{
				time_update_flag = 1;  // 仅在一分钟时设置标志位,防止直接加载update_time()导致计时延时不准确
    
			}
		}
}

uint update_second()  // 更新秒，返回一个进位标志，感觉虽然str_num是uchar类型的，但是加一确实可以让他加一[doge]
{
	if(str_num[15] < '9')  // 57 = '9'
	{
		str_num[15]++;
	}
	else 	 //秒的个位的进位
	{
		str_num[15] = '0';   // 48 = '0'
		
		if(str_num[14] < '5')  // 53 = '5'   
		{
			str_num[14]++;
		}
		else   	//秒的十位的进位
		{
			str_num[14] = '0';
			return 1;  // 秒记到59就变零进一
		}
	}
	return 0; 	
}

uint update_minute()  // 更新分钟
{
	if(str_num[12] < '9')  // 57 = '9'
	{
		str_num[12]++;
	}
	else 	 //分钟的个位的进位
	{
		str_num[12] = '0';   // 48 = '0'
		
		if(str_num[11] < '5')  // 53 = '5'   
		{
			str_num[11]++;
		}
		else   	//分钟的十位的进位
		{
			str_num[11] = '0';
			return 1;  // 分钟记到59就变零进一
		}
	}
	return 0; 	
}


void update_hour()  // 更新小时，每次调用都会加一或置零，所以最后23时必须置到00
{
	if(str_num[9] < '9' && str_num[8] < '2')  // 00~08  10~18 时个位加一
	{
		str_num[9]++;
	}
	else if(str_num[9] == '9' && str_num[8] < '2') 	 //09 19 时个位变0，十位加一
	{
		str_num[9] = '0';   // 48 = '0'
		str_num[8]++;
	}	
	else if(str_num[9] < '3' && str_num[8] == '2')   // 20 21 22 时个位加一,最大为23
	{
		str_num[9]++;
	}
	else if(str_num[9] == '3' && str_num[8] == '2')  // 23在先前进位为00
	{
		str_num[9] = '0';
		str_num[8] = '0';
	}
}


void init_time()   // 初始化第二行的显示，让他显示我想要的
{
	uint i;
	uchar inti[16] = {'T','i','m','e',' ','i','s',' ','0','0',':','0','0',':','0','0'};  // "Time is 00:00:00"
	for (i = 0; i < 16; i++) {
        str_num[i] = inti[i];  
	}
}


void update_time()   //每次执行都会让秒加一，连带着分钟和小时变化
{
	if(time_update_flag)
	{
		if(update_minute())
		{
			update_hour();
		}
	}
	time_update_flag = 0;

}

void set_time()     //设置时间
{
		
		LcdShowInit(str_num, 1, 0);  //将第二行从头开始重写，有点耗时间
		keypad_scan();
		switch(set_now)
		{			
			case 1:
				if(num_now < 3)
				{
					str_num[8] = num_now + '0';
					LcdShowInit(str_num, 1, 0);
					set_now++;
				}
				break;
				
			case 2: 
				if(num_now < 10)
				{
					str_num[9] = num_now + '0';
					LcdShowInit(str_num, 1, 0);  
					set_now++;
				}
				break;


			case 3:		
				if(num_now < 6)
				{
					str_num[11] = num_now + '0';
					LcdShowInit(str_num, 1, 0);  
					set_now++;
				}
				break;
		
			case 4:	
				if(num_now < 10)            
				{
					str_num[12] = num_now + '0';
					LcdShowInit(str_num, 1, 0); 
					set_now = 1;
				}
				break;
		}	
}


