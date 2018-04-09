//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#include "stm32f4xx.h"
#include "dwgl_chipflash.h"
/***********************************************************************
�ļ����ƣ�dwgl_chipflash.C
��    �ܣ���ɶ�Ƭ�ϴ洢�Ĳ���
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺��������ͨ���ж������ض����ַ���ȷ��һ֡�����Ƿ�����ġ�
***********************************************************************/
 
//---------------------------------------------------------------------------------
u16 base_add;
struct SOFTWAREREG software;
iapfun jump2app; 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
 
static uint32_t GetSector(uint32_t Address);

/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
  */
//--------------------------
void software_task(void)
{
	if(software.SW_state  == SW_NULL)
	{
		software.SW_state = SW_ON;
		software.CheckTimerSet = 2000;
		software.CheckTimer = time_sys;
		software.CheckRunAppTimerSet = 6000;
		software.CheckRunAppTimer = time_sys;
	}
	else
	{
		if(time_sys-software.CheckTimer>software.CheckTimerSet)		
		{
			software.CheckTimer = time_sys;
			software.KEEP_EN = fkey.KeyValue;
			Get_PRO_State();
			Check_CHIP_PRO();
			if(software.EraseApp ==1)  //
			{
				software.EraseApp = 0;
				if(device.Version[0]==BL_FLAG)
				{
					if(((*(vu32*)FLASH_APP1_ADDR)&0xFFF00000)!=0xffffffff)   //���ջ����ַ�Ƿ�Ϸ�.
					{
						/* ʹ�ܷ���FLASH���ƼĴ��� */
						FLASH_Unlock();						
						/* �������FLASH�ı�־λ */  
						FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
														FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
						/* ��ʼ�������� */
							/* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
							if (FLASH_EraseSector(GetSector(FLASH_APP1_ADDR), VoltageRange_3) != FLASH_COMPLETE)
							{ 
					//			return -1;
							}
						/* ��FLASH��������ֹ���ݱ��۸�*/
						FLASH_Lock(); 
					}
					res_sd = f_unlink((char*)"area5/area5_update.out");
					res_sd = f_unlink((char*)"area5/area5_info.get");
				}
				else
				{
					res_sd = f_unlink((char*)"area5/area5_update.out");
					res_sd = f_unlink((char*)"area5/area5_info.get");
					NVIC_SystemReset();
				}
			}
		}
		if(time_sys-software.CheckRunAppTimer>software.CheckRunAppTimerSet)		
		{
			software.CheckRunAppTimer = time_sys;
			if(device.Version[0]==BL_FLAG&&(software.KEEP_EN==0))   //
			{
				Goto_APP(FLASH_APP1_ADDR);
				software.ProStart = 0;
			}
		}
	}
}
//-----------------
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  

  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_Sector_23;  
  }
  return sector;
}


