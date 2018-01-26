//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#include "stm32f10x.h"

  u8  *DMA1_Channel1_Pointer;
  u8   DMA1_Channel1_State;
  u8   DMA1_Channel1_Error;
  u32  DMA1_Channel1_Buffer;

  u8  *DMA2_Channel1_Pointer;
  u8   DMA2_Channel1_State;
  u8   DMA2_Channel1_Error;
  u32  DMA2_Channel1_Buffer;

//*******************************************
void DMA_Configuration(void)
{
//	DMA1_Channel1_Init();
	DMA1_Channel2_Init(SPI_BUFFER,16);
//	DMA2_Channel1_Init();
}
//*******************************************
void DMA1_Channel1_Init(void)     //
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			//ADC��ַ
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_BUFFER;	//�ڴ��ַ
	
// 		/*���򣺴��ڴ浽����*/		
// 		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 320;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  				//�ڴ��ַ�̶�
// 		/*�ڴ��ַ����*/
 		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
//	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
}
//*******************************************
void DMA1_Channel2_Init(unsigned char *buffer,unsigned int len)     //
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel2);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Address;	 			//SPIDR��ַ
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buffer;	//�ڴ��ַ
	
// 		/*���򣺴��ڴ浽����*/		
// 		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  				//�ڴ��ַ�̶�
// 		/*�ڴ��ַ����*/
 		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���ֵ�����������
	//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	  //���ֵ��������ڴ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;										//��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
//   /* Enable SPI1 DMA RX request */
//   SPI1->CR2 |= 1<<0;                                                                 //���ջ�����DMAʹ��
	
	/* Enable DMA channel2 */
	DMA_Cmd(DMA1_Channel2, ENABLE);
	
//	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
}
//*******************************************
void DMA2_Channel1_Init(void)
{
	
}
//*******************************************
void DMA1_Channel1_IRQHandler(void)
{
//DMA1_Channel1_IRQn
	DMA1_Channel1_State = 1;
	ADC1_State |= 1;
	DMA_ClearFlag(DMA1_FLAG_GL1);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, DISABLE);
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
//	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	
	GPIOC->ODR ^= GPIO_Pin_3; 
}
//*******************************************
void DMA1_Channel2_IRQHandler(void)
{
//DMA1_Channel1_IRQn
	DMA1_Channel1_State = 1;
	DMA_ClearFlag(DMA1_FLAG_GL1);   //������б�־ ��DMA1_Channel2��ص�GIF��TEIF��HTIF��CIF
	
    /* Enable SPI1 DMA RX request */
    SPI1->CR2 &= ~((u32)1<<0);                    //�رս��ջ�����DMAʹ��
	
//	GPIOC->ODR ^= GPIO_Pin_3; 
}
/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
* Attention             : 
*******************************************************************************/
// void SPI1_DMA_Configuration( void )
// {
//     DMA_InitTypeDef DMA_InitStructure;
//     
//     /* DMA1 Channel2 (triggered by SPI1 Rx event) Config */
//   DMA_DeInit(DMA1_Channel2);  
//   DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Address;                          //���� SPI1 ��������(0x4001300C) ��ַ(Ŀ�ĵ�ַ)
//   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_RX_Buff;                    //���� SRAM �洢��ַ(Ŀ�ĵ�ַ)
//   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                //���䷽�� ����-�ڴ�
//   DMA_InitStructure.DMA_BufferSize = SPI1_ReciveBufferSize;                         //���� SPI1 ���ͳ���
//   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//   DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//   DMA_Init(DMA1_Channel2, &DMA_InitStructure);
//   
//   DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
//   /* Enable SPI1 DMA RX request */
//   SPI1->CR2 |= 1<<0;                                                                 //���ջ�����DMAʹ��
//   DMA_Cmd(DMA1_Channel2, ENABLE);
//     
//     
//     /* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
//   DMA_DeInit(DMA1_Channel3);  
//   DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Addr;                          //����  ��������(0x4001300C) ��ַ(Դ��ַ)
//   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_TX_Buff;                    //���� SRAM �洢��ַ(Դ��ַ)
//   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //���䷽�� �ڴ�-����
//   DMA_InitStructure.DMA_BufferSize = SPI1_SendBufferSize;                           //���� SPI1 ���ճ���
//   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //�����ַ����(����)
//   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //�ڴ��ַ����(�仯)
//   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //���贫����(�ֽ�)
//   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //�ڴ洫����(�ֽ�)
//   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
//   DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //�жϷ�ʽ-��(����)
//   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //�ڴ浽�ڴ淽ʽ��ֹ
//   DMA_Init(DMA1_Channel3, &DMA_InitStructure);
//   
//   DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);                                   //���� DMA1_Channel3 ��������ж�
//   DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);                                   //���� DMA1_Channel3 ��������ж�
//   /* Enable SPI1 DMA TX request */
//   SPI1->CR2 |= 1<<1;                                                                //���ͻ�����DMAʹ��
//   DMA_Cmd(DMA1_Channel3, DISABLE);                                                  //���� DMA ͨ�� DMA1_Channel3
// }

