#ifndef _PID_H
#define _PID_H
#include "main.h"

typedef struct 
{
	float  SET_POINT; //  目标速度
	float  ACT_POINT;
	
  float err_now;      //  当前误差
  float err_last; //  上次误差
	float derror;
	
  float Kp,Ki,Kd; //  PID系数
	
	float ki_index;
	
  float integral; //  积分累加
	
	float ctrlout;  //控制量输出	
	float Imax;
	
	float ERR1;
	float ERR2;
	float ERR3;	
}POSIT_PID;

float Posit_PID_Conver(POSIT_PID* gl_wheelx_PositPid);




#endif
