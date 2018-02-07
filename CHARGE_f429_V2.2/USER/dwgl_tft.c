//dwgl_tft.c
//for ssd1963 320*240
//20161101
#include "stm32f4xx.h"
#include "dwgl_tft.h"

struct  FatFileTable FatFile_AD;   //���
struct  FatFileTable FatFile_URL;   //���

unsigned char FatFileBuf[2][FatFileBuf_size];  //����ʹ�õ�
struct  debug_str DebugStr;
unsigned char DebugStr_str_data[str_data_size];  //����ʹ�õ�

	u32 g32_temp =0;
	u16 g16_temp =0;

/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/

void tft_debug_init(struct debug_str *str)
{
		unsigned   int i;
	
		str->state = INIT_OK;
		str->str_data = DebugStr_str_data;
		str->str_operation_point = 0;
		str->str_data_len = sizeof(DebugStr_str_data);
		for(i=0;i<str->str_data_len;i++)
		{
			DebugStr_str_data[i] = 0;
		}
}

void Str_addto_Str(struct debug_str *str_out,unsigned char *str_in)
{
	unsigned   int i,j,len;
	unsigned char *p;
	
		if(str_out->state!=INIT_OK)
		{
			tft_debug_init(str_out);
		}
		
		len =0;
		for(i=0;i<str_out->str_data_len;i++)   //����str�ĳ���
		{
			if(str_in[i]==0)
			{
					len = i;   //
					break;
			}
		}
		if (len==0)
		{
			return;
		}
		
		j= str_out->str_operation_point;			
		p = (unsigned char *)str_out->str_data;
		for(i=0;i<len;i++)   //
		{
			if(j-len+i>=str_out->str_data_len)
			{
				p[j-len+i+str_out->str_data_len] = str_in[i];		
			}
			else
			{
				p[j-len+i] = str_in[i];		
			}
		}
		str_out->str_operation_point -= len;
		if(str_out->str_operation_point>=str_out->str_data_len)
		{
			str_out->str_operation_point += str_out->str_data_len;
		}		
}
//u16 x_offset,u16 y_offset
//void tft_1bitdeep_array(u16 x,u16 y,u16 wide,u16 len,u8 *array)
void tft_1bitdeep_array(u16 x,u16 y,u16 x_offset,u16 y_offset,const u8 *array)
{
  u8 temp_char,k=0;//
  u32 i=0,n=0;//

	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
	LCD_WriteRAM_Prepare();                       //��ʼд��GRAM*6 	   	   
	n=(u32)(x_offset)*(y_offset);     
  while(n--)
		{
 			if((i%8)==0)
 			{
 			temp_char=array[i/8];
 			}
 			k= i%8;
       if ( (temp_char >> 7-k) & 0x01 == 0x01)
         {
           g16_temp=0x0000; // �ַ���ɫ
         }
         else
         {
           g16_temp=0xffff; // ������ɫ
         }
				 
			i++;
			LCD_WE_RAM(g16_temp);
		}               //��ʾ��������ɫ
	 
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
}

//		tft_16bitdeep_array(2,2,240,171,gImage1_240_171_16bitC);
void tft_16bitdeep_array(u16 x,u16 y,u16 x_offset,u16 y_offset,const u8 *array)
{
  u16 i=0;//wide
  u32 n=0;//�Ѿ���ʾ�ĵ���
//  u16 tmp_char=0;

	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
	LCD_WriteRAM_Prepare();                       //��ʼд��GRAM*6 	   	   
	n=(u32)(x_offset)*(y_offset);     
  while(n--)
		{
			g16_temp = array[i+1];	
 			g16_temp <<= 8;		
 			g16_temp += array[i];		
			i++;
			i++;
			LCD_WE_RAM(g16_temp);
		}               //��ʾ��������ɫ
	 
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
}

//��������ɨ����������
// typedef struct tagBITMAPFILEHEADER
// {
//     WORD bfType;//BM(1-2�ֽ�)
//     DWORD bfSize;//�ļ���С��Ϊ�ֽ�(3-6�ֽڵ�λ��ǰ)
//     WORD bfReserved1;//0(7-8??)
//     WORD bfReserved2;//0(9-10??)
//     DWORD bfOffBits;//ͼ������ƫ�Ƶ�ַ(11-14�ֽ�)
// }BITMAPFILEHEADER;

