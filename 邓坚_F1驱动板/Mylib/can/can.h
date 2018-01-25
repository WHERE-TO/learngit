#ifndef _CAN_H
#define _CAN_H

#include "stm32f10x.h"
//CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,
//////////////////////////////// u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode
#define CAN_mode    CAN_Mode_Normal//CAN_Mode_Silent_LoopBack//CAN_Mode_Normal
#define CAN_tsjw    CAN_SJW_1tq
#define CAN_tbs1    CAN_BS1_9tq
#define CAN_tbs2    CAN_BS2_8tq
#define CAN_brp     4

#define CAN_RX_Filter_ID  0x200   //  ID_standard
#define CAN_TX_Filter_ID  0x12   //
#define CAN_FIFOx       0      //
#define CAN_LEN         8

////////////////////////////
u8 CAN_MODE_Init(void);
u8 CAN_SEND_MSG(u8*msg,u8 len);

#endif
