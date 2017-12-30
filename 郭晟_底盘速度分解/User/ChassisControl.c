#include "ChassisControl.h"
#define MINSPEED 50

PID PIDV[4];
int WheelSpeed[4]={0,0,0,0};//PID Contorl Data
int xspeed=0;
int yspeed=0;
int rspeed=0;

void ChassisControl_Init()
{
	CANInit();
	TIM_Config();
	RC_Init();
	
	PIDInit(&PIDV[0]);
	PIDInit(&PIDV[1]);
	PIDInit(&PIDV[2]);
	PIDInit(&PIDV[3]);
	
//	SysTick_Config(168000);//168M
}


void ChassisControl(PID PIDV[4],RC_Ctl_t RC_CtrlData){
	
	if(RC_CtrlData.s1==3)
	{
		if(RC_CtrlData.ch0>1024+MINSPEED||RC_CtrlData.ch0<1024-MINSPEED)
		{
			xspeed=RC_CtrlData.ch0-1024;
		}else{
			xspeed=0;
		}
		if(RC_CtrlData.ch1>1024+MINSPEED||RC_CtrlData.ch1<1024-MINSPEED)
		{
			yspeed=RC_CtrlData.ch1-1024;
		}else{
			yspeed=0;
		}
		if(RC_CtrlData.ch2>1024+MINSPEED||RC_CtrlData.ch2<1024-MINSPEED)
		{
			rspeed=RC_CtrlData.ch2-1024;
		}else{
			rspeed=0;
		}		
		
		PIDV[0].SetPoint = 5*(+(yspeed)-(xspeed)+(rspeed));
		PIDV[1].SetPoint = 5*(-(yspeed)-(xspeed)+(rspeed));
		PIDV[2].SetPoint = 5*(+(yspeed)+(xspeed)+(rspeed));
		PIDV[3].SetPoint = 5*(-(yspeed)+(xspeed)+(rspeed));
	}else{
		PIDV[0].SetPoint =PIDV[1].SetPoint =PIDV[2].SetPoint =PIDV[3].SetPoint =0;
	}
	PIDControl();

}

//void SysTick_Handler(void)
//{				   
//	ChassisControl(PIDV,RC_CtrlData);
//}

void WheelRoll()
{
	TxMessage.StdId=0x200;
  TxMessage.ExtId=0;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.DLC=8;
	
  TxMessage.Data[0]=(WheelSpeed[0]>>8)&0xff;
	TxMessage.Data[1]=WheelSpeed[0]&0xff;
	TxMessage.Data[2]=(WheelSpeed[1]>>8)&0xff;
	TxMessage.Data[3]=WheelSpeed[1]&0xff;
	TxMessage.Data[4]=(WheelSpeed[2]>>8)&0xff;
	TxMessage.Data[5]=WheelSpeed[2]&0xff;
	TxMessage.Data[6]=(WheelSpeed[3]>>8)&0xff;
	TxMessage.Data[7]=WheelSpeed[3]&0xff;
  CAN_Transmit(CAN1, &TxMessage);
}

//PID Contorl

void PIDControl(void)
{
		WheelSpeed[0] = PIDCalc(&PIDV[0],MyWheel[0].speed);
		WheelSpeed[1] = PIDCalc(&PIDV[1],MyWheel[1].speed);
		WheelSpeed[2] = PIDCalc(&PIDV[2],MyWheel[2].speed);
		WheelSpeed[3] = PIDCalc(&PIDV[3],MyWheel[3].speed);	
		WheelRoll();
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) 
	{
		
		ChassisControl(PIDV,RC_CtrlData);
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
