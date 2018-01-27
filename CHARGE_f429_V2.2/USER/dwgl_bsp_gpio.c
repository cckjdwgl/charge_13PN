/**
 ******************************************************************************
 * @file    bsp_led.c
 * @author  fire
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   ledӦ�ú����ӿ�
 ******************************************************************************
 */

#include "dwgl_bsp_gpio.h"

/**
 * @brief  ��ʼ������LED��IO
 * @param  ��
 * @retval ��
 */
void BSP_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*ʹ��SWD ����JTAG*/

	//-----------�����----------------
		/*��������ģʽΪ���ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       
    /*�������ŵ��������Ϊ�������*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    /*��������Ϊ����ģʽ��Ĭ��LED��*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//SW883_HUB IO	
	RCC_AHB1PeriphClockCmd(L_SW883_HUB_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = L_SW883_HUB_PIN;
	GPIO_Init(L_SW883_HUB_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(R_SW883_HUB_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = R_SW883_HUB_PIN;
	GPIO_Init(R_SW883_HUB_PORT, &GPIO_InitStructure);	
//EN_HV IO	
	RCC_AHB1PeriphClockCmd(EN_HV1_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV1_PIN;
	GPIO_Init(EN_HV1_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_HV2_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV2_PIN;
	GPIO_Init(EN_HV2_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_HV3_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV3_PIN;
	GPIO_Init(EN_HV3_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_HV4_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV4_PIN;
	GPIO_Init(EN_HV4_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_HV5_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV5_PIN;
	GPIO_Init(EN_HV5_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_HV6_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_HV6_PIN;
	GPIO_Init(EN_HV6_PORT, &GPIO_InitStructure);	
//EN_TPS54336 IO
	RCC_AHB1PeriphClockCmd(EN_TPS54336_1_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_1_PIN;
	GPIO_Init(EN_TPS54336_1_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_TPS54336_2_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_2_PIN;
	GPIO_Init(EN_TPS54336_2_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_TPS54336_3_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_3_PIN;
	GPIO_Init(EN_TPS54336_3_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_TPS54336_4_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_4_PIN;
	GPIO_Init(EN_TPS54336_4_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_TPS54336_5_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_5_PIN;
	GPIO_Init(EN_TPS54336_5_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(EN_TPS54336_6_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_6_PIN;
	GPIO_Init(EN_TPS54336_6_PORT, &GPIO_InitStructure);	
//
	RCC_AHB1PeriphClockCmd(F11_EN_FAN_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = F11_EN_FAN_PIN;
	GPIO_Init(F11_EN_FAN_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(B1_EN1_9V_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = B1_EN1_9V_PIN;
	GPIO_Init(B1_EN1_9V_PORT, &GPIO_InitStructure);	
	
	RCC_AHB1PeriphClockCmd(B0_EN2_9V_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = B0_EN2_9V_PIN;
	GPIO_Init(B0_EN2_9V_PORT, &GPIO_InitStructure);	

	//-----------������----------------
	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
  /*�������Ų�����Ҳ������*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/*ѡ��Ҫ���Ƶ�GPIOA������*/
//	RCC_AHB1PeriphClockCmd(EN_TPS54336_1_CLK, ENABLE); 
//	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_1_PIN;
//	GPIO_Init(EN_TPS54336_1_PORT, &GPIO_InitStructure);
	/* д1*/
	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
	/* д0	*/
	//GPIO_ResetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_13);
	GPIO_SW_883(L_SW883_HUB_PORT, L_SW883_HUB_PIN,ON);	
	GPIO_SW_883(R_SW883_HUB_PORT, R_SW883_HUB_PIN,ON);	

	GPIO_EN_HV(EN_HV1_PORT, EN_HV1_PIN,ON);
	GPIO_EN_HV(EN_HV2_PORT, EN_HV2_PIN,ON);
	GPIO_EN_HV(EN_HV3_PORT, EN_HV3_PIN,ON);
	GPIO_EN_HV(EN_HV4_PORT, EN_HV4_PIN,ON);
	GPIO_EN_HV(EN_HV5_PORT, EN_HV5_PIN,ON);
	GPIO_EN_HV(EN_HV6_PORT, EN_HV6_PIN,ON);

	GPIO_EN_TPS54336(EN_TPS54336_1_PORT, EN_TPS54336_1_PIN,ON);
	GPIO_EN_TPS54336(EN_TPS54336_2_PORT, EN_TPS54336_2_PIN,ON);
	GPIO_EN_TPS54336(EN_TPS54336_3_PORT, EN_TPS54336_3_PIN,ON);
	GPIO_EN_TPS54336(EN_TPS54336_4_PORT, EN_TPS54336_4_PIN,ON);
	GPIO_EN_TPS54336(EN_TPS54336_5_PORT, EN_TPS54336_5_PIN,ON);
	GPIO_EN_TPS54336(EN_TPS54336_6_PORT, EN_TPS54336_6_PIN,ON);
	
	GPIO_EN_FAN(F11_EN_FAN_PORT, F11_EN_FAN_PIN,ON);
	GPIO_EN_9V(B1_EN1_9V_PORT, B1_EN1_9V_PIN,ON);
	GPIO_EN_9V(B0_EN2_9V_PORT, B0_EN2_9V_PIN,ON);	
	
}
//---------------------------------------------------------------------------------------------
void GPIO_EN_FAN(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 				//������
{
	if(ON_OFF == ON)
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
}
void GPIO_EN_9V(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 				//������
{
	if(ON_OFF == ON)
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
}
void GPIO_EN_TPS54336(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 	//�����
{
	if(ON_OFF == ON)
	{
//		GPIO_SetBits(GPIOx, GPIO_Pin);
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
//		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
}
void GPIO_EN_HV(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF)				//�����
{
	if(ON_OFF == ON)
	{
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
}
void GPIO_SW_883(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF)				//��883 DP DM
{
	if(ON_OFF == ON)
	{
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
	else
	{
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
}
//----------------------------------------------------------------------------
void GPIO_NegationBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //dwgl  ȡ��
{
	GPIOx->ODR ^= GPIO_Pin;
}

/*********************************************END OF FILE**********************/
