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
uchar int_str(uchar int_num);
void fix_str_num();
void int_timer0();


sbit LcdRs_P  = P2^0;		//LCD����1/����0   
sbit LcdRw_P  = P2^1;		//LCD��/д
sbit LcdEn_P  = P2^2;		//ʹ�ܶ�(��������Ч)

sbit pad_row1 = P1^0;   //������
sbit pad_row2 = P1^1;
sbit pad_row3 = P1^2;

sbit pad_line1 = P1^3;  	//������
sbit pad_line2 = P1^4;
sbit pad_line3 = P1^5;
sbit pad_line4 = P1^6;

sbit tele_on = P3^0;   // ����ժ�����

unsigned int time_counter = 0;   //��ʱ���ļ�������ÿ��20�α�ʾ����1�루50ms*20 = 1s��

P3_0 = 1;  // 0��ʾû��̧�𻰻�����ʱ����ʾ�绰

uchar num_now = 88;    // ��ǰ��������,88��ʾ��ǰû����������,11��ʾ*��12��ʾ#
uchar str_num[16];     // �ڶ��еĵ绰����




void main()
{ 
	uchar* str1 = " WYM 2022121218 ";
	uchar* str2 = "  To My Heart   ";
	
	uint i = 0;
	for(;i<16;i++)   // ��ʼ��str_numΪȫ�ո�
	{
		str_num[i] = 32;
	}
	
	while(1)  // ׼������������ʾ����
	{
		LcdInit();	
		LcdShowInit(str2, 0, 0);
		LcdShowInit(str1, 1, 0);
	
		while(P3_0)     // ��ʼ��ѭ����ʾ����
		{
			P1 = 0X7F;	 //�Է���һ��һ����ʼ��P1	
			keypad_scan();			//���Ǹ�������ѭ��
			if(num_now != 88)  // ��¼����������֮����ӵ���������һλ
			{
				fix_str_num();
			}
			LcdShowInit(str_num, 1, 0);
			
			if(P3_0 == 0){break;}   // ���������Ǹ���ѭ������������������ã��������ɣ�����д��
			
			DelayMs(1);
		}
	}
}




void keypad_scan()   //����ɨ�����
{
	num_now = 88;
	while(num_now == 88)  //��ǰû����ʱ����ѭ������
	{
		P1 = 0X7F;	
		// row1
		pad_row1 = 0;
		DelayMs(1);
		if(P1 != 0X7E)   //�жϰ����Ƿ���
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
			while(P1 != 0X7E);    //��ѭ���ȴ���������
		}
		pad_row1 = 1;				//����һ������Ϊ1
		
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


void LcdShowInit(uchar *str_1, uchar Y, uchar X)  // ������ʾ�������ַ��������������꣨0��1������������ʼ���꣨0~15����
{
	LcdGotoXY(Y,X);	    							
	LcdPrintStr(str_1);	
}


uchar int_str(uchar int_num)  // �������Ҳ���ú�רҵ�ģ���λ����ת������
{
	return int_num + '0';
}


void fix_str_num()   // ���µ�������ӵ���������һλ��Ȼ��֮ǰ������λ��ǰ��
{ 
	uint i;
	for(i = 0;i<15;i++)
	{
		str_num[i] = str_num[i+1];   // ������������ǰŲһλ����0λ���ڵ�һλ,Ų����ʮ��λͣ��
	}
	str_num[15] = int_str(num_now);  //��ʮ��λ�������ڰ��µ�����

}


void int_timer0()   // ��ʼ����ʱ����TMODΪ1��
{
	TMOD &= 0Xf0;
	TMOD |= 0X01;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
}


void Timer0_ISR() interrupt 1   // ��ʱ��1���ж�
{
	TH1 = 0X3C;   
	TL1 = 0XB0;


	time_counter++;

	if(time_counter > 20)
	{
		time_counter = 0;
		
	}
}