// typedef struct tagBITMAPINFOHEADER{
// DWORD biSize;//�ö��ֽ���(15-18??)
// LONG biWidth;//λͼ��(19-22??)
// LONG biHeight;//λͼ��(23-26??)
// WORD biPlanes;//???????,???1(27-28??)
// WORD biBitCount;//?????????,???1(??),(29-30??)
// //4(16?),8(256?)16(???)?24(???)??
// DWORD biCompression;//??????,???0(???),(31-34??)
// //1(BI_RLE8????)?2(BI_RLE4????)??
// DWORD biSizeImage;//?????(????????????4??????????),??????(35-38??)
// LONG biXPelsPerMeter;//???????,?????(39-42??)
// LONG biYPelsPerMeter;//???????,?????(43-46??)
// DWORD biClrUsed;//???????????????(47-50??)
// DWORD biClrImportant;//?????????????(51-54??)
// }BITMAPINFOHEADER;
//------------------------------------------------------------
void tft_1bitdeep_TXT (u16 x,u16 y, u8 *array,u16 PenColor, u16 BackColor,u8 cs)
{
  u8 temp_char,k;//
  u32 i=0,j=0;//
  u32 offset,x_offset,y_offset;  //
  u32 x_offset1;  //ʵ��Ҫ�Ĵ��ڳ���
  u32 x_wide;  //ͼƬʵ��ȡ��Ŀ�� ��Ϊ32�ı���
	offset = array[13];
	offset <<= 8;
	offset += array[12];
	offset <<= 8;
	offset += array[11];
	offset <<= 8;
	offset += array[10];
	
	x_offset = array[21];
	x_offset <<= 8;
	x_offset += array[20];
	x_offset <<= 8;
	x_offset += array[19];
	x_offset <<= 8;
	x_offset += array[18];
	
	if((x_offset%32)!=0)
		{
			x_wide = x_offset + 32;
			x_wide &= ~0x1f;			
		}
	else
	{
			x_wide = x_offset;
	}
	y_offset = array[25];
	y_offset <<= 8;
	y_offset += array[24];
	y_offset <<= 8;
	y_offset += array[23];
	y_offset <<= 8;
	y_offset += array[22];
	
if((array[0]!='B')||(array[1]!='M'))  //������Ϊ��Ч
	{
		x_offset = 0;
	}
	
	tft_cs_enable(cs);

if((x_offset>0)&&(x_offset<=240))
	{
		x_offset1 = x_offset;
		
		offset +=0;
//	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
	LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���					
	LCD_WriteRAM_Prepare();                       //��ʼд��GRAM*6 	   	   

		for(i=0;i<y_offset;i++)	
	{
	for(j=0;j<(x_offset1/8);j++)	
	{
		temp_char = 	array[offset+i*(x_wide/8)+j];

		for(k=0;k<8;k++)
		{
       if ( ((temp_char >> (7-k)) &0x01) == 0x01)
         {
           g16_temp=PenColor; // �ַ���ɫ
         }
         else
         {
           g16_temp=BackColor; // ������ɫ
         }
				 
			LCD_WE_RAM(g16_temp);
		}
	}
	}			 
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
	
	if(y+x_offset<240)   //����240���ز���Ҫ�屳��
		{			
			LCD_WindowMax (x,y+x_offset1, x+y_offset-1, 239);
			LCD_WriteRAM_Prepare();                       //��ʼд��GRAM*6 	   	   
				i= (240-y-x_offset1)*y_offset;
			while(i--)
			{
			LCD_WE_RAM(BackColor);
			}
//			LCD_WindowMax (0, 0, LCD_W, LCD_H);      	    //�ָ�����
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
		}
	}
else
if(x_offset>240)
	{
		x_offset1 = 240;
		i = array[2048];  //�ϴ���ʾ���ƽ�Ƶ�ַ
		i <<= 8;			
		i += array[2049];
		offset +=	i;		
		if((i+30)*8<x_offset)
		{
			i+=1;//ƽ���ٶ�
//			i+=2;//ƽ���ٶ�
		}
		else
		{
			i=0;
		}
		array[2048] = (i>>8)&0xff;  //�ϴ���ʾ���ƽ�Ƶ�ַ
		array[2049] = (i)&0xff;  //�ϴ���ʾ���ƽ�Ƶ�ַ
		
//	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
		LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���	
		LCD_WriteRAM_Prepare();                       //��ʼд��GRAM*6 	   	   

		for(i=0;i<y_offset;i++)	
		{
		for(j=0;j<30;j++)	
//		for(j=0;j<(x_offset1/8);j++)	
		{
			temp_char = 	array[offset+i*(x_wide/8)+j];

			for(k=0;k<8;k++)
			{
				 if ( ((temp_char >> (7-k)) &0x01) == 0x01)
					 {
						 g16_temp=PenColor; // �ַ���ɫ
					 }
					 else
					 {
						 g16_temp=BackColor; // ������ɫ
					 }
					 
				LCD_WE_RAM(g16_temp);
			}
		}
		}
				 
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
	}
	tft_cs_disable(cs);
	
}

