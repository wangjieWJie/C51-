//timer disp
#include "clock.h"


bit time_update_flag = 0; //时间更新标志，每分钟变为1之后分钟更新，然后变回0

uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）

uchar str_num[16];     // 第二行的所有的时间显示   18：24：18   总共需要8位  "Time is 18:24:18"



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
				time_update_flag = 1;  // 仅在每一分钟时设置标志位,防止直接加载update_time()导致计时延时不准确
    
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
		time_update_flag = 0;
		
		//每分钟检测一次闹钟
		if_alarming();

	}
}

