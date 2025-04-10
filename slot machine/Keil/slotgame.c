//slot game 老虎机相关函数
#include <reg52.h>
#include <stdlib.h>
#include "all_x.h"

uint wj_rand() 	//生成随机数
{
		uint random_nm;
		uchar low;
           
    //high = TH0;  //利用定时器的不断变换的高低八位来随机生成随机数
    low = TL0;		//定时器模式二高八位为自动重载值，所以只能用低八位了
		srand(low);
		random_nm = rand() % 7;  //生成0-6之间的随机数
 
		return random_nm;		//返回生成的随机数
}
	
