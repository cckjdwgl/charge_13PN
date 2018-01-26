/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 应用bsp / DMA 模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板   
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./adc/bsp_adc.h"

u16 * ADC1_Pointer = NULL;
u16  ADC1_BufSize;
u16  ADC1_Length ;
u8   ADC1_State;
u8   ADC1_Error;
u16  ADC1_Buffer;

u16  *ADC2_Pointer = NULL;
u16  ADC2_BufSize;
u16  ADC2_Length ;
u8   ADC2_State;
u8   ADC2_Error;
u16  ADC2_Buffer;

u16  *ADC3_Pointer = NULL;
u16  ADC3_BufSize;
u16  ADC3_Length ;
u8   ADC3_State;
u8   ADC3_Error;
u16  ADC3_Buffer;

u8 ADC1_channel[18]={A5_ADC1_IN5_AN4_CHx,A3_ADC1_IN3_AN5_CHx,A4_ADC1_IN4_AN11_CHx,A6_ADC1_IN6_AN12_CHx};
static u8 ADC1_channel_index = 0;

u8 ADC3_channel[18]={F10_ADC3_IN8_AN1_CHx,F8_ADC3_IN6_AN2_CHx,F6_ADC3_IN4_AN3_CHx,F4_ADC3_IN14_AN6_CHx,F9_ADC3_IN7_AN7_CHx,F7_ADC3_IN5_AN8_CHx,F5_ADC3_IN15_AN9_CHx,F3_ADC3_IN9_AN10_CHx};
static u8 ADC3_channel_index = 0;
u8 ADC1_sampling_times =0;
u8 ADC3_sampling_times =0;
u8 ADC_ANx_piont[18] = {ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*1-1,  //AN1
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*2-1, 	//AN2
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*3-1,	//AN3
												ADC_SAMPLING_TIMES*1-1,	//AN4
												ADC_SAMPLING_TIMES*2-1,	//AN5
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*4-1,	//AN6
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*5-1,	//AN7
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*6-1,	//AN8
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*7-1,	//AN9
												ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES+ADC_SAMPLING_TIMES*8-1,	//AN10
												ADC_SAMPLING_TIMES*3-1,	//AN11
												ADC_SAMPLING_TIMES*4-1	//AN12
												};
__IO uint16_t ADC_ConvertedValue;

/**
  * @brief  配置ADC的GPIO
  * @param  无
  * @retval 无
  */
