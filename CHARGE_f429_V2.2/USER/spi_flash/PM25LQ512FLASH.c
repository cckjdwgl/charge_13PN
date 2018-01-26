#include "PM25LQ512FLASH.h"
#include "stm32f4xx.h"
/*
//PM25LQ512FLASH  ISSI��˾

-  Pm25LQ040B: 4Mbit/512Kbyte
-  Pm25LQ020B: 2Mbit/256Kbyte
-  Pm25LQ010B: 1Mbit/128Kbyte
-  Pm25LQ512B: 512Kbit/64Kbyte


-  Uniform 4 Kbyte Sectors�������� or 32/64 Kbyte Blocks���飩
-  Flexible 4, 32, 64 Kbytes, or Chip Erase  ��������С��λΪ4K
-  Standard Page Program 1 to 256 bytes ÿ��д�����256�ֽڣ�ÿ��ҳΪ256�ֽ�


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


�ѵ��豸 ����256Mb

ÿҳ4KB�ֽ� 16ҳΪһ�������� ��64K�ֽ� ÿ�β�����С��λ

Sector ��1024��
  Subsector  ��16384��


  512MB N25Q��װ�ð�������256MBģ�ߡ� ���û��ĽǶ��������� ��Ϊһ����Ƭ�����ѵ��豸����Ϊ�������漰�����洢�� �Ͳ���������״̬��ѯ�� ����������һ����оƬѡ��S����; ˫оƬ�汾Ҳ���á�
   ���빤����ϵ��ȡ������Ϣ�� �ڴ�1024��64KB����֯����Ҫ���ţ���һ���ֳ�16 ÿ�������飨��16,384�����飩�� �洢���ɲ���4KB�� ������ͬһʱ�䣬64KB��ʱ�䣬���Ż�оƬ��256MB����ͬһʱ��
   �� ��ʧ�Ժͷ���ʧ���ڴ����ͨ�����д������ TECTION���ܣ�����ʵ��Ӧ�õ���Ҫ�� ���������� 64KB���������ȣ��ӷ��Ա��� ����������64һ���Կɱ�̣�OTP�������Զ�ȡ���ֽںͳ��� MED��ȡOTP�ͳ���OTP���
    ��64���ֽ�Ҳ������ ������������OTP��� ���豸��������ͣ�ͻָ�ʹ��ר�õı�̺Ͳ������� ���/��������ͻָ�ָ�


��ַ��3�ֽ�4�ֽڵ�ַģʽ ����������3�ֽڻ�4���ֽڵĵ�ַģʽ�������ڴ泬��128MB�� ��������4���ֽڵĵ�ַģʽ�����������Ҫһ����ַ������EN- tered��һ��4�ֽڵĵ�ַģʽ�����˳�
������4���ֽڵĵ�ַ ģʽ�����EXIT 4�ֽڵ�ַģʽ��� 4���ֽڵĵ�ַ ģʽҲ����ͨ������ʧ�����üĴ��������� ��μ��Ĵ��� �˽������Ϣ�� ��ӪЭ�� �ô洢���������ֲ�ͬ��Э�������
 ?��չSPI����׼SPIЭ������˫�˺��Ĳ����� ?˫I / O SPI ?��I / O SPI ��׼SPIЭ�����չ����ǿ��˫�˺��Ĳ����� �� ���⣬˫SPI����SPIЭ����������ݵĴ�ȡʱ��� ͨ��һ����һ��I / O�豸���͵�����
 ����ַ������ ���������ĸ������ߡ� ÿ��Э��������ص�������ִ�ж�����DTRģʽ�� ��ʹ��������������ͬʱ�����ڽϵ͵�ʱ��Ƶ��


��1ҳ
����Ĵ���NOR���� 3V�����I / O��4KB�������� N25Q512A �ص� ?�ѵ��豸������256MBģ�ߣ� ?SPI���ݵĴ������߽ӿ� ?˫�������ʣ�DTR��ģʽ ?2.7-3.6V����Դ��ѹ 
?108 MHz��ʱ��Ƶ�ʣ����֧������ Э���ڵ�һ�Ĵ������ʣ�STR��ģʽ ?54�׺գ�MAX��֧�����е�ʱ��Ƶ�� DTRģʽЭ�� ?˫/��I / Oָ���ṩ����ǿ�� ��������߿ɴ�54 MB / s�� ?֧�ֵ�Э�顣
 -��չSPI��˫I / O����I / O -֧�����е�DTRģʽ ?�͵�ִ�У�XIP��ģʽ��������Э�� -��ͨ����ʧ�Ի����ʧ�ԼĴ��� -�����ڴ�ֱ��XIPģʽAF- �ϵ�� ?���/����������� ?����Э�� -������
  -�ĺ˻�˫������ -�ĺ˻�˫I / O��� ?������ӦӦ�� -�����õ����������� -������������� ?�����λ ?������λ����ѡ�����ֺ��� 1 ?֧��3�ֽں�4�ֽڵĿ�Ѱַģʽ ?64�ֽڣ��û�������
  һ���Կɱ�� ��OTP��ר���� ?�������� -�����������4KB���Ⱦ��ȿ� -��������64KB���Ⱦ��ȿ� -��оƬ���� ?д���� -���д�������ܣ����������� 64KB����ͨ���ӷ�������λ 
  -Ӳ��д��������������� ����������ַ���ʧ��λ��BP0��BP1��BP2�� BP3��TB�� -��������ܱ������ܣ��ɸ������� ̽�� ?����ǩ�� -����JEDEC��׼��2���ֽڵ�ǩ����BA20h��
   -���ص�ID�루UID����17ֻ���ֽ� �����������������չ�豸ID �ֽ�ʶ���豸��ѡ��; CUS �Ĺ�������tomized��14�ֽڣ� ?���100,000ÿ������������ ?����20������ݱ��� 
   ?�����-����JEDEC��׼�����з���RoHS��׼ - V-PDFN-8/8mm x 6mm�ģ�Ҳ����ΪSON DFPN��MLP��MLF�� -��SOP2-16/300mils��Ҳ����ΪSO16W��SO16- ��SOIC-16�� - T-PBGA-24b05/6mm����8mm��Ҳ��Ϊ TBGA24��




Sector Erase Time (4Kbyte)   �C 70(T)    300(max)  ms
Chip Erase Time for 512kb    �C 0.25(T)  1(max)    s 

//-----------------FLASH--PM25LQ512ָ��------------------------------
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

*/

	u8 FLASH_u8temp =0;
	u16 FLASH_u16temp =0;


