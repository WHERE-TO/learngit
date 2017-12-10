#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>



#define PWMPeriod 2000

TIM_TimeBaseInitTypeDef  timb;
TIM_ICInitTypeDef  timic;
int count,counta,countb = 0;
float SpeedRate = 1000.0f;
float RSpeed,SpeedError;
typedef struct
{
	float P,I,D;

	float SetPoint;
	float IMax;
	float PreError,LastError,SumError;
	
	float POut,IOut,DOut;	
	
}PID;


void RCC_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void TIM_Config(void);
void PWM_Config(void);
void PIDInit(PID *P);
float PIDCalc(PID *P,float NextPoint);
PID PIDV;
int main(void)
{       

  RCC_Config();
	GPIO_Config();
	PWM_Config();
	TIM_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_Config();

	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
  TIM_ICStructInit(&timic);
  TIM_ICInit(TIM3, &timic);
	
	TIM3->CNT =0; 
  TIM_Cmd(TIM3, ENABLE);
	PIDInit(&PIDV);
  while (1){

	}
	
}

void PIDInit(PID *P){
	P->P=0.39f;
	P->I=0.073f;
	P->D=0.00f;
	P->IMax=1000.0f;
	P->SetPoint=1500.0f;
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) 
	{

		countb = TIM_GetCounter(TIM3);
		count = countb - counta;
		counta = countb;
		if(count > 20000){
			count -= 65535;
		}else if(count < -20000){
			count += 65535;
		}
		RSpeed=count;
		SpeedError = PIDCalc(&PIDV,RSpeed);

	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET) 
	{
//		if(SpeedRate>2000){
//			SpeedRate = 0;
//		}else if(SpeedRate<-2000){
//			SpeedRate = .0;
//		}
		SpeedRate = SpeedError;
		TIM_SetCompare1(TIM1,(PWMPeriod+SpeedRate)/2);
		TIM_SetCompare2(TIM1,(PWMPeriod-SpeedRate)/2);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

void RCC_Config(void)
{
  /* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM3 channel 2 pin (PA.07) Init */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void TIM_Config(void)
{
	
	timb.TIM_Period = 10000-1; 	
	timb.TIM_Prescaler=84-1; 
	timb.TIM_CounterMode=TIM_CounterMode_Up; 
	timb.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&timb);
	
	timb.TIM_Period = 1000-1; 
	timb.TIM_Prescaler=84-1; 
	TIM_TimeBaseInit(TIM4,&timb);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 
	
	TIM_Cmd(TIM2,ENABLE); 
	TIM_Cmd(TIM4,ENABLE);	
	
	TIM_DeInit(TIM3);
  TIM_TimeBaseInit(TIM3, &timb);
     
  timb.TIM_Period =0xffff; 
  timb.TIM_Prescaler =0;       
  timb.TIM_ClockDivision =TIM_CKD_DIV1 ;  
  timb.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM3, &timb);
}

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PWM_Config(){
    TIM_TimeBaseInitTypeDef TIM_BaseStructure;
		TIM_OCInitTypeDef TIM_OCStructure;
    
		GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_BaseStructure);
    TIM_BaseStructure.TIM_Prescaler = 0;
    TIM_BaseStructure.TIM_Period = PWMPeriod - 1;
    TIM_TimeBaseInit(TIM1, &TIM_BaseStructure);
	
    TIM_OCStructInit(&TIM_OCStructure);
    TIM_OCStructure.TIM_OCMode = TIM_OCMode_PWM1;
		
    TIM_OC1Init(TIM1, &TIM_OCStructure);
    TIM_OC2Init(TIM1, &TIM_OCStructure);
	
		TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
		TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
		TIM_ARRPreloadConfig(TIM1,ENABLE);

		TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);
		TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);
		
		TIM_Cmd(TIM1,ENABLE);
		TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

float PIDCalc(PID *P,float NextPoint){
	float dError,Error;
	Error = P->SetPoint - NextPoint;
	
	P->SumError += Error;
	dError = Error - P->LastError;
	P->PreError = P->LastError;
	P->LastError = Error;
	
	
	if(P->SumError >= P->IMax)
		P->SumError = P->IMax;
	else if(P->SumError <= -P->IMax)
		P->SumError = -P->IMax;		
	
	P->POut = P->P*Error;
	P->IOut = P->I*(P->SumError);
	P->DOut = P->D*dError;
	
	return P->POut+P->IOut+P->DOut;
}
