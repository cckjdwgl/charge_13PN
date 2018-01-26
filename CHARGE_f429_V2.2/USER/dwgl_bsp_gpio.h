#ifndef __DWGL_GPIO_H
#define	__DWGL_GPIO_H

//#include "stm32f10x.h"
#include "stm32f4xx.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1



//BEGIN----------------------------------------
//充电EN_TPS54336 ON/OFF
#define EN_TPS54336_1_PIN		 GPIO_Pin_6 			//GPIO_Mode_Out_PP
#define EN_TPS54336_2_PIN		 GPIO_Pin_15 		//GPIO_Mode_Out_PP
#define EN_TPS54336_3_PIN		 GPIO_Pin_11 		//GPIO_Mode_Out_PP
#define EN_TPS54336_4_PIN		 GPIO_Pin_4 		//GPIO_Mode_Out_PP
#define EN_TPS54336_5_PIN		 GPIO_Pin_3 			//GPIO_Mode_Out_PP
#define EN_TPS54336_6_PIN		 GPIO_Pin_9 		//GPIO_Mode_Out_PP

#define EN_TPS54336_1_PORT		GPIOB			 			//GPIO_Mode_Out_PP
#define EN_TPS54336_2_PORT		GPIOG			 			//GPIO_Mode_Out_PP
#define EN_TPS54336_3_PORT		GPIOG			 			//GPIO_Mode_Out_PP
#define EN_TPS54336_4_PORT		GPIOG			 			//GPIO_Mode_Out_PP
#define EN_TPS54336_5_PORT		GPIOD			 			//GPIO_Mode_Out_PP
#define EN_TPS54336_6_PORT		GPIOG			 			//GPIO_Mode_Out_PP

#define EN_TPS54336_1_CLK			RCC_AHB1Periph_GPIOB	//GPIO_Mode_Out_PP
#define EN_TPS54336_2_CLK			RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
#define EN_TPS54336_3_CLK			RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
#define EN_TPS54336_4_CLK			RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
#define EN_TPS54336_5_CLK			RCC_AHB1Periph_GPIOD	//GPIO_Mode_Out_PP
#define EN_TPS54336_6_CLK			RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
//END----------------------------------------

//BEGIN----------------------------------------
//快充切换
#define EN_HV1_PIN				  GPIO_Pin_7 			//GPIO_Mode_Out_PP
#define EN_HV2_PIN				  GPIO_Pin_5 			//GPIO_Mode_Out_PP
#define EN_HV3_PIN				  GPIO_Pin_12 		//GPIO_Mode_Out_PP
#define EN_HV4_PIN				  GPIO_Pin_3 			//GPIO_Mode_Out_PP
#define EN_HV5_PIN				  GPIO_Pin_6 			//GPIO_Mode_Out_PP
#define EN_HV6_PIN				  GPIO_Pin_10			//GPIO_Mode_Out_PP

#define EN_HV1_PORT				  GPIOB			 			//GPIO_Mode_Out_PP
#define EN_HV2_PORT				  GPIOB			 			//GPIO_Mode_Out_PP
#define EN_HV3_PORT				  GPIOG			 			//GPIO_Mode_Out_PP
#define EN_HV4_PORT				  GPIOG			 			//GPIO_Mode_Out_PP
#define EN_HV5_PORT				  GPIOD			 			//GPIO_Mode_Out_PP
#define EN_HV6_PORT				  GPIOG			 			//GPIO_Mode_Out_PP

#define EN_HV1_CLK				  RCC_AHB1Periph_GPIOB	//GPIO_Mode_Out_PP
#define EN_HV2_CLK				  RCC_AHB1Periph_GPIOB	//GPIO_Mode_Out_PP
#define EN_HV3_CLK				  RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
#define EN_HV4_CLK				  RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
#define EN_HV5_CLK				  RCC_AHB1Periph_GPIOD	//GPIO_Mode_Out_PP
#define EN_HV6_CLK				  RCC_AHB1Periph_GPIOG	//GPIO_Mode_Out_PP
//END----------------------------------------

//BEGIN----------------------------------------
//USB切换,883与HUB之间的选择
#define L_SW883_HUB_PIN				GPIO_Pin_5							//
#define L_SW883_HUB_PORT			GPIOG										//
#define L_SW883_HUB_CLK				RCC_AHB1Periph_GPIOG		//
#define R_SW883_HUB_PIN				GPIO_Pin_6							//
#define R_SW883_HUB_PORT			GPIOG										//
#define R_SW883_HUB_CLK				RCC_AHB1Periph_GPIOG		//
//END----------------------------------------

