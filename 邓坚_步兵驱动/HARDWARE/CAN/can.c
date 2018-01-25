#include "can.h"

u8 CAN1_MODE_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
		
  	CAN_InitStructure.CAN_Mode=CAN1_mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=CAN1_tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN1_tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN1_tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=CAN1_brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=1;//0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN1_FIFOx;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);// 
		
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0x02;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  
  	return 0;	
}

u8 CAN1_SEND_MSG(u8 *msg,u8 len)
{
	u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId= CAN1_TX_Filter_ID;	 // 标准标识符为0
  TxMessage.ExtId=0x00;	 // 设置扩展标示符（29位）
  TxMessage.IDE=CAN_Id_Standard;		  //
  TxMessage.RTR=CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		
}

CanRxMsg Can1RxMsg;
Info_820r Msg_Wheel[4]={0};

void CAN1_RX0_IRQHandler(void)//1 khz
{
	if( CAN_MessagePending(CAN1,CAN_FIFO0)!=0)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &Can1RxMsg);

		if(Can1RxMsg.StdId==CAN1_Wheel_LF)
		{
			Msg_Wheel[2].act_Speed= Can1RxMsg.Data[2]<<8|Can1RxMsg.Data[3];
 			Msg_Wheel[2].act_Angle= Can1RxMsg.Data[0]<<8|Can1RxMsg.Data[1];
		}
		else if(Can1RxMsg.StdId==CAN1_Wheel_RF)
		{	
    	Msg_Wheel[3].act_Speed= Can1RxMsg.Data[2]<<8|Can1RxMsg.Data[3];
 			Msg_Wheel[3].act_Angle= Can1RxMsg.Data[0]<<8|Can1RxMsg.Data[1];			
		}
		else if(Can1RxMsg.StdId==CAN1_Wheel_LR)
		{
    	Msg_Wheel[0].act_Speed= Can1RxMsg.Data[2]<<8|Can1RxMsg.Data[3]; 
 			Msg_Wheel[0].act_Angle= Can1RxMsg.Data[0]<<8|Can1RxMsg.Data[1];			
		}
		else if(Can1RxMsg.StdId==CAN1_Wheel_RR)
		{
    	Msg_Wheel[1].act_Speed= Can1RxMsg.Data[2]<<8|Can1RxMsg.Data[3]; 
 			Msg_Wheel[1].act_Angle= Can1RxMsg.Data[0]<<8|Can1RxMsg.Data[1];				
		}
	}
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
}

u8 CAN2_MODE_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTB时钟		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟	

		//初始化GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

		//引脚复用映射配置
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA11复用为CAN1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA12复用为CAN1
		
		//CAN单元设置
		CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
		CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
		CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
		CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
		CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
		CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
		
		CAN_InitStructure.CAN_Mode=CAN2_mode;	 //模式设置 
		CAN_InitStructure.CAN_SJW=CAN2_tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
		CAN_InitStructure.CAN_BS1=CAN2_tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
		CAN_InitStructure.CAN_BS2=CAN2_tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
		CAN_InitStructure.CAN_Prescaler=CAN2_brp;  //分频系数(Fdiv)为brp+1	
		CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN2
		
		//配置过滤器
		CAN_FilterInitStructure.CAN_FilterNumber=15;//;	  //过滤器
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN2_FIFOx;//过滤器0关联到FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
		CAN_FilterInit(&CAN_FilterInitStructure);// 
		
		CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    

		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0x02;     // 主优先级为1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;            // 次优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		return 0;		
}

CanTxMsg CAN2TxMsg;
u8 CAN2_SEND_MSG(u8 *msg,u8 len)
{
	u8 mbox;
  u16 i=0;


  CAN2TxMsg.StdId= CAN2_TX_Filter_ID;	 // 标准标识符为0
  CAN2TxMsg.ExtId=0x00;	 // 设置扩展标示符（29位）
  CAN2TxMsg.IDE=CAN_Id_Standard;		  //
  CAN2TxMsg.RTR=CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位
  CAN2TxMsg.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  CAN2TxMsg.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN2, &CAN2TxMsg);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;	
}

Info_6223 Gambil_Msg[2]={0};
CanRxMsg Can2RxMsg;

void CAN2_RX0_IRQHandler(void)
{
	if( CAN_MessagePending(CAN2,CAN_FIFO0)!=0)
	{
		CAN_Receive(CAN2, CAN_FIFO0, &Can2RxMsg);		
////////////////////////////////////////////////////////////
    if(Can2RxMsg.StdId==CAN2_Gambil_Pitch)
		{
       Gambil_Msg[0].act_Angle=   Can2RxMsg.Data[0]<<8|Can2RxMsg.Data[1];
			 Gambil_Msg[0].act_Current= Can2RxMsg.Data[2]<<8|Can2RxMsg.Data[3];
			 Gambil_Msg[0].set_Current= Can2RxMsg.Data[3]<<8|Can2RxMsg.Data[5];
		}
		else if(Can2RxMsg.StdId==CAN2_Gambil_Yaw)
		{
       Gambil_Msg[1].act_Angle=   Can2RxMsg.Data[0]<<8|Can2RxMsg.Data[1];
			 Gambil_Msg[1].act_Current= Can2RxMsg.Data[2]<<8|Can2RxMsg.Data[3];
			 Gambil_Msg[1].set_Current= Can2RxMsg.Data[3]<<8|Can2RxMsg.Data[5];		
		}
///////////////////////////////////////////////////////////		
	}
	CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
}





