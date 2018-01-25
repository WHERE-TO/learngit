#ifndef _PID_H
#define _PID_H
#include "main.h"

typedef struct 
{
	float  SET_POINT; //  Ŀ���ٶ�
	float  ACT_POINT;
	
  float err_now;      //  ��ǰ���
  float err_last; //  �ϴ����
	float derror;
	
  float Kp,Ki,Kd; //  PIDϵ��
	
	float ki_index;
	
  float integral; //  �����ۼ�
	
	float ctrlout;  //���������	
	float Imax;
	
	float ERR1;
	float ERR2;
	float ERR3;	
}POSIT_PID;

float Posit_PID_Conver(POSIT_PID* gl_wheelx_PositPid);




#endif
