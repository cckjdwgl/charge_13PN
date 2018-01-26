//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#include "stm32f10x.h"
/***********************************************************************
  �ļ����ƣ�SCI.C
  ��    �ܣ���ɶ�usart1��usart2�Ĳ���
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺��������ͨ���ж������ض����ַ���ȷ��һ֡�����Ƿ�����ġ�
 ***********************************************************************/
/*
/// ����USART1�����ж�
void NVIC_Configuration(void)
{
NVIC_InitTypeDef NVIC_InitStructure; 
// Configure the NVIC Preemption Priority Bits  
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

// Enable the USARTy Interrupt 
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}
 */
u8  *UART1_Receive_Pointer;
u16  UART1_Receive_BufSize=100;
u16  UART1_Receive_Length=0;
u8  *UART1_Trans_Pointer;
u16  UART1_Trans_BufSize;
u16  UART1_Trans_Length =0;
u8   UART1_TX_State;
u8   UART1_RX_State;
u8   UART1_Error;

u8  *UART2_Receive_Pointer;
u16  UART2_Receive_BufSize;
u16  UART2_Receive_Length;
u8  *UART2_Trans_Pointer;
u16  UART2_Trans_BufSize;
u16  UART2_Trans_Length =0;
u8   UART2_TX_State;
u8   UART2_RX_State;
u8   UART2_Error;

u8  *UART3_Receive_Pointer;
u16  UART3_Receive_BufSize = 100;
u16  UART3_Receive_Length;
u8  *UART3_Trans_Pointer;
u16  UART3_Trans_BufSize =100;
u16  UART3_Trans_Length =0;
u8   UART3_TX_State;
u8   UART3_RX_State;
u8   UART3_Error;

#if 0
/***********************************************************************
  �������ƣ�void RS485_Delay(uint32_t nCount)
  ��    �ܣ�RS485�շ���ʱ����
  ���������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺
 ***********************************************************************/
static void RS485_Delay(uint32_t nCount)
{
	while(nCount > 0)
	{
		nCount --;
	}
}
#endif
//*******************************************************************
static void UART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/********************����ΪUSART1����**************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin = A9_USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A10_USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;

	//USART_InitStructure.USART_WordLength = USART_WordLength_9b;//9λ����
	//USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	//USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
	USART_Init(USART1, &USART_InitStructure);

	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	/***********************************************************************************
	  void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
	  ʹ�ܻ���ʧ��USART����
	  USARTx��x������1��2����3����ѡ��USART����
NewState: ����USARTx����״̬
�����������ȡ��ENABLE����DISABLE
	 ***********************************************************************************/
	USART_Cmd(USART1, ENABLE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ
}
//*******************************************************************
void UART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	/********************����ΪUSART2����**************************/
	//  USART2_TX -> PA2 , USART2_RX ->	PA3  ;Ĭ������	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//��ӳ�����ô˳�����
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //��ӳ�书��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE); 
	USART_ClearITPendingBit(USART2, USART_IT_TC);//����ж�TCλ
}

//*******************************************************************
static void UART3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	/********************����ΪUSART3����**************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = B10_RJ45_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = B11_RJ45_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE); 
	USART_ClearITPendingBit(USART3, USART_IT_TC);//����ж�TCλ
}


/***********************************************************************
  �������ƣ�void USART1_IRQHandler(void) 
  ��    �ܣ����SCI�����ݵĽ��գ�������ʶ
  ���������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
 ***********************************************************************/
// void USART1_IRQHandler(void)  
// {
// 	time_uart1 = time_sys;
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
// 	{	
// 		
// 		if(UART1_Receive_Length<UART1_Receive_BufSize)
// 		{
// 		UART1_Receive_Pointer[UART1_Receive_Length] = USART1->DR;//
// 		UART1_Receive_Length++;
// 		}
// 		else  //�������ջ�������С
// 		{
// 		UART1_Error = USART1->DR;
// 		UART1_Error = 1;
// 		UART1_Receive_Length=0;
// 		}
// /********������ַ���Ϊһ֡���ݵĽ�����ʶ************/
// 		if(UART1_Receive_Length == 1)	//ֻ�н��յ�1���������ϲ����ж�
// 		{
// 			if(UART1_Receive_Pointer[UART1_Receive_Length-1] == frame_headerC) 	//֡��ʼ��־   
// 			{
// 				UART1_RX_State |= 1;
// 			}
// 			else
// 			{
// 				UART1_Receive_Length = 0;
// 				UART1_RX_State = 0;
// 			}
// 		}
// 		if(UART1_Receive_Length >= 2)	//ֻ�н��յ�2���������ϲ����ж�
// 		{
// 			if(UART1_Receive_Pointer[1] == UART1_Receive_Length) 	//֡��ʼ��־   
// 			{
// 				UART1_RX_State |= 2;
// 			}
// 		}	
// 		
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// 	}
// 	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
// 	{
//         USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt */
//   }	
// }