//------------------------------------------------------------------
void display_SD_BMP (u16 x,u16 y,struct FatFileTable BMP,u8 cs)
{
  u8 temp_char,k;	//
  u32 i=0,j=0;		//
  u32 offset,x_offset,y_offset;  //
  u32 x_WideBety;  //ͼƬʵ�ʵ�һ�еĿ���ֽڡ�Ϊ32�ı���
  u32 pix;  //��ʾ�����ӽ���
	unsigned int pix_temp,spi_size;
	unsigned char *buffer;
	unsigned int buffersize;
	
	buffer = BMP.FileBuf;
	buffersize = BMP.FileBufSize;
			res_sd = f_open(&fsrc, (char*)BMP.FileSourcePath, FA_OPEN_EXISTING | FA_READ); 	 		
			if(res_sd != FR_OK)
			{			return ;		}		
			res_sd = f_read (&fsrc, buffer, 40, &fnum);
				
//		SPI_FLASH_BufferRead(buffer, addr, 64);
//		FLASH2_GPIOSPI_Read (addr, buffer, 64);
		if(buffer[28] ==0x01)   //1bit
		{
			offset = buffer[13];
			offset <<= 8;
			offset += buffer[12];
			offset <<= 8;
			offset += buffer[11];
			offset <<= 8;
			offset += buffer[10];
			
			x_offset = buffer[21];
			x_offset <<= 8;
			x_offset += buffer[20];
			x_offset <<= 8;
			x_offset += buffer[19];
			x_offset <<= 8;
			x_offset += buffer[18];
			
			if((x_offset%32)!=0)
				{
					x_WideBety = x_offset + 32;
					x_WideBety &= ~0x1f;			
				}
			else
			{
					x_WideBety = x_offset;
			}
			y_offset = buffer[25];
			y_offset <<= 8;
			y_offset += buffer[24];
			y_offset <<= 8;
			y_offset += buffer[23];
			y_offset <<= 8;
			y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}
			
		if((x_offset>0)&&(x_offset<=240))
			{
				x_offset = x_offset;				
				offset +=0;			
				res_sd = f_read (&fsrc, buffer, (offset-40), &fnum);
				tft_cs_enable(cs);
				
		//	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 	
	
			for(i=0;i<y_offset;i++)	
			{
			for(j=0;j<(x_offset/8);j++)	
			{
				pix = i*(x_WideBety/8)+j; 
				if(pix%2048==0)
				{
						if((y_offset*x_WideBety/8-pix)>2048)
						{
							spi_size= 2048;
						}
						else
						{
							spi_size= (y_offset*x_WideBety/8-pix);
						}
				res_sd = f_read (&fsrc, buffer, spi_size, &fnum);
//				SPI_FLASH_BufferRead(buffer, addr+offset+pix, spi_size);
//				FLASH2_GPIOSPI_Read (addr+offset+pix, buffer, spi_size);  //����������2K
				pix_temp = pix;
				}
				temp_char = 	buffer[pix-pix_temp];

				for(k=0;k<8;k++)
				{
					 if ( ((temp_char >> (7-k)) &0x01) == 0x01)
						 {
							 g16_temp=0x0000; // �ַ���ɫ
						 }
						 else
						 {
							 g16_temp=0xffff; // ������ɫ
						 }
						 
					LCD_WE_RAM(g16_temp);
				}
			}
			}			 
			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
			}

		}
		
	if(buffer[28] ==16)   //16bit
	{
		offset = buffer[13];
		offset <<= 8;
		offset += buffer[12];
		offset <<= 8;
		offset += buffer[11];
		offset <<= 8;
		offset += buffer[10];
		
		x_offset = buffer[21];
		x_offset <<= 8;
		x_offset += buffer[20];
		x_offset <<= 8;
		x_offset += buffer[19];
		x_offset <<= 8;
		x_offset += buffer[18];
		
		if((x_offset%2)!=0)  //�涨ɨ��һ�����ݱ���4�ֽڵı���������Ĳ�0
		{
			x_WideBety = x_offset + 2;
			x_WideBety &= ~0x01;			
		}
		else
		{
				x_WideBety = x_offset;
		}
		y_offset = buffer[25];
		y_offset <<= 8;
		y_offset += buffer[24];
		y_offset <<= 8;
		y_offset += buffer[23];
		y_offset <<= 8;
		y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}

		if((x_offset>0)&&(x_offset<=240))
		{
			res_sd = f_read (&fsrc, buffer, (offset-40), &fnum);
			tft_cs_enable(cs);
			
			x_offset = x_offset;
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 
				
			global_u16p = (u16*)buffer;  //����ȡһ��ȡ16λ
							
			for(i=0;i<y_offset;i++)	
			{
				for(j=0;j<(x_offset);j++)	
				{
					pix = i*(x_WideBety)+j;  //���ؼ�1
					if(pix%2048==0)
					{
						if(y_offset*x_WideBety-pix>2048)
						{
							spi_size= 4096;
						}
						else
						{
							spi_size= (y_offset*x_WideBety-pix)*2;
						}
						res_sd = f_read (&fsrc, buffer, spi_size, &fnum);
//						SPI_FLASH_BufferRead(buffer, addr+offset+(pix<<1), spi_size);
//						FLASH2_GPIOSPI_Read (addr+offset+(pix<<1), buffer, spi_size);  //����������
						pix_temp = pix;
					}
					
// 	 				g16_temp = buffer[((pix-pix_temp)<<1)+1];	
// 	 				g16_temp <<= 8;		
// 	 				g16_temp += buffer[((pix-pix_temp)<<1)];		
					
					g16_temp =global_u16p[pix-pix_temp];
					LCD_WE_RAM(g16_temp);
				}
			}			 

			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
		}
	}
	if(buffer[28] ==24)   //24bit
	{
		offset = buffer[13];
		offset <<= 8;
		offset += buffer[12];
		offset <<= 8;
		offset += buffer[11];
		offset <<= 8;
		offset += buffer[10];
		
		x_offset = buffer[21];
		x_offset <<= 8;
		x_offset += buffer[20];
		x_offset <<= 8;
		x_offset += buffer[19];
		x_offset <<= 8;
		x_offset += buffer[18];
		
		if(((x_offset*3)%4)!=0)  //�涨ɨ��һ�����ݱ���4�ֽڵı���������Ĳ�0
		{
			x_WideBety += ((x_offset*3)%4);			
		}
		else
		{
			x_WideBety = x_offset*3;
		}
		y_offset = buffer[25];
		y_offset <<= 8;
		y_offset += buffer[24];
		y_offset <<= 8;
		y_offset += buffer[23];
		y_offset <<= 8;
		y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}

		if((x_offset>0)&&(x_offset<=240))
		{
			res_sd = f_read (&fsrc, buffer, (offset-40), &fnum);
			tft_cs_enable(cs);
			
//			x_offset1 = x_offset;
//			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���					
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 
				
//			global_u16p = (u16*)buffer;  //����ȡһ��ȡ16λ
			spi_size = 	buffersize-(buffersize%x_WideBety);  //ÿ�ζ��ĳ���	
			if(spi_size>y_offset*(x_WideBety))
			{
				spi_size = y_offset*(x_WideBety);
			}
			for(i=0;i<y_offset;i++)	
			{
				for(j=0;j<(x_offset);j++)	
				{
					pix = i*(x_WideBety)+j*3;  //���ؼ�1
					if((pix%spi_size)==0)
					{
						res_sd = f_read (&fsrc, buffer, spi_size, &fnum);
//						SPI_FLASH_BufferRead(buffer, addr+offset+(pix), spi_size);
//						FLASH2_GPIOSPI_Read (addr+offset+(pix<<1), buffer, spi_size);  //����������
						pix_temp = pix;
					}
					
				g16_temp = (buffer[(pix-pix_temp)+2]>>3);
				g16_temp <<= 6;
				g16_temp += (buffer[(pix-pix_temp)+1]>>2);
				g16_temp <<= 5;
				g16_temp += (buffer[(pix-pix_temp)]>>3);

//				g16_temp = buffer[(pix-pix_temp)+1];	
//				g16_temp <<= 8;		
//				g16_temp += buffer[(pix-pix_temp)];		
					
					LCD_WE_RAM(g16_temp);
				}
			}			 

			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
		}
	}
						
	res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
	res_sd=f_close(&fnew);

}
//-------------------------------------
void display_flash_BMP (u16 x,u16 y,unsigned int addr,u8 cs)
{
  u8 temp_char,k;	//
  u32 i=0,j=0;		//
  u32 offset,x_offset,y_offset;  //
  u32 x_WideBety;  //ͼƬʵ�ʵ�һ�еĿ���ֽڡ�Ϊ32�ı���
  u32 pix;  //��ʾ�����ӽ���
	unsigned int pix_temp,spi_size;
	unsigned char *buffer;
	unsigned int buffersize;
	
	buffer = str_buffer;
	buffersize = sizeof(str_buffer);
		SPI_FLASH_BufferRead(buffer, addr, 64);
//		FLASH2_GPIOSPI_Read (addr, buffer, 64);
		if(buffer[28] ==0x01)   //1bit
		{
			offset = buffer[13];
			offset <<= 8;
			offset += buffer[12];
			offset <<= 8;
			offset += buffer[11];
			offset <<= 8;
			offset += buffer[10];
			
			x_offset = buffer[21];
			x_offset <<= 8;
			x_offset += buffer[20];
			x_offset <<= 8;
			x_offset += buffer[19];
			x_offset <<= 8;
			x_offset += buffer[18];
			
			if((x_offset%32)!=0)
				{
					x_WideBety = x_offset + 32;
					x_WideBety &= ~0x1f;			
				}
			else
			{
					x_WideBety = x_offset;
			}
			y_offset = buffer[25];
			y_offset <<= 8;
			y_offset += buffer[24];
			y_offset <<= 8;
			y_offset += buffer[23];
			y_offset <<= 8;
			y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}
			
		if((x_offset>0)&&(x_offset<=240))
			{
			tft_cs_enable(cs);
				
				x_offset = x_offset;				
				offset +=0;				
		//	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 	
	
			for(i=0;i<y_offset;i++)	
			{
			for(j=0;j<(x_offset/8);j++)	
			{
				pix = i*(x_WideBety/8)+j; 
				if(pix%2048==0)
				{
						if((y_offset*x_WideBety/8-pix)>2048)
						{
							spi_size= 2048;
						}
						else
						{
							spi_size= (y_offset*x_WideBety/8-pix);
						}
				SPI_FLASH_BufferRead(buffer, addr+offset+pix, spi_size);
//				FLASH2_GPIOSPI_Read (addr+offset+pix, buffer, spi_size);  //����������2K
				pix_temp = pix;
				}
				temp_char = 	buffer[pix-pix_temp];

				for(k=0;k<8;k++)
				{
					 if ( ((temp_char >> (7-k)) &0x01) == 0x01)
						 {
							 g16_temp=0x0000; // �ַ���ɫ
						 }
						 else
						 {
							 g16_temp=0xffff; // ������ɫ
						 }
						 
					LCD_WE_RAM(g16_temp);
				}
			}
			}			 
			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
			}

		}
		
	if(buffer[28] ==16)   //16bit
	{
		offset = buffer[13];
		offset <<= 8;
		offset += buffer[12];
		offset <<= 8;
		offset += buffer[11];
		offset <<= 8;
		offset += buffer[10];
		
		x_offset = buffer[21];
		x_offset <<= 8;
		x_offset += buffer[20];
		x_offset <<= 8;
		x_offset += buffer[19];
		x_offset <<= 8;
		x_offset += buffer[18];
		
		if((x_offset%2)!=0)  //�涨ɨ��һ�����ݱ���4�ֽڵı���������Ĳ�0
		{
			x_WideBety = x_offset + 2;
			x_WideBety &= ~0x01;			
		}
		else
		{
				x_WideBety = x_offset;
		}
		y_offset = buffer[25];
		y_offset <<= 8;
		y_offset += buffer[24];
		y_offset <<= 8;
		y_offset += buffer[23];
		y_offset <<= 8;
		y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}

		if((x_offset>0)&&(x_offset<=240))
		{
			tft_cs_enable(cs);
			
			x_offset = x_offset;
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 
				
			global_u16p = (u16*)buffer;  //����ȡһ��ȡ16λ
							
			for(i=0;i<y_offset;i++)	
			{
				for(j=0;j<(x_offset);j++)	
				{
					pix = i*(x_WideBety)+j;  //���ؼ�1
					if(pix%2048==0)
					{
						if(y_offset*x_WideBety-pix>2048)
						{
							spi_size= 4096;
						}
						else
						{
							spi_size= (y_offset*x_WideBety-pix)*2;
						}
						SPI_FLASH_BufferRead(buffer, addr+offset+(pix<<1), spi_size);
//						FLASH2_GPIOSPI_Read (addr+offset+(pix<<1), buffer, spi_size);  //����������
						pix_temp = pix;
					}
					
// 	 				g16_temp = buffer[((pix-pix_temp)<<1)+1];	
// 	 				g16_temp <<= 8;		
// 	 				g16_temp += buffer[((pix-pix_temp)<<1)];		
					
					g16_temp =global_u16p[pix-pix_temp];
					LCD_WE_RAM(g16_temp);
				}
			}			 

			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
		}
	}
	if(buffer[28] ==24)   //24bit
	{
		offset = buffer[13];
		offset <<= 8;
		offset += buffer[12];
		offset <<= 8;
		offset += buffer[11];
		offset <<= 8;
		offset += buffer[10];
		
		x_offset = buffer[21];
		x_offset <<= 8;
		x_offset += buffer[20];
		x_offset <<= 8;
		x_offset += buffer[19];
		x_offset <<= 8;
		x_offset += buffer[18];
		
		if(((x_offset*3)%4)!=0)  //�涨ɨ��һ�����ݱ���4�ֽڵı���������Ĳ�0
		{
			x_WideBety += ((x_offset*3)%4);			
		}
		else
		{
			x_WideBety = x_offset*3;
		}
		y_offset = buffer[25];
		y_offset <<= 8;
		y_offset += buffer[24];
		y_offset <<= 8;
		y_offset += buffer[23];
		y_offset <<= 8;
		y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}

		if((x_offset>0)&&(x_offset<=240))
		{
			tft_cs_enable(cs);
			
//			x_offset1 = x_offset;
//			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���					
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 
				
//			global_u16p = (u16*)buffer;  //����ȡһ��ȡ16λ
			spi_size = 	buffersize-(buffersize%x_WideBety);  //ÿ�ζ��ĳ���	
			if(spi_size>y_offset*(x_WideBety))
			{
				spi_size = y_offset*(x_WideBety);
			}
			for(i=0;i<y_offset;i++)	
			{
				for(j=0;j<(x_offset);j++)	
				{
					pix = i*(x_WideBety)+j*3;  //���ؼ�1
					if((pix%spi_size)==0)
					{
						SPI_FLASH_BufferRead(buffer, addr+offset+(pix), spi_size);
//						FLASH2_GPIOSPI_Read (addr+offset+(pix<<1), buffer, spi_size);  //����������
						pix_temp = pix;
					}
					
				g16_temp = (buffer[(pix-pix_temp)+2]>>3);
				g16_temp <<= 6;
				g16_temp += (buffer[(pix-pix_temp)+1]>>2);
				g16_temp <<= 5;
				g16_temp += (buffer[(pix-pix_temp)]>>3);

//				g16_temp = buffer[(pix-pix_temp)+1];	
//				g16_temp <<= 8;		
//				g16_temp += buffer[(pix-pix_temp)];		
					
					LCD_WE_RAM(g16_temp);
				}
			}			 

			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
		}
	}
}