void CHIP_PRO(unsigned char* path)
{
//	unsigned int i;
	unsigned int j;
	unsigned int fnum1;
//	unsigned int f_size,chip_addr;
	unsigned int *p;
//	FLASH_Status temp;
//	unsigned short len;
//	unsigned short u16temp;
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
	unsigned int FLASH_USER_START_ADDR;   
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
	unsigned int FLASH_USER_END_ADDR;   
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
	
	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;

	res_sd = f_open(&fsrc,(char*)path, FA_OPEN_EXISTING | FA_READ); 	//��Ŀ���ļ�
	if(res_sd == FR_OK)
	{
		software.SourceBinState |= 0X01;							
		res_sd = f_read (&fsrc, FatFile_URL.FileBuf, 2048, &fnum1);
		res_sd = f_lseek(&fsrc, 0);
		FLASH_USER_START_ADDR = FLASH_APP1_ADDR;   //
		FLASH_USER_END_ADDR = FLASH_USER_START_ADDR+fsrc.fsize;
  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

  /* ��ʼ�������� */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			sprintf((char*)LocalPoint, "ERROR: FLASH_EraseSector\r\n");
			Str_addto_Str(&DebugStr,LocalPoint);	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);			
//			return -1;
    }
    /* ������ָ����һ������ */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;
    } 
    else 
    {
      uwSectorCounter += 8;
    }
  }

  /* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
  uwAddress = FLASH_USER_START_ADDR;
	p = (unsigned int*)FatFile_URL.FileBuf; //
  while (uwAddress < FLASH_USER_END_ADDR)
  {
		if((uwAddress&0xfffff800)==uwAddress)
	 {
//		 FLASH2_RAMSPI_Read (f_addr+2*i, str_buffer, 2048);   
//		 FLASH2_GPIOSPI_Read (f_addr+2*i, str_buffer, 2048);  //ý�ݳ�ʼ��
		res_sd = f_read (&fsrc, FatFile_URL.FileBuf, 2048, &fnum1);
		j=0;
	 }
		 
    if (FLASH_ProgramWord(uwAddress, p[j++]) == FLASH_COMPLETE)
    {
      uwAddress = uwAddress + 4;
    }
    else
    { 
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
//			return -1;
    }
  }	

  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	
		/* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
		/*  MemoryProgramStatus = 0: д���������ȷ
				MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
		uwAddress = FLASH_USER_START_ADDR;
		p = (unsigned int*)FatFile_URL.FileBuf; //
		res_sd = f_lseek(&fsrc, 0);    
		uwMemoryProgramStatus = 0;
		
		while (uwAddress < FLASH_USER_END_ADDR)
		{
			uwData32 = *(__IO uint32_t*)uwAddress;

		  if((uwAddress&0xfffff800)==uwAddress)
		  {
		 //		 FLASH2_RAMSPI_Read (f_addr+2*i, str_buffer, 2048);   
		 //		 FLASH2_GPIOSPI_Read (f_addr+2*i, str_buffer, 2048);  //ý�ݳ�ʼ��
			 res_sd = f_read (&fsrc, FatFile_URL.FileBuf, 2048, &fnum1);
			 j=0;
		  }
			if (uwData32 != p[j++])
			{				
				uwMemoryProgramStatus++;  
				sprintf((char*)LocalPoint, "ERROR: FLASH_EraseSector\r\n");
				Str_addto_Str(&DebugStr,LocalPoint);	
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);			
			}

			uwAddress = uwAddress + 4;
		}  
		/* ����У�鲻��ȷ */
		if(uwMemoryProgramStatus)
		{    
	//		return -1;
		}
		else /*����У����ȷ*/
		{ 
	//		return 0;   
		}
	}
	res_sd=f_close(&fsrc);	
		
}


