#include "clock.h"
#include <intrins.h>
#include <stdio.h>


uchar num_now = 88;    // 当前输入数字,88表示当前没有数字输入,11表示*，12表示#


void main()
{ 
	uchar* str1 = " WYM 2022121218 ";
	button_LED = 0;  //按键提示灯默认关闭
	speaker = 0;		//关闭蜂鸣器
	
	while(1)  // 准备待机调整时间或者开始计时
	{
		LcdInit();	
		LcdShowInit(str1, 0, 0);
		
		int_timer0(); //初始化定时器0为模式二
		init_time();  //将str_num初始化为"Time is 18:24:18" 的形式然后显示
		LcdShowInit(str_num, 1, 0);

		while(1)    //系统开始运行
		{
			if(button == 0)     // 显示时间 
			{
				
				LcdShowInit(str_num+8, 1, 8);  //从第八位往后刷新 
				update_time();						   //自动刷新时间，根据主程序来，不是死循环	
			}
			else if(button == 1)     // 调整时间 
			{
				TR0 = 0;        // 关闭定时器0
				ET0 = 0;				// 关闭中断
				
				set_time();    //设置时间，根据主程序来，不是死循环	
				
				TR0 = 1;        // 重新启动定时器0
				ET0 = 1;				// 开启中断
			}
			
			
			lesGotoAlarm();   //此函数进行一个小小的按键检测，检测是否按下*或#，如果按下就进入死循环，但是死循环中有update函数可以确保时间的刷新和更新  
			
			if(speaker == 1)
			{
				keypad_scan();
				if(num_now != 88)
				{
					speaker = 0;
				}
			}
		}
	}
}