//-------------------------------------------------------------------
void display_flash_BMPE (u16 x,u16 y,u8 area,u8 num,u8 cs)
{
	u16 i;
	u32 f_addr,f_size;
	unsigned char *buffer;
//	unsigned int buffersize;
	
	buffer = str_buffer;
//	buffersize = sizeof(str_buffer);	
	
	if(area==1)//��ά����
	{
		SPI_FLASH_BufferRead(buffer, Addr_01min, 256);
//		FLASH2_GPIOSPI_Read (Addr_01min, buffer, 256);
				f_addr = 0;
				f_size = 0;
	if(buffer[1]>13)
		{	
			buffer[1] = 13;	
		}
		
		for(i=0;i<buffer[1];i++)
		{
		if(buffer[0]!=frame_headerC)	
			{		break;	}			
				if(buffer[4+18*i+1]==num)//ȷ���ļ���
				{
				f_size = buffer[4+18*i+2];
				f_size <<= 8;
				f_size += buffer[4+18*i+3];
				f_size <<= 8;
				f_size += buffer[4+18*i+4];
				f_size <<= 8;
				f_size += buffer[4+18*i+5];	
					
				f_addr = buffer[4+18*i+14];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+15];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+16];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+17];
				break;
				}
		}
						
		if(f_size>0)
		{
				display_flash_BMP (x,y,f_addr,cs);
		}
	}
	else
	if(area==2)//������
	{
		SPI_FLASH_BufferRead(buffer, Addr_02min, 1024);
//		FLASH2_GPIOSPI_Read (Addr_02min, buffer, 1024);
				f_addr = 0;
				f_size = 0;
	if(buffer[1]>52)
		{	
			buffer[1] = 52;	
		}
		
		for(i=0;i<buffer[1];i++)
		{
		if(buffer[0]!=frame_headerC)	
			{		break;	}			
				if(buffer[4+18*i+1]==num)//ȷ���ļ���
				{
				f_size = buffer[4+18*i+2];
				f_size <<= 8;
				f_size += buffer[4+18*i+3];
				f_size <<= 8;
				f_size += buffer[4+18*i+4];
				f_size <<= 8;
				f_size += buffer[4+18*i+5];	
					
				f_addr = buffer[4+18*i+14];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+15];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+16];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+17];
				break;
				}
		}
				
		if(f_size>0)
		{
				display_flash_BMP (x,y,f_addr,cs);
		}

	}
	else
	if(area==3)//��ͼ��
	{
		SPI_FLASH_BufferRead(buffer, Addr_03min, 1024);
//		FLASH2_GPIOSPI_Read (Addr_03min, buffer, 1024);
				f_addr = 0;
				f_size = 0;
	if(buffer[1]>52)
		{	
			buffer[1] = 52;	
		}
		
		for(i=0;i<buffer[1];i++)
		{
		if(buffer[0]!=frame_headerC)	
			{		break;	}			
				if(buffer[4+18*i+1]==num)//ȷ���ļ���
				{
				f_size = buffer[4+18*i+2];
				f_size <<= 8;
				f_size += buffer[4+18*i+3];
				f_size <<= 8;
				f_size += buffer[4+18*i+4];
				f_size <<= 8;
				f_size += buffer[4+18*i+5];	
					
				f_addr = buffer[4+18*i+14];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+15];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+16];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+17];
				break;
				}
		}
				
		if(f_size>0)
		{
				display_flash_BMP (x,y,f_addr,cs);
		}
	}
	else
	if(area==4)//��ͼ��
	{
		SPI_FLASH_BufferRead(buffer, Addr_04min, 1024);
//		FLASH2_GPIOSPI_Read (Addr_04min, buffer, 1024);
				f_addr = 0;
				f_size = 0;
	if(buffer[1]>52)
		{	
			buffer[1] = 52;	
		}
		
		for(i=0;i<buffer[1];i++)
		{
		if(buffer[0]!=frame_headerC)	
			{		break;	}			
				if(buffer[4+18*i+1]==num)//ȷ���ļ���
				{
				f_size = buffer[4+18*i+2];
				f_size <<= 8;
				f_size += buffer[4+18*i+3];
				f_size <<= 8;
				f_size += buffer[4+18*i+4];
				f_size <<= 8;
				f_size += buffer[4+18*i+5];	
					
				f_addr = buffer[4+18*i+14];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+15];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+16];
				f_addr <<= 8;
				f_addr += buffer[4+18*i+17];
				break;
				}
		}				
		if(f_size>0)
		{
				display_flash_BMP (x,y,f_addr,cs);
		}
	}
}
void bmp_print_unit(u16 x,u16 y,struct FatFileTable BMP,u8 cs)//��ʾͼƬ��Ԫ��
{
	if(BMP.SW_state ==SW_ON)
	{
		if(BMP.f_area>5) //SD��
		{
			display_SD_BMP (x,y,BMP,cs);		
		}
		else
		{
			display_flash_BMPE (x,y,BMP.f_area,BMP.f_unm,cs);	
		}
	}	
}

