#ifndef _CAN_H
#define _CAN_H
#include "stm32f4xx_conf.h"

typedef struct{
	int angel;
	int speed;
}Wheel;

void CANInit(void);

#endif
