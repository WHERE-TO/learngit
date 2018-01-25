#include "motor6223.h"

void MOTOR_6223Init(void)
{
   CAN2_MODE_Init();
}	

u8 CAN2SendMsg[8]={0};
void MOTOR_6223Gambil(int pitch,int yaw)
{
	Limit(&pitch,5000,-5000);
	Limit(&yaw,5000,-5000);	
	
	CAN2SendMsg[0]= yaw>>8;
	CAN2SendMsg[1]= yaw&0xff;
	CAN2SendMsg[2]= pitch>>8;
	CAN2SendMsg[3]= pitch&0xff;	

	CAN2_SEND_MSG(CAN2SendMsg,8);
}


