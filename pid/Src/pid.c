#include "stm32f10x.h"                  
#include "pid.h"

float pidUpdate(PidObject* pid,const float error)
{
	float output;
	
	pid->error = error;
	pid->integ += pid->error*pid->dt;
	if(pid->integ>pid->iLimit)
	{
		pid->integ = pid->iLimit;
	}
	else if (pid->integ<pid->iLimitLow)
	{
		pid->integ=pid->iLimitLow;
	}
	
	pid->outP = pid->kp*pid->error;
	pid->outI = pid->ki*pid->integ;
	pid->outD = pid->kd*pid->deriv;
	
	output = pid->outP+pid->outI+pid->outD;
	pid->preError = pid ->error;
	
	return output;
}

PidObject pidInitial(float kp,float ki,float kd,float dt,float H,float L)
{
	PidObject pid;
	
	pid.dt=dt;
	pid.iLimit= H;
	pid.iLimitLow = L;
	pid.kp = kp;
	pid.ki = ki;
	pid.kd = kd;
	
	return pid;
}

