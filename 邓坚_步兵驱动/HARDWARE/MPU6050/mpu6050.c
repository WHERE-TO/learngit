#include "mpu6050.h"

u8  MPU6050_BUFF[BUFF_SIZE];//

void MPU6050_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
/////////////////// 时钟使能/////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    //接口GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);   //串口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);	    //DMA时钟
///////////////////////////////////////////////////////////////////////////////
//设置为复用	
	MPU6050_USART_GPIO_AFConfig(MPU6050_USART_TRX_GPIO_PORT,MPU6050_USART_TX_GPIO_PINSource,MPU6050_USART_AF);
	MPU6050_USART_GPIO_AFConfig(MPU6050_USART_TRX_GPIO_PORT,MPU6050_USART_RX_GPIO_PINSource,MPU6050_USART_AF);
////////////////////初始化引脚/////////////////////////////////
	//TX:PB10   RX:PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; // 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 
  GPIO_Init( GPIOB,&GPIO_InitStructure); // 
///////////////////////USART初始化///////////////////////////////////
	
	//USART_OverSampling8Cmd(USART3, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;// 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 
  USART_InitStructure.USART_Parity = USART_Parity_No;// 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;//
  USART_Init(MPU6050_USART,&USART_InitStructure);		
/////////////////////////////DMA 设置///////////////////////////////////////////////////////////////////////		
  DMA_DeInit(DMA1_Stream1);
//
	DMA_InitStructure.DMA_Channel= DMA_Channel_4;    // 
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART3->DR)) ;  //	串口3数据寄存器地址
  DMA_InitStructure.DMA_Memory0BaseAddr =(u32)MPU6050_BUFF;//	  接受缓存区地址
  DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralToMemory;//     外设到储存器
//
  DMA_InitStructure.DMA_BufferSize =11 ;//               传输的数据个数  11个
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//   外设地址非递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//            缓存区地址递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//  外设8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//           缓存区8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//     循环  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;// 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;// 
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;// 	
	DMA_Init(DMA1_Stream1,&DMA_InitStructure);
/////////////////////////////////DMA中断设置////////////////////////////////////////////////////////////////////////////
	NVIC_InitStructure.NVIC_IRQChannel= DMA1_Stream1_IRQn  ;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_Init(&NVIC_InitStructure);
	
//////////////////////使能////////////////////////////////////////////////////////////////////
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);  //使能传输完成中断         
	                                              //中断优先级配置
	USART_Cmd(USART3, ENABLE);
	DMA_Cmd(DMA1_Stream1,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
}
////////////////////////////////////////////////////
double a[3],w[3],Angle[3],T;

unsigned char pp=0x07;
/////////////////传输完成中断////////////////////////////////////////
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)!=RESET)  //DMA传输完成
	{
		//DMA_Cmd(DMA1_Stream1,DISABLE);  //暂时失能
		
		pp=0x03;
//////////////////处理接受来的数据////////////////////////
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
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);     //清除中断标志
	//	DMA_Cmd(DMA1_Stream1,ENABLE);
	}
}
