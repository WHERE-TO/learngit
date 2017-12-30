#include "pid.h"

void PIDInit(PID *P){
	P->P=4.0f;
	P->I=0.8f;
	P->D=0.00f;
	P->IMax=1000.0f;
	P->SetPoint=0.0f;
}


float PIDCalc(PID *P,float NextPoint){
	float dError,Error;
	Error = P->SetPoint - NextPoint;
	
	P->SumError += Error;
	dError = Error - P->LastError;
	P->PreError = P->LastError;
	P->LastError = Error;
	
	
	if(P->SumError >= P->IMax)
		P->SumError = P->IMax;
	else if(P->SumError <= -P->IMax)
		P->SumError = -P->IMax;		
	
	P->POut = P->P*Error;
	P->IOut = P->I*(P->SumError);
	P->DOut = P->D*dError;
	
	return P->POut+P->IOut+P->DOut;
}