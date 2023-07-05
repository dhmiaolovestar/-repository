#include <reg51.h>

sbit IN1 = P1^1;
sbit IN2 = P1^2;//右前轮

sbit IN3 = P1^3;
sbit IN4 = P1^4;//左前轮

//sbit ENA = P1^5;//左前轮开关
//sbit ENB = P1^0;//右前轮开关




void main() //主函数
{
	IN1 = 1;
	IN2 = 0;
	IN3 = 1;
	IN4 = 0;
//	ENA=1;
//	ENB=1;
//	
	}


	