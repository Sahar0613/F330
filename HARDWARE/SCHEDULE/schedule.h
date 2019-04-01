#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "timer.h"
#include "mpu6050.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "usart3.h"
#include "sbus.h"
#include "control.h"
#include "ano_tc.h"

typedef struct
{
	float Pitch;
	float Roll;
	float Yaw;
}Angle_Get;

extern Angle_Get MPU6050_Get;
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据

void schedule_init(void);
void mpu6050_get_data(void);
void Duty_1ms(void);
void Duty_2ms(void);

#endif
