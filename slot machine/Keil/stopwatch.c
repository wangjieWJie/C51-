//stopwatch秒表相关函数
#include <reg52.h>
#include "all_x.h"


uint time_counter = 0;   //定时器的计数器，每记20次表示过了1秒（50ms*20 = 1s）
bit time_update_flag = 0; //时间更新标志，每分钟变为1之后分钟更新，然后变回0

uchar str_num[16] = {'T','i','m','i','n','g',':',' ','0','0',':','0','0',':','0','0'};     // 第二行的所有的时间显示   18：24：18   总共需要8位  "Timing: 18:24:18"

uchar inti_str_num[16] = {'T','i','m','i','n','g',':',' ','0','0',':','0','0',':','0','0'};  //秒表一开始显示的文本

uchar pin_str_num[16] = {'T','i','m','e','P','i','n',':',' ',' ',':',' ',' ',':',' ','0'};

void Timer0_ISR() interrupt 1   // 定时器0的中断 ,模式2无需手动重载TH0/TL0
{
    time_counter++;             // 累计中断次数
    
    if(time_counter >= 10000)      // 10000次×100μs=1000ms
    {
      time_counter = 0;   //重新设置一秒标志
			
			if(update_second())  //每秒更新
			{
				time_update_flag = 1;  // 仅在一分钟时设置标志位,防止直接加载update_time()导致计时延时不准确
    
			}
		}
}

void timing()
{
		int i;
		while(now_mod == 'T')  //点击上键U pin时间，下键D清零，右键R暂停，左键L退出
		{
			update_time();  // 实时更新小时和分钟  
			LcdShowInit(str_num, 2, 0);
			
			get_button(5);   //检测按键，以便于暂停或者退出，或者将时间pin在第一行
			
			if(now_button == 'l')  //退出当前模式
			{
				now_mod = 't';	 		//将状态调整到上一个状态
				TR0 = 0;       			// 关闭定时器0，暂停计时，减少CPU消耗
				now_button = 'n';  	//恢复按键状态
				break;     					//停止当前循环
			}
			
			
			else if(now_button == 'r') //暂停/开始计时
			{
				TR0 = ~TR0;
			}
			
			
			else if(now_button == 'd') //时间清零
			{
				LcdShowInit("Pause:U  Clear:D", 1, 0);
				
				for (i = 0; i < 16; i++) {
					str_num[i] = inti_str_num[i];  
				}
			}
			
			
			else if(now_button == 'u') //pin当前时间
			{
				for (i = 16; i > 7; i--) {
					pin_str_num[i] = str_num[i];  
				}
				LcdShowInit(pin_str_num, 1, 0);
			}
			
			now_button = 'n';  	//恢复按键状态
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
    TR0 = 0;        // 暂时先不要开启定时器0
    EA = 1;         // 开启全局中断
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


void update_hour()  // 更新小时，为了省事直接偷了之前写过的代码，只能计数到24小时，就这样吧，懒得改
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




