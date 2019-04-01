#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"
#include "schedule.h"

typedef struct
{
	float P;
	float I;
	float D;
	float err;
	float err_last;
	float intergral;
	float output;
	float deriv;
}PID_Typedef;

void PID_Init(void);
void Position_PID(PID_Typedef * PID,float target,float measure);
void PID_yaw(PID_Typedef * PID,float target,float measure);
void Control(void);

#endif
