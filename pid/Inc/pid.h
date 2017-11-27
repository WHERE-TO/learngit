#ifndef __PID_H
#define __PID_H

typedef struct pid
{
	float kp,ki,kd,dt;
	float error,preError;
	float integ,iLimit,iLimitLow;
	float deriv;
	float outP,outI,outD;
}PidObject;

float pidUpdate(PidObject* pid,const float error);
PidObject pidInitial(float kp,float ki,float kd,float dt,float H,float L);

#endif