void FLASH2_CSSelect(unsigned char cs_select)   //Ƭѡʹ��
{
	if(cs_select==Enable)
	  {
	    	GPIO_ResetBits(FLASH1CS_PORT, FLASH1CS);   //RAMƬѡ����
	  }
		else
		{
	    	GPIO_SetBits(FLASH1CS_PORT, FLASH1CS);   //RAMƬѡ������
		}	
		
}


//-------FLASH_CMD_WRSR    0x01 //д״̬�Ĵ���;FLASH_CMD_WRFR    0x42//д���ܼĴ���---------------------------
void  FLASH2_WRREG(unsigned char FLASH_CMD,unsigned char rdsr_data)//д�Ĵ���
{
   //   RAM_CS   = 1;FLASH_CS   = 0;   
   FLASH2_CSSelect( Enable);                  

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD  ;     // д״̬�Ĵ��� FLASH_CMD_WRSR

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = rdsr_data  ;     //  д 
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
      delay_cycle(2);

//	  RAM_CS   = 1;FLASH_CS   = 1;   
	FLASH2_CSSelect(Disable); 
	  
}


//---------FLASH_CMD_RDSR    0x05//��״̬�Ĵ���; FLASH_CMD_RDFR   0x48//�����ܼĴ��� ---------------------------
unsigned char FLASH2_RDREG(unsigned char FLASH_CMD)//���Ĵ��� 
{
    unsigned char  returndata; 

   //   RAM_CS   = 1;FLASH_CS   = 0;   
   FLASH2_CSSelect( Enable);                  
                                        
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD;      //��״̬�Ĵ���

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD;      //��״̬�Ĵ���
	
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		returndata = SPI1->DR;
//	  RAM_CS   = 1;FLASH_CS   = 1;   
	FLASH2_CSSelect(Disable); 
      delay_cycle(1);

	  return returndata;	  
}

