//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*****************************************************************************
#include "stm32f4xx.h"

//变量
u8  *SPI1_Receive_Pointer;
u16  SPI1_Receive_BufSize;
u16  SPI1_Receive_Length;
u8  *SPI1_Trans_Pointer;
u16  SPI1_Trans_BufSize;
u16  SPI1_Trans_Length;
u8   SPI1_TX_State;
u8   SPI1_RX_State;
u8   SPI1_Error;

u8  *SPI2_Receive_Pointer;
u16  SPI2_Receive_BufSize;
u16  SPI2_Receive_Length;
u8  *SPI2_Trans_Pointer;
u16  SPI2_Trans_BufSize;
u16  SPI2_Trans_Length;
u8   SPI2_TX_State;
u8   SPI2_RX_State;
u8   SPI2_Error;

//*****************************************************************************
static void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);

	//初始引脚
	GPIO_InitStructure.GPIO_Pin = A7_SPI1_MOSI_PIN|A5_SPI1_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A6_SPI1_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//初始化片选引脚
	GPIO_InitStructure.GPIO_Pin = A4_SPI1_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, A4_SPI1_CS_PIN);//PA4/NSS  失能

	/* SPI Config */
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	//SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* 开中断 */
	//SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);

	/* Enable SPI2 */
	SPI_Cmd(SPI1, ENABLE);
}

void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//初始引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//初始化片选引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	/* SPI Config */ 								  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* 开中断 */
	//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);

	/* Enable SPI2 */
	SPI_Cmd(SPI2, ENABLE);
}

//*****************************************************************************
void SPI_Send_Data(SPI_TypeDef* SPIx,u8 * txp,u8 * rxp,u16 len)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI2 peripheral */
	//  SPI_I2S_SendData(SPI1, data);
	SPIx->DR = *(txp+len);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	//  return SPI_I2S_ReceiveData(SPI1);
	*(rxp+len) = SPIx->DR;
}

//*****************************************************************************
void SPI_DMASend_Data(SPI_TypeDef* SPIx,u8 * txp,u8 * rxp,u16 len)
{
	if(SPI1_Trans_Length==0)
	{
		SPI1_Receive_Pointer = rxp;
		SPI1_Trans_Pointer = txp;
		SPI1_Trans_Length = len;
	}
}


/*******************************************************************************
 * Function Name  : Flash_ReadWriteByte
 * Description    : 通过硬件SPI发送一个字节到SST25VF016B
 * Input          : - data: 发送的数据
 * Output         : None
 * Return         : SST25VF016B 返回的数据
 * Attention		 : None
 *******************************************************************************/
uint8_t Flash_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t data)		
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPIx, data);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPIx);                                              
}

//*****************************************************************************
void SPI1_IRQHandler(void)
{
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)//接收到了数据
	{
		if(SPI1_Trans_Length>0)
		{
			SPI1->DR = SPI1_Trans_Pointer[SPI1_Receive_Length];
			SPI1_Receive_Pointer[SPI1_Receive_Length] = SPI1->DR;
			SPI1_Trans_Length--;
			SPI1_Receive_Length++;
		}
		else
		{
			SPI1_TX_State = 1;
		}
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
	}
}

//*****************************************************************************
void SPI2_IRQHandler(void)  
{

}


//*****************************************************************************
void SPI_Configuration(void)
{
	SPI1_Receive_Pointer = SPI_BUFFER;
	SPI1_Trans_Pointer = SPI_BUFFER;
	SPI1_Init();
	//SPI2_Init();
	SPI1_Receive_BufSize = 128;
	//SPI2_Receive_BufSize = 0;
}
/********************************END OF FILE***********************************/

