#include "can.h"

u8 CAN_MODE_Init(void)
{
	GPIO_InitTypeDef 		    GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  		  NVIC_InitStructure;
///////////////////////////时钟//////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	         	
///////////////////////////初始化IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO	
////////////////////////////CAN初始化
	CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= CAN_mode;	        //模式设置： mode:0,普通模式;1,回环模式; 
	//设置波特率
	CAN_InitStructure.CAN_SJW=CAN_tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_tbs1; 			//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_tbs2;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=CAN_brp;        //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//初始化CAN1 
///////////////////////////筛选器///////////////////////////////////////////////////////////////////////////////////////
	CAN_FilterInitStructure.CAN_FilterNumber=0;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x00;	//32位ID       //0000 0000 0000 0000   0000 0000 0001 0010 
	CAN_FilterInitStructure.CAN_FilterIdLow=0x00;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK   //0000 0000 0000 0000   0000 0000 1111 1111
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x00;               //0xff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0

	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
//////////////////////////////////

  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
//////////////////////////////////////NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	return 0;
}

u8 CAN_SEND_MSG(u8*msg,u8 len)
{
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=CAN_TX_Filter_ID;			// 
	TxMessage.ExtId=0x00;			// 
	TxMessage.IDE=CAN_Id_Standard; 	// 
	TxMessage.RTR=CAN_RTR_Data;		// 
	TxMessage.DLC=len;				// 
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	
	if(i>=0XFFF)return 1;
	return 0;	
}


CanRxMsg RxMessage;
u8 RESIVE_BUFF[8]={0};

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if( CAN_MessagePending(CAN1,CAN_FIFO0)!=0)
	{
		u8 i=0;
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if(RxMessage.StdId==CAN_RX_Filter_ID)
		{
		 for(i=0;i<CAN_LEN;i++)
		 RESIVE_BUFF[i]=RxMessage.Data[i];
		}				
	}	
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
}