//------FLASH-����LEN���ֽ�---------
//---ע��д��Ͷ���֮������Ҫ��10MS��ʱ------------
void FLASH2_Read_to_Buffer (unsigned int write_addr, unsigned char *buffer, unsigned int len)//FLASH-����LEN���ֽ�-
{ 
  // U16  i;
   unsigned char addr1,addr2,addr3;
		addr1 = ((write_addr&0xff0000)>>16);
		addr2 = ((write_addr&0xff00)>>8);
		addr3 = ((write_addr&0xff));
  //   RAM_CS   = 1;FLASH_CS   = 0;  //Ƭѡʹ��
// delay_cycle(2);  
   FLASH2_CSSelect( Enable);                  
   
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_READ;       //��ȡ����
	
//		FLASH_u8temp=SPI1->DR;
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr1;
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr2;
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr3;

		if(fast_read ==1)
		{
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
 	  SPI1->DR = 0;      //��״̬�Ĵ���
 		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
 		FLASH_u8temp=SPI1->DR;
		while(len>0)
			{
  	 // buffer[i]= FLASH2_ReadByte();				
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = 0;      //��״̬�Ĵ���
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*buffer=SPI1->DR;
	  buffer++;
		len--;
			}

		}
		else
		{
		delay_cycle(10);  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		FLASH_u8temp=SPI1->DR;
		while(len>0)
			{
  	 // buffer[i]= FLASH2_ReadByte();
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			SPI1->DR = 0;      //��״̬�Ĵ���
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			*buffer=SPI1->DR;
			buffer++;
			len--;
			}
		}
    delay_cycle(2);
	FLASH2_CSSelect(Disable); 
}
//------FLASH-д��LEN���ֽ�---------
void FLASH2_PageWrite_from_Buffer(unsigned int write_addr, unsigned char *desbuffer,unsigned int len) // reentrant//FLASH-д��LEN���ֽ�
{ 
   unsigned char addr1,addr2,addr3;
		addr1 = ((write_addr&0xff0000)>>16);
		addr2 = ((write_addr&0xff00)>>8);
		addr3 = ((write_addr&0xff));

//	RAM_CS   = 1;FLASH_CS   = 0; 
   FLASH2_CSSelect( Enable);                  	                   
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /* Send byte through the SPI2 peripheral */
//  SPI_I2S_SendData(SPI1, data);
	  SPI1->DR = FLASH_CMD_WREN  ;     //  дʹ�� 
	
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
//  return SPI_I2S_ReceiveData(SPI1);
	FLASH_u8temp = SPI1->DR; 
	delay_cycle(2);  
	FLASH2_CSSelect(Disable);  
	
//	RAM_CS   = 1;FLASH_CS   = 0; 
   FLASH2_CSSelect( Enable);                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI1->DR  = FLASH_CMD_PP; //дҳ

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI1->DR  = addr1; 	 //д��ַ

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI1->DR  = addr2;

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI1->DR  = addr3;
	
   while(len>0)
    {
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
   SPI1->DR  = *desbuffer; 	 //д����
	  desbuffer++;
	  len--;
     }
		 
 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
     delay_cycle (2);                        
 FLASH2_CSSelect(Disable); 
		 
  // FLASH2_CSSelectEnable( cs_select);
//	Delay_us (1);
	                   
  //  SPI0DAT  = FLASH_CMD_WRDI;        //д��Ч
  //  while (!SPIF);                       
   // SPIF     = 0;                        
  
   //  RAM_CS   = 1;FLASH_CS   = 1;   
   // FLASH2_CSSelectDisable(cs_select);
   //  Delay_us (1);  

	FLASH2_IsBusy(1); 
}