/*******************************************************************************
* Function Name  : SPI1_Send
* Description    : SPI1��DMA��ʽ����
* Input          : SPI1_TX_Buff[SPI1_SendBufferSize]
* Output         : None
* Return         : None
* Attention             : �ر�DMAͨ��3֮ǰ����ȴ�TXEΪ1���ȴ�æ��־Ϊ0
*******************************************************************************/
// void SPI1_Send( u8 *buff, u32 len )
// {
//     DMA1_Channel3->CPAR = SPI1_DR_Addr; //�����ַ
//     DMA1_Channel3->CMAR = (u32) buff; //mem��ַ
//     DMA1_Channel3->CNDTR = len ; //���䳤��
//     DMA1_Channel3->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
//             (2 << 12) | // ͨ�����ȼ���
//             (0 << 11) | // �洢�����ݿ��8bit
//             (0 << 10) | // �洢�����ݿ��8bit
//             (0 <<  9) | // �������ݿ��8bit
//             (0 <<  8) | // �������ݿ��8bit
//             (1 <<  7) | // �洢����ַ����ģʽ
//             (0 <<  6) | // �����ַ����ģʽ(����)
//             (0 <<  5) | // ��ѭ��ģʽ
//             (1 <<  4) | // �Ӵ洢����
//             (1 <<  3) | // ����������ж�
//             (0 <<  2) | // ����봫���ж�
//             (1 <<  1) | // ����������ж�
//             (1);        // ͨ������
// }

/*******************************************************************************
* Function Name  : SPI1_Recive
* Description    : SPI1��DMA��ʽ����
* Input          : None
* Output         : SPI1_RX_Buff[SPI1_ReciveBufferSize]
* Return         : None
* Attention             : ����Ҫ�ȹر�ͨ��2,Ȼ��������ͨ��2�Ĳ���
*******************************************************************************/
void SPI1_DMARecive( u8 *buff, u32 len )
{
    DMA1_Channel2->CCR &= ~(1<<0);          //�ر�DMAͨ��2
    
    /* Enable SPI1 DMA RX request */
    SPI1->CR2 |= 1<<0;                    //���ջ�����DMAʹ��
	
    DMA1_Channel2->CPAR = SPI1_DR_Address; //�����ַ
    DMA1_Channel2->CMAR = (uint32_t)buff; //mem��ַ
    DMA1_Channel2->CNDTR = len ; //���䳤��
    DMA1_Channel2->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
            (2 << 12) | // ͨ�����ȼ���
            (0 << 11) | // �洢�����ݿ��8bit
            (0 << 10) | // �洢�����ݿ��8bit
            (0 <<  9) | // �������ݿ��8bit
            (0 <<  8) | // �������ݿ��8bit
            (1 <<  7) | // �洢����ַ����ģʽ
            (0 <<  6) | // �����ַ����ģʽ(����)
            (0 <<  5) | // ��ѭ��ģʽ
            (0 <<  4) | // ���䷽�� ����-�ڴ�
            (0 <<  3) | // ����������ж�
            (0 <<  2) | // ����봫���ж�
            (1 <<  1) | // ����������ж�
            (1);        // ͨ������
}
