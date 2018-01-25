#include "pid.h"

//typedef struct _posit_pid
//{
//	float  SET_SPEED; //  目标速度
//	float  ACT_SPEED;
//	
//  float err_now;      //  当前误差
//  float err_last; //  上次误差
//	float derror;
//	
//  float Kp,Ki,Kd; //  PID系数
//	
//	float ki_index;
//	float kp_index;
//	float kd_index;
//	
//  float integral; //  积分累加
//	
//	float ctrlout;  //控制量输出	
//	float Imax;
//	float derror_Max;	

//	float Imax_index;
//	
//	float ERROR1,ERROR2,ERROR3,ERROR4,ERROR5;
//	
//} POSIT_PID;


POSIT_PID gl_motor_PositPID=
{
	1000,   //SET_SPEED
	0,     //ACT_SPEED
	
	0,     //error_now
	0,     //err_last
	0,     //derror
	
	1.2,   //kp
	0,   //ki
	0,//1.7,   //kd
	
	1,     //ki_index
	1,     //kp_index
	1,     //kd_index
	
	0,     //intergrel
	0,     //ctrlout
	600,    //Imax
	15,    // derror_Max
	
	0.9,      //Imax_index
	
	10,   //ERROR1
	50,   //ERROR2
	90,   //ERROR3
	120,  //ERROR4
	300   //ERROR5
};

int Terror=0,Tintergrel=0,test=-1;//测试量
float Tindex[6]={1 , 0.8 , 0.6 , 0.4 , 0.2,0 } ;

void  Posit_PID_SetSpeed(float setSpeed)
{
	gl_motor_PositPID.SET_SPEED=setSpeed;
}
float Posit_PID_Conver(float actSpeed)
{
	gl_motor_PositPID.ACT_SPEED=actSpeed;	
	gl_motor_PositPID.err_now =gl_motor_PositPID.ACT_SPEED-gl_motor_PositPID.SET_SPEED;

//	
	Terror = gl_motor_PositPID.err_now;
//
	test=0;
///////////////////////积分分离
	
 if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR1 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR1 )//一级10
 {
	 	test=1;
		gl_motor_PositPID.ki_index=Tindex[0];
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)//
//	 {
//		 //test=1;
//		gl_motor_PositPID.ki_index=Tindex[0];	 
//	 }
//	else
//	{
//		//test=2;
//		gl_motor_PositPID.ki_index=Tindex[0];	 	
//	}	
	
 }
 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR2 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR2 ) //二级误差50 
 {
	 	test=2;
		gl_motor_PositPID.ki_index=Tindex[1];
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)//
//	 {
//		 //test=1;
//		gl_motor_PositPID.ki_index=Tindex[1];	 
//	 }
//	else
//	{
//		//test=2;
//		gl_motor_PositPID.ki_index=Tindex[1];	 	
//	}	
//	
 }
 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR3 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR3)  //三级误差90
 {
	 	test=3;
		gl_motor_PositPID.ki_index=Tindex[2];		 
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)//
//	 {
//		 //test=1;
//		gl_motor_PositPID.ki_index=Tindex[2];	 
//	 }
//	else
//	{
//		//test=2;
//		gl_motor_PositPID.ki_index=Tindex[2];	 	
//	}	
 }
 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR4 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR4)  //四级误差120
 {
	 test=4;
   gl_motor_PositPID.ki_index=Tindex[3];	 
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)//
//	 {
//		 //test=1;
//		//gl_motor_PositPID.ki_index=Tindex[3];
//		 if(gl_motor_PositPID.derror * gl_motor_PositPID.err_now>0)//远离
//		 {
//	  		gl_motor_PositPID.ki_index=Tindex[4];	 	 
//		 }
//		 else
//		 {
//			 gl_motor_PositPID.ki_index=Tindex[3];	
//		 }		 
//	 }
//	else
//	{
//		//test=2;
//		gl_motor_PositPID.ki_index=Tindex[3];	 	
//	}	
}	
 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR5 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR5)  //五级误差300
 {
	 test=5;	
  	gl_motor_PositPID.ki_index=Tindex[4];		 
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)
//	 {
//		 //test=1;
////		gl_motor_PositPID.ki_index=Tindex[5];	 
//		 if(gl_motor_PositPID.derror * gl_motor_PositPID.err_now>0)//远离
//		 {
//	  		gl_motor_PositPID.ki_index=Tindex[5];	 	 
//		 }
//		 else
//		 {
//			 gl_motor_PositPID.ki_index=0.5;	
//		 }
//	 }
//	else
//	{
//		//test=2;
//		gl_motor_PositPID.ki_index=0.5;	 	
//	}	 
 } 
 
 else
 {
	 	 test=6;
		gl_motor_PositPID.ki_index=Tindex[5];	 	 
//	 if(gl_motor_PositPID.derror>50||gl_motor_PositPID.derror<-50)
//	 {
//		 //test=8;
//		gl_motor_PositPID.ki_index=Tindex[5];	 
//	 }
//	else
//	{
//		//test=9;
//		gl_motor_PositPID.ki_index=Tindex[4];	 	
//	} 
	 
	 
	 /*
//	 if(gl_motor_PositPID.derror>200||gl_motor_PositPID.derror<-200)//不稳
//	 {
//		 gl_motor_PositPID.ki_index=0;
//		 test=1;
//	 }
//	 else                                                        // 稳定
//	 {
//		 gl_motor_PositPID.ki_index=0.8;
//		 test=2;		 
//   } 
	 	 */
  }

