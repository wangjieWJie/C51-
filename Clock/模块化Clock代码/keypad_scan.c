//keybaord scan
#include "clock.h"

/*
扫描之后将按键保存为数字值Num_Now,需要加'0'才是字符值
其中数字和按键数字一一对应,'*'用11表示, '#'用12表示
*/

void keypad_scan()   //逐列扫描键盘
{
	num_now = 88;  //自动重设，自动死循环，太高级了
	while(num_now == 88)  //当前没数的时候死循环找数，但是此时必须是调整时间时
	{
		P1 = 0X7F;	
		// row1
		pad_row1 = 0;
		DelayMs(1);
		if(P1 != 0X7E)   //判断按键是否按下
		{
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
					num_now = 11;    //11表示*
				break;	
			}
			while(P1 != 0X7E)    //死循环等待按键弹起
			{
				button_LED = 1;    //按键提示灯亮起
			}
		}
		pad_row1 = 1;				//将这一列重置为1
		
		//row2
		pad_row2 = 0;
		if(P1 != 0X7D)
		{
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
			while(P1 != 0X7D)
			{
				button_LED = 1;    //按键提示灯亮起
			}

		}
		pad_row2 = 1;
		
		//row3
		pad_row3 = 0;
		if(P1 != 0X7B)
		{
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
					num_now = 12;   //12表示#
				break;	
			}
			while(P1 != 0X7B)
			{
				button_LED = 1;    //按键提示灯亮起
			}

		}
		pad_row3 = 1;

		button_LED = 0;    //按键提示灯灭
		
		if(button == 0)   //如果处于设置模式（1）就进行死循环，否则只循环一次
		{
			break;   
		}
		else
		{
			set_twinkle();
		}	
	}
}




