#include "Remotecontrol.h"

/* ----------------------- Internal Data ----------------------------------- */

/* ----------------------- Function Implements ---------------------------- */
/******************************************************************************
* @fn RC_Init
*
* @brief configure stm32 usart2 port
* - USART Parameters
* - 100Kbps
* - 8-N-1
* - DMA Mode
*
* @return None.
*
* @note This code is fully tested on STM32F405RGT6 Platform, You can port
it
* to the other platform. Using doube buffer to receive data prevent
losing data.
*/
volatile unsigned char sbus_rx_buffer[RC_FRAME_LENGTH]; //double  sbus rx buffer to save data
RC_Ctl_t RC_CtrlData;

void RC_Init(void)
{
	/* -------------- Enable Module Clock Source ----------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	/* -------------- Configure GPIO ---------------------------------------*/
	{
		GPIO_InitTypeDef gpio;
		USART_InitTypeDef usart2;
		gpio.GPIO_Pin = GPIO_Pin_11 ;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOC, &gpio);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 

		
		USART_DeInit(UART4);
		usart2.USART_BaudRate = 100000;
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits = USART_StopBits_1;
		usart2.USART_Parity = USART_Parity_Even;
		usart2.USART_Mode = USART_Mode_Rx;
		usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		
		USART_Init(UART4,&usart2);
		USART_Cmd(UART4,ENABLE);
		USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
	}
	/* -------------- Configure NVIC ---------------------------------------*/
	{
		NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = UART4_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
	}
	/* -------------- Configure DMA -----------------------------------------*/
	{
		DMA_InitTypeDef dma;
		DMA_DeInit(DMA1_Stream2);
		dma.DMA_Channel = DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)&sbus_rx_buffer[0];
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = RC_FRAME_LENGTH;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//		DMA_DoubleBufferModeConfig(DMA1_Stream2,(uint32_t)&sbus_rx_buffer[1][0],DMA_Memory_0); //first used memory configuration
//		DMA_DoubleBufferModeCmd(DMA1_Stream2, ENABLE);
		DMA_Init(DMA1_Stream2,&dma);
		
		NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 0;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		
		DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE); //usart rx idle interrupt enabled
		DMA_Cmd(DMA1_Stream2,ENABLE);
	}
}

void DMA1_Stream2_IRQHandler(void)                	
{
	if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TCIF2)!=RESET)
	{
		if(sbus_rx_buffer == 0)	return;

		RC_CtrlData.ch0 = ((int16_t)sbus_rx_buffer[0] | ((int16_t)sbus_rx_buffer[1] << 8)) & 0x07FF;
		RC_CtrlData.ch1 = (((int16_t)sbus_rx_buffer[1] >> 3) | ((int16_t)sbus_rx_buffer[2] << 5))& 0x07FF;
		RC_CtrlData.ch2 = (((int16_t)sbus_rx_buffer[2] >> 6) | ((int16_t)sbus_rx_buffer[3] << 2) | ((int16_t)sbus_rx_buffer[4] << 10)) & 0x07FF;
//		RC_CtrlData.ch3 = (((int16_t)sbus_rx_buffer[4] >> 1) | ((int16_t)sbus_rx_buffer[5]<<7)) & 0x07FF;
		RC_CtrlData.s1 = ((sbus_rx_buffer[5] >> 4) & 0x000C) >> 2;
//		RC_CtrlData.s2 = ((sbus_rx_buffer[5] >> 4) & 0x0003);
	}
	DMA_ClearITPendingBit(DMA1_Stream2,DMA_IT_TCIF1);
	
} 