//-----��������4K-------------------------------------------
void  FLASH2_SER(unsigned int u32_addr)//������������flash  ÿ�β���4K
{
  //  RAM_CS   = 1;FLASH_CS   = 0; 
   unsigned char addr1,addr2,addr3;
		addr1 = ((u32_addr&0xff0000)>>16);
		addr2 = ((u32_addr&0xff00)>>8);
		addr3 = ((u32_addr&0xff));
   FLASH2_CSSelect( Enable);                  
	                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_WREN  ;     //  дʹ�� 
		  
  //  RAM_CS   = 1;FLASH_CS   = 1;   
    delay_cycle(10);   
   FLASH2_CSSelect(Disable);                                     
    delay_cycle(2);                      
    
   //---------FLASH_CMD_SE--------------
  //  RAM_CS   = 1;FLASH_CS   = 0;  
	 FLASH2_CSSelect(Enable);   
	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_SER;//         0xD7        //��������
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr1;
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr2;
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = addr3;
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    delay_cycle(2);

//	RAM_CS   = 1;FLASH_CS   = 1; 
 FLASH2_CSSelect(Disable); 

  
///-------ָ����������------------------	 
//	 RAM_CS   = 1;FLASH_CS   = 0;   
	FLASH2_IsBusy(1); 
	delay_cycle(2);  
     
}
//-----------------------------------------------------------------------------
//  FLASH_BE
//  ���ܣ�������Ƭflash ����оƬ512Mb
//------------------------------------------------------------------------------
void  FLASH2_CER(unsigned char cs_select) //������ƬFLASH
{

/*   FLASH2_IsWriteEnble(0x01); //�ж��Ƿ���д  ����
    FLASH2_WDExtended_AddressRegister(cs_select ,0x00); //д��ַ�����λ31-25    zai 24-25   00  Ĭ��128       ��ַ00000000h-00FFFFFFh
     FLASH2_IsBusy( cs_select);

    Delay_us (200); 
   FLASH2_DIE_ERASE( cs_select,0x00,0x00, 0x00);//��ҳ��������flash  ÿ�β���256mb  
*/
//	EA = 0;
	FLASH2_IsBusy(1);
	
   FLASH2_CSSelect( Enable);                  	                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_WREN  ;     //  дʹ�� 
	delay_cycle(10);                  
   FLASH2_CSSelect(Disable);                                     
	
   FLASH2_CSSelect( Enable);                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR  =	FLASH_CMD_CER;  
	delay_cycle(10);                  
	FLASH2_CSSelect(Disable); 

   FLASH2_CSSelect( Enable);                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR  =	FLASH_CMD_CER;  
	delay_cycle(10);                  
	FLASH2_CSSelect(Disable); 
  delay_cycle (10000);
 
 
///-------ָ����������------------------	 
	FLASH2_IsBusy(1); 
//	EA = 1;
}

//--------------�ж��Ƿ���д  ����--------------------
void FLASH2_IsBusy(unsigned char cs_select) //�ж��Ƿ���д  ����
{
//   unsigned char i;

  do
   { 
	 FLASH2_CSSelect(Enable);   
		delay_cycle(10);  
		FLASH_u8temp = SPI1->DR;
		 
    //  SPI0DAT  = FLASH_CMD_RDSTATUSR;   
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	   SPI1->DR  =	FLASH_CMD_RDSR  ;  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		FLASH_u8temp = SPI1->DR;
    
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	   SPI1->DR  =	FLASH_CMD_RDSR  ;       
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		FLASH_u8temp = SPI1->DR;
		 
	FLASH2_CSSelect(Disable); 
	  	//---LED��ʾ----------
	/*			 i++;
			if(i==250)
			 {
				   i=0;
				   j++;
				   if(j==250)
				    { 
					  j=0;				  
					  LED =~LED;				  
					}
				}
				*/	 
     } 
//		 while(((SPI1->DR) & 0x01) == 0x01); //Write In Progress Bit:"0" indicates the device is ready (default);"1" indicates a write cycle is in progress and the device is busy.
		 while(((FLASH_u8temp) & 0x01) == 0x01); //Write In Progress Bit:"0" indicates the device is ready (default);"1" indicates a write cycle is in progress and the device is busy.

}
//-------------  ȥFLASH��Ƭд����  ��ϵͳ�ϵ�ʱ����һ��-----------------------   
void UnprotectFlash2(void) //  ȥFLASH��Ƭд����
{   
    FLASH2_WRREG(FLASH_CMD_WRSR,0x00);   
} 
//-----------��ID-------------
void  FLASH2_ReadID(unsigned char cs_select,unsigned char *buffer)//N25Q256 20 BA 19 10 00 00 23 14 75 44 08 00 41 00 25 27 01 12 C5 0E
{
     
     unsigned char  i;

   //   RAM_CS   = 1;FLASH_CS   = 0;   
   FLASH2_CSSelect( Enable);                  
                                        
      delay_cycle(10);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_RDID;  	     //�����ӱ�ǩ
        for(i=0;i<4;i++)
		{
		  buffer[i]=0;
		} 
        for(i=0;i<4;i++)
		{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = 0;      //��
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			buffer[i]=SPI1->DR;
		}

//	  RAM_CS   = 1;FLASH_CS   = 1;   
	FLASH2_CSSelect(Disable); 
      delay_cycle(1);
	  
}

