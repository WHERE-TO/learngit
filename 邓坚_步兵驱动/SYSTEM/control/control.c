#include "control.h"



extern  RC_Ctl_t RC_Ctl;//

extern POSIT_PID gl_wheelLF_PositPid;
extern POSIT_PID gl_wheelLR_PositPid;
extern POSIT_PID gl_wheelRF_PositPid;
extern POSIT_PID gl_wheelRR_PositPid;
extern POSIT_PID gl_gambilPitch_PositPid;

int carSpeedx=0, carSpeedy=0,carSpeedw=0,carGambilp=0;
u8 speedFACTOR=10,AngleFACTOR=12*5;

void Limit(int*Num,int max,int min)// 数值限幅
{
	 if(*Num > max )
		 *Num=max;
	 else if(*Num < min)
		 *Num=min;	
}

void Control(void)
{
	if(RC_Ctl.rc.s1==3)  //启动
	{		
		int setLF=0,setLR=0,setRF=0,setRR=0;
//处理remote信号		
   	carSpeedx=1024-RC_Ctl.rc.ch0;
		carSpeedy=1024-RC_Ctl.rc.ch1;
		carSpeedw=1024-RC_Ctl.rc.ch2;
		
		carGambilp=RC_Ctl.rc.ch3-1024;
//麦克纳姆轮速度分解		
		setLF=	speedFACTOR*(-carSpeedy - carSpeedx - carSpeedw);
		setLR=  speedFACTOR*(-carSpeedy + carSpeedx - carSpeedw);
		setRF=  speedFACTOR*(+carSpeedy - carSpeedx - carSpeedw);
		setRR=	speedFACTOR*(+carSpeedy + carSpeedx - carSpeedw);		
		Limit(&setLF,7500,-7500 );
		Limit(&setLR,7500,-7500 );
		Limit(&setRF ,7500,-7500 );
		Limit(&setRR,7500,-7500 );
		
		gl_wheelLF_PositPid.SET_POINT=setLF;
		gl_wheelLR_PositPid.SET_POINT=setLR;
		gl_wheelRF_PositPid.SET_POINT=setRF;
		gl_wheelRR_PositPid.SET_POINT=setRR;

    gl_gambilPitch_PositPid.SET_POINT = AngleFACTOR*carGambilp;	
	//电机的PID控制，改变setpoint	
	}
	else if(RC_Ctl.rc.s1==2)//位置闭环测试模式
	{
		//处理remote信号
		carGambilp=RC_Ctl.rc.ch3-1024;		
		gl_gambilPitch_PositPid.SET_POINT = AngleFACTOR*carGambilp;	
	}
  else if(RC_Ctl.rc.s1==1)//进机关闭
	{
		MOTOR_820rWheel(0,0,0,0);           //	
		
	}
}






