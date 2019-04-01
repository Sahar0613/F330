#include "schedule.h"

void schedule_init(void)
{
	SystemInit();
	delay_init();
	uart_init(500000);
	motor_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM3_Int_Init(9,7199);
	MPU_Init();
	USART3_Init_Config();
	
	while(mpu_dmp_init())
	{
		printf("��ʼ��δ�ɹ�\r\n");
	}	
}

void mpu6050_get_data(void)
{
	if(mpu_dmp_get_data(&MPU6050_Get.Pitch,&MPU6050_Get.Roll,&MPU6050_Get.Yaw)==0)
	{
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//		ano_send_status((int)(MPU6050_Get.Roll*100),(int)(-MPU6050_Get.Pitch*100),(int)(-MPU6050_Get.Yaw*100));
//		ano_send_senser(aacx,aacy,aacz,-gyrox,gyroy,-gyroz);
	}
}

void Duty_1ms(void)
{
	PID_Init();
}

void Duty_2ms(void)
{
	Control();
}
	
