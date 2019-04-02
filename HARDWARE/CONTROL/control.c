#include "control.h"

//----PID结构实体化----
PID_Typedef pitch_angle_PID;	//pitch角度环PID
PID_Typedef pitch_rate_PID;		//pitch角速率环PID

PID_Typedef roll_angle_PID;   //roll角度环PID
PID_Typedef roll_rate_PID;    //roll角速率环PID

PID_Typedef yaw_angle_PID;    //yaw角度环PID 
PID_Typedef yaw_rate_PID;     //yaw角速率环PID

int16_t Motor[4];
float Thro=0,Roll=0,Pitch=0,Yaw=0;  //存储串级PID最后调整的值
#define intergral_max 30   //此处暂时为任意定义的值，用于积分抗饱和
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
位置式PID
第一个参数为串级PID的结构体存储比例系数、积分系数、微分系数的指
第二个参数为PID目标值，第三个参数为PID输入值
*****/
void Position_PID(PID_Typedef * PID,float target,float measure)
{
	PID->err =target - measure;
	PID->intergral += PID->err;
	//积分限幅
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
	//积分限幅
	if(PID->intergral>intergral_max){PID->intergral=intergral_max;}
	if(PID->intergral<-intergral_max){PID->intergral=-intergral_max;}
	PID->output = PID->P*PID->err+PID->I*PID->intergral+PID->D*(PID->err-PID->err_last);
	PID->err_last = PID->err;	
}

void Control(void)
{
	
	//读取遥控器值
	sbus_analysis();
	romote_process();
	if(mode == 1)
	{
		
		//读取当前MPU6050信息
		mpu6050_get_data();
		
		//打印遥控器值
		printf("%f %f %f %f\r\n",Target.roll,Target.pitch,Target.thro,Target.yaw);
//		printf("%d %d %d %d\r\n",Target.roll,Target.pitch,Target.thro,Target.yaw);
		//打印MPU6050值
//		printf("%f %f %f  ",MPU6050_Get.Pitch,MPU6050_Get.Roll,MPU6050_Get.Yaw);
//		printf("%d,%d,%d,%d,%d,%d\r\n",aacx,aacy,aacz,gyrox,gyroy,gyroz);
		
		//外环控制，输入为当前角度，输出为角速度，作为内环的输入
		Position_PID(&pitch_angle_PID,-Target.pitch, MPU6050_Get.Pitch);
		Position_PID(&roll_angle_PID,-Target.roll, MPU6050_Get.Roll);
		//外环(角度环)Z轴PID，目标量为YawLock存储的当前偏航角度，输入为当前实际偏航角度
		if(Target.yaw == 0)
		{
			if(YawLockState == 0)
			{
				YawLock = MPU6050_Get.Yaw;
				YawLockState = 1;
			}
			else
			{
				YawLockState = 0;
				YawLock = MPU6050_Get.Yaw;
			}
		}
		PID_yaw(&yaw_angle_PID, YawLock, MPU6050_Get.Yaw);
		
//		printf("%f %f %f\r\n",pitch_angle_PID.output,roll_angle_PID.output,roll_angle_PID.output);
		
		//内环(速度环)X,Y,Z轴PID,目标量为上一级外环PID输出量，输入为当前三轴角速度
		PID_yaw(&yaw_rate_PID, yaw_angle_PID.output+Target.yaw, -gyroz);
		Position_PID(&pitch_rate_PID, pitch_angle_PID.output, -gyroy);
		Position_PID(&roll_rate_PID, roll_angle_PID.output, gyrox);
		
		//内环PID的输出值赋给3个变量
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
