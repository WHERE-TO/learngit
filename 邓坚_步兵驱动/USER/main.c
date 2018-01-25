#include "main.h"


int main()
{
	NVIC_PriorityGroupConfig(0x02);
	TIM3_OC_Init();	
  MPU6050_Init();
	TIM6_DELAY_Init();
	TIM7_PIDControl_Init();	
	MOTOR_820r_Init();
	REMOTE_Init();
  MOTOR_6223Init();

	while(1)
	{
		Control();		
		TIM6_delay(10);
	}
}
