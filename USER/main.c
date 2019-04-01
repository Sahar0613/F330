#include "stm32f10x.h"
#include "schedule.h"

//#define PB0 PBout(0)

//void LED_Init()
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//先使能外设IO PORTA时钟
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 // 端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
//}

u8 Flag = 0;
_SBUS_RECEIVE_DATA Sbus_Receive_Data;
unsigned char sbus_rx_buffer[25];
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
Angle_Get MPU6050_Get;
RC_ANGLE Target;

int main(void)
{
	schedule_init();
	
	while(1)
	{
//		printf("a ");
		if(loop1msFlag)
		{
			Duty_1ms();
			loop1msFlag = 0;
		}
		if(loop2msFlag)
		{
			Duty_2ms();
			loop2msFlag = 0;
		}
	}
}
 
