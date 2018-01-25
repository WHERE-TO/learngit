#include "timer.h"

#define Angle_MAX  8191 

void TIM3_OC_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
//////////////////////////////////////ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	    //TIM3ʱ��ʹ��
//////////////////////////////////////���ų�ʼ��////////////////////////	
  //PB0
  //PB1	
	//���Ÿ���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//////////////////////////////////////TIM3��ʱ���ʼ��
  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;                    //Ƶ��50hz
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
//////////////////////////////////////TIM3�Ƚ�ͨ����ʼ��
//CH3  CH4
//���ϼ�����PWM1ģʽ,ռ�ձȳ�ʼ��Ϊ5%,Ƶ��50hz
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse=1000;                                    //ռ�ձ�5%��
//CH3	
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);	
//CH4
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
//ʹ��
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






void TIM7_PIDControl_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
//ʱ�ӿ���	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
//ʱ������
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=500-1;//0.5ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);	
//NVIC	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
  NVIC_Init(&NVIC_InitStructure);
//ʹ���ж� ��ʱ��
	TIM_Cmd(TIM7,ENABLE);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
}



int ctrLF=0,ctrRF=0,ctrLR=0,ctrRR=0,ctrYAW=0,ctrPITCH=0;
int ctrgambilP=0;

extern POSIT_PID gl_wheelLF_PositPid;
extern POSIT_PID gl_wheelLR_PositPid;
extern POSIT_PID gl_wheelRF_PositPid;
extern POSIT_PID gl_wheelRR_PositPid;
extern POSIT_PID gl_gambilPitch_PositPid;
extern Info_820r Msg_Wheel[4];
extern  RC_Ctl_t RC_Ctl;//

int test_Ang=0;

int Angle_Last=0,Angle_Now=0,Angle_dCge=0,Angle_ICge=0,Origin=0;
u8 first=1;

////////////////////////////
//int YAW=0,PITCH=0;

//////////////////////////////

void TIM7_IRQHandler()
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{	
	  if(RC_Ctl.rc.s1==3)  //����ģʽ		
		{
		// �ɼ��ٶ�		
				gl_wheelLF_PositPid.ACT_POINT=Msg_Wheel[2].act_Speed;
				gl_wheelLR_PositPid.ACT_POINT=Msg_Wheel[0].act_Speed;
				gl_wheelRF_PositPid.ACT_POINT=Msg_Wheel[3].act_Speed;
				gl_wheelRR_PositPid.ACT_POINT=Msg_Wheel[1].act_Speed;			
		//PID����
				ctrLF=Posit_PID_Conver(&gl_wheelLF_PositPid);	
				ctrRF=Posit_PID_Conver(&gl_wheelRF_PositPid);
				ctrLR=Posit_PID_Conver(&gl_wheelLR_PositPid);
				ctrRR=Posit_PID_Conver(&gl_wheelRR_PositPid);
			
				MOTOR_820rWheel(ctrLF,ctrRF,ctrLR,ctrRR);           //
				//����PID����
////////////////////////////////////////////////////////////
			
			
			MOTOR_6223Gambil(ctrPITCH,ctrYAW);
////////////////////////////////////////////////////////////
		}
		else if(RC_Ctl.rc.s1==2) //λ�ñջ�����ģʽ
		{
			test_Ang=Msg_Wheel[3].act_Angle;
			
/////////////////////��ȡAngle_ICge
			Angle_Now=Msg_Wheel[3].act_Angle;	
			Angle_dCge=Angle_Now-Angle_Last;
			
			if(first==1)
			{
				Angle_dCge =0;
				first=0;
			}
			else
			{				
					//�� Angle_dCge �����㴦��
					if(Angle_dCge>4000)         
					{
						Angle_dCge-=Angle_MAX;
					}
					else if(Angle_dCge<-4000) //�������
					{
						Angle_dCge+=Angle_MAX;
					}
		  }
			Angle_Last=Angle_Now;	
			Angle_ICge+=Angle_dCge;
/////////////////////////////////////////					
			//λ�û�
			gl_gambilPitch_PositPid.ACT_POINT=Angle_ICge;//	
			ctrgambilP= Posit_PID_Conver(&gl_gambilPitch_PositPid);
		
			//�ٶȻ�
			gl_wheelLF_PositPid.SET_POINT =  -ctrgambilP; 
			gl_wheelLR_PositPid.SET_POINT =  -ctrgambilP; 
			gl_wheelRF_PositPid.SET_POINT =  ctrgambilP; 
			gl_wheelRR_PositPid.SET_POINT =  ctrgambilP; 			
////			
			gl_wheelLF_PositPid.ACT_POINT=Msg_Wheel[2].act_Speed;
			gl_wheelLR_PositPid.ACT_POINT=Msg_Wheel[0].act_Speed;
			gl_wheelRF_PositPid.ACT_POINT=Msg_Wheel[3].act_Speed;
			gl_wheelRR_PositPid.ACT_POINT=Msg_Wheel[1].act_Speed;	

			ctrLF=Posit_PID_Conver(&gl_wheelLF_PositPid);	
			ctrRF=Posit_PID_Conver(&gl_wheelRF_PositPid);
			ctrLR=Posit_PID_Conver(&gl_wheelLR_PositPid);
			ctrRR=Posit_PID_Conver(&gl_wheelRR_PositPid);
			
			MOTOR_820rWheel(ctrLF,ctrRF,ctrLR,ctrRR);           //			
			
		}
		else if(RC_Ctl.rc.s1==1)
		{
			
			
		}	
	
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}


void TIM6_DELAY_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//ʱ�ӿ���	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
//ʱ������
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=1000-1;//1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);	
//ʹ���ж� ��ʱ��
	TIM_Cmd(TIM6,DISABLE);
}

void TIM6_delay(u16 ms)
{
	TIM_Cmd(TIM6,ENABLE);
  TIM_SetCounter(TIM6,0);

  while(ms)
	{
		while(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET);
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);			
		ms--;
	}		
}

