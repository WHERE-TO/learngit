#ifndef _PID_H
#define _PID_H

#include "stm32f10x.h"

#include<stdio.h>
#include<stdlib.h>

//struct _incre_pid
//{
//	float SetSpeed; // 
//	float ActualSpeed; // 
//	float err; // 
//	float err_next; // 
//	float err_last; // 
//	float Kp,Ki,Kd; // 
//} ;
typedef struct _posit_pid
{
	float  SET_SPEED; //  目标速度
	float  ACT_SPEED;
	
  float err_now;      //  当前误差
  float err_last; //  上次误差
	float derror;
	
  float Kp,Ki,Kd; //  PID系数
	
	float ki_index;
	float kp_index;
	float kd_index;
	
  float integral; //  积分累加
	
	float ctrlout;  //控制量输出	
	float Imax;
	float derror_Max;
	
	float Imax_index;
	
	float ERROR1,ERROR2,ERROR3,ERROR4,ERROR5;
	
} POSIT_PID;

extern POSIT_PID  gl_motor_PositPID;


 //POSIT_PID gl_motor_PositPID;

 void  Posit_PID_SetSpeed(float setSpeed);
 float Posit_PID_Conver(float actSpeed);

//positional pid
//
//void Posit_PID_Set(struct _POSIT_PID* posit_pid);

//u16 Posit_PID_Conver(struct _POSIT_PID posit_pid,u16 act_cntx);




//incremental pid;
//void Incre_PID_Init(void);
//u16 Incre_PID_Conver(u16 ECODE_CNTx);   //输入编码器的值   输出PID修改后的 控制电机的PWM值 compare

#endif
