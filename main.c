#include <reg51.h>

sbit IN1 = P1^1;
sbit IN2 = P1^2;//右前轮

sbit IN3 = P1^3;
sbit IN4 = P1^4;//左前轮

sbit ENA = P1^5;//左前轮开关
sbit ENB = P1^0;//右前轮开关

sbit LEFT_IR	= P1^6;
sbit RIGHT_IR	= P1^7;

sbit LED = P2^2;

unsigned char PWMA,PWMB;

unsigned char receivedata=5;

int up=0;

void xunji();
void turnleft();
void turnright();
void straight();
void stop();
void delay_ms(unsigned int n);
void receive(unsigned char m);
void left();
void right();
void down();
void speeddown();
void speedup();
void speedback();

void receive(unsigned char m)
{
	switch(m)
	{
		case'2':
		straight();
		break;
		
		case'3':
		left();
		break;
		
		case'4':
		right();
		break;
		
		case'5':
		stop();
		break;
		
		case'6':
		down();
		break;
		
		case'7':
		turnright();
		break;
		
		case'8':
		turnleft();
		break;
		
		case'9':
		speedup();
		break;
		
		case'd':
		speeddown();
		break;
		
		case'f':
		speedback();
		break;
	}

}

void Timer0_Init()		
{
	TMOD |= 0x01;		//设置定时器模式
	TL0 = (65536-100)%256;		//设置定时初始值
	TH0 = (65536-100)/256;		//设置定时初始值
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}


void Timer0_Routine() interrupt 1
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
	PWMA = 70+up;
	IN3 = 1;
	IN4 = 0;
	PWMB = 70+up;
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
void left() 
{
	IN1 = 1;
	IN2 = 0;
	PWMA = 70+up;
	IN3 = 1;
	IN4 = 0;
	PWMB = 30;
}
void right() 
{
	IN1 = 1;
	IN2 = 0;
	PWMA = 10;
	IN3 = 1;
	IN4 = 0;
	PWMB = 70+up;
}
void down() 
{
	IN1 = 0;
	IN2 = 1;
	PWMA = 50+up;
	IN3 = 0;
	IN4 = 1;
	PWMB = 50+up;
}
void speedup() 
{
	up=up+10;
}

void speeddown() 
{
	up=up-10;
}

void speedback()
{
	up=0;
}
void delay_ms(unsigned int n)
{
	unsigned char i;
    while(n--)
        for(i = 0; i < 113; i++);
}

void Uart_Init()		//9600bps@11.0592MHz
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
	
void Uart_Routine() interrupt 4 //中断函数
{
	stop();
	RI = 0;//清除接收中断
	receivedata=SBUF;//出去接受到的数据
	receive(receivedata);
}



void main() //主函数
{
	Timer0_Init();
	Uart_Init();
	while(1)
	{		
		if(receivedata==1) xunji();
	}
}


	