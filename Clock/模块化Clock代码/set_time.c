//时间设置函数
#include "clock.h"


uint set_now = 1;  // 设置时间时确定现在正在设置哪一位

uint if_twinkle = 5;//控制闪烁的频率

void set_time()     //设置时间
{
	
		LcdShowInit(str_num+8, 1, 8);  //从第八位往后刷新
		keypad_scan();  //死循环找数，只在设置里会死循环
		switch(set_now)
		{			
			case 1:
				if(num_now < 3)
				{
					str_num[8] = num_now + '0';
					LcdShowInit(str_num+8, 1, 8);  
					set_now++;
				}
				break;
				
			case 2: 
				if((num_now<10 && (str_num[8]-'0') < 2) || ( num_now<4 && (str_num[8]-'0')==2))
				{
					str_num[9] = num_now + '0';
					LcdShowInit(str_num+8, 1, 8);  
					set_now++;
				}
				break;


			case 3:		
				if(num_now < 6)
				{
					str_num[11] = num_now + '0';
					LcdShowInit(str_num+8, 1, 8);   
					set_now++;
				}
				break;
		
			case 4:	
				if(num_now < 10)            
				{
					str_num[12] = num_now + '0';
					LcdShowInit(str_num+8, 1, 8);  
					set_now = 1;
				}
				break;
		}	
}



void set_twinkle()
{
	if(if_twinkle > 0 && if_twinkle <= 20)   //正常显示时间
	{
		LcdShowInit(str_num+8, 1, 8); 
		P3 = 0X0F;
	}
	else if(if_twinkle > 20)   //熄灭正在设置的位
	{
		
		switch(set_now)
		{			
			case 1:
				P3 = 0XF0;
				LCDwAOne(' ', 1, 8);
				break;
				
			case 2:
				P3 = 0XF0;
				LCDwAOne(' ', 1, 9);				
				break;


			case 3:	
				P3 = 0XF0;
				LCDwAOne(' ', 1, 11);				
				break;
		
			case 4:	
				P3 = 0XF0;
				LCDwAOne(' ', 1, 12);
				break;
		}
	}
	else if(if_twinkle == 0)
	{
		if_twinkle = 40;
	}

	
	if_twinkle--;  //每次执行这个函数时减一
}




