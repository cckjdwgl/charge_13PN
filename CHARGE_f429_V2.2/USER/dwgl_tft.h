//dwgl_tft.h

#ifndef __dwgl_tft_H
#define __dwgl_tft_H

//#include "dwgl_tft.h"
#include "stm32f4xx.h"


#define INIT_UNLL    0
#define INIT_OK    		1
#define INIT_ERR    	2
#define FatFileBuf_size     4100
#define str_data_size       2048

//#define tft_debug_x    (320-16*1)
#define tft_debug_x    (160-16*1)
#define tft_debug_y    (0)
//bmp显示用到的结构体
struct  FatFileTable {
	unsigned  char SW_state;  	//连接状态开关
	unsigned  char NetState;    //状态
	#if 1      //no FAT
	u8 f_area;
	u8 f_unm;
	u8 f_addr[4];   //文件起始地址。
	#endif
	unsigned char FileSourcePath[128];    //源路径 
	unsigned char FileDestPath[128];			//目的路径
	unsigned int FileHeadLen;       //文件头有效长度
	unsigned char FileHead[256];	  //文件头缓冲区
	unsigned int FileTolLen;        //总长
	unsigned int FileHandleLen;     //已经完成的长度
	unsigned int FileBufSize;       //缓冲大小
	unsigned int FileBufLen;  	    //缓冲区内有效长度
	unsigned char *FileBuf;							    //缓冲区
	
	unsigned int FileBufSlaveSize;    //备用缓冲大小
	unsigned int FileBufSlaveLen;  	  //备用缓冲区内有效长度
	unsigned char *FileBufSlave;							  //备用缓冲区
};	  

struct  debug_str {
	unsigned char state;
	unsigned int str_operation_point;  //已经读出的长度
	void *str_data;
	unsigned int str_data_len;
};	  

extern struct  FatFileTable FatFile_AD;   //广告
extern struct  FatFileTable FatFile_URL;   //下载
extern unsigned char FatFileBuf[4][FatFileBuf_size];  //调试使用的
extern struct  debug_str DebugStr;
extern unsigned char DebugStr_str_data[str_data_size];  //调试使用的
extern u32 g32_temp;
extern u16 g16_temp;

extern const unsigned char gImage10_16_288_1bitC[640];
extern const unsigned char gImage12_128_128_1bitC[2110];



//extern const unsigned char gImage6_200_24_1bitC[600];
extern void tft_debug_init(struct debug_str *str);
extern void Str_addto_Str(struct debug_str *str_out,unsigned char *str_in);	

extern void tft_Clear(u16 x,u16 y,u16 x_offset,u16 y_offset,u16 Color,u8 cs);
extern void tft_1bitdeep_array (u16 x,u16 y,u16 x_offset,u16 y_offset,const u8 *array);
extern void tft_16bitdeep_array (u16 x,u16 y,u16 x_offset,u16 y_offset,const u8 *array);

extern void display_flash_BMP (u16 x,u16 y,unsigned int addr,u8 cs);//从FLASH按地址调图片
extern void display_flash_BMPE (u16 x,u16 y,u8 area,u8 num,u8 cs);//从FLASH按协议调图片
extern void display_SD_BMP (u16 x,u16 y,struct FatFileTable BMP,u8 cs);//从SD调图片
extern void display_IC_BMP (u16 x,u16 y,const unsigned char *p,u8 cs);  //从单片机ROM调图片
extern void tft_1bitdeep_TXT (u16 x,u16 y, u8 *array,u16 PenColor, u16 BackColor,u8 cs);   //滚动输出文字

extern void bmp_print_unit(u16 x,u16 y,struct FatFileTable BMP,u8 cs);//显示图片单元。

extern void tft_ShowHz(u16 x, u16 y, u8 *pcStr, u16 PenColor, u16 BackColor);
extern void tft_ShowHzE(u16 x, u16 y, u8 *pcStr, u16 PenColor, u16 BackColor);
extern void tft_ShowChar(unsigned int x, unsigned int y, unsigned char num,u16 PenColor, u16 BackColor);
extern void tft_ShowCharE(unsigned int x, unsigned int y, unsigned char num,u16 PenColor, u16 BackColor);
// extern void tft_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor);
extern void tft_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor,u8 cs);
extern void tft_DisplayStr_debug(unsigned int x, unsigned int y, struct debug_str *str,u16 PenColor, u16 BackColor,u8 cs);
extern void tft_cs_enable(u8 cs);
extern void tft_cs_disable(u8 cs);

#endif