/////////////////////// 积分饱和判断
 if( gl_motor_PositPID.integral < gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax && gl_motor_PositPID.integral > - gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax)
 {
	 gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;
 }
 else if(gl_motor_PositPID.integral>= gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax)
 {
	 if(gl_motor_PositPID.err_now<0)
	  gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;
	 else
		gl_motor_PositPID.integral=gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax;
 }
 else if(gl_motor_PositPID.integral <= -gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax)
 {
	 if(gl_motor_PositPID.err_now>0)
	  gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;
	 else
		gl_motor_PositPID.integral=-gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax;
 }

 //////////////////////
	Tintergrel= gl_motor_PositPID.integral;
//	 
	gl_motor_PositPID.ctrlout=gl_motor_PositPID.Kp * gl_motor_PositPID.kp_index * gl_motor_PositPID.err_now+
		                        gl_motor_PositPID.Ki * gl_motor_PositPID.ki_index * gl_motor_PositPID.integral+
		                        gl_motor_PositPID.Kd * gl_motor_PositPID.kd_index * gl_motor_PositPID.derror;
	return gl_motor_PositPID.ctrlout;

/*	
//	gl_motor_PositPID.ACT_SPEED=actSpeed;
//	
//	gl_motor_PositPID.err_now =gl_motor_PositPID.ACT_SPEED-gl_motor_PositPID.SET_SPEED;
//	
//	Terror = gl_motor_PositPID.err_now;

//		 if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR1 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR1 )//一级误差10
//		 {	
//			 test=10;
//   		 gl_motor_PositPID.ki_index=1;
//			 gl_motor_PositPID.kd_index=1;		
//			 gl_motor_PositPID.kp_index=1;			 
//		 }			 
//		 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR2 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR2 ) //二级误差50
//		 {  
////		    gl_motor_PositPID.ki_index=0.8;
//			  if(gl_motor_PositPID.derror > gl_motor_PositPID.derror_Max||gl_motor_PositPID.derror < -gl_motor_PositPID.derror_Max)//不稳
//				{
//				    if( gl_motor_PositPID.err_now*gl_motor_PositPID.derror >=0 )  //急速离开
//						{
//							 test=10;
//							
//							 gl_motor_PositPID.ki_index=0.8;
//							 gl_motor_PositPID.kd_index=0.5;		
//							 gl_motor_PositPID.kp_index=1;							 
//						}
//						else
//						{
//							 test=20;
//							 gl_motor_PositPID.ki_index=0.8;
//							 gl_motor_PositPID.kd_index=1;		
//							 gl_motor_PositPID.kp_index=0.5;								
//						}
//				}
//				else                                     //稳定
//				{
//					 if(gl_motor_PositPID.err_now>=0)
//					 {
//						 
//						 test=30;
//						 
//							 gl_motor_PositPID.ki_index=1;
//							 gl_motor_PositPID.kd_index=1;		
//							 gl_motor_PositPID.kp_index=1;						 
//					 }
//					 else 
//					 {
//						 test=40;
//						 
//							 gl_motor_PositPID.ki_index=1;
//							 gl_motor_PositPID.kd_index=1;		
//							 gl_motor_PositPID.kp_index=1;
//						 
//					 }
////					test=30;
////					 gl_motor_PositPID.ki_index=Tindex[0];
////					 gl_motor_PositPID.kd_index=1;
////					
////					 gl_motor_PositPID.kp_index=1.1;
//				}
//		 }
//		 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR3 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR3)  //三级误差90
//		 {
////			  gl_motor_PositPID.ki_index=0.6;
//			 
//			 	if(gl_motor_PositPID.derror > gl_motor_PositPID.derror_Max||gl_motor_PositPID.derror < -gl_motor_PositPID.derror_Max)  //不稳
//				{
//					test=40;
//					gl_motor_PositPID.ki_index=0.6;
//					gl_motor_PositPID.kd_index=0.6;
//					gl_motor_PositPID.kp_index=1;
//					
////					 if(gl_motor_PositPID.derror * gl_motor_PositPID.err_now>=0)
////					 {
////						 
////						 test=50;
////						 
////							 gl_motor_PositPID.ki_index=0.6;
////							 gl_motor_PositPID.kd_index=0.8;		
////							 gl_motor_PositPID.kp_index=1;						 
////						 
////					 }
////					 else 
////					 {
////						 
////						 test=60;
////						 
////							 gl_motor_PositPID.ki_index=0.6;
////							 gl_motor_PositPID.kd_index=0.8;		
////							 gl_motor_PositPID.kp_index=0.5;						 
////					 }							
//				}
//				else                                            //稳定
//				{
////					 if(gl_motor_PositPID.err_now>=0)
////					 {
////						 test=70;
////						 
////							 gl_motor_PositPID.ki_index=1;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=0.9;						 
////					 }
////					 else 
////					 {
////						 test=80;
////						 
////							 gl_motor_PositPID.ki_index=1;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=0.9;						 
////					 }		
//					
//					test=50;
//					 gl_motor_PositPID.ki_index=Tindex[2];
//					 gl_motor_PositPID.kd_index=1;
//					
//					gl_motor_PositPID.kp_index=1.2;
//				}
//		 }
//		 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR4 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR4) //四级误差120
//		 {			 
////		    gl_motor_PositPID.ki_index=0.2;
//			 
//			 	if(gl_motor_PositPID.derror > gl_motor_PositPID.derror_Max||gl_motor_PositPID.derror < -gl_motor_PositPID.derror_Max)
//				{
//					test=60;
//					gl_motor_PositPID.ki_index=0;
//					gl_motor_PositPID.kd_index=0.6;
//					gl_motor_PositPID.kp_index=1;
//					//gl_motor_PositPID.Imax_index=0.5;
//					
////					 if(gl_motor_PositPID.derror * gl_motor_PositPID.err_now>=0)
////					 {
////						 test=90;
////						 
////						 gl_motor_PositPID.ki_index=0.2;
////						 gl_motor_PositPID.kd_index=0.5;		
////						 gl_motor_PositPID.kp_index=1.1;					 
////					 }
////					 else 
////					 {
////						 test=100;
////						 
////							 gl_motor_PositPID.ki_index=0.2;
////							 gl_motor_PositPID.kd_index=0.5;		
////							 gl_motor_PositPID.kp_index=0.9;						 
////					 }					
//				}
//				else
//				{
//					test=70;
//					 gl_motor_PositPID.ki_index=Tindex[3];
//					 gl_motor_PositPID.kd_index=1;
//					 gl_motor_PositPID.Imax_index=1;
//					
//					gl_motor_PositPID.kp_index=1.2;
//					
////					 if(gl_motor_PositPID.err_now>=0)
////					 {
////						 test=110;
////						 
////							 gl_motor_PositPID.ki_index=0.6;
////							 gl_motor_PositPID.kd_index=0.5;		
////							 gl_motor_PositPID.kp_index=1.1;								 
////					 }
////					 else 
////					 {
////						 test=120;
////						 
////							 gl_motor_PositPID.ki_index=0.6;
////							 gl_motor_PositPID.kd_index=0.5;		
////							 gl_motor_PositPID.kp_index=1.1;								 
////					 }					
//				}
//				
//		 }
//		 else if( gl_motor_PositPID.err_now > -gl_motor_PositPID.ERROR5 && gl_motor_PositPID.err_now < gl_motor_PositPID.ERROR5) //五级误差300
//     {	
////		    gl_motor_PositPID.ki_index=0;	
//			 
//			  if(gl_motor_PositPID.derror > gl_motor_PositPID.derror_Max||gl_motor_PositPID.derror < -gl_motor_PositPID.derror_Max)
//				{
//					test=80;
//					gl_motor_PositPID.ki_index=0.6;
//					gl_motor_PositPID.kd_index=0.6;
//					
//					gl_motor_PositPID.kp_index=1;
//					//gl_motor_PositPID.Imax_index=0.5;
//					
////					 if(gl_motor_PositPID.derror*gl_motor_PositPID.err_now>=0)
////					 {
////						 test=130;
////						 
////							 gl_motor_PositPID.ki_index=0.5;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=1.1;								 
////					 }
////					 else 
////					 {
////						 test=140;
////						 
////							 gl_motor_PositPID.ki_index=0.5;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=0.9;								 
////					 }					
//				}
//				else                                    //稳定
//				{
//					test=90;
//					 gl_motor_PositPID.ki_index= 1;//Tindex[4];
//					 gl_motor_PositPID.kd_index=1;
//					gl_motor_PositPID.Imax_index=1;
//					
//					gl_motor_PositPID.kp_index=1.2;
//					
////					 if(gl_motor_PositPID.err_now>=0)
////					 {
////						 test=150;
////						 
////							 gl_motor_PositPID.ki_index=1;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=1;								 
////					 }
////					 else 
////					 {
////						 test=160;
////						 
////							 gl_motor_PositPID.ki_index=1;
////							 gl_motor_PositPID.kd_index=1;		
////							 gl_motor_PositPID.kp_index=1;								 
////					 }					
//				}			 
//		 }
//		 else 
//		 {
//			 test=170;
//							 gl_motor_PositPID.ki_index=0;
//							 gl_motor_PositPID.kd_index=1;		
//							 gl_motor_PositPID.kp_index=1.1;	 
//		 }
//		 
//		 
//		 
//		 if( gl_motor_PositPID.integral < gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax && gl_motor_PositPID.integral > - gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax)
//		   gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;
//		 else if(gl_motor_PositPID.integral> gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax && gl_motor_PositPID.err_now<0)
//       gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;
//		 else if(gl_motor_PositPID.integral < -gl_motor_PositPID.Imax_index*gl_motor_PositPID.Imax && gl_motor_PositPID.err_now>0)
//       gl_motor_PositPID.integral+=gl_motor_PositPID.err_now;

//	//
//		Tintergrel= gl_motor_PositPID.integral;
//	//	 
//	gl_motor_PositPID.ctrlout=gl_motor_PositPID.Kp * gl_motor_PositPID.kp_index * gl_motor_PositPID.err_now+
//		                        gl_motor_PositPID.Ki * gl_motor_PositPID.ki_index * gl_motor_PositPID.integral+
//		                        gl_motor_PositPID.Kd * gl_motor_PositPID.kd_index * gl_motor_PositPID.derror;
//	return gl_motor_PositPID.ctrlout;
*/







}
//





