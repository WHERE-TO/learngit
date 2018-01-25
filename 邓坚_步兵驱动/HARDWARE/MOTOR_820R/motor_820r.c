#include "motor_820r.h"

void MOTOR_820r_Init(void)
{
	CAN1_MODE_Init();	
}
///////////////////////////////////////////
u8 CAN1SendMsg[8]={0};

void MOTOR_820rWheel(int LFwheel,int RFwheel,int LRwheel,int RRwheel)//LF RF LR RR 设置四个轮子的速度
{
	Limit(&LFwheel,10000,-10000);
	Limit(&RFwheel,10000,-10000);
	Limit(&LRwheel,10000,-10000);
	Limit(&RRwheel,10000,-10000);	
	
	CAN1SendMsg[0]=LRwheel>>8;
	CAN1SendMsg[1]=LRwheel&0xff;	
	
	CAN1SendMsg[2]=RRwheel>>8;
	CAN1SendMsg[3]=RRwheel&0xff;
	
	CAN1SendMsg[4]=LFwheel>>8;
	CAN1SendMsg[5]=LFwheel&0xff;
	
	CAN1SendMsg[6]=RFwheel>>8;	
	CAN1SendMsg[7]=RFwheel&0xff;
	
	CAN1_SEND_MSG(CAN1SendMsg,8);
}

