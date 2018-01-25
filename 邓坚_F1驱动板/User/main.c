#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "motor.h"
#include "timer.h"
#include "pid.h"
#include "can.h"

//extern POSIT_PID  gl_motor_PositPID;
void delay_ms(u32 ms);
int plus=101;
int speed=0,flag=0;

int test_for_SetSpeed;

extern u8 RESIVE_BUFF[8];

u8 SEND_BUFF[8]={'s','t','m','3','2','f','1','0'};

int main()
{
	u16 flag=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	MOTOR_Inite();
	TIM3_ECODE_Init();
  TIM4_GetSpeed_Init();
	TIM2_PIDcontrl_Init();
	
	CAN_MODE_Init();
	

	
	

		
	while(1)
	{
		//CAN_SEND_MSG(SEND_BUFF,8);
		test_for_SetSpeed=gl_motor_PositPID.SET_SPEED;

	}	
}

void delay_ms(u32 ms)
{
	u16 i,j;
	for(i=0;i<ms;i++)
	  for(j=1000;j>0;j--);
}