static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
		
	// 配置 IO
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(F10_ADC3_IN8_AN1_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F10_ADC3_IN8_AN1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F10_ADC3_IN8_AN1_PORT, &GPIO_InitStructure);			
	// 配置 IO
	RCC_AHB1PeriphClockCmd(F8_ADC3_IN6_AN2_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F8_ADC3_IN6_AN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F8_ADC3_IN6_AN2_PORT, &GPIO_InitStructure);			
	// 配置 IO
	RCC_AHB1PeriphClockCmd(F6_ADC3_IN4_AN3_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F6_ADC3_IN4_AN3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F6_ADC3_IN4_AN3_PORT, &GPIO_InitStructure);			
	// 配置 IO
	RCC_AHB1PeriphClockCmd(A5_ADC1_IN5_AN4_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A5_ADC1_IN5_AN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(A5_ADC1_IN5_AN4_PORT, &GPIO_InitStructure);			
	// 配置 IO
	RCC_AHB1PeriphClockCmd(A3_ADC1_IN3_AN5_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A3_ADC1_IN3_AN5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(A3_ADC1_IN3_AN5_PORT, &GPIO_InitStructure);			
	// 配置 IO
	RCC_AHB1PeriphClockCmd(F4_ADC3_IN14_AN6_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F4_ADC3_IN14_AN6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F4_ADC3_IN14_AN6_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(F9_ADC3_IN7_AN7_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F9_ADC3_IN7_AN7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F9_ADC3_IN7_AN7_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(F7_ADC3_IN5_AN8_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F7_ADC3_IN5_AN8_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F7_ADC3_IN5_AN8_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(F5_ADC3_IN15_AN9_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F5_ADC3_IN15_AN9_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F5_ADC3_IN15_AN9_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(F3_ADC3_IN9_AN10_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F3_ADC3_IN9_AN10_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(F3_ADC3_IN9_AN10_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(A4_ADC1_IN4_AN11_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A4_ADC1_IN4_AN11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(A4_ADC1_IN4_AN11_PORT, &GPIO_InitStructure);			
	// 配置 IO 
	RCC_AHB1PeriphClockCmd(A6_ADC1_IN6_AN12_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A6_ADC1_IN6_AN12_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(A6_ADC1_IN6_AN12_PORT, &GPIO_InitStructure);			
}

/**
  * @brief  配置ADC，DMA传输
  * @param  无
  * @retval 无
  */
static void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
#if defined USER_ADC1_DMA_EN
	DMA_InitTypeDef DMA_InitStructure;
#endif
	
#if defined USER_ADC1	
  // 开启ADC时钟
	RCC_APB2PeriphClockCmd(USER_ADC1_CLK , ENABLE);
	
#if defined USER_ADC1_DMA_EN
	// ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(USER_ADC1_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_BUFFER;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为1，缓冲区的大小应该等于存储器的大小
	DMA_InitStructure.DMA_BufferSize = 1;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = USER_ADC1_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(USER_ADC1_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(USER_ADC1_DMA_STREAM, ENABLE);
#endif

  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 禁止扫描模式，多通道采集才需要	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // 连续转换	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //使用软件触发，外部触发不用配置，注释掉即可
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(USER_ADC1, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
  ADC_RegularChannelConfig(USER_ADC1, ADC1_channel[ADC1_channel_index], 1, ADC_SampleTime_3Cycles); 
#if defined USER_ADC1_DMA_EN	
  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(USER_ADC1, ENABLE);
// 使能ADC DMA
  ADC_DMACmd(USER_ADC1, ENABLE);
#endif
  // 使能ADC
  ADC_Cmd(USER_ADC1, ENABLE);  

/* 开中断*/
	ADC_ITConfig(USER_ADC1, ADC_IT_EOC, ENABLE);

  //开始adc转换，软件触发
  ADC_SoftwareStartConv(USER_ADC1);
#endif

#if defined USER_ADC3
  // 开启ADC时钟
	RCC_APB2PeriphClockCmd(USER_ADC3_CLK , ENABLE);

#if defined USER_ADC3_DMA_EN
	// ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC3使用DMA2，数据流1，通道2，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(USER_ADC3_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_BUFFER[ADC_BUFFER_SIZE/2];  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为1，缓冲区的大小应该等于存储器的大小
	DMA_InitStructure.DMA_BufferSize = 1;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = USER_ADC3_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(USER_ADC3_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(USER_ADC3_DMA_STREAM, ENABLE);
#endif

  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 禁止扫描模式，多通道采集才需要	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // 连续转换	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //使用软件触发，外部触发不用配置，注释掉即可
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(USER_ADC3, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
  ADC_RegularChannelConfig(USER_ADC3, ADC3_channel[ADC3_channel_index], 1, ADC_SampleTime_3Cycles); 
#if defined USER_ADC3_DMA_EN	
  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(USER_ADC3, ENABLE);
// 使能ADC DMA
  ADC_DMACmd(USER_ADC3, ENABLE);
#endif
  // 使能ADC
  ADC_Cmd(USER_ADC3, ENABLE);  
	
	/* 开中断*/
	ADC_ITConfig(USER_ADC3, ADC_IT_EOC, ENABLE);

  //开始adc转换，软件触发
  ADC_SoftwareStartConv(USER_ADC3);
#endif
}

//***********************************************************************
void ADC_IRQHandler(void)
{
	
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC)!= RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		
		ADC1_Pointer[ADC1_channel_index*ADC_SAMPLING_TIMES+ADC1_sampling_times] = ADC_GetConversionValue(ADC1);
		
		ADC1_sampling_times++;
		if(ADC1_sampling_times >= ADC_SAMPLING_TIMES)
		{
			ADC1_sampling_times = 0;

			ADC1_channel_index++;
			if(ADC1_channel_index >= ADC1_ENABLE_CHANNEL_NUM) ADC1_channel_index = 0;
			
			ADC_RegularChannelConfig(ADC1, ADC1_channel[ADC1_channel_index], 1, ADC_SampleTime_112Cycles);
		}
	}
	
	if(ADC_GetITStatus(ADC3, ADC_IT_EOC)!= RESET)
	{
		ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
		
		ADC3_Pointer[ADC3_channel_index*ADC_SAMPLING_TIMES+ADC3_sampling_times] = ADC_GetConversionValue(ADC3);
		
		ADC3_sampling_times++;
		if(ADC3_sampling_times >= ADC_SAMPLING_TIMES)
		{
			ADC3_sampling_times = 0;

			ADC3_channel_index++;
			if(ADC3_channel_index >= ADC3_ENABLE_CHANNEL_NUM) ADC3_channel_index = 0;
			
			ADC_RegularChannelConfig(ADC3, ADC3_channel[ADC3_channel_index], 1, ADC_SampleTime_112Cycles);
		}
	}
}

/**
  * @brief  ADC1初始化
  * @param  无
  * @retval 无
  */
void BSP_ADC_Init(void)
{
	
	ADC1_Pointer= &ADC_BUFFER[0];
	ADC3_Pointer= &ADC_BUFFER[ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES];  //
	
	ADC_GPIO_Config();
	ADC_Mode_Config();
}
/*********************************************END OF FILE**********************/
