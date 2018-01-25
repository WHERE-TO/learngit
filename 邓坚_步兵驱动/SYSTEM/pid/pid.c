#include "pid.h"

int TEST_err=0;


POSIT_PID gl_wheelLF_PositPid=
{
  0,//SET_SPEED
  0,//ACT_SPEED

  0,//  err_now
	0,//  err_last
	0,// derror
	
	10,//kp  10
	0.8,//ki  0.8
	1,//kd   1
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	1000,//Imax	
	
	10,  //ERR1
	50,  //ERR2
	100  //ERR3
};
POSIT_PID gl_wheelLR_PositPid=
{
  0,//SET_SPEED
  0,//ACT_SPEED

  0,//  err_now
	0,//  err_last
	0,// derror
	
	10,//kp 
	0.8,//ki 
	1,//kd    
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	1000,//Imax

	10,  //ERR1
	50,  //ERR2
	100  //ERR3
};

POSIT_PID gl_wheelRF_PositPid=
{
  0,//SET_SPEED
  0,//ACT_SPEED

  0,//  err_now
	0,//  err_last
	0,// derror
	
	9,//kp
	0.2,//ki
	1,//kd
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	1000,//Imax

	10,  //ERR1
	50,  //ERR2
	100  //ERR3	
};

POSIT_PID gl_wheelRR_PositPid=
{
  0,//SET_SPEED
  0,//ACT_SPEED

  0,//  err_now
	0,//  err_last
	0,// derror
	
	10,//kp
	1,//ki
	1,//kd
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	1000,//Imax	
	
	10,  //ERR1
	50,  //ERR2
	100  //ERR3	
};




POSIT_PID gl_gambilPitch_PositPid=
{
  0,//SET_SPEED
  0,//ACT_SPEED

  0,//  err_now
	0,//  err_last
	0,// derror
	
	1,//kp  1
	0.1,//ki  0.1
	1,//kd   1
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	500,//Imax	
	
	0,
	0,
	0,
};



POSIT_PID gl_gambil_PitchCur_PositPid=
{
	0,//SET
  0,//ACT

  0,//  err_now
	0,//  err_last
	0,// derror
	
	0,//kp  1
	0,//ki  0.1
	1,//kd   1
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	500,//Imax	
	
	0,
	0,
	0,
	
};

POSIT_PID gl_gambil_PitchAng_PositPid=
{
  0,//SET
  0,//ACT

  0,//  err_now
	0,//  err_last
	0,// derror
	
	1,//kp  1
	0.1,//ki  0.1
	1,//kd   1
	
	1,//ki_index
	
	0,//integral
	
	0,//ctrlout
	500,//Imax	
	
	0,
	0,
	0,	
	
};

float Posit_PID_Conver(POSIT_PID* gl_wheelx_PositPid)
{
//	
	gl_wheelx_PositPid->err_now = gl_wheelx_PositPid->SET_POINT - gl_wheelx_PositPid->ACT_POINT;
	gl_wheelx_PositPid->derror = gl_wheelx_PositPid->err_now - gl_wheelx_PositPid->err_last;
	
	TEST_err=gl_wheelx_PositPid->err_now;
	//积分分离
	if(gl_wheelx_PositPid->err_now < gl_wheelx_PositPid->ERR1 && gl_wheelx_PositPid->err_now > -gl_wheelx_PositPid->ERR1)
	{
		gl_wheelx_PositPid->ki_index=1;
	}
	else if(gl_wheelx_PositPid->err_now <gl_wheelx_PositPid->ERR2 && gl_wheelx_PositPid->err_now > -gl_wheelx_PositPid->ERR2)
	{
		gl_wheelx_PositPid->ki_index=0.8;
	}
	else if(gl_wheelx_PositPid->err_now <gl_wheelx_PositPid->ERR3 && gl_wheelx_PositPid->err_now > -gl_wheelx_PositPid->ERR3)
	{
		gl_wheelx_PositPid->ki_index=0.6;
	}
	else
	{
		gl_wheelx_PositPid->ki_index=0.2;
	}
	
	
	//积分饱和判断
	if(gl_wheelx_PositPid->integral > -gl_wheelx_PositPid->Imax && gl_wheelx_PositPid->integral < gl_wheelx_PositPid->Imax)
	   gl_wheelx_PositPid->integral += gl_wheelx_PositPid->err_now;
	else if(gl_wheelx_PositPid->integral >= gl_wheelx_PositPid->Imax)
	{
		if(gl_wheelx_PositPid->err_now < 0 )
		   gl_wheelx_PositPid->integral += gl_wheelx_PositPid->err_now;
		else
			 gl_wheelx_PositPid->integral = gl_wheelx_PositPid->Imax;
	}
	else if(gl_wheelx_PositPid->integral <= -gl_wheelx_PositPid->Imax)
	{
		if(gl_wheelx_PositPid->err_now > 0 )
		   gl_wheelx_PositPid->integral += gl_wheelx_PositPid->err_now;
		else
			 gl_wheelx_PositPid->integral = -gl_wheelx_PositPid->Imax;
	}
  //
	gl_wheelx_PositPid->err_last = gl_wheelx_PositPid->err_now;
	gl_wheelx_PositPid->ctrlout= gl_wheelx_PositPid->Kp * gl_wheelx_PositPid->err_now +
	                             gl_wheelx_PositPid->ki_index * gl_wheelx_PositPid->Ki * gl_wheelx_PositPid->integral +
	                             gl_wheelx_PositPid->Kd * gl_wheelx_PositPid->derror ;
	
  return gl_wheelx_PositPid->ctrlout;
}