//-------------------------------------------------------------------
void display_IC_BMP (u16 x,u16 y,const u8 *p,u8 cs)  
{
  u8 temp_char,k;//
  u32 i=0,j=0,n=0;//
  u32 offset,x_offset,y_offset;  //
  u32 x_wide;  //ͼƬʵ��ȡ��Ŀ�� ��Ϊ32�ı���
  u32 x_offset1;  //ʵ��Ҫ�Ĵ��ڳ���
  u32 pix;  //��ʾ�����ӽ���
	unsigned int pix_temp;
	unsigned char *buffer;
//	unsigned int buffersize;
	
	buffer = str_buffer;
//	buffersize = sizeof(str_buffer);
	
//		FLASH2_GPIOSPI_Read (addr, buffer, 64);
		for(n=0;n<64;n++)
		{
			buffer[n] = p[n];
		}
		if(buffer[28] ==0x01)   //1bit
		{
			offset = buffer[13];
			offset <<= 8;
			offset += buffer[12];
			offset <<= 8;
			offset += buffer[11];
			offset <<= 8;
			offset += buffer[10];
			
			x_offset = buffer[21];
			x_offset <<= 8;
			x_offset += buffer[20];
			x_offset <<= 8;
			x_offset += buffer[19];
			x_offset <<= 8;
			x_offset += buffer[18];
			
			if((x_offset%32)!=0)
				{
					x_wide = x_offset + 32;
					x_wide &= ~0x1f;			
				}
			else
			{
					x_wide = x_offset;
			}
			y_offset = buffer[25];
			y_offset <<= 8;
			y_offset += buffer[24];
			y_offset <<= 8;
			y_offset += buffer[23];
			y_offset <<= 8;
			y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}
			
		if((x_offset>0)&&(x_offset<=240))
			{
			tft_cs_enable(cs);
				
				x_offset1 = x_offset;				
				offset +=0;				
		//	LCD_WindowMax (x, y, x+x_offset-1, y+y_offset-1);       //���ô���	
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 	
	
			for(i=0;i<y_offset;i++)	
			{
			for(j=0;j<(x_offset1/8);j++)	
			{
				pix = i*(x_wide/8)+j; 
				if(pix%2048==0)
				{
//				FLASH2_GPIOSPI_Read (addr+offset+pix, buffer, 2048);  //����������2K
				for(n=0;n<2048;n++)
				{
					buffer[n] = p[offset+pix+n];
				}
				pix_temp = pix;
				}
				temp_char = 	buffer[pix-pix_temp];

				for(k=0;k<8;k++)
				{
					 if ( ((temp_char >> (7-k)) &0x01) == 0x01)
						 {
							 g16_temp=0x0000; // �ַ���ɫ
						 }
						 else
						 {
							 g16_temp=0xffff; // ������ɫ
						 }
						 
					LCD_WE_RAM(g16_temp);
				}
			}
			}			 
			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
			}

		}
		
	if(buffer[28] ==0x10)   //16bit
	{
		offset = buffer[13];
		offset <<= 8;
		offset += buffer[12];
		offset <<= 8;
		offset += buffer[11];
		offset <<= 8;
		offset += buffer[10];
		
		x_offset = buffer[21];
		x_offset <<= 8;
		x_offset += buffer[20];
		x_offset <<= 8;
		x_offset += buffer[19];
		x_offset <<= 8;
		x_offset += buffer[18];
		
		if((x_offset%2)!=0)  //�涨ɨ��һ�����ݱ���4�ֽڵı���������Ĳ�0
		{
			x_wide = x_offset + 2;
			x_wide &= ~0x01;			
		}
		else
		{
				x_wide = x_offset;
		}
		y_offset = buffer[25];
		y_offset <<= 8;
		y_offset += buffer[24];
		y_offset <<= 8;
		y_offset += buffer[23];
		y_offset <<= 8;
		y_offset += buffer[22];
			
		if((buffer[0]!='B')||(buffer[1]!='M'))  //������Ϊ��Ч
			{
				x_offset = 0;
			}
		if(y_offset>320)  //������Ϊ��Ч
			{
				x_offset = 0;
			}

		if((x_offset>0)&&(x_offset<=240))
		{
			tft_cs_enable(cs);
			
			x_offset1 = x_offset;
			LCD_WindowMax (x, y, x+y_offset-1, y+x_offset1-1);       //���ô���					
			LCD_WriteRAM_Prepare();  				//��ʼд��GRAM*6 
				
			global_u16p = (u16*)buffer;  //����ȡһ��ȡ16λ
							
			for(i=0;i<y_offset;i++)	
			{
				for(j=0;j<(x_offset);j++)	
				{
					pix = i*(x_wide)+j;  //���ؼ�1
					if(pix%2048==0)
					{
//					FLASH2_GPIOSPI_Read (addr+offset+(pix<<1), buffer, 4096);  //����������
				for(n=0;n<4096;n++)
				{
					buffer[n] = p[offset+(pix<<1)+n];
				}
					pix_temp = pix;
					}
					
	// 				g16_temp = buffer[pix+1-pix_temp];	
	// 				g16_temp <<= 8;		
	// 				g16_temp += buffer[pix-pix_temp];		
					
					g16_temp =global_u16p[pix-pix_temp];
					LCD_WE_RAM(g16_temp);
				}
			}			 

			LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
			tft_cs_disable(cs);				
		}
	}
}