void USART1_IRQHandler(void)
{
	time_uart1 = time_sys;
	led_power_ctrl(LED_INDEX, LED_TURN_ON);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		UART1_Receive_Pointer[UART1_RXBUFFE_LAST] = USART1->DR;
		UART1_RXBUFFE_LAST++;
		UART1_RXBUFFE_LAST &= 0x1ff;//����ֽ�

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt */
	}
}


/***********************************************************************
  �������ƣ�void USART2_IRQHandler(void) 
  ��    �ܣ����SCI�����ݵĽ��գ�������ʶ
  ���������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺RS232�õ���USART2
 ***********************************************************************/
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�������
	{	
		if(UART2_Receive_Length<UART2_Receive_BufSize)
		{
			UART2_Receive_Pointer[UART2_Receive_Length] = USART2->DR;//
			UART2_Receive_Length++;
		}
		else
		{
			UART2_Error = 1;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
	{
		USART_ClearITPendingBit(USART2, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}
}
/***********************************************************************
  �������ƣ�void USART3_IRQHandler(void) 
  ��    �ܣ����SCI�����ݵĽ��գ�������ʶ
  ���������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺
 ***********************************************************************/
// void USART3_IRQHandler(void)  
// {
// 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�������
// 	{	
// 		if(UART3_Receive_Length<UART3_Receive_BufSize)
// 		{
// 		UART3_Receive_Pointer[UART3_Receive_Length] = USART3->DR;//
// 		UART3_Receive_Length++;
// 		}
// 		else
// 		{
// 		UART3_Error = 1;
// 		}
// 		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
// 	}
// 	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) 
// 	{
//         USART_ClearITPendingBit(USART3, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
//     }
// }

void USART3_IRQHandler(void)
{
	time_uart3 = time_sys;
	//led_power_ctrl(LED_INDEX, LED_TURN_ON);
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		UART3_Receive_Pointer[UART3_RXBUFFE_LAST] = USART3->DR;
		UART3_RXBUFFE_LAST++;
		UART3_RXBUFFE_LAST &= UART3_RX_MAX;//����ֽ�

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}
}


/***********************************************************************
  �������ƣUUART1_Send_Data(u8 * p,u16 len)
  ���������p:�����͵�����ָ�룻len�����͵����ݳ��ȣ��ַ�������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺
 ***********************************************************************/
void UART1_Send_Data(u8 * p,u16 len)
{
	unsigned int i = 0;

	while((time_sys-time_uart1)<2);//�ȴ����߿���

	GPIO_SetBits(EN_485_PORT, EN_485_PIN);
	i = USART1->SR;//�ȶ�״̬�Ĵ������ٷ��͡�
	i = 255;
	while(i--);

	for(i=0;i<len;i++)
	{
		USART1->DR = p[i];
		while((USART1->SR&0X40)==0);
	}

	GPIO_ResetBits(EN_485_PORT, EN_485_PIN);
}

/***********************************************************************
  �������ƣUUART2_Send_Data(u8 * p,u16 len)
  ���������p:�����͵�����ָ�룻len�����͵����ݳ��ȣ��ַ�������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺
 ***********************************************************************/
void UART2_Send_Data(u8 * p,u16 len)
{
	unsigned int i = 0;

	for(i = 0;i < len;i ++)
	{			
		USART2->DR = p[i];
		while((USART2->SR&0X40)==0);	
	}	
}

/***********************************************************************
  �������ƣUUART3_Send_Data(u8 * p,u16 len)
  ���������p:�����͵�����ָ�룻len�����͵����ݳ��ȣ��ַ�������
  ���������
  ��дʱ�䣺2012.11.22
  �� д �ˣ�
  ע    �⣺
 ***********************************************************************/
void UART3_Send_Data(u8 * p,u16 len)
{
	unsigned int i = 0;
	#ifndef BOOTLOADER_SOURCECODE//ZHZQ_CHANGE
	i = USART3->SR;//�ȶ�״̬�Ĵ������ٷ��͡�
	#endif
	for(i=0;i<len;i++)
	{
		USART3->DR = p[i];
		while((USART3->SR&0X40)==0);
	}
}
//*******************************************************************
void UART_Configuration(void)
{
	UART1_Receive_Pointer = UART1_RXBUFFER;
	UART2_Receive_Pointer = UART2_RXBUFFER;
	UART3_Receive_Pointer = UART3_RXBUFFER;
	UART1_Init();
	//UART2_Init();
	UART3_Init();
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
}

