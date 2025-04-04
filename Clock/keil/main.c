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

sbit button = P2^7;

uint time_counter = 0;   //��ʱ���ļ�������ÿ��20�α�ʾ����1�루50ms*20 = 1s��
bit time_update_flag = 0; //ʱ����±�־��ÿ���ӱ�Ϊ1֮����Ӹ��£�Ȼ����0

uchar num_now = 88;    // ��ǰ��������,88��ʾ��ǰû����������,11��ʾ*��12��ʾ#
uchar str_num[16];     // �ڶ��е����е�ʱ����ʾ   18��24��18   �ܹ���Ҫ8λ  "Time is 18:24:18"
																																				//"     		8  11 14"  '0'��ascaii����48������57  ð����58
uint set_now = 1;  // ����ʱ��ʱȷ����������������һλ



//P3_0 = 1;  // 0��ʾû��̧�𻰻�����ʱ����ʾ�绰
sbit LED = P2^6;


void main()
{ 
	uchar* str1 = " WYM 2022121218 ";
	uchar* str2 = "  To My Heart   ";
	
	
	while(1)  // ׼����������ʱ����߿�ʼ��ʱ
	{
		LcdInit();	
		LcdShowInit(str2, 0, 0);
		LcdShowInit(str1, 1, 0);
	
		int_timer0(); //��ʼ����ʱ��0Ϊģʽ��
		init_time();  //��str_num��ʼ��Ϊ"Time is 18:24:18" ����ʽȻ����ʾ

		while(1)
		{
			LED = button;
			if(button == 0)     // ��ʾʱ�� 
			{
				
				LcdShowInit(str_num, 1, 0);  //���ڶ��д�ͷ��ʼ��д���е��ʱ��
				update_time();							
			}
			else if(button == 1)     // ����ʱ�� 
			{
				TR0 = 0;        // �رն�ʱ��0
				ET0 = 0;				// �ر��ж�
				
				set_time(); 
				
				TR0 = 1;        // ����������ʱ��0
				ET0 = 1;				// �����ж�
			}
		}

	}
}




void keypad_scan()   //����ɨ�����
{
	num_now = 88;  //�Զ����裬�Զ���ѭ����̫�߼���
	while(num_now == 88 && button == 1)  //��ǰû����ʱ����ѭ�����������Ǵ�ʱ�����ǵ���ʱ��ʱ
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




void int_timer0()   // ��ʼ����ʱ��0Ϊģʽ2���Զ����أ�
{
    TMOD &= 0XF0;   // ���㶨ʱ��0��ģʽλ��������ʱ��1�����ã�
    TMOD |= 0X02;   // ���ö�ʱ��0Ϊģʽ2��8λ�Զ����أ�
    TH0 = 0xA4;     // �Զ�����ֵ��100��s��ʱ��ֵ��11.0592MHz��
    TL0 = 0xA4;     // �״�����ʱ�ĳ�ֵ��������TH0��ͬ��
    ET0 = 1;        // ����ʱ��0�ж�
    TR0 = 1;        // ������ʱ��0
    EA = 1;         // ����ȫ���ж�
}

void Timer0_ISR() interrupt 1   // ��ʱ��0���ж� ,ģʽ2�����ֶ�����TH0/TL0
{
    time_counter++;             // �ۼ��жϴ���
    
    if(time_counter >= 10000)      // 10000�Ρ�100��s=1000ms
    {
      time_counter = 0;
			if(update_second())
			{
				time_update_flag = 1;  // ����һ����ʱ���ñ�־λ,��ֱֹ�Ӽ���update_time()���¼�ʱ��ʱ��׼ȷ
    
			}
		}
}

uint update_second()  // �����룬����һ����λ��־���о���Ȼstr_num��uchar���͵ģ����Ǽ�һȷʵ����������һ[doge]
{
	if(str_num[15] < '9')  // 57 = '9'
	{
		str_num[15]++;
	}
	else 	 //��ĸ�λ�Ľ�λ
	{
		str_num[15] = '0';   // 48 = '0'
		
		if(str_num[14] < '5')  // 53 = '5'   
		{
			str_num[14]++;
		}
		else   	//���ʮλ�Ľ�λ
		{
			str_num[14] = '0';
			return 1;  // ��ǵ�59�ͱ����һ
		}
	}
	return 0; 	
}

uint update_minute()  // ���·���
{
	if(str_num[12] < '9')  // 57 = '9'
	{
		str_num[12]++;
	}
	else 	 //���ӵĸ�λ�Ľ�λ
	{
		str_num[12] = '0';   // 48 = '0'
		
		if(str_num[11] < '5')  // 53 = '5'   
		{
			str_num[11]++;
		}
		else   	//���ӵ�ʮλ�Ľ�λ
		{
			str_num[11] = '0';
			return 1;  // ���Ӽǵ�59�ͱ����һ
		}
	}
	return 0; 	
}


void update_hour()  // ����Сʱ��ÿ�ε��ö����һ�����㣬�������23ʱ�����õ�00
{
	if(str_num[9] < '9' && str_num[8] < '2')  // 00~08  10~18 ʱ��λ��һ
	{
		str_num[9]++;
	}
	else if(str_num[9] == '9' && str_num[8] < '2') 	 //09 19 ʱ��λ��0��ʮλ��һ
	{
		str_num[9] = '0';   // 48 = '0'
		str_num[8]++;
	}	
	else if(str_num[9] < '3' && str_num[8] == '2')   // 20 21 22 ʱ��λ��һ,���Ϊ23
	{
		str_num[9]++;
	}
	else if(str_num[9] == '3' && str_num[8] == '2')  // 23����ǰ��λΪ00
	{
		str_num[9] = '0';
		str_num[8] = '0';
	}
}


void init_time()   // ��ʼ���ڶ��е���ʾ��������ʾ����Ҫ��
{
	uint i;
	uchar inti[16] = {'T','i','m','e',' ','i','s',' ','0','0',':','0','0',':','0','0'};  // "Time is 00:00:00"
	for (i = 0; i < 16; i++) {
        str_num[i] = inti[i];  
	}
}


void update_time()   //ÿ��ִ�ж��������һ�������ŷ��Ӻ�Сʱ�仯
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

void set_time()     //����ʱ��
{
		
		LcdShowInit(str_num, 1, 0);  //���ڶ��д�ͷ��ʼ��д���е��ʱ��
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


