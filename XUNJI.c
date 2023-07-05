#include <reg51.h>					//通用51头文件

sbit IN1 = P1^1;
sbit IN2 = P1^2;//右前轮

sbit IN3 = P1^3;
sbit IN4 = P1^4;//左前轮

sbit ENB = P1^0;//右前轮开关
sbit ENA = P1^5;//左前轮开关

sbit LEFT_IR	= P1^6;
sbit RIGHT_IR	= P1^7;

sbit LED = P2^2;

unsigned char PWMA,PWMB;
void xunji();
void turnleft();
void turnright();
void straight();
void stop();
void delay_ms(unsigned int n);


void Timer0Init()		
{
	TMOD |= 0x01;		//设置定时器模式
	TL0 = (65536-100)%256;		//设置定时初始值
	TH0 = (65536-100)/256;		//设置定时初始值
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}


void T0isp() interrupt 1
{
	unsigned char i,j;
	TL0 = (65536-200)%256;		//设置定时初始值
	TH0 = (65536-200)/256;		//设置定时初始值
	i++;
	j++;
	if(i < PWMA)
	{
		ENA = 1;
	}
	else 
	{
		ENA = 0;
		if(i >= 100)
		{
			i = 0;
		}
	}
	if(j < PWMB)
	{
		ENB = 1;
	}
	else 
	{
		ENB = 0;
		if(j >= 100)
		{
			j = 0;
		}
	}
}

void xunji()
{
	int o=0;
	if(LEFT_IR == 0 && RIGHT_IR == 0) //2个探头均未压线，直行
	{
		straight();//直行
	}
	if(LEFT_IR == 1 && RIGHT_IR == 0) //左探头压线，车子右倾，左转
	{
		turnleft();//左转
		delay_ms(50);
	}
	if(LEFT_IR == 0 && RIGHT_IR == 1) //右探头压线，车子左倾，右转
	{
		turnright();//右转
		delay_ms(50);
	}
	if(LEFT_IR == 1 && RIGHT_IR == 1) //2个探头均压线，停止
	{
		stop();//停车
		while(o<5)
		{
		LED = 1;
		delay_ms(500);
		LED = 0;
		delay_ms(500);//到终点LED闪烁；
		o++;
		}
		delay_ms(1000000);
	}
}

void straight()	
{
	IN1 = 1;
	IN2 = 0;
	PWMA = 70;
	IN3 = 1;
	IN4 = 0;
	PWMB = 70;
}
void turnleft() 
{
	IN1 = 0;
	IN2 = 1;
	PWMA = 100;
	IN3 = 1;
	IN4 = 0;
	PWMB = 100;
}
void turnright()	
{
	IN1 = 1;
	IN2 = 0;
	PWMA = 100;
	IN3 = 0;
	IN4 = 1;
	PWMB = 100;
}
void stop()	//PWM 0
{
	IN1 = 0;
	IN2 = 0;
	PWMA = 0;
	IN3 = 0;
	IN4 = 0;
	PWMB = 0;
}
void delay_ms(unsigned int n)
{
	unsigned char i;
    while(n--)
        for(i = 0; i < 113; i++);
}

void main()
{
	straight()	;
}