//------------------------------------------------------
void Check_CHIP_PRO(void)   //���������
{
// 	unsigned char temp = 0;
// 	unsigned short  i= 0;
// 	unsigned char  PRO_EN= 0;
  err_t wr_err = ERR_OK;
	unsigned short len;
	unsigned short u16temp;
	unsigned int fnum1;
//	unsigned int fnum2;
	
	if(device.Version[0]==BL_FLAG)
	{
		FatFile_URL.FileHead[0] = 0;
		res_sd = f_open(&fsrc, "area5/area5_config.txt", FA_OPEN_EXISTING | FA_READ);    //�Ȳ��ֶ�����			
		if(res_sd == FR_OK)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum1);
			FatFile_URL.FileBuf[fnum1]=0;
			if(res_sd== FR_OK)
			{
				len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"bin\":\"",(unsigned  char*)"\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{		
					software.SourceBinState |= 0X80;
				}
			}
			res_sd=f_close(&fsrc);			
		}
		else		
		{
			res_sd=f_close(&fsrc);			
			res_sd = f_open(&fsrc, "area5/area5_info.txt", FA_OPEN_EXISTING | FA_READ); 	//�ٲ��̨����	
			if(res_sd == FR_OK)
			{
				res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum1);
				if(res_sd== FR_OK)
				{
//					len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"softwareNewVersion\":\"",(unsigned  char*)"\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
					cut_between_strs_inverted((unsigned  char*)FatFile_URL.FileBuf,(unsigned  char*)"_",(unsigned  char*)".bin", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);		
					if(strlen((char*)FatFile_URL.FileHead))
					{			
						software.SourceBinState |= 0X40;
						len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"downloadUrl\":\"",(unsigned  char*)".bin\"", FatFile_URL.FileDestPath,sizeof(FatFile_URL.FileDestPath),&u16temp);
						res_sd = f_open(&fnew, "area5/area5_info.get", FA_OPEN_EXISTING | FA_READ); 	//	
						if(res_sd == FR_OK)
						{
							res_sd = f_read (&fnew, FatFile_URL.FileBufSlave, FatFile_URL.FileBufSlaveSize, &fnum1);
							len = cut_between_strs(FatFile_URL.FileBufSlave,(unsigned  char*)"\"handle\":\"",(unsigned  char*)FatFile_URL.FileDestPath, FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
							if(FatFile_URL.FileSourcePath[0]=='T')
							{
								software.SourceBinState |= 0X20;		////bin�ļ��������					
							}
							else
							{
								FatFile_URL.FileHead[0] = 0;
							}
						}
						else
						{
							FatFile_URL.FileHead[0] = 0;
						}
						res_sd=f_close(&fnew);			
					}
				}
			}			
			res_sd=f_close(&fsrc);			
		}
		res_sd=f_close(&fsrc);			
		//��԰汾���Ƿ�Ҫ����
		if(software.ProStart  ==0)
		{
			software.ProStart  =1;
			sprintf((char*)software.SourcePath,"area5/%s.bin",(char*)FatFile_URL.FileHead);  //��ȡBIN�ļ�·��
			res_sd = f_open(&fsrc, "area5/area5_update.out", FA_OPEN_EXISTING | FA_READ);    //
			if(res_sd == FR_OK)
			{
				res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum1);
				FatFile_URL.FileBuf[fnum1]=0;
				len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"bin\":\"",(unsigned  char*)"\"", FatFile_URL.FileDestPath,sizeof(FatFile_URL.FileDestPath),&u16temp);
				if(strcmp((char *)FatFile_URL.FileHead, (char *)FatFile_URL.FileDestPath)==0)
				{
					len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"run\":\"",(unsigned  char*)"\"", FatFile_URL.FileDestPath,sizeof(FatFile_URL.FileDestPath),&u16temp);
					{
						if(FatFile_URL.FileDestPath[0]=='Y')
						{
							software.ProStart =0;    //
						}
					}
				}			
			}
			res_sd=f_close(&fsrc);			
			if(software.ProStart==1)  //
			{
				if(((*(vu32*)FLASH_APP1_ADDR)&0xFFF00000)!=0xffffffff)   //���ջ����ַ�Ƿ�Ϸ�.
				{
					/* ʹ�ܷ���FLASH���ƼĴ��� */
					FLASH_Unlock();
						
					/* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
					/* �������FLASH�ı�־λ */  
					FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
													FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

					/* ��ʼ�������� */
						/* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
						if (FLASH_EraseSector(GetSector(FLASH_APP1_ADDR), VoltageRange_3) != FLASH_COMPLETE)
						{ 
							/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
				//			return -1;
						}
					/* ��FLASH��������ֹ���ݱ��۸�*/
					FLASH_Lock(); 
				}
				res_sd = f_open(&fsrc, "area5/area5_update.out", FA_CREATE_ALWAYS | FA_WRITE);    //
				sprintf((char*)FatFile_URL.FileBuf,"[\"bin\":\"%s\",\"run\":\"0\"]",(char*)FatFile_URL.FileHead);
				res_sd=f_write(&fsrc,FatFile_URL.FileBuf,strlen((char*)FatFile_URL.FileBuf),&fnum1);						
				res_sd=f_close(&fsrc);			
				{
					CHIP_PRO(software.SourcePath);  //�����µ�
					software.CheckRunAppTimer = time_sys;
				}
			}
		}		
	}
	else
	{
	//A������к˶Ը������
		res_sd = f_open(&fsrc, "area5/area5_update.out", FA_OPEN_EXISTING | FA_READ|FA_WRITE); 	//
		if(res_sd == FR_OK)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum1);
			FatFile_URL.FileBuf[fnum1]=0;
			len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"bin\":\"",(unsigned  char*)"\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
			if(strcmp((char *)device.Version, (char *)FatFile_URL.FileHead)==0)
			{
				len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"run\":\"",(unsigned  char*)"\"", FatFile_URL.FileDestPath,sizeof(FatFile_URL.FileHead),&u16temp);
				{
					if(FatFile_URL.FileDestPath[0]!='Y')
					{							
							res_sd = f_lseek(&fsrc, u16temp);
							sprintf((char*)FatFile_URL.FileBufSlave, "Y");
							res_sd=f_write(&fsrc,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);																		
					}
				}
			}
			else
			{
				len = cut_between_strs(FatFile_URL.FileBuf,(unsigned  char*)"\"run\":\"",(unsigned  char*)"\"", FatFile_URL.FileDestPath,sizeof(FatFile_URL.FileHead),&u16temp);
				{
					if(FatFile_URL.FileDestPath[0]=='Y')
					{							
							res_sd = f_lseek(&fsrc, u16temp);
							sprintf((char*)FatFile_URL.FileBufSlave, "N");
							res_sd=f_write(&fsrc,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);																		
					}
				}				
			}
		}
		res_sd=f_close(&fsrc);			
	}
		
}
//-------------------------------------------
void Goto_APP(u32 appxaddr)  //��ת���û�����
{
 	unsigned char  en= 0;
	if(((*(vu32*)(appxaddr+4))&0xFFF00000)==0x08000000)   //
	{
		en |= 0x01; 
	}
	if(((*(vu32*)appxaddr)&0xFFF00000)==0x20000000)   //���ջ����ַ�Ƿ�Ϸ�.
	{
		en |= 0x02; 
	}
	if(en ==3)	//
	{ 
		__disable_irq();
		__set_MSP(*(__IO uint32_t*) appxaddr); 
//		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		jump2app();									//��ת��APP.
	}
}
//------------------------------------------
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
void Get_PRO_State(void)
{
// 	unsigned char  temp= 0;
 	unsigned int   aa,bb;
	software.HaveApp = 0;
	
//	FLASH2_GPIOSPI_Read (Addr_info2, &info2STR.head[0], sizeof(info2STR));  //ý�ݳ�ʼ��
//	if(info2STR.item21_data[0]==1)//��������
//	{
//		software.PRO_State |= 0x01; 
//	}
//	if((info2STR.item15[1]<=16)&&(info2STR.item15[1]>0))//�о���
//	{
//		software.PRO_State |= 0x02; 
//	}
		aa = ((*(vu32*)(FLASH_APP1_ADDR)));
		bb = ((*(vu32*)(FLASH_APP1_ADDR+4)));
		aa = ((*(vu32*)(FLASH_APP1_ADDR)));
		bb = ((*(vu32*)(FLASH_APP1_ADDR+4)));
	if((aa&0XFFF00000)==0x20000000&&(bb&0XFFF00000)==0x08000000)   //�г���
	{
		software.HaveApp  = 0x01; 
	}
	else
	{
		software.HaveApp  = 0x02; 
	}
//	if(KEEP_EN==0xff)
//	{
//		software.PRO_State |= 0x08; 
//	}
//	if(info2STR.item21_data[1]>0)   //����ʧ��
//	{
//		temp = info2STR.item21_data[1]&0x0f;
//		temp <<=4;
//		software.PRO_State |= temp; 
//	}
}
//�������е�LOGO
void Run_Logo(void)
{
 	unsigned int   X,Y;
 	u16  i,temp;
 	unsigned int   len,wide;
	
		i=100;
		while(i--);			
	X=290;
	Y=40;
//	base_add+=0x0841;
	base_add+=1;
	tft_cs_enable(3);
	for(len=0;len<20;len++)
	{
		for(wide=0;wide<20;wide++)
		{
//		LCD_SetCursor(len+X,wide+Y);           //���ù��λ��  
		i=10;
		while(i--);			
		LCD_WR_REG(0x2C);  			//��ʼд��GRAM 
		i=10;
		while(i--);
//		temp = 	(((len*len+)+wide*wide)|base_add);	
		temp = 	((len+base_add)*(len+base_add)+(wide+base_add)*(wide+base_add));	
		LCD_WE_RAM(temp); 
		i=10;
		while(i--);			
		}
	}
	tft_cs_disable(3);
	
}
