//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#ifndef  __DWGL_SPI_H
#define  __DWGL_SPI_H
#include "stm32f4xx.h"

//define 
//#define	SPI1_init_Normal//�ֶ����·�ʽ
#define	SPI1_init_DMA      // DMA��ʽ

#ifndef A7_SPI1_MOSI_PIN
#define A7_SPI1_MOSI_PIN			GPIO_Pin_7 		//GPIO_Mode_Out_PP
#endif

#ifndef A6_SPI1_MISO_PIN
#define A6_SPI1_MISO_PIN			GPIO_Pin_6		//GPIO_Mode_IN_FLOATING
#endif

#ifndef A5_SPI1_CLK_PIN
#define A5_SPI1_CLK_PIN				GPIO_Pin_5 		//GPIO_Mode_Out_PP
#endif

#ifndef A4_SPI1_CS_PIN
#define A4_SPI1_CS_PIN				GPIO_Pin_4 		//GPIO_Mode_Out_PP
#endif


//����
 extern  u8  *SPI1_Receive_Pointer;
 extern  u16  SPI1_Receive_BufSize;
 extern  u16  SPI1_Receive_Length;
 extern  u8  *SPI1_Trans_Pointer;
 extern  u16  SPI1_Trans_BufSize;
 extern  u16  SPI1_Trans_Length;
 extern  u8   SPI1_TX_State;
 extern  u8   SPI1_RX_State;
 extern  u8   SPI1_Error;

 extern  u8  *SPI2_Receive_Pointer;
 extern  u16  SPI2_Receive_BufSize;
 extern  u16  SPI2_Receive_Length;
 extern  u8  *SPI2_Trans_Pointer;
 extern  u16  SPI2_Trans_BufSize;
 extern  u16  SPI2_Trans_Length;
 extern  u8   SPI2_TX_State;
 extern  u8   SPI2_RX_State;
 extern  u8   SPI2_Error;

//�ӳ���
extern void SPI1_Enable(void);
extern void SPI1_Disable(void);
extern void SPI1_Init(void);
extern void SPI1_Set(u8 baud,u8 work_mode);

extern void SPI2_Enable(void);
extern void SPI2_Disable(void);
extern void SPI2_Init(void);
extern void SPI2_Set(u8 baud,u8 work_mode);

//extern void SPI1_Send_Data(u8 * p,u16 len);
extern void SPI_Send_Data(SPI_TypeDef* SPIx,u8 * txp,u8 * rxp,u16 len);
extern void SPI_DMASend_Data(SPI_TypeDef* SPIx,u8 * txp,u8 * rxp,u16 len);
extern uint8_t Flash_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t data);		

extern void SPI_Configuration(void);

#endif

