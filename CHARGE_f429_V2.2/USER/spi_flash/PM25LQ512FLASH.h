#ifndef  PM25LQ512FLASH_H
#define  PM25LQ512FLASH_H
// #define FLASH_IC  PM25LV512   //512Kbit PMC��˾
// #define FLASH_IC  PM25LQ512   //512Kbit ISSI��˾
// #define FLASH_IC  W25Q128   //128Mbit   Win bond ����
// #define FLASH_IC  N25Q256   //256Mbit ����(Micron)

#include "stm32f4xx.h"
//define
//#define   PM25LV512   //512Kbit PMC��˾
//#define   PM25LQ512   //512Kbit ISSI��˾
#define   W25Q128   //128Mbit   Win bond ����
//#define   N25Q256   //256Mbit ����(Micron)


//-----------------FLASH--PM25LQ512ָ��------------------------------
#if defined (PM25LQ512) 
	#define  FLASH_CMD_READ        0x03        //��ȡ����
	#define  FLASH_CMD_FAST_READ   0x0B        //���ٶ�ȡ����
	#define  FLASH_CMD_PP          0x02        //ҳ������д��
	#define  FLASH_CMD_WRDI        0x04        //д��Ч
	#define  FLASH_CMD_WREN        0x06        //дʹ��
	#define  FLASH_CMD_RDSR        0x05        //��״̬�Ĵ���
	#define  FLASH_CMD_WRSR        0x01        //д״̬�Ĵ���
	#define  FLASH_CMD_RDFR        0x48        //�����ܼĴ���
	#define  FLASH_CMD_WRFR        0x42        //д���ܼĴ���
	#define  FLASH_CMD_SER         0xD7        //�������� 
	#define  FLASH_CMD_SE          0xD8        //�������
	#define  FLASH_CMD_CER         0xC7  	     //��Ƭ����
	#define  FLASH_CMD_RDID        0xAB  	     //�����ӱ�ǩ
	#define  FLASH_CMD_RDJDID      0x9F  	     //����ʶ
	
	#define  SPI_FLASH_PageSize    256       //ÿ��ҳ���ֽڴ�С
	#define  Sector_Size        	 4096  	   //�ֽ�
	#define  Block_Size        	   32768  	 //�ֽ�
	#define  Chip_Size        	   65536  	 //�ֽ�
	#define  FLASH_MAX_CAP         0X00FFFF  	   //����ַ
#endif

#if defined (W25Q128) 
	#define  FLASH_CMD_READ        0x03        //��ȡ����
	#define  FLASH_CMD_FAST_READ   0x0B        //���ٶ�ȡ����
	#define  FLASH_CMD_PP          0x02        //ҳ������д��
	#define  FLASH_CMD_WRDI        0x04        //д��Ч
	#define  FLASH_CMD_WREN        0x06        //дʹ��
	#define  FLASH_CMD_RDSR        0x05        //��״̬�Ĵ���
	#define  FLASH_CMD_WRSR        0x01        //д״̬�Ĵ���
	#define  FLASH_CMD_RDFR        0x48        //�����ܼĴ���
	#define  FLASH_CMD_WRFR        0x42        //д���ܼĴ���
	#define  FLASH_CMD_SER         0x20        //�������� 
	#define  FLASH_CMD_SE          0xD8        //�������
	#define  FLASH_CMD_CER         0xC7  	     //��Ƭ����
	#define  FLASH_CMD_RDID        0xAB  	     //�����ӱ�ǩ
	#define  FLASH_CMD_RDJDID      0x9F  	     //����ʶ
	
	#define  SPI_FLASH_PageSize    256       //ÿ��ҳ���ֽڴ�С
	#define  Sector_Size        	 4096  	   //�ֽ�
	#define  Block_Size        	   32768  	 //�ֽ�
	#define  Chip_Size        	   16777216  	 //�ֽ�
	#define  FLASH_MAX_CAP         0XFFFFFF  	   //����ַ
#endif

#if defined (N25Q256) 
	#define  FLASH_CMD_READ        0x03        //��ȡ����
	#define  FLASH_CMD_FAST_READ   0x0B        //���ٶ�ȡ����
	#define  FLASH_CMD_PP          0x02        //ҳ������д��
	#define  FLASH_CMD_WRDI        0x04        //д��Ч
	#define  FLASH_CMD_WREN        0x06        //дʹ��
	#define  FLASH_CMD_RDSR        0x05        //��״̬�Ĵ���
	#define  FLASH_CMD_WRSR        0x01        //д״̬�Ĵ���
	#define  FLASH_CMD_RDFR        0x48        //�����ܼĴ���
	#define  FLASH_CMD_WRFR        0x42        //д���ܼĴ���
	#define  FLASH_CMD_SER         0x20        //�������� 
	#define  FLASH_CMD_SE          0xD8        //�������
	#define  FLASH_CMD_CER         0xC7  	     //��Ƭ����
	#define  FLASH_CMD_RDID        0xAB  	     //�����ӱ�ǩ
	#define  FLASH_CMD_RDJDID      0x9F  	     //����ʶ
