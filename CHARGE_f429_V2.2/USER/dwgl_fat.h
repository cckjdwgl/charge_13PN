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

//fat32
extern 	FATFS fs;													/* FatFs�ļ�ϵͳ���� */
extern 	FIL fnew,fsrc;													/* �ļ����� */
extern 	FRESULT res_sd;                /* �ļ�������� */
extern 	UINT fnum;            					  /* �ļ��ɹ���д���� */
extern 	DIR dirs;
//extern 	BYTE ReadBuffer[1024];        /* �������� */
//extern 	BYTE WriteBuffer[1024];      /* д������*/

extern void dwgl_FatInit(void);

