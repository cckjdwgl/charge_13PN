//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#ifndef  __DWGL_CHIPFLASH_H
#define  __DWGL_CHIPFLASH_H
#include "stm32f4xx.h"

//define
//#define  UART1_ISR_Choose     2  //??????
//#define  UART0_ISR_Choose     1  //??????
//#define  UART0_ISR_Choose     0  //??????  ?????
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.

#define FLASH_APP1_ADDR		0x08040000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)  //��256K��ʼ
#define FLASH_BL_ADDR     0x08000000

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */

#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base address of Sector 12, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base address of Sector 13, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base address of Sector 14, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base address of Sector 15, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base address of Sector 16, 64 Kbytes  */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base address of Sector 17, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base address of Sector 18, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base address of Sector 19, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base address of Sector 20, 128 Kbytes */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base address of Sector 21, 128 Kbytes */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base address of Sector 22, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base address of Sector 23, 128 Kbytes */
//����
extern 	u16 base_add;

struct  SOFTWAREREG {
	u8 head;
	u8 SW_state;  					   //״̬����
	unsigned char KEEP_EN;     //����BL״̬
	unsigned char HaveApp;     //��A��
	unsigned char SourceBinState;  	//BIN�ļ�״̬
	unsigned char ProStart;    //��ʼ���
	unsigned char EraseApp;  	 //����ʹ��
	u32 CheckTimerSet;			   //���ʱ����
	u32 CheckTimer;					   //���ʱ�䶨ʱ��
	u32 CheckRunAppTimerSet;	 //���ʱ����
	u32 CheckRunAppTimer;			 //���ʱ�䶨ʱ��
	u8 SourcePath[128];			   //��Դ��ַ
};	  
extern struct SOFTWAREREG software;

//�ӳ���
extern void RAMSPI_init(void);
extern unsigned char RAMSPI_SR_BYTE(unsigned char data);
extern void FLASH2_RAMSPI_Read (unsigned int write_addr, unsigned char *buffer, unsigned int len);//FLASH-����LEN���ֽ�-
extern void FLASH_WriteNWord(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int nWord);
extern void FLASH_ReadNWord(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int nWord);

extern void software_task(void);
extern void CHIP_PRO(unsigned char* path);
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




