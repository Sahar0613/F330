#include "schedule.h"

void schedule_init(void)
{
	SystemInit();
	delay_init();
	uart_init(500000);
	motor_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM3_Int_Init(9,7199);
	MPU_Init();
	USART3_Init_Config();
	
	while(mpu_dmp_init())
	{
		printf("初始化未成功\r\n");
	}	
}

void mpu6050_get_data(void)
{
	if(mpu_dmp_get_data(&MPU6050_Get.Pitch,&MPU6050_Get.Roll,&MPU6050_Get.Yaw)==0)
	{
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
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
	
