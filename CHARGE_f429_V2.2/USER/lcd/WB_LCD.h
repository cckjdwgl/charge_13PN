#ifndef __WB_LCD_H
#define __WB_LCD_H

#include "stm32f10x_conf.h"
#include "stdio.h"
#include "stdlib.h"

#define LCD_RAM    *(__IO uint16_t *) ((uint32_t)0x6C000002)    	//��ַ�Ĵ���
#define LCD_REG    *(__IO uint16_t *) ((uint32_t)0x6C000000)	 	  //ָ��Ĵ���


#define LCD_WR_REG(LCD_Reg)  LCD_REG=(LCD_Reg)              //д����


#define LCD_WE_RAM(RGB_Code) LCD_RAM=(RGB_Code)             //д����


#define LCD_W   800
#define LCD_H   480


#define   HDP	  799

#define   HT	  1000
#define   HPS	  51
#define   LPS	  3
#define   HPW	  8

#define   VDP	  479
#define   VT	  530
#define   VPS	  24
#define   FPS	  23
#define   VPW	  3

#define MAX_HZ_POSX    784
#define MAX_HZ_POSY    464

#define MAX_CHAR_POSX  792
#define MAX_CHAR_POSY  464

#define Horizontal     0x00
#define Vertical       0x01

/*Private define-------------------------*/
#define POINT_COLOR WHITE
#define BACK_COLOR	BLACK

/*--------16λ��ɫֵ---------------*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ



/*----------��������------------*/
/* ˢ�� */
void LCD_Clear(uint16_t Color);
u16  LCD_GetPoint(u16 x,u16 y);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t colour);
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_DrawHVLine(uint16_t xsta, uint16_t ysta, uint16_t Length,uint16_t Color,uint8_t Direction);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len, u16 PenColor, u16 BackColor);//��ʾ����
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic);     //��ʾͼƬ gImageȡģ bmp��ʽ

void LCD_IOConfig(void);
void LCD_Init(void);
void LCD_DisplayStr(unsigned int x, unsigned int y, unsigned char *s,u16 PenColor, u16 BackColor);
void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue);
#endif
