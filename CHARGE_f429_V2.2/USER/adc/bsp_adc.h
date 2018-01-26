#ifndef __ADC_H
#define	__ADC_H


#include "stm32f4xx.h"

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR    (0)
//#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC IO宏定义
#define AN1  F10_ADC3_IN8_AN1 				//GPIO_Mode_AIN
#define AN2  F8_ADC3_IN6_AN2 					//GPIO_Mode_AIN
#define AN3  F6_ADC3_IN4_AN3					//GPIO_Mode_AIN
#define AN4  A5_ADC1_IN5_AN4					//GPIO_Mode_AIN
#define AN5  A3_ADC1_IN3_AN5					//GPIO_Mode_AIN
#define AN6  F4_ADC3_IN14_AN6					//GPIO_Mode_AIN
//输入电压监测
#define AN7  F9_ADC3_IN7_AN7 					//GPIO_Mode_AIN
#define AN8  F7_ADC3_IN5_AN8 					//GPIO_Mode_AIN
#define AN9  F5_ADC3_IN15_AN9					//GPIO_Mode_AIN
#define AN10 F3_ADC3_IN9_AN10 				//GPIO_Mode_AIN
#define AN11 A4_ADC1_IN4_AN11					//GPIO_Mode_AIN
#define AN12 A6_ADC1_IN6_AN12 				//GPIO_Mode_AIN

//输入电流监测
#define F10_ADC3_IN8_AN1_PIN					GPIO_Pin_10 		//GPIO_Mode_AIN
#define F8_ADC3_IN6_AN2_PIN						GPIO_Pin_8 		//GPIO_Mode_AIN
#define F6_ADC3_IN4_AN3_PIN						GPIO_Pin_6 		//GPIO_Mode_AIN
#define A5_ADC1_IN5_AN4_PIN						GPIO_Pin_5 		//GPIO_Mode_AIN
#define A3_ADC1_IN3_AN5_PIN						GPIO_Pin_3		//GPIO_Mode_AIN
#define F4_ADC3_IN14_AN6_PIN					GPIO_Pin_4		//GPIO_Mode_AIN
//输入电压监测
#define F9_ADC3_IN7_AN7_PIN						GPIO_Pin_9 		//GPIO_Mode_AIN
#define F7_ADC3_IN5_AN8_PIN						GPIO_Pin_7 		//GPIO_Mode_AIN
#define F5_ADC3_IN15_AN9_PIN					GPIO_Pin_5 		//GPIO_Mode_AIN
#define F3_ADC3_IN9_AN10_PIN					GPIO_Pin_3 		//GPIO_Mode_AIN
#define A4_ADC1_IN4_AN11_PIN					GPIO_Pin_4 		//GPIO_Mode_AIN
#define A6_ADC1_IN6_AN12_PIN					GPIO_Pin_6 		//GPIO_Mode_AIN

//输入电流监测
#define F10_ADC3_IN8_AN1_PORT					GPIOF     //GPIO_Mode_AIN
#define F8_ADC3_IN6_AN2_PORT				  GPIOF 		//GPIO_Mode_AIN
#define F6_ADC3_IN4_AN3_PORT				  GPIOF 		//GPIO_Mode_AIN
#define A5_ADC1_IN5_AN4_PORT				  GPIOA 		//GPIO_Mode_AIN
#define A3_ADC1_IN3_AN5_PORT				  GPIOA 		//GPIO_Mode_AIN
#define F4_ADC3_IN14_AN6_PORT					GPIOF 	 	//GPIO_Mode_AIN
//输入电压监测
#define F9_ADC3_IN7_AN7_PORT				  GPIOF 		//GPIO_Mode_AIN
#define F7_ADC3_IN5_AN8_PORT				  GPIOF 		//GPIO_Mode_AIN
#define F5_ADC3_IN15_AN9_PORT				  GPIOF 		//GPIO_Mode_AIN
#define F3_ADC3_IN9_AN10_PORT			  	GPIOF 		//GPIO_Mode_AIN
#define A4_ADC1_IN4_AN11_PORT					GPIOA 		//GPIO_Mode_AIN
#define A6_ADC1_IN6_AN12_PORT					GPIOA 		//GPIO_Mode_AIN

//输入电流监测
#define F10_ADC3_IN8_AN1_CLK				  RCC_AHB1Periph_GPIOF     //GPIO_Mode_AIN
#define F8_ADC3_IN6_AN2_CLK				  	RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define F6_ADC3_IN4_AN3_CLK					  RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define A5_ADC1_IN5_AN4_CLK					  RCC_AHB1Periph_GPIOA 		//GPIO_Mode_AIN
#define A3_ADC1_IN3_AN5_CLK					  RCC_AHB1Periph_GPIOA 		//GPIO_Mode_AIN
#define F4_ADC3_IN14_AN6_CLK					RCC_AHB1Periph_GPIOF 	 	//GPIO_Mode_AIN
//输入电压监测
#define F9_ADC3_IN7_AN7_CLK				  	RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define F7_ADC3_IN5_AN8_CLK					  RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define F5_ADC3_IN15_AN9_CLK					RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define F3_ADC3_IN9_AN10_CLK			    RCC_AHB1Periph_GPIOF 		//GPIO_Mode_AIN
#define A4_ADC1_IN4_AN11_CLK				  RCC_AHB1Periph_GPIOA 		//GPIO_Mode_AIN
#define A6_ADC1_IN6_AN12_CLK					RCC_AHB1Periph_GPIOA 		//GPIO_Mode_AIN

