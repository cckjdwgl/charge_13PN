//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#ifndef  __DWGL_CHIPFLASH_H
#define  __DWGL_CHIPFLASH_H
#include "stm32f10x.h"

//define
//#define  UART1_ISR_Choose     2  //??????
//#define  UART0_ISR_Choose     1  //??????
//#define  UART0_ISR_Choose     0  //??????  ?????
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.

#define FLASH_APP1_ADDR		0x08010000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
#define FLASH_BL_ADDR     0x08000000
//����
extern 	u16 base_add;
extern 	unsigned char APP_EN;
extern 	unsigned char KEEP_EN;   //����BL״̬
extern 	unsigned char PRO_State;

//�ӳ���
extern void RAMSPI_init(void);
extern unsigned char RAMSPI_SR_BYTE(unsigned char data);
extern void FLASH2_RAMSPI_Read (unsigned int write_addr, unsigned char *buffer, unsigned int len);//FLASH-����LEN���ֽ�-
extern void FLASH_WriteNWord(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int nWord);
extern void FLASH_ReadNWord(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int nWord);

extern void CHIP_PRO(void);
extern void Check_CHIP_PRO(void);
extern void Get_PRO_State(void);
extern void Run_Logo(void);
extern void Goto_APP(u32 appxaddr);

//����Ϊ��ຯ��
// void WFI_SET(void);		//ִ��WFIָ��
// void INTX_DISABLE(void);//�ر������ж�
// void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

extern void __init_all_on(void);
extern void __init_all_off(void);
extern void __init_all_ctrl(unsigned int on_off);
extern unsigned int __init_all_get(void);

#endif




