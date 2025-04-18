//alarm set
#include "clock.h"

char all_alarm[3][16] = {"123412341234","Alarm1>06:30 OFF", "Alarm2>06:31 OFF"};

char set_alarm = 0;  //当前设置的闹钟是哪个
char set_now_alarm = 1;		//当前设置的闹钟的位数

int i=0;

void lesGotoAlarm()
{
			pad_row1 = 0;
			set_alarm = 1; //设置闹钟一
			alarm();    	//设置闹钟，此函数内含一个死循环,但是死循环中有update函数可以确保时间的刷新和更新
			pad_row1 = 1;

			pad_row3 = 0;
			set_alarm = 2;  //设置闹钟2
			alarm(); 
			pad_row3 = 1;
			set_alarm = 0; //闹钟设置完成,清除闹钟标志
}



void alarm()
{	
	if(pad_line4 == 0 && button != 1)  //如果星号或者井号被按下，那么开始设置闹钟，可以设置两个，但是此时不能是设置模式，设置模式不但键盘被不断扫描而且BUG重重
	{
		DelayMs(1);    //算是一个按键防抖
		while(pad_line4 == 0)  //确保按键抬起
		{
			button_LED = 1;   //提示灯亮
		}
		button_LED = 0;		//提示灯灭
		pad_row1 = 1;	//恢复键盘状态
		pad_row3 = 1;
			
		
		if(set_alarm == 1 || set_alarm == 2)   // *？：进入闹钟设置
		{
			LcdShowInit(all_alarm[set_alarm],0,0);  //初始化第一行
			while(1)
			{
				LcdShowInit(str_num, 1, 0);  //就算是设置闹钟，第二行的时间也得不断计数
				update_time();

				set_alarming();
				

				for(i=7;i<12;i++)  //更新后十位的值
				{
					LCDwAOne(all_alarm[set_alarm][i],0,i);
				}
				
				twinkle();   //设置位闪烁,放在合适的位置会让闪烁更加自然
				
				if(set_alarm == 0)
				{
					LcdShowInit(" WYM 2022121218 ", 0, 0);
					break;
				}
			}
		}
	}
}

void set_alarming()
{
		keypad_scan();  //找数，非死循环
		switch(set_now_alarm)
		{			
			case 1:
				if(num_now < 3)
				{
					all_alarm[set_alarm][7] = num_now + '0';
					set_now_alarm++;
				}
				else
				{
					on_off();
				}
				break;
				
			case 2: 
				if((num_now<10 && (all_alarm[set_alarm][7]-'0') < 2) || ( num_now<4 && (all_alarm[set_alarm][7]-'0')==2))
				{
					all_alarm[set_alarm][8] = num_now + '0';
					set_now_alarm++;
				}
				else
				{
					on_off();
				}
				break;


			case 3:		
				if(num_now < 6)
				{
					all_alarm[set_alarm][10] = num_now + '0';
					set_now_alarm++;
				}
				else
				{
					on_off();
				}
				break;
		
			case 4:	
				if(num_now < 10)            
				{
					all_alarm[set_alarm][11] = num_now + '0';
					set_now_alarm = 1;
				}
				else
				{
					on_off();
				}
				break;
		}	
}

	
void on_off()
{
				if(num_now == 11 && set_alarm == 1)  //如果在闹钟1的设置中点击*,那么切换闹钟开关          
				{
					if(all_alarm[set_alarm][15] == 'N')
					{
						all_alarm[set_alarm][13] = 'O';
						all_alarm[set_alarm][14] = 'F';
						all_alarm[set_alarm][15] = 'F';
						LcdShowInit("OFF", 0, 13);
					}
					else if(all_alarm[set_alarm][15] == 'F')
					{
						all_alarm[set_alarm][13] = ' ';
						all_alarm[set_alarm][14] = 'O';
						all_alarm[set_alarm][15] = 'N';
						LcdShowInit(" ON", 0, 13);
					}
					
				}
				else if(num_now == 12 && set_alarm == 1) //如果在闹钟1的设置中点击#,那么退出闹钟设置 
				{
					set_alarm = 0;
				}
				
				
				else if(num_now == 12 && set_alarm == 2) //如果在闹钟2的设置中点击#,那么切换闹钟开关  
				{
					if(all_alarm[set_alarm][15] == 'N')
					{
						all_alarm[set_alarm][13] = 'O';
						all_alarm[set_alarm][14] = 'F';
						all_alarm[set_alarm][15] = 'F';
						LcdShowInit("OFF", 0, 13);
					} 
					else if(all_alarm[set_alarm][15] == 'F')
					{
						all_alarm[set_alarm][13] = ' ';
						all_alarm[set_alarm][14] = 'O';
						all_alarm[set_alarm][15] = 'N';
						LcdShowInit(" ON", 0, 13);
					}
				}
				else if(num_now == 11 && set_alarm == 2) //如果在闹钟2的设置中点击*,那么退出闹钟设置 
				{
					set_alarm = 0;
				}
}
	
	
void twinkle()
{
				switch(set_now_alarm)//产生闪烁效果
				{
					case 1:
						LCDwAOne(' ',0,7);
					break;
					case 2:
						LCDwAOne(' ',0,8);
						break;
					case 3:
						LCDwAOne(' ',0,10);
						break;
					case 4:
						LCDwAOne(' ',0,11);
						break;
				}

}
	
void if_alarming()   //检测是否要响闹钟
	{
    speaker = 0;
    for(i=1; i<=2; i++) 
		{
			if(all_alarm[i][15] == 'N') 
			{
        if((all_alarm[i][7] == str_num[8]) & (all_alarm[i][8] == str_num[9]) & (all_alarm[i][10] == str_num[11]) & (all_alarm[i][11] == str_num[12])) 
				{
            speaker = 1;
        }
    }
	}
}

