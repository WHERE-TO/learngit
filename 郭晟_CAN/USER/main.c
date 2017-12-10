#include "stm32f10x.h"


/* Private function prototypes -----------------------------------------------*/

void GPIOInit(void);
void NVICInit(void);
void CANInit(void);
void CANTrans(void);
void CANRecv(void);

int main(void)
{
  /* NVIC Configuration */
	GPIOInit();
  NVICInit();
	CANInit();
  /* CAN transmit at 125Kb/s and receive by polling in loopback mode */
  //CAN_Polling();

  /* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */

  while (1)
  {
		CANTrans();
		CANRecv();
  }
}

void GPIOInit(void)
{
	GPIO_InitTypeDef  gpio;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &gpio);
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
	gpio.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpio);

}

void NVICInit(void)
{
  NVIC_InitTypeDef nvic;

  nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
}


void CANInit(void)
{
  CAN_InitTypeDef        can;
  CAN_FilterInitTypeDef  canfilter;	

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
  CAN_StructInit(&can);

  /* CAN cell init */
  can.CAN_TTCM=DISABLE;
  can.CAN_ABOM=DISABLE;
  can.CAN_AWUM=DISABLE;
  can.CAN_NART=DISABLE;
  can.CAN_RFLM=DISABLE;
  can.CAN_TXFP=DISABLE;
  can.CAN_Mode=CAN_Mode_Normal;
	
	//CAN_Mode_Normal CAN_Mode_LoopBack
  can.CAN_SJW=CAN_SJW_1tq;
  
  /* Baudrate = 1 Mbps */
  can.CAN_BS1=CAN_BS1_9tq;
  can.CAN_BS2=CAN_BS2_8tq;
  can.CAN_Prescaler=2;
  CAN_Init(CAN1, &can);

  /* CAN filter init */

  canfilter.CAN_FilterNumber=1;

  canfilter.CAN_FilterMode=CAN_FilterMode_IdMask;
  canfilter.CAN_FilterScale=CAN_FilterScale_32bit;
  canfilter.CAN_FilterIdHigh=0x0000;
  canfilter.CAN_FilterIdLow=0x0000;
  canfilter.CAN_FilterMaskIdHigh=0x0000;
  canfilter.CAN_FilterMaskIdLow=0x0000;
  canfilter.CAN_FilterFIFOAssignment=CAN_FIFO0;
  canfilter.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&canfilter);
	
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
}

void CANTrans(void)
{

  CanTxMsg TxMessage;

  /* transmit 1 message */
  TxMessage.StdId=0;
  TxMessage.ExtId=0x1234;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.DLC=8;
  TxMessage.Data[0]=1;
	TxMessage.Data[1]=1;
	TxMessage.Data[2]=4;
	TxMessage.Data[3]=5;
	TxMessage.Data[4]=1;
	TxMessage.Data[5]=4;
	TxMessage.Data[6]=1;
	TxMessage.Data[7]=9;
  CAN_Transmit(CAN1, &TxMessage);
	
}
void CANRecv(void)
{
	CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x1234;
  RxMessage.IDE=0;
  RxMessage.DLC=8;
  RxMessage.FMI=0;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
     && (RxMessage.DLC==8) )
  {
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
  }
  else
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
  }	
}