//通道控制
//输入电流监测
#define F10_ADC3_IN8_AN1_CHx				  ADC_Channel_8    //GPIO_Mode_AIN
#define F8_ADC3_IN6_AN2_CHx				  	ADC_Channel_6 		//GPIO_Mode_AIN
#define F6_ADC3_IN4_AN3_CHx					  ADC_Channel_4 		//GPIO_Mode_AIN
#define A5_ADC1_IN5_AN4_CHx					  ADC_Channel_5 		//GPIO_Mode_AIN
#define A3_ADC1_IN3_AN5_CHx					  ADC_Channel_3 		//GPIO_Mode_AIN
#define F4_ADC3_IN14_AN6_CHx					ADC_Channel_14 	 	//GPIO_Mode_AIN
//输入电压监测
#define F9_ADC3_IN7_AN7_CHx				  	ADC_Channel_7 		//GPIO_Mode_AIN
#define F7_ADC3_IN5_AN8_CHx					  ADC_Channel_5 		//GPIO_Mode_AIN
#define F5_ADC3_IN15_AN9_CHx					ADC_Channel_15 		//GPIO_Mode_AIN
#define F3_ADC3_IN9_AN10_CHx			    ADC_Channel_9 		//GPIO_Mode_AIN
#define A4_ADC1_IN4_AN11_CHx				  ADC_Channel_4 		//GPIO_Mode_AIN
#define A6_ADC1_IN6_AN12_CHx					ADC_Channel_6 		//GPIO_Mode_AIN

// ADC 宏定义
#define USER_ADC1              ADC1
#define USER_ADC1_CLK          RCC_APB2Periph_ADC1

#define USER_ADC3              ADC3
#define USER_ADC3_CLK          RCC_APB2Periph_ADC3

// ADC DMA 通道宏定义，这里我们使用DMA传输
//#define USER_ADC1_DMA_EN
#define USER_ADC1_DMA_CLK      RCC_AHB1Periph_DMA2
#define USER_ADC1_DMA_CHANNEL  DMA_Channel_0
#define USER_ADC1_DMA_STREAM   DMA2_Stream0   

//#define USER_ADC3_DMA_EN
#define USER_ADC3_DMA_CLK      RCC_AHB1Periph_DMA2
#define USER_ADC3_DMA_CHANNEL  DMA_Channel_2
#define USER_ADC3_DMA_STREAM   DMA2_Stream1   

//变量
extern  u16 *ADC1_Pointer;
extern  u16  ADC1_BufSize;
extern  u16  ADC1_Length ;
extern  u8   ADC1_State;
extern  u8   ADC1_Error;
extern  u16  ADC1_Buffer;

extern  u16 *ADC2_Pointer;
extern  u16  ADC2_BufSize;
extern  u16  ADC2_Length ;
extern  u8   ADC2_State;
extern  u8   ADC2_Error;
extern  u16  ADC2_Buffer;

extern  u16 *ADC3_Pointer;
extern  u16  ADC3_BufSize;
extern  u16  ADC3_Length ;
extern  u8   ADC3_State;
extern  u8   ADC3_Error;
extern  u16  ADC3_Buffer;

extern  u8 ADC1_sampling_times;
extern  u8 ADC3_sampling_times;
#if 0
//子程序
extern void ADC1_Enable(void);
extern void ADC1_Disable(void);
extern void ADC1_Init(void);
extern void ADC1_Set(u8 baud,u8 work_mode);

extern void ADC2_Enable(void);
extern void ADC2_Disable(void);
extern void ADC2_Init(void);
extern void ADC2_Set(u8 baud,u8 work_mode);

extern void ADC3_Enable(void);
extern void ADC3_Disable(void);
extern void ADC3_Init(void);
extern void ADC3_Set(u8 baud,u8 work_mode);
#endif

#define ADC_SAMPLING_TIMES					(3) // max sampling times is 10

#define ADC1_ENABLE_CHANNEL_NUM				(4)
#define ADC3_ENABLE_CHANNEL_NUM				(8)
#define ADC1_3_ENABLE_CHANNEL_NUM			(ADC1_ENABLE_CHANNEL_NUM+ADC3_ENABLE_CHANNEL_NUM)

//#define ADC1_Pointer_Began				(0)
//#define ADC3_Pointer_Began				(100)

extern  u8 ADC1_channel[18];
extern  u8 ADC3_channel[18];

extern  u8 ADC_ANx_piont[18];

void BSP_ADC_Init(void);


#endif /* __ADC_H */