void tft_Clear(u16 x,u16 y,u16 x_offset,u16 y_offset,u16 Color,u8 cs)
{
//   u16 temp_x=0;
//   u16 temp_y=0;
	tft_cs_enable(cs);
	LCD_Fill(x, y, x+x_offset-1, y+y_offset-1,  Color);
	tft_cs_disable(cs);
}

void tft_ShowHz(u16 x, u16 y, u8 *pcStr, u16 PenColor, u16 BackColor)
{	
  u16 temph,templ;
  u8 temp;
  u8 pos,t;
  u32 offset;
	
//  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		 
	
		 	LCD_WR_REG(0x0036);             //rotation
			LCD_WE_RAM(0x00E0);//dwgl				//����1
 //		 LCD_WindowMax(x,y,x+16-1,y+16-1);	        //���ô���  ����1
			LCD_WindowMax(x,y,x+16-1,y+16-1);	           //���ô���	dwgl	����1
	
	LCD_WriteRAM_Prepare();                                //��ʼд��GRAM   
	temph = *pcStr;	
	temph -= 0xb0; 
	templ = *(pcStr+1);
	templ -= 0xa1; 
	offset = temph*94*32+templ*32;
	for(pos=0;pos<32;pos+=2)
	{
		temp=HzLib[offset+pos+1];		                     //����1616����
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x01)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp>>=1; 
	  }
		
		temp=HzLib[offset+pos];		                     //����1608����			
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x01)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp>>=1; 
	  }
	}			
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
		 	LCD_WR_REG(0x0036);                                //rotation
			LCD_WE_RAM(0x0023);//dwgl				//����1

}

