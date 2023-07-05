#include <reg51.h>
#include "XUNJI.h"

unsigned char receivedata;

void receive(unsigned char m)
{
	switch(m)
	{
		case'2':
		straight();
		break;
		
		case'4':
		turnleft();
		break;
		
		case'6':
		turnright();
		break;
		
		case'8':
		stop();
		break;
	}
}

void UartInit()		//9600bps@11.0592MHz
{
	PCON = 0x00;		//波特率不倍速
	SCON = 0x50;    
	TMOD = 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFd;		//设定定时初值
	TH1 = 0xFd;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA = 1;   //总中断
	ES = 1;   //打开串口中断
}
	
void UartRoutine() interrupt 4 //中断函数
{
	stop();
	RI = 0;//清除接收中断
	receivedata=SBUF;//出去接受到的数据
	receive(receivedata);
}


