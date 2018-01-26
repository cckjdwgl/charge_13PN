/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������   
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
  * @brief  ����ADC��GPIO
  * @param  ��
  * @retval ��
  */
static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
		
	// ���� IO
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(F10_ADC3_IN8_AN1_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F10_ADC3_IN8_AN1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F10_ADC3_IN8_AN1_PORT, &GPIO_InitStructure);			
	// ���� IO
	RCC_AHB1PeriphClockCmd(F8_ADC3_IN6_AN2_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F8_ADC3_IN6_AN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F8_ADC3_IN6_AN2_PORT, &GPIO_InitStructure);			
	// ���� IO
	RCC_AHB1PeriphClockCmd(F6_ADC3_IN4_AN3_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F6_ADC3_IN4_AN3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F6_ADC3_IN4_AN3_PORT, &GPIO_InitStructure);			
	// ���� IO
	RCC_AHB1PeriphClockCmd(A5_ADC1_IN5_AN4_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A5_ADC1_IN5_AN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(A5_ADC1_IN5_AN4_PORT, &GPIO_InitStructure);			
	// ���� IO
	RCC_AHB1PeriphClockCmd(A3_ADC1_IN3_AN5_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A3_ADC1_IN3_AN5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(A3_ADC1_IN3_AN5_PORT, &GPIO_InitStructure);			
	// ���� IO
	RCC_AHB1PeriphClockCmd(F4_ADC3_IN14_AN6_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F4_ADC3_IN14_AN6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F4_ADC3_IN14_AN6_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(F9_ADC3_IN7_AN7_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F9_ADC3_IN7_AN7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F9_ADC3_IN7_AN7_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(F7_ADC3_IN5_AN8_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F7_ADC3_IN5_AN8_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F7_ADC3_IN5_AN8_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(F5_ADC3_IN15_AN9_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F5_ADC3_IN15_AN9_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F5_ADC3_IN15_AN9_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(F3_ADC3_IN9_AN10_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = F3_ADC3_IN9_AN10_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(F3_ADC3_IN9_AN10_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(A4_ADC1_IN4_AN11_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A4_ADC1_IN4_AN11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(A4_ADC1_IN4_AN11_PORT, &GPIO_InitStructure);			
	// ���� IO 
	RCC_AHB1PeriphClockCmd(A6_ADC1_IN6_AN12_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = A6_ADC1_IN6_AN12_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(A6_ADC1_IN6_AN12_PORT, &GPIO_InitStructure);			
}

/**
  * @brief  ����ADC��DMA����
  * @param  ��
  * @retval ��
  */
static void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
#if defined USER_ADC1_DMA_EN
	DMA_InitTypeDef DMA_InitStructure;
#endif
	
#if defined USER_ADC1	
  // ����ADCʱ��
	RCC_APB2PeriphClockCmd(USER_ADC1_CLK , ENABLE);
	
#if defined USER_ADC1_DMA_EN
	// ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(USER_ADC1_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_BUFFER;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ1���������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStructure.DMA_BufferSize = 1;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = USER_ADC1_DMA_CHANNEL; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(USER_ADC1_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(USER_ADC1_DMA_STREAM, ENABLE);
#endif

  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // ����ת��	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ʹ������������ⲿ�����������ã�ע�͵�����
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(USER_ADC1, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(USER_ADC1, ADC1_channel[ADC1_channel_index], 1, ADC_SampleTime_3Cycles); 
#if defined USER_ADC1_DMA_EN	
  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(USER_ADC1, ENABLE);
// ʹ��ADC DMA
  ADC_DMACmd(USER_ADC1, ENABLE);
#endif
  // ʹ��ADC
  ADC_Cmd(USER_ADC1, ENABLE);  

/* ���ж�*/
	ADC_ITConfig(USER_ADC1, ADC_IT_EOC, ENABLE);

  //��ʼadcת�����������
  ADC_SoftwareStartConv(USER_ADC1);
#endif

#if defined USER_ADC3
  // ����ADCʱ��
	RCC_APB2PeriphClockCmd(USER_ADC3_CLK , ENABLE);

#if defined USER_ADC3_DMA_EN
	// ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC3ʹ��DMA2��������1��ͨ��2��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(USER_ADC3_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_BUFFER[ADC_BUFFER_SIZE/2];  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ1���������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStructure.DMA_BufferSize = 1;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = USER_ADC3_DMA_CHANNEL; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(USER_ADC3_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(USER_ADC3_DMA_STREAM, ENABLE);
#endif

  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // ����ת��	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ʹ������������ⲿ�����������ã�ע�͵�����
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(USER_ADC3, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(USER_ADC3, ADC3_channel[ADC3_channel_index], 1, ADC_SampleTime_3Cycles); 
#if defined USER_ADC3_DMA_EN	
  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(USER_ADC3, ENABLE);
// ʹ��ADC DMA
  ADC_DMACmd(USER_ADC3, ENABLE);
#endif
  // ʹ��ADC
  ADC_Cmd(USER_ADC3, ENABLE);  
	
	/* ���ж�*/
	ADC_ITConfig(USER_ADC3, ADC_IT_EOC, ENABLE);

  //��ʼadcת�����������
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
  * @brief  ADC1��ʼ��
  * @param  ��
  * @retval ��
  */
void BSP_ADC_Init(void)
{
	
	ADC1_Pointer= &ADC_BUFFER[0];
	ADC3_Pointer= &ADC_BUFFER[ADC1_ENABLE_CHANNEL_NUM*ADC_SAMPLING_TIMES];  //
	
	ADC_GPIO_Config();
	ADC_Mode_Config();
}
/*********************************************END OF FILE**********************/
