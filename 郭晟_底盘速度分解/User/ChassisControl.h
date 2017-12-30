#include "stm32f4xx_conf.h"
#include "can.h"
#include "pid.h"
#include "tim.h"
#include "Remotecontrol.h"

extern Wheel MyWheel[4];
extern CanTxMsg TxMessage;
extern RC_Ctl_t RC_CtrlData;

void ChassisControl_Init();
void ChassisControl(PID PIDV[4],RC_Ctl_t RC_CtrlData);
void SysTick_Handler(void);
void WheelRoll();
void PIDControl(void);