void tft_ShowHzE(u16 x, u16 y, u8 *pcStr, u16 PenColor, u16 BackColor)
{	
  u16 temph,templ;
  u8 temp;
  u8 pos,t;
  u32 offset;
	
	LCD_WindowMax(x,y,x+16-1,y+16-1);	           //���ô���	dwgl	����1
	
	LCD_WriteRAM_Prepare();                                //��ʼд��GRAM   
	temph = *pcStr;	
	temph -= 0xb0; 
	templ = *(pcStr+1);
	templ -= 0xa1; 
	offset = temph*94*32+templ*32;
	pos=32;
	while(pos)
//	for(pos=0;pos<32;pos+=2)
	{
		pos--; 
		pos--; 
		temp=HzLib[offset+pos];		                     //����1608����
			
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x80)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp<<=1; 
	  }
		
		temp=HzLib[offset+pos+1];		                     //����1616����
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x80)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp<<=1; 
	  }
	}			
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С

}
//--------------------------------------------------------------------------------------
void tft_ShowChar(unsigned int x, unsigned int y, unsigned char num,u16 PenColor, u16 BackColor)
{
  u8 temp;
  u32 offset;
  u8 pos,t;
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		 
  
		 	LCD_WR_REG(0x0036);                                //rotation
			LCD_WE_RAM(0x00E0);//dwgl				//����1
 //		 LCD_WindowMax(x,y,x+8-1,y+16-1);	        //���ô���  ����1
			LCD_WindowMax(x,y,x+16-1,y+8-1);	           //���ô���	dwgl	����1
	
	LCD_WriteRAM_Prepare();                                //��ʼд��GRAM   
	num=num-' ';                                            //�õ�ƫ�ƺ��ֵ 
	offset =num*16;                                            //�õ�ƫ�ƺ��ֵ 
	for(pos=0;pos<16;pos++)
	{
		temp=AsciiLib[offset+pos];		                     //����1608����
			
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x01)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp>>=1; 
	  }
	}			
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
		 	LCD_WR_REG(0x0036);                                //rotation
			LCD_WE_RAM(0x0023);//dwgl				//����1
}
void tft_ShowCharE(unsigned int x, unsigned int y, unsigned char num,u16 PenColor, u16 BackColor)
{
  u8 temp;
  u32 offset;
  u8 pos,t;
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		 
  
			LCD_WindowMax(x,y,x+16-1,y+8-1);	           //���ô���	dwgl	����1
	
	LCD_WriteRAM_Prepare();                                //��ʼд��GRAM   
	num=num-' ';                                            //�õ�ƫ�ƺ��ֵ 
	offset =num*16;                                            //�õ�ƫ�ƺ��ֵ 
	pos=16;
	while(pos)
//	for(pos=0;pos<16;pos++)
	{
		pos--;
		temp=AsciiLib[offset+pos];		                     //����1608����
			
		for(t=0;t<8;t++)
	  {                 
	    if(temp&0x80)			                                   //�ӵ�λ��ʼ
			 {
				 LCD_WE_RAM(PenColor);                             //��������ɫ һ����
			 }
			 else 
			 {
				 LCD_WE_RAM(BackColor);	                           //��������ɫ һ����      
			 }
			temp<<=1; 
	  }
	}			
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
}
//--------------------------------------------------------------------------------------
void tft_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor,u8 cs)
{
	tft_cs_enable(cs);
  while (*s) 
	{  	
		if(*s>0x80)                                            //��ʾ����
		{
			if(y+16<240)
			{
			  y += 16;
			}
			else
			{
			  x -= 16;
			  y = 16;
			}
			if((*s>=0XB0)&&(*s<=0XF7)&&(*(s+1)>=0XA1)&&(*(s+1)<=0XFE))  //GB2312���������� GBK/2:B0A1-F7FE����¼ GB 2312 ���� 6763 ������ԭ˳�����С�
			{
#if use_hzlib
//			tft_ShowHz(x, y, s, PenColor, BackColor);
			tft_ShowHzE(x, y-16, s, PenColor, BackColor);
#endif
				s += 2;
			}
			else
			{
				s += 1;
			}
		}
  	else                                                 //�Ǻ���
		{	
			if(y+8<240)
			{
			  y += 8;
			}
			else
			{
			  x -= 16;
			  y = 8;
			}
//			tft_ShowChar(x, y, *s++, PenColor, BackColor);
			tft_ShowCharE(x, y-8, *s++, PenColor, BackColor);
//			 s += 1;
		}
  }
	tft_cs_disable(cs);
}

