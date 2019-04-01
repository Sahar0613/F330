#include "control.h"

//----PID�ṹʵ�廯----
PID_Typedef pitch_angle_PID;	//pitch�ǶȻ�PID
PID_Typedef pitch_rate_PID;		//pitch�����ʻ�PID

PID_Typedef roll_angle_PID;   //roll�ǶȻ�PID
PID_Typedef roll_rate_PID;    //roll�����ʻ�PID

PID_Typedef yaw_angle_PID;    //yaw�ǶȻ�PID 
PID_Typedef yaw_rate_PID;     //yaw�����ʻ�PID

int16_t Motor[4];
float Thro=0,Roll=0,Pitch=0,Yaw=0;  //�洢����PID��������ֵ
#define intergral_max 30   //�˴���ʱΪ���ⶨ���ֵ�����ڻ��ֿ�����
float YawLock = 0;
vu8 YawLockState = 0;

void PID_Init()
{
	pitch_angle_PID.P = 11.2;
	
	pitch_rate_PID.P = 0.16;
	pitch_rate_PID.I = 0.0;
	pitch_rate_PID.D = 0.4;
	
	yaw_angle_PID.P = 5.3;
	
	yaw_rate_PID.P = 0.16;
	yaw_rate_PID.I = 0.0;
	yaw_rate_PID.D	= 0.4;
	
	roll_angle_PID.P = 11.2;
	
	roll_rate_PID.P = 0.16;
	roll_rate_PID.I = 0.0;
	roll_rate_PID.D = 0.4;
}

/*****
λ��ʽPID
��һ������Ϊ����PID�Ľṹ��洢����ϵ��������ϵ����΢��ϵ����ָ
�ڶ�������ΪPIDĿ��ֵ������������ΪPID����ֵ
*****/
void Position_PID(PID_Typedef * PID,float target,float measure)
{
	PID->err =target - measure;
	PID->intergral += PID->err;
	//�����޷�
	if(PID->intergral>intergral_max){PID->intergral=intergral_max;}
	if(PID->intergral<-intergral_max){PID->intergral=-intergral_max;}
	PID->deriv = PID->err -PID->err_last;
	PID->output = PID->P*PID->err + PID->I*PID->intergral + PID->D*PID->deriv;
	PID->err_last = PID->err;
}

void PID_yaw(PID_Typedef * PID,float target,float measure)
{
	PID->err = target - measure;
	PID->intergral += PID->err;
	//�����޷�
	if(PID->intergral>intergral_max){PID->intergral=intergral_max;}
	if(PID->intergral<-intergral_max){PID->intergral=-intergral_max;}
	PID->output = PID->P*PID->err+PID->I*PID->intergral+PID->D*(PID->err-PID->err_last);
	PID->err_last = PID->err;	
}

void Control(void)
{
	
	//��ȡң����ֵ
	sbus_analysis();
	romote_process();
	if(mode == 1)
	{
		
		//��ȡ��ǰMPU6050��Ϣ
		mpu6050_get_data();
		
		//��ӡң����ֵ
		printf("%f %f %f %f\r\n",Target.roll,Target.pitch,Target.thro,Target.yaw);
//		printf("%d %d %d %d\r\n",Target.roll,Target.pitch,Target.thro,Target.yaw);
		//��ӡMPU6050ֵ
//		printf("%f %f %f  ",MPU6050_Get.Pitch,MPU6050_Get.Roll,MPU6050_Get.Yaw);
//		printf("%d,%d,%d,%d,%d,%d\r\n",aacx,aacy,aacz,gyrox,gyroy,gyroz);
		
		//�⻷���ƣ�����Ϊ��ǰ�Ƕȣ����Ϊ���ٶȣ���Ϊ�ڻ�������
		Position_PID(&pitch_angle_PID,-Target.pitch, MPU6050_Get.Pitch);
		Position_PID(&roll_angle_PID,-Target.roll, MPU6050_Get.Roll);
		//�⻷(�ǶȻ�)Z��PID��Ŀ����ΪYawLock�洢�ĵ�ǰƫ���Ƕȣ�����Ϊ��ǰʵ��ƫ���Ƕ�
		if(Target.yaw == 0)
		{
			if(YawLockState == 0)
			{
				YawLock = MPU6050_Get.Yaw;
				YawLockState = 1;
			}
			else
			{
				YawLockState = 1;
				YawLock = MPU6050_Get.Yaw;
			}
		}
		PID_yaw(&yaw_angle_PID,-YawLock, MPU6050_Get.Yaw);
		
//		printf("%f %f %f\r\n",pitch_angle_PID.output,roll_angle_PID.output,roll_angle_PID.output);
		
		//�ڻ�(�ٶȻ�)X,Y,Z��PID,Ŀ����Ϊ��һ���⻷PID�����������Ϊ��ǰ������ٶ�
		PID_yaw(&yaw_rate_PID,roll_angle_PID.output+Target.yaw,-gyroz);
		Position_PID(&pitch_rate_PID, pitch_angle_PID.output, -gyroy);
		Position_PID(&roll_rate_PID, roll_angle_PID.output, gyrox);
		
		//�ڻ�PID�����ֵ����3������
		Pitch = pitch_rate_PID.output;
		Roll = roll_rate_PID.output;
		Yaw = yaw_rate_PID.output;

		Thro = Target.thro;
		
		Motor[0] = (int16_t)(Thro - Pitch + Roll + Yaw);
		Motor[1] = (int16_t)(Thro + Pitch - Roll + Yaw);
		Motor[2] = (int16_t)(Thro - Pitch - Roll - Yaw);
		Motor[3] = (int16_t)(Thro + Pitch + Roll - Yaw);
		
		if(Thro <1900)
		{
			MotorPwmFlash(0,0,0,0);
		}
		else
		{
//			MotorPwmFlash(Thro,Thro,Thro,Thro);
			MotorPwmFlash(Motor[0], Motor[1], Motor[2], Motor[3]);
//			printf("%d %d %d %d\r\n",Motor[0],Motor[1],Motor[2],Motor[3]);
		}
	}
	mode = 0;
}
