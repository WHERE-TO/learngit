#ifndef _CAN1_H
#define _CAN1_H

#include "main.h"

//////////////////////////////// CAN1
#define CAN1_mode    CAN_Mode_Normal//CAN_Mode_Silent_LoopBack//
#define CAN1_tsjw    CAN_SJW_1tq
#define CAN1_tbs1    CAN_BS1_7tq
#define CAN1_tbs2    CAN_BS2_6tq
#define CAN1_brp     3

//#define CAN1_RX_Filter_ID  0x201   //  ID_standard
#define CAN1_TX_Filter_ID  0x200   //

#define CAN1_Wheel_LF   0x203
#define CAN1_Wheel_RF   0x204
#define CAN1_Wheel_LR   0x201
#define CAN1_Wheel_RR   0x202

#define CAN1_FIFOx       0      //

////////////////////////////////////CAN2
#define CAN2_mode    CAN_Mode_Normal//CAN_Mode_LoopBack//
#define CAN2_tsjw    CAN_SJW_1tq
#define CAN2_tbs1    CAN_BS1_7tq
#define CAN2_tbs2    CAN_BS2_6tq
#define CAN2_brp     3

//#define CAN2_RX_Filter_ID  0x200   //  ID_standard
#define CAN2_TX_Filter_ID  0x1ff   //  ±ê×¼

#define CAN2_Gambil_Pitch 0x205 
#define CAN2_Gambil_Yaw   0x206

#define CAN2_FIFOx       0      //
////////////////////////////////////////////////////
typedef struct
{
	short act_Speed;
	unsigned short act_Angle; 	
}Info_820r;

typedef struct
{
	short act_Current;
	short set_Current;
	unsigned short act_Angle;
}Info_6223;

////////////////////////////
u8 CAN1_MODE_Init(void);
u8 CAN1_SEND_MSG( u8 *msg,u8 len);
//////////////////////////////////////
u8 CAN2_MODE_Init(void);
u8 CAN2_SEND_MSG(u8 *msg,u8 len);

#endif

