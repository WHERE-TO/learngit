#include "motor.h"

void MOTOR_Inite()
{
	TIM1_OC_Init();
}

void MOTOR_SPEED_PositCrl(float cntx)
{
	Posit_PID_SetSpeed( cntx);
}	
void MOTOR_SET(int plus)
{
	if(plus>=0&&plus<=1000)
	{
		TIM1_OCH1_setForward(plus);
	}
	else if(plus<0&&plus>=-1000)
	{
		TIM1_OCH2_setBack(-plus);
	}
	else if(plus>1000)
	{
		plus=1000;
		TIM1_OCH1_setForward(plus);
	}
	else if(plus<-1000)
	{
		plus=1000;
		TIM1_OCH2_setBack(plus);		
	}
}


