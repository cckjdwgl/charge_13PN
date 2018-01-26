/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "sdio/bsp_sdio_sd.h"
#include "string.h"
#include "dwgl_fat.h"

//fat32
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew,fsrc;								/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
DIR dirs;
//BYTE ReadBuffer[1024]={0};        /* �������� */
//BYTE WriteBuffer[1024] ={0};      /* д������*/

void dwgl_FatInit(void)
{
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	
	res_sd = f_mount(&fs,"0:",1);
	
	if(FatFile_AD.SW_state ==SW_NULL)
	{
		FatFile_AD.SW_state = SW_ON;    //����ʹ��
		FatFile_AD.NetState = NET_FLASH_ON;
		FatFile_AD.FileBuf = (void *)FatFileBuf[0];		
		FatFile_AD.FileBufSize  = sizeof(FatFileBuf[0]);		
		FatFile_AD.FileBufLen  = 0;		
		FatFile_AD.FileBufSlave  = (void *)FatFileBuf[1];		
		FatFile_AD.FileBufSlaveSize  = sizeof(FatFileBuf[1]);			
		FatFile_AD.FileBufSlaveLen  = 0;			
	
		FatFile_AD.SW_state = SW_OFF; //�ر�ʱ����ʹ��
		
		FatFile_URL.SW_state = SW_ON;    //����ʹ��
		FatFile_URL.NetState = NET_FLASH_ON;
//		FatFile_URL.FileBuf = (void *)FatFileBuf[0];		
//		FatFile_URL.FileBufSize  = sizeof(FatFileBuf[0]);		
		FatFile_URL.FileBufLen  = 0;		
		FatFile_URL.FileBufSlave  = (void *)FatFileBuf[1];		
		FatFile_URL.FileBufSlaveSize  = sizeof(FatFileBuf[1]);			
		FatFile_URL.FileBufSlaveLen  = 0;			
	
		FatFile_URL.SW_state = SW_OFF; //�ر�ʱ����ʹ��
		
		
	}
/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_sd == FR_NO_FILESYSTEM)
	{
 		Str_addto_Str(&DebugStr,"SD CARD BEGAN......\r\n");	
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		time_ms=time_sys;
		while(time_sys-time_ms<1000);
   /* ��ʽ�� */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL,"0:",1);			
      /* ���¹���	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			printf("������ʽ��ʧ�ܡ�����\r\n");
		}
	}
  else if(res_sd!=FR_OK)
  {
    printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
    printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
//		int i;
//  DIR dirs;
//	u8 temp_fattrib[3];
//  char path[50]={""};
//  disk_initialize(0);
//	res_sd = f_opendir(&dirs, (const TCHAR*)FatFile_AD.FileSourcePath);
//	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
	
		sprintf((char*)FatFile_AD.FileSourcePath, "area1");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
		sprintf((char*)FatFile_AD.FileSourcePath, "area2");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
		sprintf((char*)FatFile_AD.FileSourcePath, "area3");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
		sprintf((char*)FatFile_AD.FileSourcePath, "area4");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
		sprintf((char*)FatFile_AD.FileSourcePath, "area5");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
		sprintf((char*)FatFile_AD.FileSourcePath, "logs");
	res_sd = f_mkdir((const TCHAR*)FatFile_AD.FileSourcePath);
	
	res_sd = f_open(&fnew, "dwgl_fat_info.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_sd == FR_OK )
	{
 		Str_addto_Str(&DebugStr,"f_open(&fnew, dwgl_fat_info.txt,FA_CREATE_ALWAYS | FA_WRITE )\r\n");	
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		time_ms=time_sys;
		while(time_sys-time_ms<1000);
    /* ��ָ���洢������д�뵽�ļ��� */
		sprintf((char*)FatFile_AD.FileBuf, "SD CARD INIT OK. TIME:");
		FatFile_AD.FileBufLen = strlen((char*)FatFile_AD.FileBuf);
		res_sd=f_write(&fnew,FatFile_AD.FileBuf,FatFile_AD.FileBufLen,&fnum);
    if(res_sd==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",FatFile_AD.FileBuf);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
	}
	else
	{	
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_sd = f_open(&fnew, "dwgl_fat_info.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
 		Str_addto_Str(&DebugStr,"f_open(&fnew, dwgl_fat_info.txt,FA_OPEN_EXISTING | FA_READ )\r\n");	
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		time_ms=time_sys;
		while(time_sys-time_ms<1000);
		
		res_sd = f_read(&fnew, FatFile_AD.FileBufSlave, FatFile_AD.FileBufSize, &fnum); 
    if(res_sd==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", FatFile_AD.FileBufSlave);	
			
			sprintf((char*)LocalPoint, "fnum=%d;%s\r\n",fnum,(char*)FatFile_AD.FileBufSlave);
			Str_addto_Str(&DebugStr,LocalPoint);	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
			time_ms=time_sys;
			while(time_sys-time_ms<1000);
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
    }		
	}
	else
	{
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"0:",1);
  
  /* ������ɣ�ͣ�� */
}

