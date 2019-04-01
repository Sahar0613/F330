#include "stm32f10x.h"
#include "schedule.h"

//#define PB0 PBout(0)

//void LED_Init()
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��ʹ������IO PORTAʱ��
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 // �˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA
//}

u8 Flag = 0;
_SBUS_RECEIVE_DATA Sbus_Receive_Data;
unsigned char sbus_rx_buffer[25];
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
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
 
