#ifndef _MOTOR_820R_H
#define _MOTOR_820R_H

#include "sys.h"
#include "can.h"
#include "pid.h"

void MOTOR_820r_Init(void);
void MOTOR_820rWheel(int LFwheel,int RFwheel,int LRwheel,int RRwheel);

#endif
