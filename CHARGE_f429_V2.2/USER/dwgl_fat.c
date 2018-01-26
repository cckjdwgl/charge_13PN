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
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew,fsrc;								/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
DIR dirs;
//BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[1024] ={0};      /* 写缓冲区*/

void dwgl_FatInit(void)
{
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	
	res_sd = f_mount(&fs,"0:",1);
	
	if(FatFile_AD.SW_state ==SW_NULL)
	{
		FatFile_AD.SW_state = SW_ON;    //申请使用
		FatFile_AD.NetState = NET_FLASH_ON;
		FatFile_AD.FileBuf = (void *)FatFileBuf[0];		
		FatFile_AD.FileBufSize  = sizeof(FatFileBuf[0]);		
		FatFile_AD.FileBufLen  = 0;		
		FatFile_AD.FileBufSlave  = (void *)FatFileBuf[1];		
		FatFile_AD.FileBufSlaveSize  = sizeof(FatFileBuf[1]);			
		FatFile_AD.FileBufSlaveLen  = 0;			
	
		FatFile_AD.SW_state = SW_OFF; //关闭时申请使用
		
		FatFile_URL.SW_state = SW_ON;    //申请使用
		FatFile_URL.NetState = NET_FLASH_ON;
//		FatFile_URL.FileBuf = (void *)FatFileBuf[0];		
//		FatFile_URL.FileBufSize  = sizeof(FatFileBuf[0]);		
		FatFile_URL.FileBufLen  = 0;		
		FatFile_URL.FileBufSlave  = (void *)FatFileBuf[1];		
		FatFile_URL.FileBufSlaveSize  = sizeof(FatFileBuf[1]);			
		FatFile_URL.FileBufSlaveLen  = 0;			
	
		FatFile_URL.SW_state = SW_OFF; //关闭时申请使用
		
		
	}
/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
 		Str_addto_Str(&DebugStr,"SD CARD BEGAN......\r\n");	
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		time_ms=time_sys;
		while(time_sys-time_ms<1000);
   /* 格式化 */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			printf("》SD卡已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			printf("《《格式化失败。》》\r\n");
		}
	}
  else if(res_sd!=FR_OK)
  {
    printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
    printf("！！可能原因：SD卡初始化不成功。\r\n");
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
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
    /* 将指定存储区内容写入到文件内 */
		sprintf((char*)FatFile_AD.FileBuf, "SD CARD INIT OK. TIME:");
		FatFile_AD.FileBufLen = strlen((char*)FatFile_AD.FileBuf);
		res_sd=f_write(&fnew,FatFile_AD.FileBuf,FatFile_AD.FileBufLen,&fnum);
    if(res_sd==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",FatFile_AD.FileBuf);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_sd);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		printf("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
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
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", FatFile_AD.FileBufSlave);	
			
			sprintf((char*)LocalPoint, "fnum=%d;%s\r\n",fnum,(char*)FatFile_AD.FileBufSlave);
			Str_addto_Str(&DebugStr,LocalPoint);	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
			time_ms=time_sys;
			while(time_sys-time_ms<1000);
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_sd);
    }		
	}
	else
	{
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
//	f_mount(NULL,"0:",1);
  
  /* 操作完成，停机 */
}

