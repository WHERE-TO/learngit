#ifndef _MPU6050_H
#define _MPU6050_H

#include "sys.h"

//////////////////////////MPU6050对应的串口/////////////////////////////////////////////
#define MPU6050_USART    USART3        //串口3     
#define MPU6050_USART_bound  115200    
#define MPU6050_USART_CLK    RCC_APB1Periph_USART3   //时钟
#define MPU6050_USART_CLKCMD   (RCC_APB1PeriphClockCmd)
///////////////////MPU6050对应的串口引脚///////////////////////////////
#define MPU6050_USART_GPIO_APBxCLKCMD  (RCC_AHB1PeriphClockCmd)    //时钟
#define MPU6050_USART_GPIO_CLK   RCC_AHB1Periph_GPIOB

#define MPU6050_USART_GPIO_AFConfig    (GPIO_PinAFConfig)          //复用
#define MPU6050_USART_TX_GPIO_PINSource  GPIO_PinSource10
#define MPU6050_USART_RX_GPIO_PINSource  GPIO_PinSource11
#define MPU6050_USART_AF                    GPIO_AF_USART3

#define MPU6050_USART_TRX_GPIO_PORT GPIOB
#define MPU6050_USART_TX_GPIO_PIN  GPIO_Pin_10
#define MPU6050_USART_RX_GPIO_PIN  GPIO_Pin_11
////////////////////MPU6050 DMA设置//////////////////////////////////////////////
#define BUFF_SIZE   11
//时钟
#define MPU6050_DMA_CLK       RCC_AHB1Periph_DMA1
#define MPU6050_DMA_CLKCMD    (RCC_AHB1PeriphClockCmd)
//  DMA1 通道4 数据流1



//////////////////////////////////////////////////////////////////////////////////
void MPU6050_Init(void);
#endif
