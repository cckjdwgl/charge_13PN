/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dwgl_global.h"
//#include "stm32f4xx_it.h"
//#include "./Bsp/usart/bsp_debug_usart.h"
//#include "./Bsp/systick/bsp_SysTick.h"

/** @addtogroup Template_Project
  * @{
  */

unsigned  int time_sys = 0;
unsigned  int time_ms = 0;
unsigned  int send_times = 0;
unsigned  char  data_b[2048]={0};
struct  device_table device;
struct  Addr_info2STR info2STR;
struct  LCDREG  LCDC;
struct  KEYREG fkey;

u16 step;
u16 time_s;
u16 testcmd1_time;
u16 testcmd3_time;
u32 time_s_temp;
u16 global_u16temp;
u16 *global_u16p;

volatile unsigned char touch_flag;
u32 time_uart1;
u32 time_uart3;
u8 str_buffer[4100];

u8 LCD_TxtBuffer[2][2050];		//做显示的内存

u8 Dport_State[6];
u8 checking_port[2];

u32 file_addr;
u32 Rfile_addr;
u8 file_hook;
u8 file_wr;
u8 file_id;						//当前正写文件的ID
u32 NextFileAddr;
u32 check_time;

u8  UART_BUFFER[128];
u8  UART1_TXBUFFER[128];
u8  UART1_RXBUFFER[UART1_RXBUFFER_SIZE];
u16  UART1_RXBUFFE_HEAD =0;		//有效内容的第一个
u16  UART1_RXBUFFE_LAST =0;		//有效内容的最后一个
u8  UART2_TXBUFFER[128];
u8  UART2_RXBUFFER[128];
u8  UART3_TXBUFFER[128];
u8  UART3_RXBUFFER[UART3_RXBUFFER_SIZE];
u16  UART3_RXBUFFE_HEAD;		//有效内容的第一个
u16  UART3_RXBUFFE_LAST;		//有效内容的最后一个
u8  SPI_BUFFER[128];

u16 ADC_BUFFER[ADC_BUFFER_SIZE];

//u8 AINx_ADCch[18]={ADC_Channel_13,ADC_Channel_12,ADC_Channel_0,ADC_Channel_1,ADC_Channel_3,ADC_Channel_2,ADC_Channel_11,ADC_Channel_10,0};
u8 AINx_ADCch[18]={ADC_Channel_13,ADC_Channel_12,ADC_Channel_0,ADC_Channel_1,ADC_Channel_3,ADC_Channel_0,ADC_Channel_2,ADC_Channel_1,0};

u16 ADC_Base0[18];				//ADC静态值
u8 device_num[20];
#ifndef BOOTLOADER_SOURCECODE//ZHZQ_CHANGE
u16 AD_count[64];				//广告计数
u8 charge_speed[2];				//充电速度	
#endif
u8  SF_REG = 7;					//0-7. =7时，采样速度最慢 
u8  GAIN_REG;
u8  KEEP_EN;


/* 波形数据 ---------------------------------------------------------*/
uint16_t Sine12bit[32] = {
	2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
	3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
	156,39,0,39,156,345,600,910,1264,1648,2048
};
/* 坐标数据 ---------------------------------------------------------*/
uint16_t co_x[10] = {
	103,152,201,
	103,152,201,
	103,152,201,
	0
};
/* 坐标数据 ---------------------------------------------------------*/
uint16_t co_y[10] = {
	50,50,50,
	99,99,99,
	148,148,148,
	0
};
/* 九宫数据 ---------------------------------------------------------*/
uint16_t xy_3[3][3] = {
	0,1,2,
	3,4,5,
	6,7,8
};

//要管理中断的优先级分组
void NVIC_Configuration(void)
{
	////智嵌上的配置
	NVIC_InitTypeDef   NVIC_InitStructure;
	//设定向量表所处的位置，必须在分组前设置好，默认的是NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	// Set the Vector Table base location at 0x08000000
	// NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	// 2 bit for pre-emption priority, 2 bits for subpriority 
	//复位后默认为分组0,工程中只设置一次
	//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

	// Enable the USART1 global Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART2 global Interrupt 
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure); 

	// Enable the USART3 global Interrupt 
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	// Enable the SPI1 global Interrupt 
	// NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;	  
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);

	// Enable the ADC1,ADC2,ADC3 global Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the DMA1_Channel1 global Interrupt. ADC1 DMA 中断方式
	// NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;	  
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure); 

	// Enable the TIM3 global Interrupt 
	// NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 	NVIC_Init(&NVIC_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 	NVIC_Init(&NVIC_InitStructure);
}