//--------------------------------------------------------------------------------------
void tft_DisplayStr_debug(unsigned int x, unsigned int y, struct debug_str *str,u16 PenColor, u16 BackColor,u8 cs)
{
	unsigned char *s;
	unsigned int len;	
	s= str->str_data; 
	len =str->str_operation_point;
	
	if(LCDC.Mode[0] ==LCD_MODE_TEST&&LCDC.Mode[1] ==LCD_MODE_TEST)
	{
		tft_cs_enable(cs);
		LCD_Fill(x, y, x+16-1, y+240-1,BackColor);
		while (s[len]) 
		{
			if(s[len]>0x80)                                       //��ʾ����
			{
				if(y+16<240)
				{
					y -= 16;
				}
				else
				{
					x -= 16;
					y = 16;
				}
				
				if(x>=320)
				{
						break;
				}
				if((s[len]>=0XB0)&&(s[len]<=0XF7)&&(s[len+1]>=0XA1)&&(s[len+1]<=0XFE))  //GB2312���������� GBK/2:B0A1-F7FE����¼ GB 2312 ���� 6763 ������ԭ˳�����С�
				{
#if use_hzlib
	//			tft_ShowHz(x, y, s, PenColor, BackColor);
				tft_ShowHzE(x, y-16, &s[len], PenColor, BackColor);
#endif
					len += 2;
				}
				else
				{
					len += 1;
				}
					if(len>=str->str_data_len )
					{
						len -= str->str_data_len;
					}
			}
			else                                                 //�Ǻ���
			{	
				if(s[len]=='\n')      //����
				{
					x -= 16;
					y = 0;
					if(x>=320)
					{
							break;
					}
					else
					{
					LCD_Fill(x, y, x+16-1, y+240-1,BackColor);
					}
				}
				else if(y+8<240)
				{
					y += 8;
				}
				else
				{
					x -= 16;
					y = 8;
					if(x>=320)
					{
							break;
					}
					else
					{
						LCD_Fill(x, y-8, x+16-1, y-8+240-1,BackColor);
					}
				}
	//			tft_ShowChar(x, y, *s++, PenColor, BackColor);
				if(s[len]!='\n')      //no����
				{
					tft_ShowCharE(x, y-8, s[len], PenColor, BackColor);
				}
					len += 1;
					if(len>=str->str_data_len )
					{
						len -= str->str_data_len;
					}
			}
		}
		tft_cs_disable(cs);
	}
}

void tft_cs_enable(u8 cs)
{
	if((cs&0x01)==1)
	{
 	GPIO_ResetBits(LCD_CS1_PORT, LCD_CS1_PIN);
	}
	if((cs&0x02)==2)
	{
 	GPIO_ResetBits(LCD_CS2_PORT, LCD_CS2_PIN);
	}
}
void tft_cs_disable(u8 cs)
{
	unsigned char i;	
	i = 100;
	while(i--);
	if((cs&0x01)==1)
	{
 	GPIO_SetBits(LCD_CS1_PORT, LCD_CS1_PIN);
	}
	if((cs&0x02)==2)
	{
 	GPIO_SetBits(LCD_CS2_PORT, LCD_CS2_PIN);
	}
}
