#include "mpu6050.h"

u8  MPU6050_BUFF[BUFF_SIZE];//

void MPU6050_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
/////////////////// ʱ��ʹ��/////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    //�ӿ�GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);   //����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);	    //DMAʱ��
///////////////////////////////////////////////////////////////////////////////
//����Ϊ����	
	MPU6050_USART_GPIO_AFConfig(MPU6050_USART_TRX_GPIO_PORT,MPU6050_USART_TX_GPIO_PINSource,MPU6050_USART_AF);
	MPU6050_USART_GPIO_AFConfig(MPU6050_USART_TRX_GPIO_PORT,MPU6050_USART_RX_GPIO_PINSource,MPU6050_USART_AF);
////////////////////��ʼ������/////////////////////////////////
	//TX:PB10   RX:PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; // 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 
  GPIO_Init( GPIOB,&GPIO_InitStructure); // 
///////////////////////USART��ʼ��///////////////////////////////////
	
	//USART_OverSampling8Cmd(USART3, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;// 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 
  USART_InitStructure.USART_Parity = USART_Parity_No;// 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;//
  USART_Init(MPU6050_USART,&USART_InitStructure);		
/////////////////////////////DMA ����///////////////////////////////////////////////////////////////////////		
  DMA_DeInit(DMA1_Stream1);
//
	DMA_InitStructure.DMA_Channel= DMA_Channel_4;    // 
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART3->DR)) ;  //	����3���ݼĴ�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr =(u32)MPU6050_BUFF;//	  ���ܻ�������ַ
  DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralToMemory;//     ���赽������
//
  DMA_InitStructure.DMA_BufferSize =11 ;//               ��������ݸ���  11��
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//   �����ַ�ǵ���
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//            ��������ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//  ����8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//           ������8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//     ѭ��  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;// 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;// 
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;// 	
	DMA_Init(DMA1_Stream1,&DMA_InitStructure);
/////////////////////////////////DMA�ж�����////////////////////////////////////////////////////////////////////////////
	NVIC_InitStructure.NVIC_IRQChannel= DMA1_Stream1_IRQn  ;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_Init(&NVIC_InitStructure);
	
//////////////////////ʹ��////////////////////////////////////////////////////////////////////
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);  //ʹ�ܴ�������ж�         
	                                              //�ж����ȼ�����
	USART_Cmd(USART3, ENABLE);
	DMA_Cmd(DMA1_Stream1,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
}
////////////////////////////////////////////////////
double a[3],w[3],Angle[3],T;

unsigned char pp=0x07;
/////////////////��������ж�////////////////////////////////////////
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)!=RESET)  //DMA�������
	{
		//DMA_Cmd(DMA1_Stream1,DISABLE);  //��ʱʧ��
		
		pp=0x03;
//////////////////���������������////////////////////////
		if(MPU6050_BUFF[0]==0x55)
		{
					switch(MPU6050_BUFF[1])
						{
						 case 0x51:
						 a[0] = ( (short)MPU6050_BUFF[3]<<8|MPU6050_BUFF[2]  )/32768.0*16;
						 a[1] = ( (short)MPU6050_BUFF[5]<<8|MPU6050_BUFF[4] )/32768.0*16;
						 a[2] = ( (short) MPU6050_BUFF[7]<<8|MPU6050_BUFF[6])/32768.0*16;
						 T = ( (short)MPU6050_BUFF[9]<<8|MPU6050_BUFF[8] )/340.0+36.25;
						 //printf("a = %4.3f\t%4.3f\t%4.3f\t\r\n",a[0],a[1],a[2]);
						 break;
						 case 0x52:
						 w[0] = ((short) MPU6050_BUFF[3]<<8|MPU6050_BUFF[2])/32768.0*2000;
						 w[1] = ((short)MPU6050_BUFF[5]<<8|MPU6050_BUFF[4])/32768.0*2000;
						 w[2] = ((short)MPU6050_BUFF[7]<<8|MPU6050_BUFF[6])/32768.0*2000;
						 T =  ((short)MPU6050_BUFF[9]<<8|MPU6050_BUFF[8])/340.0+36.25;
						 //printf("w = %4.3f\t%4.3f\t%4.3f\t\r\n",w[0],w[1],w[2]);
						 break;
						 case 0x53:
						 Angle[0] = ((short)MPU6050_BUFF[3]<<8|MPU6050_BUFF[2])/32768.0*180;
						 Angle[1] = ((short)MPU6050_BUFF[5]<<8|MPU6050_BUFF[4])/32768.0*180;
						 Angle[2] = ((short)MPU6050_BUFF[7]<<8|MPU6050_BUFF[6])/32768.0*180;
						 T =        ((short)MPU6050_BUFF[9]<<8|MPU6050_BUFF[8])/340.0+36.25;
						 //printf("Angle = %4.2f\t%4.2f\t%4.2f\tT=%4.2f\r\n",Angle[0],Angle[1],Angle[2],T);
						 break;
						}
		}
//////////////////////////////////////////////////////////
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);     //����жϱ�־
	//	DMA_Cmd(DMA1_Stream1,ENABLE);
	}
}
