/**
 ******************************************************************************
 * @file    bsp_led.c
 * @author  fire
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   led应用函数接口
 ******************************************************************************
 */

#include "dwgl_bsp_gpio.h"

/**
 * @brief  初始化控制LED的IO
 * @param  无
 * @retval 无
 */
void BSP_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG*/

	//-----------输出类----------------
		/*设置引脚模式为输出模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       
    /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    /*设置引脚为上拉模式，默认LED亮*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	/*设置引脚速率为50MHz */
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

	//-----------输入类----------------
	/*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  /*设置引脚为输入模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
  /*设置引脚不上拉也不下拉*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/*选择要控制的GPIOA组引脚*/
//	RCC_AHB1PeriphClockCmd(EN_TPS54336_1_CLK, ENABLE); 
//	GPIO_InitStructure.GPIO_Pin = EN_TPS54336_1_PIN;
//	GPIO_Init(EN_TPS54336_1_PORT, &GPIO_InitStructure);
	/* 写1*/
	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
	/* 写0	*/
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
void GPIO_EN_FAN(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 				//开风扇
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
void GPIO_EN_9V(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 				//开灯箱
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
void GPIO_EN_TPS54336(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF) 	//开充电
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
void GPIO_EN_HV(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF)				//开快充
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
void GPIO_SW_883(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF)				//开883 DP DM
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
void GPIO_NegationBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //dwgl  取反
{
	GPIOx->ODR ^= GPIO_Pin;
}

/*********************************************END OF FILE**********************/
