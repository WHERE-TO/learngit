#include "stm32f4xx.h"
#include "delay.h"
#include "timer.h"
#include "mpu6050.h"

int main()
{
  delay_init(10);
	TIM3_OC_Init();
	NVIC_PriorityGroupConfig(0x02);
  MPU6050_Init();

	while(1);
}