/////////////////////////N25q256ʹ��/////////////////////////////
/////////////////////////N25q256ʹ��/////////////////////////////
void FLASH_Read_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4, unsigned char *buffer, unsigned int len)  //FLASH-д��LEN���ֽڣ�LEN<=256
{
//���ô˺�����ȷ���ѿ���4��ַ(�鿴��־λ��
                      
	unsigned int i;

		FLASH2_CSSelect( Enable);                  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = FLASH_CMD_ENTER_4BYTEADDRESS_MODE;  //����4��ַ    
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	
		FLASH2_CSSelect(Disable); 

		FLASH2_CSSelect( Enable);                  
	
//   SPI0DAT  = FLASH_CMD_READ;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = FLASH_CMD_4BYTE_READ;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr1;  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr2; 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr3;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr4;
   for(i=0;i<len;i++)
    {
  	 // buffer[i]= FLASH2_ReadByte();
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = 0;      //��״̬�Ĵ���
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	   buffer[i]= SPI1->DR;
    }
    delay_cycle (2);     
		FLASH2_CSSelect(Disable); 
}


void FLASH_Write_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4, unsigned char *buffer, unsigned int len)  //FLASH-д��LEN���ֽڣ�LEN<=256
{
//���ô˺�����ȷ���ѿ���4��ַ(�鿴��־λ��
//���ô˺�����ȷ��дʹ�ܱ���(�鿴��־λ��
/*
	  FLASH1CS = 0;
	  FLASH1CS = 0;
	  SPI0DAT  = FLASH_CMD_WENBLE;  //д��ʹ��         
      while (!SPIF);                       
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;
	  FLASH1CS = 1;
	  FLASH1CS = 1;
	  Delay_us(100);

	  FLASH1CS = 0;
	  FLASH1CS = 0;
	  SPI0DAT  = FLASH_CMD_ENTER_4BYTEADDRESS_MODE;  //����4��ַ         
      while (!SPIF);                       
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;
	  FLASH1CS = 1;
	  FLASH1CS = 1;
	  Delay_us(100);
*/
	unsigned int i;

//	FLASH2_IsBusy(1); //�ж��Ƿ���д  ����

		FLASH2_CSSelect( Enable);                  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = FLASH_CMD_ENTER_4BYTEADDRESS_MODE;  //����4��ַ    
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	  delay_cycle(2);
		FLASH2_CSSelect(Disable); 

   FLASH2_CSSelect( Enable);                  	                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_WREN  ;     //  дʹ�� 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	  delay_cycle(2);
   FLASH2_CSSelect(Disable);                                     
/*
	  FLASH1CS = 0;
	  FLASH1CS = 0;
	  SPI0DAT  = FLASH_CMD_RDSTATUSR;  //��״̬       
      while (!SPIF);                       
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;

	  SPI0DAT  = FLASH_CMD_RDSTATUSR;            
      while (!SPIF);                        
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;
	  FLASH1CS = 1;
	  FLASH1CS = 1;

	  FLASH1CS = 0;
	  FLASH1CS = 0;
	  SPI0DAT  = FLASH_CMD_RDFLAGSTATUSR;  //����־       
      while (!SPIF);                       
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;

	  SPI0DAT  = FLASH_CMD_RDFLAGSTATUSR;            
      while (!SPIF);                        
      SPIF     = 0;
  	  globle8_temp = SPI0DAT;
	  FLASH1CS = 1;
	  FLASH1CS = 1;
*/
   FLASH2_CSSelect( Enable);                  	                  

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = FLASH_CMD_PP;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr1;  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr2; 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr3;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr4;
   for(i=0;i<len;i++)
    {
//  	 FLASH2_WriteByte(buffer[i]);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = buffer[i];
    }
		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    delay_cycle (2);     
   FLASH2_CSSelect(Disable);                                     
		                                     
	FLASH2_IsBusy(1); //�ж��Ƿ���д  ����
	                  
}

void  FLASH_ERASE4K_4Byte(unsigned char addr1,unsigned char addr2, unsigned char addr3,unsigned char addr4)//��ҳ��������flash  ÿ�β���4K
{
        
   FLASH2_CSSelect( Enable);                  	                  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  SPI1->DR = FLASH_CMD_WREN  ;     //  дʹ�� 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    delay_cycle (2);     
   FLASH2_CSSelect(Disable);                                     

   FLASH2_CSSelect( Enable);                  	                  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = FLASH_CMD_PP;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr1;  
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr2; 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr3;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR  = addr4;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    delay_cycle (2);     
   FLASH2_CSSelect(Disable);                                     

///-------ָ����������------------------	 
	FLASH2_IsBusy(1); 
//	FLASH2_IsWriteEnble(1);          
}

/////��ʱ---------------------------------------------------------------------------------
void  delay_cycle(unsigned int cycles)
{	
	u32 i;
	i = cycles;
	while(i--);
}

