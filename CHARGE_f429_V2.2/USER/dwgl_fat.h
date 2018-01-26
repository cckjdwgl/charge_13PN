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
extern 	FATFS fs;													/* FatFs文件系统对象 */
extern 	FIL fnew,fsrc;													/* 文件对象 */
extern 	FRESULT res_sd;                /* 文件操作结果 */
extern 	UINT fnum;            					  /* 文件成功读写数量 */
extern 	DIR dirs;
//extern 	BYTE ReadBuffer[1024];        /* 读缓冲区 */
//extern 	BYTE WriteBuffer[1024];      /* 写缓冲区*/

extern void dwgl_FatInit(void);

