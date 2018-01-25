#include "can.h"

u8 CAN1_MODE_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
		
  	CAN_InitStructure.CAN_Mode=CAN1_mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=CAN1_tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN1_tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN1_tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=CAN1_brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=1;//0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN1_FIFOx;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);// 
		
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0x02;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  
  	return 0;	
}

u8 CAN1_SEND_MSG(u8 *msg,u8 len)
{
	u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId= CAN1_TX_Filter_ID;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x00;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=CAN_Id_Standard;		  //
  TxMessage.RTR=CAN_RTR_Data;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
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

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTBʱ��		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN2ʱ��	

		//��ʼ��GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

		//���Ÿ���ӳ������
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA11����ΪCAN1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA12����ΪCAN1
		
		//CAN��Ԫ����
		CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
		CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
		CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
		CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
		CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
		
		CAN_InitStructure.CAN_Mode=CAN2_mode;	 //ģʽ���� 
		CAN_InitStructure.CAN_SJW=CAN2_tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
		CAN_InitStructure.CAN_BS1=CAN2_tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
		CAN_InitStructure.CAN_BS2=CAN2_tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
		CAN_InitStructure.CAN_Prescaler=CAN2_brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
		CAN_Init(CAN2, &CAN_InitStructure);   // ��ʼ��CAN2
		
		//���ù�����
		CAN_FilterInitStructure.CAN_FilterNumber=15;//;	  //������
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN2_FIFOx;//������0������FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
		CAN_FilterInit(&CAN_FilterInitStructure);// 
		
		CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    

		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0x02;     // �����ȼ�Ϊ1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;            // �����ȼ�Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		return 0;		
}

CanTxMsg CAN2TxMsg;
u8 CAN2_SEND_MSG(u8 *msg,u8 len)
{
	u8 mbox;
  u16 i=0;


  CAN2TxMsg.StdId= CAN2_TX_Filter_ID;	 // ��׼��ʶ��Ϊ0
  CAN2TxMsg.ExtId=0x00;	 // ������չ��ʾ����29λ��
  CAN2TxMsg.IDE=CAN_Id_Standard;		  //
  CAN2TxMsg.RTR=CAN_RTR_Data;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  CAN2TxMsg.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  CAN2TxMsg.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN2, &CAN2TxMsg);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
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





