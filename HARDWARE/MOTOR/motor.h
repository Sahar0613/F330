#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"
#include "delay.h"
#define Moto_PwmMax 3600
#define Moto_PwmMin 1800

void MotorPwmFlash(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM);
void TIM2_PWM_Init(u16 arr,u16 psc);
void motor_Stop_output(void);
void motor_init(void);
void motor_start_output(u16 m);

#endif