// 	#define  FLASH_CMD_ENTER_4BYTEADDRESS_MODE   0xb7//����4��ַ  N25Q256����
// 	#define  FLASH_CMD_4BYTE_READ                0x0c  	     //
	
	#define  SPI_FLASH_PageSize    256       //ÿ��ҳ���ֽڴ�С
	#define  Sector_Size        	 4096  	   //�ֽ�
	#define  Block_Size        	   65536  	 //�ֽ�
	#define  Chip_Size        	   33554432  	 //�ֽ�
	#define  FLASH_MAX_CAP         0X1FFFFFF  	   //����ַ
#endif
	#define  FLASH_CMD_ENTER_4BYTEADDRESS_MODE   0xb7//����4��ַ  N25Q256����
	#define  FLASH_CMD_4BYTE_READ                0x0c  	     //

/////////////////////////////////////////////////////////////////////////////////

#define  FLASH_CMD_ERASE_RESUME       0x7A       //�����ָ�
#define  FLASH_CMD_ERASE_SUSPEND      0x75       //��������
#define  FLASH_CMD_SECUNLOCK          0x26       //��������
#define  FLASH_CMD_SECLOCK            0x24       //��������


#define  FLASH1CS_PORT     GPIOA
#define  FLASH1CS          GPIO_Pin_4
// #define  FLASH1CS_PORT     GPIOC
// #define  FLASH1CS          GPIO_Pin_13
#define  Enable            1
#define  Disable           0
//#define  fast_read         1
#define  fast_read         0

extern 	u8 FLASH_u8temp;
extern 	u16 FLASH_u16temp;
	//addr:0x000000-0x003fff  16kB ��ά����
	//addr:0x004000-0x00ffff  48kB ������
	//addr:0x01ffff-0xFFFFFF  ?kB ��ͼ��

void FLASH2_CSSelect(unsigned char cs_select); //Ƭѡ�ź�
void FLASH2_WRREG(unsigned char FLASH_CMD,unsigned char rdsr_data);		//д�Ĵ��� FLASH_CMD_WRSR 
unsigned char FLASH2_RDREG(unsigned char FLASH_CMD);                  //���Ĵ��� FLASH_CMD_WRSR readsrname�Ĵ�����ȡ����
void FLASH2_WriteEnable(unsigned char cs_select); //����дʹ��	 
void FLASH2_WRDI(unsigned char cs_select);				//дʹ����ЧWRDI


void FLASH2_IsBusy(unsigned char cs_select);     //�ж��Ƿ���д  ����      
void UnprotectFlash2(void); 	            //  ȥFLASH��Ƭд��


unsigned char FLASH2_ReadByte(void); 									//��FLASH��ȡһ���ֽ�
void FLASH2_WriteByte(unsigned char value); 					//��FLASHдһ���ֽ�
void FLASH2_Read_to_Buffer (unsigned int write_addr,unsigned char *buffer, unsigned int len);//FLASH-����LEN���ֽ�
void FLASH2_PageWrite_from_Buffer(unsigned int write_addr, unsigned char *desbuffer,unsigned int len); // reentrant//FLASH-д��LEN���ֽ�

void  FLASH2_SER(unsigned int u32_addr);//��ҳ��������flash  ÿ�β���4K
void FLASH2_CER(unsigned char cs_select); //������ƬFLASH	

void  FLASH2_ReadID(unsigned char cs_select,unsigned char *buffer);//��ID 
unsigned char  FLASH2_RDSR(unsigned char cs_select,unsigned char readsrname);//��ȡ״̬�Ĵ��� readsrname�Ĵ�����ȡ����


//dwgl-20160811�޸�
void FLASH_Read_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4, unsigned char *buffer, unsigned int len);//FLASH-����LEN���ֽ�
//void FLASH_Read_3Byte(void);
void FLASH_Write_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4, unsigned char *buffer, unsigned int len);//FLASH-д��LEN���ֽڣ�LEN<=256
//void FLASH_Read_3Byte(void);
//void ENTER_4BYTEADDRESS_MODE(void);
//void EXIT_4BYTEADDRESS_MODE(void);
void  FLASH_ERASE4K_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4);//��ҳ��������flash  ÿ�β���4K

void  delay_cycle(unsigned int cycles);

#endif  //end PM25LQ512_H



/*

CPHA 
0
1
CPOL
0
1

PM25LQ512SPI����ģʽ CLK����=0���½��ظ������ݣ��������������ݡ�

*/

