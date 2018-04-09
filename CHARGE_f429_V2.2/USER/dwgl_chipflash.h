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
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define FLASH_APP1_ADDR		0x08040000  	//第一个应用程序起始地址(存放在FLASH)  //从256K开始
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
//变量
extern 	u16 base_add;

struct  SOFTWAREREG {
	u8 head;
	u8 SW_state;  					   //状态开关
	unsigned char KEEP_EN;     //保持BL状态
	unsigned char HaveApp;     //有A类
	unsigned char SourceBinState;  	//BIN文件状态
	unsigned char ProStart;    //开始编程
	unsigned char EraseApp;  	 //擦除使能
	u32 CheckTimerSet;			   //检查时间间隔
	u32 CheckTimer;					   //检查时间定时器
	u32 CheckRunAppTimerSet;	 //检查时间间隔
	u32 CheckRunAppTimer;			 //检查时间定时器
	u8 SourcePath[128];			   //资源地址
};	  
extern struct SOFTWAREREG software;

//子程序
extern void RAMSPI_init(void);
extern unsigned char RAMSPI_SR_BYTE(unsigned char data);
extern void FLASH2_RAMSPI_Read (unsigned int write_addr, unsigned char *buffer, unsigned int len);//FLASH-读出LEN个字节-
extern void FLASH_WriteNWord(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int nWord);
extern void FLASH_ReadNWord(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int nWord);

extern void software_task(void);
extern void CHIP_PRO(unsigned char* path);
extern void Check_CHIP_PRO(void);
extern void Get_PRO_State(void);
extern void Run_Logo(void);
extern void Goto_APP(u32 appxaddr);

//以下为汇编函数
// void WFI_SET(void);		//执行WFI指令
// void INTX_DISABLE(void);//关闭所有中断
// void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

extern void __init_all_on(void);
extern void __init_all_off(void);
extern void __init_all_ctrl(unsigned int on_off);
extern unsigned int __init_all_get(void);

#endif




