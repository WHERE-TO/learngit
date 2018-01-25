#include "timer.h"
#include "pid.h"
#include "motor.h"

void TIM1_OC_Init(void)//
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
////////////////// ʱ��ʹ��////////////////////////////////////////
	RCC_APB2PeriphClockCmd(TIM1_CH1_GPIO_CLK,ENABLE);    //GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //TIM1
/////////////////////ͨ������///////////////////////////////////////////////	
//ͨ��1 PA8	
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin= TIM1_OCH1_GPIOx_PIN;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(TIM1_OCH1_GPIOx,&GPIO_InitStructure);
//ͨ��2 PA9	
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin= TIM1_OCH2_GPIOx_PIN;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(TIM1_OCH2_GPIOx,&GPIO_InitStructure);	
	
//  // ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��
//	RCC_APB2PeriphClockCmd(TIM1_BRKIN_GPIO_CLK,ENABLE);
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Pin= TIM1_BRKIN_GPIOX_PIN;
//  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
//	GPIO_Init(TIM1_BRKIN_GPIOx,&GPIO_InitStructure);

//  GPIO_ResetBits(TIM1_BRKIN_GPIOx,TIM1_BRKIN_GPIOX_PIN);
////////////////////////////////////////////////////////////	
/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitStructure.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=TIM1_PER;
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM1_PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode=  TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=   TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//ʧ��
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;//ʹ��
	TIM_OCInitStructure.TIM_Pulse=TIM1_PULSE;
//ͨ��1	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
//ͨ��2
  TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Disable;//ʹ��
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//ʹ��	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
/////////////////////////////////////////////////////////////////
/*-------------------ɲ���������ṹ���ʼ��-------------------*/	        //���ù�
  TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;                //ʧ��
	TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_DeadTime=0;
	TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
////////////////////////////ʹ��////////////////////////////////////	
// ʹ�ܼ�����
	TIM_Cmd(TIM1, ENABLE);	
// �����ʹ��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void TIM1_OCH1_setForward(u16 plus)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode=  TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=   TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//ʧ��
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;//ʹ��
	TIM_OCInitStructure.TIM_Pulse=plus;
//ͨ��1	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//ͨ��2
  TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Disable;//ʹ��
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
//ʹ��	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
}
void TIM1_OCH2_setBack(u16 plus)
{
   TIM_OCInitTypeDef  TIM_OCInitStructure;
	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode=  TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=   TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//ʧ��
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;//ʹ��
	TIM_OCInitStructure.TIM_Pulse=plus;
//ͨ��2	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
//ͨ��1
  TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Disable;//ʹ��
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//ʹ��	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
}

///////////////////////////////////������/////////////////////////////////////////////
//PA6   TIM3CH1
//PA7   TIM3CH2
void TIM3_ECODE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
//  NVIC_InitTypeDef        NVIC_InitStructure;
//////////////////////ʱ��ʹ��//////////////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//////////////////ͨ��GPIO����//////////////////////////////
//////CH1
//////CH2	
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;   //��������
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	  

//////////////////ʱ����Ԫ����//////////////////////////////
  TIM_TimeBaseInitStructure.TIM_ClockDivision=  TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65535;
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;       //û�õ������ùܡ�
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
//////////////////������ģʽ����//////////////////////////////
//T1T2   ˫����
  TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
///////////////////�����˲���///////////////////////////////////////////////////////////////////////////////////////////////
	TIM_ICStructInit(&TIM_ICInitStructure);																																
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
//����������
  TIM_SetCounter(TIM3, 0);
//////////////////����ж�����//////////////////////////////////////////////////////////////////////////////////
//  NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
//	NVIC_Init(&NVIC_InitStructure);
//////////////ʹ�ܶ�ʱ�����ж�//////////////////////////////////	
  //TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);	
	//TIM3->CNT
}

/////////////////////////////////


void TIM4_GetSpeed_Init(void)                                  //�ٶȲ���
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
 	NVIC_InitTypeDef        NVIC_InitStructure;
///////////////////////////ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	
//ʱ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=  TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10000-1;   //10ms  100000
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;       
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);	

  NVIC_InitStructure.NVIC_IRQChannel= TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_Init(&NVIC_InitStructure);
////////////////ʹ�ܶ�ʱ�����ж�//////////////////////////////////	
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM4, ENABLE);	
}


int Verlocity=0;
int ECODE_CNTx=0,ECODE_CNT1=0,ECODE_CNT2=0;

void TIM4_IRQHandler()
{
   if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	 {
		 ECODE_CNT2=TIM_GetCounter(TIM3);//��ȡ��ǰ����
		 ECODE_CNTx=ECODE_CNT2-ECODE_CNT1;//�仯��
		 ECODE_CNT1=ECODE_CNT2;          //Ϊ��һ�β�����׼
		// ���㴦���������	
		if(ECODE_CNTx<-20000)//�������
		{
			ECODE_CNTx+=0xffff;
		}
		else if(ECODE_CNTx>20000)             //�������
		{
			ECODE_CNTx-=0xffff;
		} 
		Verlocity=ECODE_CNTx;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	 }	
}//  512*4

void TIM2_PIDcontrl_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
 	NVIC_InitTypeDef        NVIC_InitStructure;
///////////////////////////ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	
//ʱ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=  TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=1000-1;   //1ms  
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;       
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	

  NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStructure);
////////////////ʹ�ܶ�ʱ�����ж�//////////////////////////////////	
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);
}

int Tderror=0;   //������
int Ctrout=0,time=0;

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		//
		time++;
		if(time==30)    //30ms
		{		
			gl_motor_PositPID.derror=gl_motor_PositPID.err_now-gl_motor_PositPID.err_last;
			Tderror=gl_motor_PositPID.derror;
			gl_motor_PositPID.err_last=gl_motor_PositPID.err_now;	
			time=0;
		}
    //
		Ctrout=Posit_PID_Conver(Verlocity);
		MOTOR_SET( Ctrout);
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		
	}	
}