//BEGIN----------------------------------------
//风扇控制
#define F11_EN_FAN_PIN				GPIO_Pin_11							//
#define F11_EN_FAN_PORT				GPIOF										//
#define F11_EN_FAN_CLK				RCC_AHB1Periph_GPIOF		//
//灯箱电源控制
#define B1_EN1_9V_PIN					GPIO_Pin_1 		//
#define B1_EN1_9V_PORT				GPIOB										//
#define B1_EN1_9V_CLK					RCC_AHB1Periph_GPIOB		//
#define B0_EN2_9V_PIN					GPIO_Pin_0 		//
#define B0_EN2_9V_PORT				GPIOB										//
#define B0_EN2_9V_CLK					RCC_AHB1Periph_GPIOB		//
//END----------------------------------------

//BEGIN----------------------------------------
#define F14_L_LED_PIN				GPIO_Pin_14 		//GPIO_Mode_Out_PP
#define F15_R_LED_PIN				GPIO_Pin_15 		//GPIO_Mode_Out_PP
#define G0_LED_PIN					GPIO_Pin_0 		//GPIO_Mode_Out_PP
#define G7_KEY_PIN					GPIO_Pin_7		//GPIO_Mode_IPU

#define LED_PIN						G0_LED_PIN
#define LED_PORT					GPIOG
#define LED_CLK					  RCC_AHB1Periph_GPIOG
#define R_LED_PIN					F15_R_LED_PIN
#define R_LED_PORT				GPIOF
#define R_LED_CLK					RCC_AHB1Periph_GPIOF
#define L_LED_PIN					F14_L_LED_PIN
#define L_LED_PORT				GPIOF
#define L_LED_CLK					RCC_AHB1Periph_GPIOF

#define KEY_PIN						G7_KEY_PIN
#define KEY_PORT					GPIOG
#define KEY_GPIO_CLK      RCC_AHB1Periph_GPIOG
//END----------------------------------------

//引脚定义(PAx~PGx)
/*******************************************************/

#define D0_LCD_PD0_PIN				GPIO_Pin_0 		//
#define D1_LCD_PD1_PIN				GPIO_Pin_1 		//
#define D4_LCD_PD4_PIN				GPIO_Pin_4 		//
#define D5_LCD_PD5_PIN				GPIO_Pin_5 		//
#define D7_LCD_PD7_PIN				GPIO_Pin_7 		//
#define D8_LCD_PD8_PIN				GPIO_Pin_8 		//
#define D9_LCD_PD9_PIN				GPIO_Pin_9 		//
#define D10_LCD_PD10_PIN			GPIO_Pin_10		//
#define D11_LCD_PD11_PIN			GPIO_Pin_11		//

#define D14_LCD_PD14_PIN			GPIO_Pin_14		//
#define D15_LCD_PD15_PIN			GPIO_Pin_15		//

#define E7_LCD_PE7_PIN				GPIO_Pin_7 		//
#define E8_LCD_PE8_PIN				GPIO_Pin_8 		//
#define E9_LCD_PE9_PIN				GPIO_Pin_9 		//
#define E10_LCD_PE10_PIN			GPIO_Pin_10		//
#define E11_LCD_PE11_PIN			GPIO_Pin_11		//
#define E12_LCD_PE12_PIN			GPIO_Pin_12		//
#define E13_LCD_PE13_PIN			GPIO_Pin_13		//
#define E14_LCD_PE14_PIN			GPIO_Pin_14		//
#define E15_LCD_PE15_PIN			GPIO_Pin_15		//

#define G5_LCD_LED_SEG_PIN		GPIO_Pin_5 		//

#define G2_LCD_RST_PIN				GPIO_Pin_2 		//GPIO_Mode_Out_PP
#define D12_LCD_CS1_PIN				GPIO_Pin_12		//GPIO_Mode_Out_PP
#define D13_LCD_CS2_PIN				GPIO_Pin_13		//GPIO_Mode_Out_PP
#define LCD_RST_PIN						G2_LCD_RST_PIN
#define LCD_RST_PORT					GPIOG
#define LCD_CS1_PIN						D12_LCD_CS1_PIN
#define LCD_CS1_PORT					GPIOD
#define LCD_CS2_PIN						D13_LCD_CS2_PIN
#define LCD_CS2_PORT					GPIOD
/*******************************************************/

void GPIO_EN_FAN(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF); 				//  ON
void GPIO_EN_9V(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF); 				//  ON
void GPIO_EN_TPS54336(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF); 	//
void GPIO_EN_HV(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF); 				//
void GPIO_SW_883(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 ON_OFF); 				//
void BSP_GPIO_Config(void);
void GPIO_NegationBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin); //dwgl  取反

#endif /* __LED_H */
