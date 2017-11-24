#include "timer.h"

void TIM3_OC_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
//////////////////////////////////////时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3,ENABLE);	    //TIM3时钟使能
//////////////////////////////////////引脚初始化////////////////////////	
  //PB0
  //PB1	
	//引脚复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//////////////////////////////////////TIM3计时体初始化
  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;                    //频率50hz
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
//////////////////////////////////////TIM3比较通道初始化
//CH3  CH4
//向上计数，PWM1模式,占空比初始化为5%,频率50hz
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse=1000;                                    //占空比5%，
//CH3	
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);	
//CH4
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
//使能
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
  TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_Cmd(TIM3, ENABLE);

}


void TIM3_OCH3_PLUSE(u32 pluse)
{
	TIM_SetCompare3(TIM3, pluse);
}

void TIM3_OCH4_PLUSE(u32 pluse)
{
	TIM_SetCompare4(TIM3, pluse);
}

