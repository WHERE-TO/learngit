#include "stm32f4xx_conf.h"

typedef struct
{
	float P,I,D;

	float SetPoint;
	float IMax;
	float PreError,LastError,SumError;
	
	float POut,IOut,DOut;	
	
}PID;

void PIDInit(PID *P);
float PIDCalc(PID *P,float NextPoint);
