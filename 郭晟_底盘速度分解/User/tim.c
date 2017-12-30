#include "tim.h"

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  timb;
  NVIC_InitTypeDef nvic;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	timb.TIM_Period = 10000-1; 	
	timb.TIM_Prescaler=84-1; 
	timb.TIM_CounterMode=TIM_CounterMode_Up; 
	timb.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&timb);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 

	TIM_Cmd(TIM2,ENABLE);
 	nvic.NVIC_IRQChannel = TIM2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);	
}
