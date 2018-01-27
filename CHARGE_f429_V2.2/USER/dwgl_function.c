/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dwgl_function.h"

//*******************************************
//u16 ×ª×Ö·û
//V1.1 20160401
//*******************************************
void u16toStr(u16 u_data,u8 *p)	 //u16 ×ª×Ö·û
{
	u16 temp,j;
	u8 i;
	temp = u_data;
	if(temp==0)
	{
		*p = '0';
		p++;
	}
	else
	{
		j=10000;
		for(i=0;i<5;i++)
		{
		if(temp/j)
			{break;}
		j =j/10;
		}
		for(;i<5;i++)
		{
		*p=(temp/j)+0X30;
		p++;
		temp = temp%j;	
		j =j/10;
		}
	}
	*p = '\0';
}
//---------------------------------------------------------------------------------
void Device_Rst(void)
{
	NVIC_SystemReset();
}
//
void make_device_mess(void)
{
	unsigned  char i;
//	unsigned  char number[8];
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
	sn0+=*(vu32*)(0x1FFF7A14);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
	sn0+=*(vu32*)(0x1FFF7A18);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
	sn0+=(sn0>>16);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
	
	sn0 = (sn0&0xffff);   //È¡ºÍµÄ×î16Î»
	sn0 += 100000;
	
	device.SW_state  = SW_ON; 	
//	sprintf((char*)device.Toolname,"CCKJSZ0001");
//	sprintf((char*)device.Toolname,"99110000001000");
	sprintf((char*)device.Toolname,"99110%d000",sn0);
	sprintf((char*)device.Version ,"N13.17.15");
	device.PortNum =DefPortNum;
	for(i=0;i<sizeof(device.PortId);i++)
	{
		device.PortId[i] = i;
	}
	device.province = 1;  //Ê¡·Ý
	device.ChargeTimerSet = 1000;   //³äµçÊ±¼ä¼ì²é
	device.ChargeTimer =0;
		
	for(i=0;i<DefPortNum;i++)	{
	device.PortPowerSetTime[i] = 0; 
	}
	for(i=0;i<DefPortNum;i++)	{
	device.PortPowerUseTime[i] = 0; 
	}
	for(i=0;i<DefPortNum;i++)	{
	device.PortPowerShowTime[i] = 0; 
	}
}
//---------------------------------------------------------------------------------
void cmd_Power_on(void)
{
}

//---------------------------------------------------------------------------------
void cmd_Power_off(void)
{
}
//------0x11---------------------------------------------------------------------------	
void cmd_File_Requst(void)
{	
	u16 i,EN,len;
	u32 f_size,f_temp;
	unsigned char *buffer;
//	unsigned int buffersize;
	
	buffer = str_buffer;
//	buffersize = sizeof(str_buffer);	
	
//		SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
		SPI_FLASH_BufferRead(buffer, Addr_01min, 256);
//		FLASH2_GPIOSPI_Read (Addr_01min, buffer, 256);
// 		file_wr = 1;	//ÎÄ¼þ²Ù×÷ÔÊÐí
// 		file_id = UART1_RXBUFFER[(UART1_RXBUFFE_HEAD+7)&UART1_RX_MAX];  //µ±Ç°Ð´µÄ

		if((buffer[0]==frame_headerC))
		{
			len = buffer[1];		
			len = len*18+6;
			file_addr = buffer[2];//ÏÂ´ÎÊ¼ÓÃÆðÊ¼Ò³ u16
			file_addr <<=8;
			file_addr += buffer[3];
			file_addr <<=8;
						
			f_temp  = FatFile_URL.FileTolLen;
			f_size = file_addr+f_temp;	
			
			if((f_size<Addr_01max)&&(buffer[1]<=13))
				{
				EN  = 0xff;
				if((f_size&0xfff)==0)
				{
				NextFileAddr = f_size;				
				}
				else
				{
				NextFileAddr = (f_size & 0xfffff000);				
				NextFileAddr += 0x1000;
				}
//void SPI_FLASH_SectorErase(u32 SectorAddr);
				SPI_FLASH_SectorErase(Addr_01min);
//				FLASH2_GPIOSPI_SER(Addr_01min);
				buffer[len-2+0] =	1;   //AREA = 0
				buffer[len-2+1] =	buffer[1];
				buffer[len-2+2] =	(FatFile_URL.FileTolLen>>24)&0XFF;  //ÎÄ¼þ´óÐ¡
				buffer[len-2+3] =	(FatFile_URL.FileTolLen>>16)&0XFF;
				buffer[len-2+4] =	(FatFile_URL.FileTolLen>>8)&0XFF;
				buffer[len-2+5] =	(FatFile_URL.FileTolLen>>0)&0XFF;
				buffer[len-2+6] = 4;//ÏÈÐ´¼ÙÎÄ¼þÃû¡£¹Ì¶¨¼Ó4ÔÚÎÄ¼þÃû×î¸ß×Ö½Ú¡£
	//		buffer[len-2+6]~buffer[len-2+13];  //ÎÄ¼þÃû
				buffer[len-2+14]	= (file_addr>>24);   //ÆðÊ¼µØÖ·
				buffer[len-2+15]	= (file_addr>>16)&0xff;
				buffer[len-2+16]	= (file_addr>>8)&0xff;
				buffer[len-2+17]	= (file_addr)&0xff;
					
				buffer[0]	= frame_headerC;
				buffer[1]	+= 1;
				buffer[2]	= (NextFileAddr>>16)&0xff;
				buffer[3]	= (NextFileAddr>>8)&0xff;
				len = buffer[1];		
				len = len*18+6;
				buffer[len-1]	= 0;
				for(i=1;i<(len-2);i++)
				{
					buffer[len-1] += buffer[i];
				}
				buffer[len-1]	= frame_last;			
//void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
				SPI_FLASH_BufferWrite(buffer, Addr_01min, len);					
//				FLASH2_GPIOSPI_Write(Addr_01min, buffer, len);	
				}
			else
				{
				EN  = 0x00;
				}
		}
		else
		{
		buffer[0]= 0x67;	
		buffer[1]= 0x00;	
		//ÎÄ¼þ´ÓÍ··ÅAddr_01min ÎÄ¼þÄÚÈÝ´ÓAddr_01min+0X1000	
		f_temp = Addr_01min + 0x1000;
		buffer[2]= ((f_temp>>16)&0xff);	
		buffer[3]= ((f_temp>>8)&0xff);	
		len = buffer[1];		
		len = len*18+6;
		file_addr = buffer[2];
		file_addr <<=8;
		file_addr += buffer[3];
		file_addr <<=8;
			
		f_temp  = FatFile_URL.FileTolLen;
		f_size = file_addr+f_temp;	
		
		if((f_size<Addr_01max)&&(buffer[1]<=13))
			{
			EN  = 0xff;
			if((f_size&0xfff)==0)
			{
			NextFileAddr = f_size;				
			}
			else
			{
			NextFileAddr = (f_size & 0xfffff000);				
			NextFileAddr += 0x1000;
			}
			SPI_FLASH_SectorErase(Addr_01min);
			
			buffer[len-2+0] =	1;   //AREA = 0
			buffer[len-2+1] =	buffer[1];
			buffer[len-2+2] =	(FatFile_URL.FileTolLen>>24)&0XFF;  //ÎÄ¼þ´óÐ¡
			buffer[len-2+3] =	(FatFile_URL.FileTolLen>>16)&0XFF;
			buffer[len-2+4] =	(FatFile_URL.FileTolLen>>8)&0XFF;
			buffer[len-2+5] =	(FatFile_URL.FileTolLen>>0)&0XFF;
			buffer[len-2+6] = 4;//ÏÈÐ´¼ÙÎÄ¼þÃû¡£¹Ì¶¨¼Ó4ÔÚÎÄ¼þÃû×î¸ß×Ö½Ú¡£
//		buffer[len-2+6]~buffer[len-2+13];  //ÎÄ¼þÃû
			buffer[len-2+14]	= (file_addr>>24);   //ÆðÊ¼µØÖ·
				i++;
			buffer[len-2+15]	= (file_addr>>16)&0xff;
				i++;
			buffer[len-2+16]	= (file_addr>>8)&0xff;
				i++;
			buffer[len-2+17]	= (file_addr)&0xff;
				
			buffer[0]	= frame_headerC;
			buffer[1]	+= 1;
			buffer[2]	= (NextFileAddr>>16)&0xff;
			buffer[3]	= (NextFileAddr>>8)&0xff;
			len = buffer[1];		
			len = len*18+6;
			buffer[len-1]	= 0;
			for(i=1;i<(len-2);i++)
				{
				buffer[len-1] += buffer[i];
				}
			buffer[len-1]	= frame_last;			
			SPI_FLASH_BufferWrite(buffer, Addr_01min, len);					
			}
		else
			{
			EN  = 0x00;
			}
		}

}

//-----0x12----------------------------------------------------------------------------
void cmd_File_Tx(void)
{
	u16 i;
	u16 EN;
//	u32 len;
	u8 *mp,*sp;
		EN = 0XFF;
		mp = (u8*)FatFile_URL.FileBuf;
		sp = (u8*)FatFile_URL.FileBufSlave;
		
//		len = file_addr/0x1000;
		if((file_addr/0x1000)<((file_addr+FatFile_URL.FileBufLen)/0x1000))
		{
//			len = (file_addr&(0xfff000))+0x1000;
			SPI_FLASH_SectorErase((file_addr&(0xfff000))+0x1000);  //
		}
//		if(len==266) //ÊÇ²»ÊÇÂúÒ³
		{
			SPI_FLASH_BufferWrite(FatFile_URL.FileBuf, file_addr, FatFile_URL.FileBufLen);					
//			SPI_FLASH_PageWrite(FatFile_URL.FileDataIn, file_addr, FatFile_URL.FileDataInLen);					
//			FLASH2_GPIOSPI_Write(file_addr, buffer, 256);
			
			SPI_FLASH_BufferRead(FatFile_URL.FileBufSlave, file_addr, FatFile_URL.FileBufLen);
//			FLASH2_GPIOSPI_Read(file_addr, &buffer[1000], 256);
			for(i=0;i<FatFile_URL.FileBufLen;i++)
			{
				if(mp[i]!=sp[i])
				{
					EN = 0; break;
				}
			}			
			file_addr +=FatFile_URL.FileBufLen;
			FatFile_URL.FileHandleLen += FatFile_URL.FileBufLen;
		}
		if(FatFile_URL.FileHandleLen==FatFile_URL.FileTolLen)
		{
			for(i=0;i<DefPortNum;i++)
			{
//				if((strcmp((char *)FatFile_URL.FileSourcePath,(char *)ChargeM_init.) == 0))
			
			}
		}
}

//----0x13-----------------------------------------------------------------------------
void cmd_File_Recall(void)
{
	if(FatFile_AD.SW_state == SW_OFF)  //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
	{
		FatFile_AD.SW_state = SW_ON;    //
		FatFile_AD.NetState = NET_FLASH_ON;
		LCDC.SPID[0]++;		
		if(LCDC.SPID[0]>=DefPortNum)
		{
			LCDC.SPID[0] = 0;
		}
		FatFile_AD.f_unm = LCDC.SPID[0]; 
		FatFile_AD.f_area = 1;
	//extern void bmp_print_unit(u16 x,u16 y,struct FatFileTable BMP,u8 cs);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£
		bmp_print_unit(176,(240-128)>>1,FatFile_AD,3);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
		FatFile_AD.SW_state = SW_OFF; //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
	}
}

//----0x14-----------------------------------------------------------------------------
void cmd_File_Erase(void)
{
}
//--------------------------------------------------------------------------------------
void Flash_Erase(unsigned int addr,unsigned int size)
{
	u32 temp;
	u32 len;
	temp = addr;
	len = addr+size;
//	temp &= 0xfff
	for(;temp<len;)
	{
		SPI_FLASH_SectorErase(temp);	
		temp += 4096;
//		SPI_FLASH_BulkErase();
	}
}
//---------------------------------------------------------------------------------
void cmd_Port_Info(void)   //PC»úÒªµÄÉÏµçÇé¿öÐÅÏ¢
{

}

void cmd_Device_Check(void)//ºË¶ÔÐÅÏ¢
{
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//¹ã¸æ°æ±¾¶ÁÈ¡ 0X33
void cmd_Get_Version(void)
{
}
//---------------------------------------------------------------------------------
void cmd_Device_num(void)
{
	
}

//---------------------------------------------------------------------------------
void port_Charge_State(u8 lcd_index)
{
}

//------------------------------------------------------

void DisplayADC_BL(unsigned int x, unsigned int y, u16 *s,u16 PenColor, u16 BackColor,u8 cs)
{
	u8 i,j;
	u32 temp;
	for(i=0;i<6;i++)
	{
		temp = *s++;
		j=0;
		temp *= 6600;//1000±¶·Å´ó
		temp /=4096;
		UART_BUFFER[j++] = temp%10000/1000+'0';
		UART_BUFFER[j++] = temp%1000/100+'0';
		UART_BUFFER[j++] = temp%100/10+'0';
		UART_BUFFER[j++] = temp%10+'0';
		UART_BUFFER[j++] = 0;
		tft_DisplayStr(x-i*20, y, UART_BUFFER,POINT_COLOR, BACK_COLOR,cs);
	}
}

//-------------------------------------

void get_ADC1_3_data(u16 * ADC_data)
{
	u8 i;
	for(i=0;i<ADC1_3_ENABLE_CHANNEL_NUM;i++)
	{
		if(i < 3) ADC_data[i] = ADC3_Pointer[ADC3_channel[i]*ADC_SAMPLING_TIMES+2];
		else if(i < 6) ADC_data[i] = ADC1_Pointer[ADC1_channel[i-3]*ADC_SAMPLING_TIMES+2];
		else if(i < 9) ADC_data[i] = ADC3_Pointer[ADC3_channel[i-3]*ADC_SAMPLING_TIMES+2];
		else if(i < 12) ADC_data[i] = ADC1_Pointer[ADC1_channel[i-6]*ADC_SAMPLING_TIMES+2];
		else ADC_data[i] = ADC3_Pointer[ADC3_channel[i-6]*ADC_SAMPLING_TIMES+2];
	}
}

void cmd_Get_ADC(void)
{
	u8 i;
	u16 ADC_data[ADC1_3_ENABLE_CHANNEL_NUM] = {0};

	get_ADC1_3_data(ADC_data);

	for(i=0;i<ADC1_3_ENABLE_CHANNEL_NUM;i++)
	{
		UART1_TXBUFFER[2*i] = (ADC_data[i]>>8);
		UART1_TXBUFFER[2*i+1] = (ADC_data[i]&0xff);
	}

	for(i=0;i<ADC1_3_ENABLE_CHANNEL_NUM;i++)
	{
		UART1_TXBUFFER[ADC1_3_ENABLE_CHANNEL_NUM*2+2*i] = (ADC_Base0[i]>>8);
		UART1_TXBUFFER[ADC1_3_ENABLE_CHANNEL_NUM*2+2*i+1] = (ADC_Base0[i]&0xff);
	}
	Debug_UART_Send_Data(UART1_TXBUFFER,ADC1_3_ENABLE_CHANNEL_NUM*4);

}
//-----------------------------------------
u8 Frame_check_cmd1(void)
{
	u8 temp;
	u16 i,len;
	len = UART1_RXBUFFER[(UART1_RXBUFFE_HEAD+1)&UART1_RX_MAX];
	len <<= 1;
	temp = 0;
	for(i=1;i<len-2;i++)
	{
		temp += UART1_RXBUFFER[(UART1_RXBUFFE_HEAD+i)&UART1_RX_MAX];
	}
	if(temp==UART1_RXBUFFER[(UART1_RXBUFFE_HEAD+i)&UART1_RX_MAX])
	{
		temp = 0xff;    //Ð£ÑéÍ¨¹ý
	}
	else
	{
		temp = 0x00;
	}
	return temp;
}


/**
 * @brief  Power on or Power off the specified USB port.
 * @param  usb_port: It can be 0, 1, 2, 3, 4 or 5 to select the USB port, use USB_ALL_INDEX will select all USB port.
 * @param  new_state: new state of the specified USB port.
 *   This parameter can be: USB_POWER_ON or USB_POWER_OFF.
 * @retval None
 */


/**
 * @brief  Turn on or Turn off the specified LED.
 * @param  led_index: It can be LED_INDEX, LED1_INDEX or LED2_INDEX to select LED, use LED_ALL_INDEX will select all LED.
 * @param  new_state: new state of the specified LED.
 *   This parameter can be: LED_TURN_ON, LED_TURN_OFF or LED_TURN_NEGATION.
 * @retval None
 */

void Get_ADC_BaseLine(void)
{
	u8 i;

//	for(i=0;i<ADC1_3_ENABLE_CHANNEL_NUM;i++)
//	{
//		if(i < 3) ADC_Base0[i] = ADC3_Pointer[ADC3_channel[i]*ADC_SAMPLING_TIMES+2];
//		else if(i < 6) ADC_Base0[i] = ADC1_Pointer[ADC1_channel[i-3]*ADC_SAMPLING_TIMES+2];
//		else if(i < 9) ADC_Base0[i] = ADC3_Pointer[ADC3_channel[i-3]*ADC_SAMPLING_TIMES+2];
//		else if(i < 12) ADC_Base0[i] = ADC1_Pointer[ADC1_channel[i-6]*ADC_SAMPLING_TIMES+2];
//		else ADC_Base0[i] = ADC3_Pointer[ADC3_channel[i-6]*ADC_SAMPLING_TIMES+2];
//	}
	
	for(i=0;i<ADC1_3_ENABLE_CHANNEL_NUM;i++)
	{
		ADC_Base0[i] = ADC_BUFFER[ADC_ANx_piont[i]];
	}
}

void LCD_TEST(void)
{
	//0xE0 set_pll ;Start the PLL. Before the start, the system was operated with the crystal oscillator or clock input 
	//0xE2  set_pll_mn;  Set the PLL 
	//0xE3  get_pll_mn;  Get the PLL settings 
	//0xE4  get_pll_status;  Get the current PLL status 	
	u8 en,i;
	u16 temp;
	u32 temp1,temp2;
	//	0x0036
	en = 0x00;
	GPIO_ResetBits(LCD_CS1_PORT, LCD_CS1_PIN);	
	LCD_REG= 0XB7; 
	i=10;
	while(i--);
	temp= LCD_RAM; 
	temp<<= 8; 
	temp+= LCD_RAM; 
	GPIO_SetBits(LCD_CS1_PORT, LCD_CS1_PIN);	

	i=10;
	while(i--);	
	GPIO_ResetBits(LCD_CS1_PORT, LCD_CS1_PIN);	
	LCD_REG= 0XE3; 
	i=100;
	while(i--);
	temp1= LCD_RAM; 
	temp1<<= 8; 
	temp1+= LCD_RAM; 
	temp1<<= 8; 
	temp1+= LCD_RAM; 
	GPIO_SetBits(LCD_CS1_PORT, LCD_CS1_PIN);	

	i=10;
	while(i--);	
	GPIO_ResetBits(LCD_CS1_PORT, LCD_CS1_PIN);	
	LCD_REG= 0XE7; 
	i=100;
	while(i--);
	temp2= LCD_RAM; 
	temp2<<= 8; 
	temp2+= LCD_RAM; 
	temp2<<= 8; 
	temp2+= LCD_RAM; 
	GPIO_SetBits(LCD_CS1_PORT, LCD_CS1_PIN);	
	if((temp!=VT)||(temp1!=0x230204)||(temp2!=0x0293e0))
	{
		en = 0xff;
	}

	if(en == 0xff)
	{
		LCD_Init();
	}

	en = 0x00;
	GPIO_ResetBits(LCD_CS2_PORT, LCD_CS2_PIN);	
	LCD_REG= 0XB7; 
	i=10;
	while(i--);
	temp= LCD_RAM; 
	temp<<= 8; 
	temp+= LCD_RAM; 
	GPIO_SetBits(LCD_CS2_PORT, LCD_CS2_PIN);

	i=10;
	while(i--);	
	GPIO_ResetBits(LCD_CS2_PORT, LCD_CS2_PIN);	
	LCD_REG= 0XE3; 
	i=100;
	while(i--);
	temp1= LCD_RAM; 
	temp1<<= 8; 
	temp1+= LCD_RAM; 
	temp1<<= 8; 
	temp1+= LCD_RAM; 
	GPIO_SetBits(LCD_CS2_PORT, LCD_CS2_PIN);	
	if((temp!=VT)||(temp1!=0x230204))
	{
		en = 0xff;
	}

	if(en == 0xff)
	{
		LCD_Init1();
	}

}
void Version_display(u16 x,u8 *p)
{
	UART_BUFFER[0] = 'V';
	UART_BUFFER[1] = 'e';
	UART_BUFFER[2] = 'r';
	UART_BUFFER[3] = 's';
	UART_BUFFER[4] = 'i';
	UART_BUFFER[5] = 'o';
	UART_BUFFER[6] = 'n';
	UART_BUFFER[7] = ':';
	UART_BUFFER[8] = p[0];
	UART_BUFFER[9] = p[1];
	UART_BUFFER[10] = p[2];
	UART_BUFFER[11] = p[3];
	UART_BUFFER[12] = p[4];
	UART_BUFFER[13] = p[5];
	UART_BUFFER[14] = p[6];
	UART_BUFFER[15] = p[7];
	UART_BUFFER[16] = p[8];
	UART_BUFFER[17] = 0;
	tft_DisplayStr(x, (240-8*17)/2, UART_BUFFER,POINT_COLOR, BLUE,3);
}



//	unsigned  int time_ms = 0;
err_t dwgl_ms_send(struct tcp_pcb *tpcb, void *arg,err_t err)
{
//  struct echoclient *es = NULL;
	u16 i;
  struct pbuf *ptr,*free_ptr;
  err_t wr_err = ERR_OK;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(LocalPoint[0]!=0)
	{
		for(i=0;i<2048;i++)
		{
			LocalPoint[i] = 0;
		}
	}	
//	if(tpcb->unsent->p->len >= tcp_sndbuf(tpcb))
	if(tpcb->snd_queuelen >=5)
	{
		return ERR_INPROGRESS;
	}
	for(i=0;i<60;i++)
		{
      sprintf((char*)LocalPoint, "You may not use this%s", LocalPoint);              
		}
      sprintf((char*)LocalPoint, "%s %d %d", LocalPoint,time_ms,send_times);              
      /* allocate pbuf */
      ptr = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)LocalPoint) , PBUF_POOL);
//			es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)LocalPoint) , PBUF_POOL);
        /* copy data to pbuf */
      pbuf_take(ptr, (char*)LocalPoint, strlen((char*)LocalPoint));
//static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
 
  while ((wr_err == ERR_OK) &&
         (ptr != NULL) && 
         (ptr->len <= tcp_sndbuf(tpcb)))
  {
    free_ptr =ptr;
    /* get pointer on pbuf from es structure */
//    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    
    if (wr_err == ERR_OK)
    { 
      /* continue with next pbuf in chain (if any) */
      ptr = ptr->next;
      
      if(ptr != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(ptr);
      }
      
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(free_ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
//     es->p_tx = ptr;
   }
   else
   {		 
     /* other problem ?? */
   }
	 
		tcp_output(tpcb);		//½«·¢ËÍ»º³å¶ÓÁÐÖÐµÄÊý¾ÝÁ¢¼´·¢ËÍ³öÈ¥
  }
		
		
  return wr_err;
}
//	unsigned  int time_ms = 0;
err_t dwgl_ms_send1(struct tcp_pcb *tpcb, void *arg,err_t err)
{
//  struct echoclient *es = NULL;
	u16 i;
  struct pbuf *ptr,*free_ptr;
  err_t wr_err = ERR_OK;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	
	if(LocalPoint[0]!=0)
	{
		for(i=0;i<2048;i++)
		{
			LocalPoint[i] = 0;
		}
	}	
//	if(tpcb->unsent->p->len >= tcp_sndbuf(tpcb))
	if(tpcb->snd_queuelen >=5)
	{
		return ERR_INPROGRESS;
	}
//	for(i=0;i<60;i++)
//		{
//      sprintf((char*)LocalPoint, "You may not use this%s", LocalPoint);              
//		}
//      sprintf((char*)LocalPoint, "%s %d %d", LocalPoint,time_ms,send_times);              
      sprintf((char*)LocalPoint, "%s %s", LocalPoint,(char *)arg);              
      /* allocate pbuf */
      ptr = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)LocalPoint) , PBUF_POOL);
//			es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)LocalPoint) , PBUF_POOL);
        /* copy data to pbuf */
      pbuf_take(ptr, (char*)LocalPoint, strlen((char*)LocalPoint));
//static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
 
  while ((wr_err == ERR_OK) &&
         (ptr != NULL) && 
         (ptr->len <= tcp_sndbuf(tpcb)))
  {
    free_ptr =ptr;
    /* get pointer on pbuf from es structure */
//    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    
    if (wr_err == ERR_OK)
    { 
      /* continue with next pbuf in chain (if any) */
      ptr = ptr->next;
      
      if(ptr != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(ptr);
      }
      
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(free_ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
//     es->p_tx = ptr;
   }
   else
   {		 
     /* other problem ?? */
   }
	 
		tcp_output(tpcb);		//½«·¢ËÍ»º³å¶ÓÁÐÖÐµÄÊý¾ÝÁ¢¼´·¢ËÍ³öÈ¥
  }
		
		
  return wr_err;
}
//	unsigned  int time_ms = 0;
err_t dwgl_hex_send(struct tcp_pcb *tpcb, void *arg,u16 len)
{
//  struct echoclient *es = NULL;
//	u16 i;
  struct pbuf *ptr,*free_ptr;
  err_t wr_err = ERR_OK;
    /* allocate structure es to maintain tcp connection informations */
	
	udp_demo_senddata(udpdebug_pcb,arg);
//	if(tpcb->unsent->p->len >= tcp_sndbuf(tpcb))
	if(tpcb->snd_queuelen >=5)
	{
		return ERR_INPROGRESS;
	}

      /* allocate pbuf */
      ptr = pbuf_alloc(PBUF_TRANSPORT, len , PBUF_POOL);
//			es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)LocalPoint) , PBUF_POOL);
        /* copy data to pbuf */
			if(ptr==NULL)
			{	return ERR_MEM;	}
			
      pbuf_take(ptr, (char*)arg, len);
//static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
 
  while ((wr_err == ERR_OK) &&
         (ptr != NULL) && 
         (ptr->len <= tcp_sndbuf(tpcb)))
  {
    free_ptr =ptr;
    /* get pointer on pbuf from es structure */
//    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    
    if (wr_err == ERR_OK)
    { 
      /* continue with next pbuf in chain (if any) */
      ptr = ptr->next;
      
      if(ptr != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(ptr);
      }
      
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(free_ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
//     es->p_tx = ptr;
   }
   else
   {		 
     /* other problem ?? */
   }
	 
		tcp_output(tpcb);		//½«·¢ËÍ»º³å¶ÓÁÐÖÐµÄÊý¾ÝÁ¢¼´·¢ËÍ³öÈ¥
  }
		
		
  return wr_err;
}
err_t tcpl_task(struct tcp_pcb *tpcb)
{
  err_t wr_err = ERR_OK;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	tcpl_cmd(tpcb);
	if(ChargeM_init.SW_state ==SW_ON)
	{		
		if((time_sys-ChargeM_init.LastRxTime>ChargeM_init.HeartTime* DefineHeartTry))  //ÐÄÌø¶ÏÁË
		{
			ChargeM_init.LastRxTime = time_sys;
			ChargeM_init.NetState = NET_OFF;
			ChargeM_init.SW_state = SW_OFF;
			HttpM_init.SW_state = SW_ON;			//¿ªÆôµÄHTTP
			HttpU_buffer[0].SW_state = SW_ON;			//¿ªÆôµÄSSIÈÎÎñ	
			HttpU_buffer[0].NetState = NET_ON;			//¿ªÆôµÄSSIÈÎÎñ	
			HttpM_init.HttpUrl->NetState = NET_OFF_DONE;

			Str_addto_Str(&DebugStr,(unsigned char*)"LostHeartTime\r\n");
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
			printf("LostHeartTime\r\n");
			res_sd = f_open(&fnew, "logs/log_socket.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
			res_sd = f_lseek(&fnew, fnew.fsize);
			sprintf((char*)FatFile_URL.FileBufSlave,"\r\ntime_sys:%d LostHeartTime\r\n",time_sys);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_close(&fnew);	
		}

		if((time_sys-ChargeM_init.LastTxTime >ChargeM_init.HeartTime))    
		{
			ChargeM_init.LastTxTime = time_sys;			
			if((ChargeM_init.NetState==NET_DNS_OK))  //DNS³É¹¦
			{
				if(tpcb==NULL)
				{
					soket_lwip.remoteip[0] = ChargeM_init.DnsIP[0];
					soket_lwip.remoteip[1] = ChargeM_init.DnsIP[1];
					soket_lwip.remoteip[2] = ChargeM_init.DnsIP[2];
					soket_lwip.remoteip[3] = ChargeM_init.DnsIP[3];
					soket_lwip.remote_port = ChargeM_init.Port;
					printf("SOKET connect:%d %d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
					sprintf((char*)LocalPoint, "SOKET connect:%d %d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
					Str_addto_Str(&DebugStr,LocalPoint);	
					tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					soket_pcb=tcp_echoclient_connect(&soket_lwip);
				}
				else
				if(tpcb->state ==ESTABLISHED)
				{
					make_soket_10000(tcp1_txbuf);
					dwgl_hex_send(tpcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));					
					sprintf((char*)LocalPoint, "SOKET send:%d ID=10000 IP=%d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
					Str_addto_Str(&DebugStr,LocalPoint);
					tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					printf("SOKET send:%d ID=10000\r\n",time_sys);
				}
				else
				{
					sprintf((char*)LocalPoint, "SOKET CLOSED:state=%d\r\n",tpcb->state);
					Str_addto_Str(&DebugStr,LocalPoint);
					tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					if(tpcb->state==9)
					{
	//					tpcb->state = 9;
						soket_pcb =NULL;
					}
					else
					{
						soket_pcb=tcp_echoclient_disconnect(tpcb);
					}
				}
			}
			else if((ChargeM_init.NetState==NET_SOCKET_OK))  //SOCKET³É¹¦
			{
				if(tpcb->state ==ESTABLISHED)
				{
					make_soket_10001(tcp1_txbuf);
					dwgl_hex_send(tpcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));
					sprintf((char*)LocalPoint, "SOKET send:%d ID=10001 IP=%d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
					Str_addto_Str(&DebugStr,LocalPoint);
					printf("SOKET send:%d ID=10001\r\n",time_sys);
				}
				else
				{
					sprintf((char*)LocalPoint, "SOKET CLOSED:state=%d\r\n",tpcb->state);
					Str_addto_Str(&DebugStr,LocalPoint);
					tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					if(tpcb->state==9)
					{
	//					tpcb->state = 9;
						soket_pcb =NULL;
					}
					else
					{
						soket_pcb=tcp_echoclient_disconnect(tpcb);
					}
				}
			}
			else if((ChargeM_init.NetState==NET_ON))
			{
				get_input_dns(ChargeM_init.DnsNameStr);
			}
		}
	}
	else if(ChargeM_init.SW_state ==SW_OFF)
	{
		ChargeM_init.SW_state =SW_NULL;
		Str_addto_Str(&DebugStr,"SOKET task CLOSED\r\n");
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		if(tpcb!=NULL)
		{
			soket_pcb = tcp_echoclient_disconnect(tpcb);
		}
	}
	
  return wr_err;
}
err_t tcp2_task(struct tcp_pcb *tpcb)
{
  err_t wr_err = ERR_OK;
//	unsigned  char i;
//	struct http_url *HttpU;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	tcp2_cmd(tpcb);
	http_url_select(&HttpM_init);
	if(HttpM_init.SW_state ==SW_ON )  //HTTP SW¿ªÆô
	{
		if((time_sys-HttpM_init.LastTxTime >HttpM_init.HeartTime)&&(HttpM_init.NetState==NET_DNS_OK))    //DNS³É¹¦
		{
			HttpM_init.LastTxTime = time_sys;						
			if(tpcb==NULL)
			{
				http_lwip.remoteip[0] = HttpM_init.DnsIP[0];
				http_lwip.remoteip[1] = HttpM_init.DnsIP[1];
				http_lwip.remoteip[2] = HttpM_init.DnsIP[2];
				http_lwip.remoteip[3] = HttpM_init.DnsIP[3];
				http_lwip.remote_port = HttpM_init.Port;
				printf("HTTP connect:%d %d.%d.%d.%d.%d\r\n",time_sys,http_lwip.remoteip[0],http_lwip.remoteip[1],http_lwip.remoteip[2],http_lwip.remoteip[3],http_lwip.remote_port);
				sprintf((char*)LocalPoint, "HTTP connect:%d %d.%d.%d.%d.%d\r\n",time_sys,http_lwip.remoteip[0],http_lwip.remoteip[1],http_lwip.remoteip[2],http_lwip.remoteip[3],http_lwip.remote_port);
				Str_addto_Str(&DebugStr,LocalPoint);	
//				Str_addto_Str(&DebugStr,"connect to HTTP\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				http_pcb=tcp_echoclient_connect(&http_lwip);
			}
			else if(HttpM_init.HttpUrl->NetState ==NET_OFF)
			{
				HttpM_init.HttpUrl->NetState =NET_OFF_DONE;
				Str_addto_Str(&DebugStr,"CLOSED HTTP task\r\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				printf("CLOSED HTTP task\r\n");
				if(tpcb!=NULL)
				{
					http_pcb=tcp_echoclient_disconnect(tpcb);
				}
			}
			else if((time_sys-HttpM_init.LastRxTime>HttpM_init.HeartTime* DefineHeartTry))
			{
				HttpM_init.LastRxTime = time_sys;
				sprintf((char*)LocalPoint, "HTTP time out:state=%d\r\n",tpcb->state);
				Str_addto_Str(&DebugStr,LocalPoint);					
//					Str_addto_Str(&DebugStr,"CLOSED HTTP\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				printf("HTTP time out:state=%d\r\n",tpcb->state);
				if(tpcb->state==9)
				{
//					tpcb->state = 9;
					http_pcb =NULL;
				}
				else
				{
				http_pcb=tcp_echoclient_disconnect(tpcb);
				}
			}
			else if((tpcb->state ==ESTABLISHED)&& (HttpM_init.HttpUrl->NetState==NET_ON))  //HTTPÃ»ÓÐÏìÓ¦ÖØ·¢
			{
//				make_http_post(tcp2_txbuf);
				HttpM_init.HttpUrl->sent(tcp2_txbuf); 
				dwgl_hex_send(tpcb, tcp2_txbuf,strlen((char*)tcp2_txbuf));			
				sprintf((char*)LocalPoint, "HTTP send:%d\r\n",time_sys);
				Str_addto_Str(&DebugStr,LocalPoint);
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				printf("HTTP send:%d:%s\r\n",time_sys,tcp2_txbuf);
			}
			else if(tpcb->state !=ESTABLISHED)
			{
				sprintf((char*)LocalPoint, "HTTP CLOSED:state=%d\r\n",tpcb->state);
				Str_addto_Str(&DebugStr,LocalPoint);					
//					Str_addto_Str(&DebugStr,"CLOSED HTTP\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				printf("HTTP CLOSED:state=%d\r\n",tpcb->state);
				if(tpcb->state==9)
				{
//					tpcb->state = 9;
					http_pcb =NULL;
				}
				else
				{
				http_pcb=tcp_echoclient_disconnect(tpcb);
				}
			}
		}
		else if(HttpM_init.HttpUrl->NetState ==NET_OFF)   //URL¼¶ÉêÇë¹Ø±Õ
		{
			HttpM_init.HttpUrl->NetState =NET_OFF_DONE;
			Str_addto_Str(&DebugStr,"HTTP task CLOSED\r\n");
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
			printf("HTTP task CLOSED\r\n");
			if(tpcb!=NULL)
			{
				http_pcb=tcp_echoclient_disconnect(tpcb);
			}
		}
	}
	else if(HttpM_init.SW_state ==SW_OFF)
	{
		HttpM_init.SW_state =SW_NULL;
		Str_addto_Str(&DebugStr,"HTTP task CLOSED\r\n");
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		printf("HTTP task CLOSED\r\n");
		if(tpcb!=NULL)
		{
			http_pcb=tcp_echoclient_disconnect(tpcb);
		}
	}
  return wr_err;
}
err_t udp1_task(struct udp_pcb *upcb)
{
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	udp1_cmd(upcb); 
	if(	UdpdebugM_init.SW_state  == SW_ON)
	{
		if(time_sys-UdpdebugM_init.LastTxTime >UdpdebugM_init.HeartTime )
		{
			UdpdebugM_init.LastTxTime = time_sys;			
			if(UdpdebugM_init.NetState == NET_ON )
			{
				sprintf((char*)LocalPoint, "who are you?");
				udp_demo_senddata(upcb,LocalPoint);
				
				if(time_sys-UdpdebugM_init.LastRxTime>UdpdebugM_init.HeartTime*DefineHeartTry)
				{
					sprintf((char*)LocalPoint, "bye bey");
					udp_demo_senddata(upcb,LocalPoint);
					UdpdebugM_init.NetState = NET_NULL;
					
					udpdebug_lwip.remoteip[0]=0;	
					udpdebug_lwip.remoteip[1]=0;
					udpdebug_lwip.remoteip[2]=0;
					udpdebug_lwip.remoteip[3]=0;	
					udpdebug_lwip.remote_port = 32768;
					
					udp_demo_connection_close(udpdebug_pcb); //ÏÈ¹Ø±Õ
//					upcb->remote_ip.addr = IP_ADDR_ANY->addr; 				//¼ÇÂ¼Ô¶³ÌÖ÷»úµÄIPµØÖ·
//						upcb->remote_port=port;  			//¼ÇÂ¼Ô¶³ÌÖ÷»úµÄ¶Ë¿ÚºÅ
				}
			}	
			else if(UdpdebugM_init.NetState == NET_UDP_OK )
			{
					sprintf((char*)LocalPoint, "udpdebug ok");
					udp_demo_senddata(upcb,LocalPoint);
			}
			else if(UdpdebugM_init.NetState == NET_NULL )
			{
				if(udpdebug_pcb == NULL)
				{
					udp_demo_test();//´ò¿ª
				}
				else if(!(udpdebug_pcb->flags)&UDP_FLAGS_CONNECTED)
				{
					udp_demo_connection_close(udpdebug_pcb); //ÏÈ¹Ø±Õ
					udp_demo_test();//ÔÙÖØÐÂ´ò¿ª
				}
			}
			
		}
			
	}		
	return ERR_OK;
}
//---------------------------------------------------------------- 
err_t tcpl_cmd(struct tcp_pcb *tpcb)
{
  err_t wr_err = ERR_OK;	
	unsigned  char *cmd_bufer;
	u16 mess_id;
	u16 i;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	cmd_bufer = tcp1_rxbuf;
	mess_id =cmd_bufer[0]*256;
	mess_id +=cmd_bufer[1];	
	if(mess_id==0)
	{
		return wr_err;
	}
	sprintf((char*)LocalPoint, "SOKET cmd:%d ID=%d\n",time_sys,mess_id);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
	udp_demo_senddata(udpdebug_pcb,LocalPoint);
	res_sd = f_open(&fnew, "logs/log_socket.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d ID=%d\r\n",time_sys,mess_id);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_close(&fnew);	
	switch (mess_id){
		case  10000: 			
					if(cmd_bufer[24]=='0')
					{
						ChargeM_init.NetState = NET_SOCKET_OK;
						ChargeM_init.LastRxTime = time_sys;
					}
//					else
//					{
//						ChargeM_init.NetState = NET_OFF;;
//					}
						break;
		case  10001: 
						ChargeM_init.LastRxTime = time_sys;
						break;
		case  10003:      //¶Ë¿Ú²éÑ¯Í¨Öª
					make_soket_rxok(tcp1_txbuf);
					dwgl_hex_send(soket_pcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));
					make_soket_10004(tcp1_txbuf,cmd_bufer);
					dwgl_hex_send(soket_pcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));
					break;
		case  10004:   		//¶Ë¿Ú²éÑ¯½á¹û   ÖÕ¶Ë·¢Æð
			
						break;
		case  10005: 	 		//¶Ë¿ÚÉÏµçÍ¨Öª			
					make_soket_rxok(tcp1_txbuf);
					dwgl_hex_send(soket_pcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));
					cmd_soket_10005(cmd_bufer);
					make_soket_10006(tcp1_txbuf,cmd_bufer);
					dwgl_hex_send(soket_pcb, tcp1_txbuf,(tcp1_txbuf[2]*245+tcp1_txbuf[3]+41));
						break;
		case  10006: 			//ÉÏµç½á¹û   ÖÕ¶Ë·¢Æð
			
						break;
		case  10009: 			//¹ã¸æÍ³¼ÇÉÏ±¨ÃüÁî    ÖÕ¶Ë·¢Æð
			
						break;
		case  10010: 			//´íÎóÉÏ±¨  ÖÕ¶Ë·¢Æð
			
						break;
		case  10011: 			//³äµçµçÁ÷½á¹û  ÖÕ¶Ë·¢Æð
			
						break;
		
		 default:
						break;
	}			
	cmd_bufer[0] =0;
	cmd_bufer[1] =0;
  return wr_err;	
}

//---------------------------------------------------------------- 
err_t tcp2_cmd(struct tcp_pcb *tpcb)
{
  err_t wr_err = ERR_OK;
	unsigned  char *cmd_bufer;
//	u16 i,j;
	u16 u16temp;
	struct http_url *HttpU;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;	
	if(HttpM_init.RxLength==0)
	{
		return wr_err;
	}
	cmd_bufer = tcp2_rxbuf;
	
	sprintf((char*)LocalPoint, "HTTP cmd:%d\n",time_sys);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
	udp_demo_senddata(udpdebug_pcb,tcp2_rxbuf);

	if((HttpM_init.HttpUrl->type[0])=='G')
	{
	res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"\r\ntime_sys:%d\r\n",time_sys);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);		
	sprintf((char*)FatFile_URL.FileBufSlave,"URL:%s\r\nReceive: %d B",HttpM_init.HttpUrl->url,HttpM_init.RxLength);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_close(&fnew);	
	}
	else
	{
	res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"\r\ntime_sys:%d\r\n",time_sys);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_write(&fnew,cmd_bufer,strlen((char*)cmd_bufer),&fnum);
	res_sd=f_close(&fnew);	
	}
	
	HttpM_init.LastRxTime = time_sys;   //µ±Ç°TCP½ÓÊÕ
	HttpM_init.HttpUrl->LastRxTime = time_sys; //µ±Ç°URL½ÓÊÕ
	
	HttpU = (struct http_url*)HttpM_init.HttpUrl;
	if(strcmp("SSI", (char*)HttpU->type) == 0)
	{	
//		res_sd = f_mount(&fs,"0:",1);		
//		res_sd = f_opendir (&dirs, "area1");
		res_sd = f_open(&fnew, "area1/area1_info.txt", FA_CREATE_ALWAYS | FA_WRITE); 	 		
    /* ½«Ö¸¶¨´æ´¢ÇøÄÚÈÝÐ´Èëµ½ÎÄ¼þÄÚ */
		sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
		res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
		res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
    res_sd=f_close(&fnew);
//		res_sd=f_closedir (&dirs);
//	res_sd=f_unlink (const TCHAR* path);  //É¾³ý
		
		make_area1_get(cmd_bufer);				
		HttpM_init.HttpUrl->SW_state = SW_OFF;		//µ±Ç°µÄSSIÈÎÎñÍê³É¹ÒÆð						
		HttpM_init.HttpUrl->NetState=NET_OFF;			//µ±Ç°µÄSSIÈÎÎñÍê³É		
		HttpU_buffer[1].SW_state = SW_ON;		//µ±Ç°µÄÈÎÎñ¿ªÆô		
		HttpU_buffer[2].SW_state = SW_ON;		//µ±Ç°µÄÈÎÎñ¿ªÆô		
		HttpU_buffer[3].SW_state = SW_ON;		//µ±Ç°µÄÈÎÎñ¿ªÆô		
		HttpU_buffer[4].SW_state = SW_ON;		//µ±Ç°µÄÈÎÎñ¿ªÆô				
	}
	if(strcmp("S20", (char*)HttpU->type) == 0)
	{
			if(HttpM_init.HttpUrl->NetState == NET_ON)
			{
			HttpM_init.HttpUrl->NetState= NET_HTTP_OK;
			res_sd = f_open(&fnew, "area2/area2_info.txt", FA_CREATE_ALWAYS | FA_WRITE); 	 		
			sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);			
			}
			else if(HttpM_init.HttpUrl->NetState == NET_HTTP_OK)
			{
			res_sd = f_open(&fnew, "area2/area2_info.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
			res_sd = f_lseek(&fnew, fnew.fsize);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);			
			}
			if(HttpM_init.RxLength<1420)
			{
			wr_err = make_area2_get(cmd_bufer);		
			res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
			res_sd = f_lseek(&fnew, fnew.fsize);
			sprintf((char*)FatFile_URL.FileBufSlave,"make_area2_get:%d\r\n",wr_err);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_close(&fnew);			
			HttpM_init.HttpUrl->SW_state = SW_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹ÒÆð		
			HttpM_init.HttpUrl->NetState = NET_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹Ø±ÕTCP	
			}							
	}
	if(strcmp("S40", (char*)HttpU->type) == 0)
	{
			if(HttpM_init.HttpUrl->NetState == NET_ON)
			{
			HttpM_init.HttpUrl->NetState= NET_HTTP_OK;
			res_sd = f_open(&fnew, "area4/area4_info0.txt", FA_CREATE_ALWAYS | FA_WRITE); 	 		
			sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);			
			}
			else if(HttpM_init.HttpUrl->NetState == NET_HTTP_OK)
			{
			res_sd = f_open(&fnew, "area4/area4_info0.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
			res_sd = f_lseek(&fnew, fnew.fsize);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);			
			}
			if(HttpM_init.RxLength<1420)
			{
//			wr_err = make_area4_get(cmd_bufer);		
//			res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
//			res_sd = f_lseek(&fnew, fnew.fsize);
//			sprintf((char*)FatFile_URL.FileBufSlave,"area4/area4_get:%d\r\n",wr_err);
//			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
//			res_sd=f_close(&fnew);			
			HttpM_init.HttpUrl->SW_state = SW_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹ÒÆð		
			HttpM_init.HttpUrl->NetState = NET_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹Ø±ÕTCP	
			}							
	}
	if(strcmp("S41", (char*)HttpU->type) == 0)
	{
			HttpM_init.HttpUrl->SW_state = SW_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹ÒÆð		
			HttpM_init.HttpUrl->NetState = NET_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹Ø±ÕTCP	
			res_sd = f_open(&fnew, "area4/area4_info1.txt", FA_CREATE_ALWAYS | FA_WRITE); 	 		
			sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);
	}
	if(strcmp("S50", (char*)HttpU->type) == 0)
	{
			HttpM_init.HttpUrl->SW_state = SW_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹ÒÆð		
			HttpM_init.HttpUrl->NetState = NET_OFF;		//µ±Ç°µÄÈÎÎñÍê³É¹Ø±ÕTCP	
			res_sd = f_open(&fnew, "area5/area5_info.txt", FA_CREATE_ALWAYS | FA_WRITE); 	 		
			sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
			res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);
	}
	
//	if(strcmp("G10", (char*)HttpU->type) == 0)
	if(HttpU->type[0] == 'G')
	{
		if(HttpM_init.HttpUrl->NetState == NET_ON)
		{
			cut_between_strs(cmd_bufer,(unsigned  char*)"Content-Length: " ,(unsigned  char*)"\r\n", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
			if(strlen((char*)FatFile_URL.FileHead))     //ÓÐÊÕµ½³¤¶È
			{
				HttpM_init.HttpUrl->TotLength = atoi((char*)FatFile_URL.FileHead);
				HttpM_init.HttpUrl->NetState= NET_HTTP_OK;
				HttpM_init.HttpUrl->PresentLength = 0;						
				
				FatFile_URL.SW_state = SW_ON;
				FatFile_URL.NetState = NET_ON;
				FatFile_URL.FileTolLen = HttpM_init.HttpUrl->TotLength;					
				FatFile_URL.FileBufLen  = 0;
				sprintf((char*)FatFile_URL.FileSourcePath,"%s",(char*)HttpM_init.HttpUrl->url);
				cmd_File_Requst();
				
				res_sd = f_open(&fnew, (char*)HttpM_init.HttpUrl->DestPath, FA_CREATE_ALWAYS | FA_WRITE); 	
				res_sd=f_close(&fnew);
				
				res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
				res_sd = f_lseek(&fnew, fnew.fsize);
				sprintf((char*)FatFile_URL.FileBufSlave,"\r\ntime_sys:%d\r\n",time_sys);
				res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
				res_sd=f_write(&fnew,cmd_bufer,strlen((char*)cmd_bufer),&fnum);
				res_sd=f_close(&fnew);	
			}			
		}
		else if(HttpM_init.HttpUrl->NetState == NET_HTTP_OK)
		{
			FatFile_URL.FileBufLen = HttpM_init.RxLength;
			FatFile_URL.FileBuf = cmd_bufer;
			cmd_File_Tx();
		
			res_sd = f_open(&fnew, (char*)HttpM_init.HttpUrl->DestPath, FA_OPEN_ALWAYS | FA_WRITE); 	
			res_sd = f_lseek(&fnew, fnew.fsize);
			res_sd=f_write(&fnew,cmd_bufer,HttpM_init.RxLength,&fnum);
			res_sd=f_close(&fnew);

			HttpM_init.HttpUrl->PresentLength += HttpM_init.RxLength; 
			if(HttpM_init.HttpUrl->PresentLength>=HttpM_init.HttpUrl->TotLength)
			{
				HttpM_init.HttpUrl->SW_state = SW_NULL;
				HttpM_init.HttpUrl->NetState = NET_OFF;
				HttpM_init.HttpUrl->PresentLength = 0;
				if(strcmp("G10", (char*)HttpU->type) == 0)
				{
						done_area1_get(cmd_bufer);		
				}
				if(strcmp("G20", (char*)HttpU->type) == 0)
				{
						done_area2_get(cmd_bufer);		
				}
				if(strcmp("G30", (char*)HttpU->type) == 0)
				{
					
				}
				if(strcmp("G40", (char*)HttpU->type) == 0)
				{
					
				}
				if(strcmp("G50", (char*)HttpU->type) == 0)
				{
					
				}									
			}
		}		
	}
	HttpM_init.RxLength = 0;
  return wr_err;	
}

extern err_t udp1_cmd(struct udp_pcb *upcb)
{
  err_t wr_err = ERR_OK;
	u16 i,j;
	unsigned  char *cmd_bufer;
	unsigned  char *key_point;
	unsigned  char port_n =0;
	unsigned  char number[8];
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(UdpdebugM_init.RxLength ==0)
	{
		return wr_err;
	}
	cmd_bufer = udp1_rxbuf;
	UdpdebugM_init.LastRxTime = time_sys;
	
	if(UdpdebugM_init.NetState == NET_ON)
	{
		if(cmd_bufer[0] =='W'&&cmd_bufer[1] =='A'&&cmd_bufer[2] =='Y')   //who are you
		{
			UdpdebugM_init.NetState = NET_UDP_OK;
		}
	}
	else if(UdpdebugM_init.NetState == NET_UDP_OK)
	{
		if(cmd_bufer[0] =='D'&&cmd_bufer[1] =='N'&&cmd_bufer[2] =='S')
		{
			for(i=0;i<DNS_MAX_NAME_LENGTH;i++)
			{
					if((cmd_bufer[3+i]==0))
					{
						break;
					}
			}
			if(i!=DNS_MAX_NAME_LENGTH)
			{			
				for(i=0;i<DNS_MAX_NAME_LENGTH;i++)
				{
						if((cmd_bufer[3+i]>='a'&&cmd_bufer[3+i]<='z')||(cmd_bufer[3+i]>='A'&&cmd_bufer[3+i]<='Z'))
						{
							break;
						}
				}
			}
			if(i!=DNS_MAX_NAME_LENGTH)
			{			
				get_input_dns(&cmd_bufer[3+i]);
			}
		}
		else if(cmd_bufer[0] =='U'&&cmd_bufer[1] =='R'&&cmd_bufer[2] =='L')		//URLÐÅÏ¢
		{
		
		}
		else if(cmd_bufer[0] =='S'&&cmd_bufer[1] =='S'&&cmd_bufer[2] =='I')   //·þÎñÆ÷ÐÅÏ¢
		{
		
		}
		else if(cmd_bufer[0] =='R'&&cmd_bufer[1] =='E'&&cmd_bufer[2] =='A'&&cmd_bufer[3] =='D')   //¶ÁÐÅÏ¢
		{
			for(i=0;i<strlen((char*)cmd_bufer);i++)
			{
				wr_err = ERR_OK;
				sprintf((char*)LocalPoint, "mcuid?");
				for(j=0;j<(sizeof("mcuid?"))-1;j++)
				{
					if(LocalPoint[j] != cmd_bufer[i+j])
					{
						wr_err = ERR_VAL;
						break;
					}				
				}
				
				if(wr_err == ERR_OK)  //ÓÐÊÕµ½³¤¶È
				{
				//	u32 sn0_temp;
					u32 sn0;
					sn0=*(vu32*)(0x1FFF7A10);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
					sprintf((char*)LocalPoint, "mcuid:%X",sn0);
					sn0=*(vu32*)(0x1FFF7A14);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
					sprintf((char*)LocalPoint, "%s %X",(char*)LocalPoint,sn0);
					sn0=*(vu32*)(0x1FFF7A18);//»ñÈ¡STM32µÄÎ¨Ò»IDµÄÇ°24Î»×÷ÎªMACµØÖ·ºóÈý×Ö½Ú
					sprintf((char*)LocalPoint, "%s %X",(char*)LocalPoint,sn0);
					udp_demo_senddata(upcb,LocalPoint);					
				}	
			}
		}	
		else if(cmd_bufer[0] =='S'&&cmd_bufer[1] =='E'&&cmd_bufer[2] =='T')   //POWER  ÉÏµç 
		{
			for(i=0;i<strlen((char*)cmd_bufer);i++)
			{
				wr_err = ERR_OK;
				sprintf((char*)LocalPoint, "chargeidandtime:");
				for(j=0;j<(sizeof("chargeidandtime:"))-1;j++)
				{
					if(LocalPoint[j] != cmd_bufer[i+j])
					{
						wr_err = ERR_VAL;
						break;
					}				
				}
				
				if(wr_err == ERR_OK)  //ÓÐÊÕµ½³¤¶È
				{
					key_point = &cmd_bufer[i+j];  //È¡µØÖ·Í·
					for(j=0;j<sizeof(number)-1;j++)
					{
						if(key_point[j]==',')
						{
									break;
						}
						number[j] = key_point[j];
					}
					number[j] = 0;
					port_n = atoi((char*)number);

					key_point = &key_point[j+1];  //È¡µØÖ·Í·
					for(j=0;j<sizeof(number)-1;j++)
					{
						if(key_point[j]==',')
						{
									break;
						}
						number[j] = key_point[j];
					}
					number[j] = 0;
					if(port_n<DefPortNum)
					{
					device.PortPowerSetTime[port_n] = atoi((char*)number);
					device.PortPowerUseTime[port_n] = device.PortPowerSetTime[port_n];
					}
				}			
			}	
			
			for(i=0;i<strlen((char*)cmd_bufer);i++)
			{
				wr_err = ERR_OK;
				sprintf((char*)LocalPoint, ":");
				for(j=0;j<(sizeof("chargeidandtime:"))-1;j++)
				{
					if(LocalPoint[j] != cmd_bufer[i+j])
					{
						wr_err = ERR_VAL;
						break;
					}				
				}
				
				if(wr_err == ERR_OK)  //ÓÐÊÕµ½³¤¶È
				{
					key_point = &cmd_bufer[i+j];  //È¡µØÖ·Í·
					for(j=0;j<sizeof(number)-1;j++)
					{
						if(key_point[j]==',')
						{
									break;
						}
						number[j] = key_point[j];
					}
					number[j] = 0;
					port_n = atoi((char*)number);
				}			
			}	
		}
	}
	else
	{
		UdpdebugM_init.NetState = NET_ON;
		udp_demo_connection_close(udpdebug_pcb); //ÏÈ¹Ø±Õ
	}
			
	UdpdebugM_init.RxLength = 0;
  return wr_err;
}

//-----------------------------------------------------------
void make_soket_mess(void)
{	
	ChargeM_init.ID =NULL;
	ChargeM_init.LEN =NULL;
	ChargeM_init.NUMBER = 1000;
	ChargeM_init.encryption = 1;
	ChargeM_init.longitude[0] = 'E';
	ChargeM_init.longitude[1] = '1';
	ChargeM_init.longitude[2] = '1';
	ChargeM_init.longitude[3] = '6';
	ChargeM_init.longitude[4] = '.';
	ChargeM_init.longitude[5] = '2';
	ChargeM_init.longitude[6] = '3';
	ChargeM_init.longitude[7] = '0';
	ChargeM_init.latitude[0] = 'N';
	ChargeM_init.latitude[1] = '0';
	ChargeM_init.latitude[2] = '3';
	ChargeM_init.latitude[3] = '9';
	ChargeM_init.latitude[4] = '.';
	ChargeM_init.latitude[5] = '5';
	ChargeM_init.latitude[6] = '4';
	ChargeM_init.latitude[7] = '0';
	
	sprintf((char*)ChargeM_init.SN,"%s",device.Toolname);

	sprintf((char*)ChargeM_init.TOKEN,"TOK");
	
	sprintf((char*)ChargeM_init.MD5,"MD5=NULL");

	ChargeM_init.HeartTime =  DefineHeartTime;
	ChargeM_init.SW_state = SW_NULL;
	ChargeM_init.NetState = NET_NULL;
}	
//-----------------------------------------------------------
void make_http_mess(void)
{
	unsigned  char i;
	//Ä¬ÈÏºóÌ¨
//	unsigned  char temp_url[]="http://wctestapi.freechargechina.com:8011/siteApi/site/v1/device/register ";   
	unsigned  char BaseUrl[]="http://wctestapi.freechargechina.com:8011/siteApi";
//	unsigned  char BaseUrl[]="http://112.65.187.86:8098/siteApi";
	
	for(i=0;i<HttpU_BUFSIZE;i++)
	{
		HttpU_buffer[i].SW_state = SW_NULL;  //Ã»ÓÐÊ¹ÓÃ
		HttpU_buffer[i].next  = &HttpU_buffer[i+1];  //ÐÎ³ÉÁ´
	}	
	HttpU_buffer[i-1].next  =&HttpU_buffer[0];  //ÐÎ³ÉÁ´
	
//³õÊ¼»¯Ä¬ÈÏºóÌ¨³õÊ¼	
	HttpU_buffer[0].SW_state = SW_ON;  //Ê¹ÓÃ
	HttpU_buffer[0].NetState  = NET_ON;  //NET¹¤×÷¿ªÆô
	sprintf((char*)HttpU_buffer[0].type , "SSI");  //URLÀàÐÍ
	sprintf((char*)HttpU_buffer[0].url, "%s/site/v1/device/register",BaseUrl);
	HttpU_buffer[0].HeartTime = DefineHeartTime;
//	HttpU_buffer[0].sent = make_http_post;
	HttpU_buffer[0].sent = make_http_InitSoket;
	
//³õÊ¼»¯Ä¬ÈÏ±³¾°Í¼Æ¬
	HttpU_buffer[1].SW_state = SW_ON;  //Ê¹ÓÃ
	HttpU_buffer[1].NetState  = NET_OFF_DONE;  //¹ÒÆðÈÎÎñ
	sprintf((char*)HttpU_buffer[1].type , "S20");  //URLÀàÐÍ
	sprintf((char*)HttpU_buffer[1].url, "%s/site/v1/images",BaseUrl);
	HttpU_buffer[1].HeartTime = DefineHeartTime;
	HttpU_buffer[1].sent = make_http_get;
	
//³õÊ¼»¯Ä¬ÈÏADS
	HttpU_buffer[2].SW_state = SW_ON;  //Ê¹ÓÃ
	HttpU_buffer[2].NetState  = NET_OFF_DONE;  //¹ÒÆðÈÎÎñ
	sprintf((char*)HttpU_buffer[2].type , "S40");  //URLÀàÐÍ
	sprintf((char*)HttpU_buffer[2].url, "%s/appsite/v1/get_version",BaseUrl);
	sprintf((char*)HttpU_buffer[2].DestPath, "{\r\n\"version\": \"0\",\r\n\"type\": \"9\",\r\n\"province\": \"1\",\r\n\"terminal\": \"%s\"\r\n}",device.Toolname);
	HttpU_buffer[2].HeartTime = DefineHeartTime;
	HttpU_buffer[2].sent = make_http_post;
	
//³õÊ¼»¯Ä¬ÈÏIDS
	HttpU_buffer[3].SW_state = SW_ON;  //Ê¹ÓÃ
	HttpU_buffer[3].NetState  = NET_OFF_DONE;  //¹ÒÆðÈÎÎñ
	sprintf((char*)HttpU_buffer[3].type , "S41");  //URLÀàÐÍ
	sprintf((char*)HttpU_buffer[3].url, "%s/site/v1/ads/ids",BaseUrl);
	HttpU_buffer[3].HeartTime = DefineHeartTime;
	HttpU_buffer[3].sent = make_http_get;
	
//³õÊ¼»¯Ä¬ÈÏVERSION
	HttpU_buffer[4].SW_state = SW_ON;  //Ê¹ÓÃ
	HttpU_buffer[4].NetState  = NET_OFF_DONE;  //¹ÒÆðÈÎÎñ
	sprintf((char*)HttpU_buffer[4].type , "S50");  //URLÀàÐÍ
//	sprintf((char*)HttpU_buffer[4].url, "http://wctestapi.freechargechina.com:8011/siteApi/site/v1/software/version?deviceNo=SNT-AA&currentVersion=123");
	sprintf((char*)HttpU_buffer[4].url, "%s/site/v1/software/version?deviceNo=",BaseUrl);
	strcat((char*)HttpU_buffer[4].url, (char*)device.Toolname);
	strcat((char*)HttpU_buffer[4].url, "&currentVersion=");
	strcat((char*)HttpU_buffer[4].url, (char*)device.Version);
	HttpU_buffer[4].HeartTime = DefineHeartTime;
	HttpU_buffer[4].sent = make_http_get;
	
	HttpM_init.HeartTime =  DefineHeartTime;
	HttpM_init.LastTxTime  = 0;
	HttpM_init.LastRxTime  = 0;
	HttpM_init.SW_state  = SW_ON;
	HttpM_init.NetState  = NET_ON;
	HttpM_init.HttpUrl =NULL;
}	

//-----------------------------------------------------------
void make_udpdebug_mess(void)
{
	
	UdpdebugM_init.HeartTime =  DefineHeartTime;
	UdpdebugM_init.LastTxTime  = 0;
	UdpdebugM_init.LastRxTime  = 0;
//	UdpdebugM_init.SW_state  = SW_OFF;
	UdpdebugM_init.SW_state  = SW_ON;
	UdpdebugM_init.NetState  = NET_NULL;
}	

//struct tcp1_msee_all 
//{
//	u16 ID;              //IDÊ¶±ðºÅ
//	u16 LEN;             //°ü³¤
//	u16 NUMBER;          //Á÷Ë®ºÅ
//	u16 encryption;      //¼ÓÃÜ·½Ê½
//	u8 longitude[8];      //¾­¶È
//	u8 latitude[8];      //Î³¶È
//	
//	void *data;
//	u8 MD5[16];	    //MD5Öµ	
//};
//SOKET ³õÊ¼
err_t make_soket_10000(unsigned  char *buffer)
{
	u16 i;
	u16 len;
	
	ChargeM_init.ID = 10000;
	len = 0;
	for(i=0;i<2;i++)
	{
	buffer[len]	= *(((unsigned  char *)&ChargeM_init.ID)+1-i);
	len++;
	}
	for(i=0;i<2;i++)
	{
	buffer[len]	= *(((unsigned  char *)&ChargeM_init.LEN)+1-i);
	len++;
	}
	ChargeM_init.NUMBER++;
	if(ChargeM_init.NUMBER>10000)
	{		ChargeM_init.NUMBER = 1000;	}
	for(i=0;i<2;i++)
	{
	buffer[len]	= *(((unsigned  char *)&ChargeM_init.NUMBER)+1-i);
	len++;
	}
	for(i=0;i<2;i++)
	{
	buffer[len]	= *(((unsigned  char *)&ChargeM_init.encryption)+1-i);
	len++;
	}
	for(i=0;i<8;i++)
	{
	buffer[len]	= ChargeM_init.longitude[i];
	len++;
	}
	for(i=0;i<8;i++)
	{
	buffer[len]	= ChargeM_init.latitude[i];
	len++;
	}
	
//ÏûÏ¢Ìå
	for(i=0;i<sizeof(ChargeM_init.SN);i++)
	{
	if(ChargeM_init.SN[i]==0)
	{		break;				}
	buffer[len]	= ChargeM_init.SN[i];
	len++;
	}
	if(i>0)
	{
	buffer[len++] =',';
	}
	
	for(i=0;i<4;i++)
	{
	if(ChargeM_init.TOKEN[i]==0)
	{		break;				}
	buffer[len]	= ChargeM_init.TOKEN[i];
	len++;
	}
	if(i>0)
	{
	buffer[len++] =',';
	}
	
	for(i=0;i<16;i++)
	{
	buffer[len]	= ChargeM_init.MD5[i];
	len++;
	}
	if(len>40)
	{
	len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
	len = 0;
	return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	return ERR_OK;
}
//SOKET ÐÄÌø
err_t make_soket_10001(unsigned  char *buffer)
{
	u16 i;
	u16 len;	
	
	ChargeM_init.ID = 10001;
	len = 0;
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.ID)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.LEN)+1-i);
		len++;
	}
	ChargeM_init.NUMBER++;
	if(ChargeM_init.NUMBER>10000)
	{		ChargeM_init.NUMBER = 1000;	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.NUMBER)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.encryption)+1-i);
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.longitude[i];
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.latitude[i];
		len++;
	}
	
//ÏûÏ¢Ìå
	for(i=0;i<sizeof(ChargeM_init.SN);i++)
	{
		if(ChargeM_init.SN[i]==0)
		{			break;			}
		buffer[len]	= ChargeM_init.SN[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
		
	for(i=0;i<16;i++)
	{
		buffer[len]	= ChargeM_init.MD5[i];
		len++;
	}
	if(len>40)
	{
		len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
		len = 0;
		return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	return ERR_OK;
}
err_t make_soket_10004(unsigned  char *buffer,unsigned  char *cmd)
{
	unsigned  char i;
	unsigned  short len;	
	unsigned  char port_n=0;
	unsigned  char number[16];
	unsigned  char *key_point;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
//	key_point = &cmd[24];
	key_point = &cmd[25];
	for(i=0;i<sizeof(device.PortName[0])-1;i++)
	{
		if(key_point[i] >'9'||key_point[i] <'0')			
		{
			break;
		}
		LocalPoint[i] = key_point[i];
	}
	LocalPoint[i] = 0;
	for(i=0;i<DefPortNum;i++)
	{
		if ((strcmp((char *)LocalPoint, (char *)device.PortName[i]) == 0))
		{
			port_n = i;			
			break;
		}
	}
		
	ChargeM_init.ID = 10004;
	len = 0;
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.ID)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.LEN)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
//		buffer[len]	= *(((unsigned  char *)&ChargeM_init.NUMBER)+1-i);
		buffer[len]	= cmd[len];
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.encryption)+1-i);
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.longitude[i];
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.latitude[i];
		len++;
	}
	
//ÏûÏ¢Ìå
	for(i=0;i<sizeof(ChargeM_init.SN);i++)
	{
		if(ChargeM_init.SN[i]==0)
		{			break;			}
		buffer[len]	= ChargeM_init.SN[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
	for(i=0;i<sizeof(device.PortName[0]);i++)
	{
		if(device.PortName[port_n][i]==0)
		{			break;			}
		buffer[len]	= device.PortName[port_n][i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}

	u16toStr(device.PortPowerUseTime[port_n],number);
	for(i=0;i<sizeof(number);i++)
	{
		if(number[i]==0)
		{			break;			}
		buffer[len]	= number[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
//ÏûÏ¢Ìå end 		
	for(i=0;i<16;i++)
	{
		buffer[len]	= ChargeM_init.MD5[i];
		len++;
	}
	if(len>40)
	{
		len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
		len = 0;
		return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	sprintf((char*)LocalPoint, "SOKET send:%d ID=10004 IP=%d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);	
	udp_demo_senddata(udpdebug_pcb,LocalPoint);
	return ERR_OK;
}
//
err_t cmd_soket_10005(unsigned  char *cmd)
{
	unsigned  short i;
	unsigned  short j;
	unsigned  char port_n;
	unsigned  char number[16];
	unsigned  char *key_point;
//		ChargeM_init
		
//	key_point = &cmd[24];
	key_point = &cmd[25];
	for(j=0;j<sizeof(number)-1;j++)
	{
		if(key_point[j] >'9'||key_point[j] <'0')			
		{
				break;
		}
		number[j] = key_point[j];
	}
	number[j] = 0; 
	for(i=0;i<DefPortNum;i++)
	{
		if ((strcmp((char *)number, (char *)device.PortName[i]) == 0))
		{
			port_n = i;			
			break;
		}
	}
	
	key_point = &key_point[j+1];
	for(j=0;j<sizeof(number)-1;j++)
	{
		if(key_point[j] >'9'||key_point[j] <'0')			
		{
					break;
		}
		number[j] = key_point[j];
	}
	number[j] = 0;
	if(port_n<DefPortNum)
	{
	device.PortPowerSetTime[port_n] = atoi((char*)number);
	device.PortPowerUseTime[port_n] = device.PortPowerSetTime[port_n];
	}
	return ERR_OK;	
}
err_t make_soket_10006(unsigned  char *buffer,unsigned  char *cmd)
{
	unsigned  char i;
	unsigned  short j;
	unsigned  short len;	
	unsigned  char port_n =0;
	unsigned  char number[16];
	unsigned  char *key_point;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
//	key_point = &cmd[24];
	key_point = &cmd[25];
	for(j=0;j<sizeof(number)-1;j++)
	{
		if(key_point[j] >'9'||key_point[j] <'0')			
		{
				break;
		}
		number[j] = key_point[j];
	}
	number[j] = 0; 
	for(j=0;j<DefPortNum;j++)
	{
		if ((strcmp((char *)number, (char *)device.PortName[j]) == 0))
		{
			port_n = j;			
			break;
		}
	}
	
	ChargeM_init.ID = 10006;
	len = 0;
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.ID)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.LEN)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
//		buffer[len]	= *(((unsigned  char *)&ChargeM_init.NUMBER)+1-i);
		buffer[len]	= cmd[len];
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.encryption)+1-i);
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.longitude[i];
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.latitude[i];
		len++;
	}
	
//ÏûÏ¢Ìå
	for(i=0;i<sizeof(ChargeM_init.SN);i++)
	{
		if(ChargeM_init.SN[i]==0)
		{			break;			}
		buffer[len]	= ChargeM_init.SN[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
	for(i=0;i<sizeof(device.PortName[0]);i++)
	{
		if(device.PortName[port_n][i]==0)
		{			break;			}
		buffer[len]	= device.PortName[port_n][i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}

	u16toStr(device.PortPowerSetTime[port_n],number);
	for(i=0;i<sizeof(number);i++)
	{
		if(number[i]==0)
		{			break;			}
		buffer[len]	= number[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
//ÏûÏ¢Ìå end 		
	for(i=0;i<16;i++)
	{
		buffer[len]	= ChargeM_init.MD5[i];
		len++;
	}
	if(len>40)
	{
		len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
		len = 0;
		return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	sprintf((char*)LocalPoint, "SOKET send:%d ID=10006 IP=%d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);	
	udp_demo_senddata(udpdebug_pcb,LocalPoint);
	return ERR_OK;
}
//---------------------------------------------------
err_t cmd_soket_10011(unsigned  char *buffer)
{
	unsigned  char i;
	unsigned  short len;	
	unsigned  char port_n =0;
	unsigned  char number[16];
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	ChargeM_init.ID = 10011;
	len = 0;
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.ID)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.LEN)+1-i);
		len++;
	}
	ChargeM_init.NUMBER++;
	if(ChargeM_init.NUMBER>10000)
	{		ChargeM_init.NUMBER = 1000;	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.NUMBER)+1-i);
		len++;
	}
	for(i=0;i<2;i++)
	{
		buffer[len]	= *(((unsigned  char *)&ChargeM_init.encryption)+1-i);
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.longitude[i];
		len++;
	}
	for(i=0;i<8;i++)
	{
		buffer[len]	= ChargeM_init.latitude[i];
		len++;
	}
	
//ÏûÏ¢Ìå
	for(i=0;i<sizeof(ChargeM_init.SN);i++)
	{
		if(ChargeM_init.SN[i]==0)
		{			break;			}
		buffer[len]	= ChargeM_init.SN[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
	for(i=0;i<sizeof(device.PortName[0]);i++)
	{
		if(device.PortName[port_n][i]==0)
		{			break;			}
		buffer[len]	= device.PortName[port_n][i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}

	u16toStr(device.PortCurrent[port_n][0],number);  //µçÁ÷
	for(i=0;i<sizeof(number);i++)
	{
		if(number[i]==0)
		{			break;			}
		buffer[len]	= number[i];
		len++;
	}
	if(i>0)
	{
		buffer[len++] =',';
	}
	
//ÏûÏ¢Ìå end 		
	for(i=0;i<16;i++)
	{
		buffer[len]	= ChargeM_init.MD5[i];
		len++;
	}
	if(len>40)
	{
		len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
		len = 0;
		return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	sprintf((char*)LocalPoint, "SOKET send:%d ID=10006 IP=%d.%d.%d.%d.%d\r\n",time_sys,soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);	
	udp_demo_senddata(udpdebug_pcb,LocalPoint);
	return ERR_OK;
}
//----------------------------------------------------
//½ÓÊÕ³É¹¦
err_t make_soket_rxok(unsigned  char *buffer)
{
	u16 i;
	u16 len;	
	
	len = 0;
	for(i=0;i<24;i++)
	{
		buffer[len++] = tcp1_rxbuf[i];
	}
	
	buffer[len++] = '0';
	buffer[len++] = ',';
	
	for(i=0;i<16;i++)
	{
	buffer[len]	= ChargeM_init.MD5[i];
	len++;
	}
	if(len>40)
	{
	len -=41;//24×Ö½ÚÍ·+16×Ö½ÚµÄ¼ìÑé¡
	}
	else
	{
	len = 0;
	return ERR_BUF;
	}
	buffer[2] = (len>>8);  //Í³¼Æ×Ü³¤¶È
	buffer[3] = (len&0XFF);  //Í³¼Æ×Ü³¤¶È
	
	return ERR_OK;
}

err_t http_url_select(struct http_msee *HttpM)
{
  err_t wr_err = ERR_OK;
	unsigned  int i,j;
	struct http_url *HttpU;
	unsigned  char *key_piont;
	unsigned  char temp;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
			HttpU = (struct http_url*)HttpM_init.HttpUrl; 
			if(HttpU!=NULL)
			{	
				if(HttpU->NetState==NET_OFF_DONE)   //µ±Ç°µÄÍê³É
				{
					HttpU->NetState=NET_NULL;
					HttpU = HttpU->next;
				}
				else if(HttpU->NetState==NET_NULL)   //Á´Îª¿ÕÊ±¼ì²éÓÐÃ»ÐÂµÄHTTP
				{			
					wr_err =ERR_OK;
					if(time_sys-HttpU->LastRxTime >(HttpU->HeartTime))
					{
						HttpU->LastRxTime = time_sys;
						if(wr_err==ERR_OK)  //ÎÞ¸üÐÂ
						{				wr_err = do_area1_get(&temp);			}
						if(wr_err==ERR_OK)	//ÎÞ¸üÐÂ
						{				wr_err = do_area2_get(&temp);			}
						if(wr_err!=ERR_OK)	//ÓÐ¸üÐÂ
						{
							HttpU->NetState=NET_OFF_DONE;
						}
						else
						{
							HttpM_init.NetState = NET_NULL;
						}
					}
					else
					{
							HttpM_init.NetState = NET_NULL;
					}
					return wr_err;
				}
				else if(time_sys-HttpU->LastRxTime >(HttpU->HeartTime* DefineURLTry))//µ±Ç°URL³¬Ê±
				{
					HttpU->NetState=NET_OFF;  //µ±Ç°URL¹Ø±Õ
					HttpU->LastRxTime = time_sys;
					printf("URL time out:%d\r\n",time_sys);
					return wr_err;
				}
				else if(HttpU->NetState==NET_ON)   //µ±Ç°µÄ¿ªÊ¼»ñÈ¡DNS
				{
					if(strlen((char*)HttpM_init.DnsNameStr)&&time_sys-HttpU->LastTxTime >(HttpU->HeartTime))
					{
					HttpU->LastTxTime = time_sys;
					get_input_dns(HttpM_init.DnsNameStr);
					}
					return wr_err;
				}
				else
				{
					return wr_err;
				}
			}
			else
			{
				HttpU = &HttpU_buffer[0];
			}
						
			for(i=0;i<=HttpU_BUFSIZE;i++)   //È·¶¨µ±Ç°µÄHTTP URL
			{
				if(HttpU->SW_state == SW_ON)
				{
					HttpU->NetState=NET_ON;  //µ±Ç°URL¿ªÊ¼
					HttpU->LastRxTime = time_sys;
					HttpM_init.HttpUrl = HttpU;	
					HttpM_init.LastRxTime = time_sys;
					HttpM_init.NetState = NET_ON;  //µ±Ç°URL¿ªÊ¼
					break;
				}
				else
				{
					HttpU = HttpU->next;
				}
			}
//È·¶¨Ö÷»úÃûÓë¶Ë¿ÚºÅ			
			HttpU = (struct http_url*)HttpM_init.HttpUrl; 
			
			wr_err = ERR_OK;
			sprintf((char*)LocalPoint, "http://");		
			j= sizeof("http://");
			for(j=0;j<(sizeof("http://")-1);j++)
			{
				if(LocalPoint[j] != HttpU->url[j])
				{
					wr_err = ERR_VAL;
					break;
				}				
			}
			if(wr_err == ERR_OK)  //ÓÐÊÕµ½URL
			{
				key_piont = &(HttpU->url[sizeof("http://")-1]);
				j=0;
				for(i=0;i<(sizeof(HttpM_init.DnsNameStr)-1);i++)
				{
					if(key_piont[j]==':')
					{
						HttpM_init.DnsNameStr[i] =0;
						break;
					}					
					HttpM_init.DnsNameStr[i]= key_piont[j];
					j++;
				}
				if(key_piont[j]==':')
				{
					j++;
					for(i=0;i<(sizeof(HttpM_init.PortStr)-1);i++)
					{
						if(key_piont[j]>'9'||key_piont[j]<'0')   
						{
							HttpM_init.PortStr[i]=0;
							break;
						}
						HttpM_init.PortStr[i] = key_piont[j];	
						j++;
					}	
				}	
				HttpM_init.Port = atoi((char*)HttpM_init.PortStr);
			}

			else
			{
				Str_addto_Str(&DebugStr,"URL INPUT ERRORR\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
			}
			
	return wr_err;
}
//extern err_t handle_area1_info(unsigned  char *buffer);
//extern err_t handle_area2_info(unsigned  char *buffer);

err_t make_area1_get(unsigned  char *buffer)
{
  err_t wr_err = ERR_OK;
	u16 i;
//	u16 j;
	u16 u16temp;
	u16 len;
	unsigned  char port_n =0;
		res_sd = f_open(&fsrc, "area1/area1_info.txt", FA_OPEN_EXISTING | FA_READ); 	
		if(res_sd != FR_OK)
		{
			return res_sd;
		}		
		res_sd = f_open(&fnew, "area1/area1_info.get", FA_CREATE_ALWAYS | FA_WRITE); 	
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
		{
//			res_sd = f_read (FIL* fp, void* buff, UINT btr, UINT* br);
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
			
			i=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"portCode\":",(unsigned  char*)",", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					i += len; 
					len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"qcodeUrl\"",(unsigned  char*)"}", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);					
					if(strlen((char*)FatFile_URL.FileSourcePath))
					{
						i += len; 
						strcat((char*)FatFile_URL.FileBufSlave,"[\"name\":\""); 		
						strcat((char*)FatFile_URL.FileBufSlave,(char*)FatFile_URL.FileHead); 		
						strcat((char*)FatFile_URL.FileBufSlave,"\","); 							
						strcat((char*)FatFile_URL.FileBufSlave,"\"handle\":\"0\","); 		
						strcat((char*)FatFile_URL.FileBufSlave,"\"G10\""); 		
						strcat((char*)FatFile_URL.FileBufSlave,(char*)FatFile_URL.FileSourcePath); 		
						strcat((char*)FatFile_URL.FileBufSlave,",]\r\n"); 												
						res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);		
						
						sprintf((char*)device.PortName[port_n],"%s",(char*)FatFile_URL.FileHead);
						cut_between_strs_inverted((unsigned  char*)FatFile_URL.FileSourcePath,(unsigned  char*)"/",(unsigned  char*)"\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
						sprintf((char*)device.PortSourcePath[port_n++],"area1/%s",(char*)FatFile_URL.FileHead);
						if(port_n>=DefPortNum)
						{
							port_n = DefPortNum-1;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		
			i=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"microChargeUrl\":\"",(unsigned  char*)"\",", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					i += len; 					
				  len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"microChargePort\":\"",(unsigned  char*)"\",", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);					
					if(strlen((char*)FatFile_URL.FileSourcePath))
					{
					i += len; 						
					strcat((char*)FatFile_URL.FileBufSlave,"[\"name\":\""); 		
					strcat((char*)FatFile_URL.FileBufSlave,(char*)FatFile_URL.FileHead); 		
					strcat((char*)FatFile_URL.FileBufSlave,"\","); 		
					strcat((char*)FatFile_URL.FileBufSlave,"\"handle\":\"0\","); 								
					strcat((char*)FatFile_URL.FileBufSlave,"\"K10\":\""); 		//SOCKET ÀàÐÍ
					strcat((char*)FatFile_URL.FileBufSlave,(char*)FatFile_URL.FileSourcePath); 		
					strcat((char*)FatFile_URL.FileBufSlave,"\",]\r\n"); 												
					res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);		
						
					sprintf((char*)ChargeM_init.DnsNameStr,"%s",(char*)FatFile_URL.FileHead);
					sprintf((char*)ChargeM_init.PortStr,"%s",(char*)FatFile_URL.FileSourcePath);
					ChargeM_init.Port = atoi((char*)ChargeM_init.PortStr);							
					ChargeM_init.SW_state = SW_ON;		
					ChargeM_init.NetState  = NET_ON;		
					ChargeM_init.LastRxTime = time_sys;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}		
					
			if(FatFile_URL.FileBufSize > fnum)
			{
				break;
			}			
		}
		res_sd=f_close(&fnew);			
		res_sd=f_close(&fsrc);			
	return wr_err;
}
err_t make_area2_get(unsigned  char *buffer)
{
//	http://wctestapi.freechargechina.com:8888
  err_t wr_err = ERR_OK;
	u16 i;
	u16 len;
	u16 u16temp;
	u32 fnum1;
	u32 fnum2;
	
		res_sd = f_open(&fsrc, "area2/area2_info.txt", FA_OPEN_EXISTING | FA_READ); 	
		if(res_sd != FR_OK)
		{
			return res_sd;
		}		
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		wr_err = ERR_OK;
		res_sd = f_open(&fnew, "area2/area2_info.get", FA_OPEN_EXISTING | FA_READ); 
		if(res_sd == FR_OK)
		{
			for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
			{
				res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum1);
				res_sd = f_read (&fnew, FatFile_URL.FileBufSlave, FatFile_URL.FileBufSlaveSize, &fnum2);
				i=0;
				while(1)
				{
					len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"http:",(unsigned  char*)"bmp\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
					if(strlen((char*)FatFile_URL.FileHead))
					{
						i += len; 
						len = cut_between_strs(FatFile_URL.FileBufSlave,(unsigned  char*)FatFile_URL.FileHead, (unsigned  char*)"\"",FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
						if((strlen((char*)FatFile_URL.FileSourcePath))==0)
						{
							wr_err = ERR_VAL;
							break;
						}
					}
					else
					{
						break;
					}
				}
				if(FatFile_URL.FileBufSize > fnum1)
				{
					break;
				}			
			}
		}
		else
		{
			wr_err = ERR_VAL;
		}
		res_sd=f_close(&fnew);	
		
		if(wr_err == ERR_VAL)
		{
			res_sd = f_open(&fnew, "area2/area2_info.get", FA_CREATE_ALWAYS | FA_WRITE);
			res_sd = f_lseek(&fsrc, 0);
			for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
			{
				res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
				i=0;
				while(1)
				{
					FatFile_URL.FileBufSlave[0] = 0;
					len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"\"http:",(unsigned  char*)"bmp\"", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
					if(strlen((char*)FatFile_URL.FileHead))
					{
						i += len; 
						strcat((char*)FatFile_URL.FileBufSlave,"[\"handle\":\"0\","); 		
						
						strcat((char*)FatFile_URL.FileBufSlave,"\"G20\":\"http:"); 		
						strcat((char*)FatFile_URL.FileBufSlave,(char*)FatFile_URL.FileHead); 		
						strcat((char*)FatFile_URL.FileBufSlave,"bmp\",]\r\n"); 		

						res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);						
					}
					else
					{
						break;
					}
				}
				if(FatFile_URL.FileBufSize > fnum)
				{
					break;
				}			
			}
			res_sd=f_close(&fnew);					
		}
		res_sd=f_close(&fsrc);			

		return wr_err;		
}
//ret=0,ÎÞ¸üÐÂ£»ret=1,ÓÐ¸üÐÂ¡£flag=0,ÎÞÒç³ö£»flag=1,ÓÐÒç³ö¡£
err_t do_area1_get(unsigned  char *buffer)
{
	unsigned  char temp;
	u16 i,j;
	u16 len;
	u16 u16temp;
	struct http_url *HttpU;	
  err_t wr_err;   //1,ÓÐ¸üÐÂ
	HttpU = HttpU_buffer;
	 
		*buffer = ERR_OK;
		wr_err = ERR_OK;
		res_sd = f_open(&fsrc, "area1/area1_info.get", FA_OPEN_EXISTING | FA_READ); 	
		if(res_sd != FR_OK)
		{			return res_sd;		}		
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
			i=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"[",(unsigned  char*)"]", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					i += len; 
					len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"handle\":\"",(unsigned  char*)"\",", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);					
					if(FatFile_URL.FileSourcePath[0]!='T')
					{
						len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"G10\":\"",(unsigned  char*)"\",", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);		
						if(strlen((char*)FatFile_URL.FileSourcePath))
						{
							wr_err = ERR_ISCONN; 
							j=0;
							while(j<HttpU_BUFSIZE)   //ÏÖÓÐµÄBUFFERÓÐÃ»ÓÐÁ¬½Ó
							{ 
								if(strcmp((char *)FatFile_URL.FileHead, (char *)HttpU->url) == 0)
								{	break;	}
								j++;
								HttpU = HttpU->next;
							}
							if(j==HttpU_BUFSIZE)    
							{
								j=0;
								while(HttpU->SW_state != SW_NULL)//ÏÖÓÐµÄBUFFERÓÐÃ»¿Õ¼ä
								{
									if(j>HttpU_BUFSIZE)
									{	break;	}
									j++;
									HttpU = HttpU->next;
								}
								if(HttpU->SW_state==SW_NULL)   
								{	
								//³õÊ¼»¯ÐÂÔöµÄURL
									HttpU->SW_state = SW_ON;  //Ê¹ÓÃ
									HttpU->NetState  = NET_ON;  //NET¹¤×÷¿ªÆô
									HttpU->HeartTime = DefineHeartTime;
									HttpU->sent = 	make_http_get;	
									sprintf((char*)HttpU->type, "G10");
									sprintf((char*)HttpU->url,"%s",(char*)FatFile_URL.FileSourcePath);
									temp = 0;
									cut_between_strs_inverted((unsigned  char*)HttpU->url,(unsigned  char*)"/",&temp, FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
									sprintf((char*)HttpU->DestPath,"area1/%s",(char*)FatFile_URL.FileSourcePath);
								}
								else
								{
									*buffer = (char)ERR_VAL;
									break;  
								}	
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			if(FatFile_URL.FileBufSize > fnum)
			{
				break;
			}			
		}
		res_sd=f_close(&fsrc);				
	return wr_err;
}
err_t do_area2_get(unsigned  char *buffer)
{
	unsigned  char temp;
	u16 i,j;
	u16 len;
	u16 u16temp;
	struct http_url *HttpU;	
  err_t wr_err;   //1,ÓÐ¸üÐÂ
	HttpU = HttpU_buffer;
	 
		*buffer = ERR_OK;
		wr_err = ERR_OK;
		res_sd = f_open(&fsrc, "area2/area2_info.get", FA_OPEN_EXISTING | FA_READ); 	
		if(res_sd != FR_OK)
		{			return res_sd;		}		
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
			i=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"[",(unsigned  char*)"]", FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					i += len; 
					len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"handle\":\"",(unsigned  char*)"\",", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);					
					if(FatFile_URL.FileSourcePath[0]!='T')
					{
						len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"G20\":\"",(unsigned  char*)"\",", FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);		
						if(strlen((char*)FatFile_URL.FileSourcePath))
						{
							wr_err = ERR_ISCONN; 
							j=0;
							while(j<HttpU_BUFSIZE)   //BUFFERÖÐÓÐÃ»ÓÐ´ËÁ¬½Ó
							{ 
								if(strcmp((char *)FatFile_URL.FileHead, (char *)HttpU->url) == 0)
								{	break;	}
								j++;
								HttpU = HttpU->next;
							}
							if(j==HttpU_BUFSIZE)    
							{
								j=0;
								while(HttpU->SW_state != SW_NULL)//BUFFERÓÐÃ»¿Õ¼ä
								{
									if(j>HttpU_BUFSIZE)
									{	break;	}
									j++;
									HttpU = HttpU->next;
								}
								if(HttpU->SW_state==SW_NULL)   
								{	
								//³õÊ¼»¯ÐÂÔöµÄURL
									HttpU->SW_state = SW_ON;  //Ê¹ÓÃ
									HttpU->NetState  = NET_ON;  //NET¹¤×÷¿ªÆô
									HttpU->HeartTime = DefineHeartTime;
									HttpU->sent = 	make_http_get;	
									sprintf((char*)HttpU->type, "G20");
									sprintf((char*)HttpU->url,"%s",(char*)FatFile_URL.FileSourcePath);
									temp = 0;
									cut_between_strs_inverted((unsigned  char*)HttpU->url,(unsigned  char*)"/",&temp, FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
									sprintf((char*)HttpU->DestPath,"area2/%s",(char*)FatFile_URL.FileSourcePath);
								}
								else
								{
									*buffer = (char)ERR_VAL;
									break;  
								}	
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			if(FatFile_URL.FileBufSize > fnum)
			{
				break;
			}			
		}
		res_sd=f_close(&fsrc);				
	return wr_err;
}
err_t done_area1_get(unsigned  char *buffer)
{
	u16 i,j;
	u16 len;
	u16 u16temp;
	struct http_url *HttpU;	
  err_t wr_err = ERR_OK;
	HttpU = HttpM_init.HttpUrl;
	
		res_sd = f_open(&fsrc, "area1/area1_info.get", FA_OPEN_EXISTING |FA_READ |FA_WRITE); 	
		if(res_sd != FR_OK)
		{			return res_sd;		}		
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
			i=0;
			j=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"[" ,(unsigned  char*)"]" ,FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					j = i+u16temp;
					i += len; 
					len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"handle\":\"" ,HttpU->url, FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
					if(strlen((char*)FatFile_URL.FileSourcePath))
					{
						j +=u16temp;
						res_sd = f_lseek(&fsrc, j);
						sprintf((char*)FatFile_URL.FileBufSlave, "T");
						res_sd=f_write(&fsrc,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);											
					}					
				}
				else
				{
					break;
				}
			}
			if(FatFile_URL.FileBufSize > fnum)
			{
				break;
			}			
		}
		res_sd=f_close(&fsrc);				
	return wr_err;
}
err_t done_area2_get(unsigned  char *buffer)
{
	u16 i,j;
	u16 len;
	u16 u16temp;
	struct http_url *HttpU;	
  err_t wr_err = ERR_OK;
	HttpU = HttpM_init.HttpUrl;
	
		res_sd = f_open(&fsrc, "area2/area2_info.get", FA_OPEN_EXISTING |FA_READ |FA_WRITE); 	
		if(res_sd != FR_OK)
		{			return res_sd;		}		
		FatFile_URL.FileBuf = str_buffer;		
		FatFile_URL.FileBufSize  = sizeof(str_buffer);		
		FatFile_URL.FileBufLen  = 0;		
		for(FatFile_URL.FileHandleLen=0;FatFile_URL.FileHandleLen <fsrc.fsize;)
		{
			res_sd = f_read (&fsrc, FatFile_URL.FileBuf, FatFile_URL.FileBufSize, &fnum);
			i=0;
			j=0;
			while(1)
			{
				FatFile_URL.FileBufSlave[0] = 0;
				len = cut_between_strs(&FatFile_URL.FileBuf[i],(unsigned  char*)"[" ,(unsigned  char*)"]" ,FatFile_URL.FileHead,sizeof(FatFile_URL.FileHead),&u16temp);
				if(strlen((char*)FatFile_URL.FileHead))
				{
					j = i+u16temp;
					i += len; 
					len = cut_between_strs(FatFile_URL.FileHead,(unsigned  char*)"\"handle\":\"" ,HttpU->url, FatFile_URL.FileSourcePath,sizeof(FatFile_URL.FileSourcePath),&u16temp);
					if(strlen((char*)FatFile_URL.FileSourcePath))
					{
						j +=u16temp;
						res_sd = f_lseek(&fsrc, j);
						sprintf((char*)FatFile_URL.FileBufSlave, "T");
						res_sd=f_write(&fsrc,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);											
					}					
				}
				else
				{
					break;
				}
			}
			if(FatFile_URL.FileBufSize > fnum)
			{
				break;
			}			
		}
		res_sd=f_close(&fsrc);				
	return wr_err;
}
//·µ»Ø×îÏÈºÏÒªÇóµÄ
unsigned short cut_between_strs(unsigned  char *buffer,unsigned  char *began,unsigned  char *end, unsigned  char *ret,unsigned short maxret,unsigned short *retpoint)
{
	u32 len;
	u16 i,j;
	u16 PointBegan=0;
	u16 PointEnd=0;	
	u8 FlagBegan=0;	
	u8 FlagEnd=0;	
  err_t wr_err = ERR_OK;
	
		*retpoint = 0;
		ret[0] = 0;
		len = strlen((char*)buffer);
		if(len>0xffff)
		{
			return 0;
		}
		for(i=0;i<strlen((char*)buffer);i++)
		{
			if(FlagBegan==0)
			{
				wr_err = ERR_OK;
				len = strlen((char*)began);
				if(len ==0)
				{
						FlagBegan = 1;
						PointBegan = 0;		
						*retpoint = PointBegan;
				}
				else
				{
					for(j=0;j<(strlen((char*)began));j++)
					{
						if(began[j] != buffer[i+j])
						{
							wr_err = ERR_VAL;
							break;
						}				
					}
					if(wr_err == ERR_OK)
					{
						FlagBegan = 1;
						PointBegan = i+j;		
						*retpoint = PointBegan;
					}
				}
			}
			if(FlagBegan>0)  //ÓÐÊÕµ½³¤¶È
			{
				wr_err = ERR_OK;
				len = strlen((char*)end);
				if(len ==0)
				{
					FlagEnd = 1;
					PointEnd = strlen((char*)buffer);
				}
				else
				{
					for(j=0;j<(strlen((char*)end));j++)
					{
						if(end[j] != buffer[i+j])
						{
							wr_err = ERR_VAL;
							break;
						}				
					}
					if(wr_err == ERR_OK)
					{
						FlagEnd = 1;
						PointEnd = i;
					}
				}
				if(FlagEnd>0)  //ÓÐÊÕµ½³¤¶È
				{
					len = PointEnd - PointBegan;
					if(len < maxret )
					{
						j = PointBegan;			
						for(i=0;i<len;i++)
						{
							ret[i] = buffer[j];
							j++;
						}
						ret[i] = 0;
					}
					break;
				}			
			}
	  }
		
		if(PointEnd==0)
		{
		return 0;
		}
		else
		{
		return (PointEnd+strlen((char*)end));	
		}	
}
extern unsigned short cut_between_strs_inverted(unsigned  char *buffer,unsigned  char *began,unsigned  char *end, unsigned  char *ret,unsigned short maxret,unsigned short *retpoint)
{
	u32 len;
	u16 i,j;
	u16 base;
	u16 PointBegan=0;
	u16 PointEnd=0;	
	u8 FlagBegan=0;	
	u8 FlagEnd=0;	
  err_t wr_err = ERR_OK;
	
		*retpoint = 0;
		ret[0] = 0;
		len = strlen((char*)buffer);
		if(len>0xffff)
		{
			return 0;
		}
		base = len - strlen((char*)end);
		for(i=base;;i--)
		{
			if(FlagEnd==0)  //ÓÐÊÕµ½³¤¶È
			{
				wr_err = ERR_OK;
				len = strlen((char*)end);
				if(len ==0)
				{
					FlagEnd = 1;
					PointEnd = strlen((char*)buffer);
				}
				else
				{
					for(j=0;j<(strlen((char*)end));j++)
					{
						if(end[j] != buffer[i+j])
						{
							wr_err = ERR_VAL;
							break;
						}				
					}
					if(wr_err == ERR_OK)
					{
						FlagEnd = 1;
						PointEnd = i;
					}
				}
			}
			
			if(FlagEnd>0)
			{
				wr_err = ERR_OK;
				len = strlen((char*)began);
				if(len ==0)
				{
						FlagBegan = 1;
						PointBegan = 0;		
						*retpoint = PointBegan;
				}
				else
				{
					for(j=0;j<(strlen((char*)began));j++)
					{
						if(began[j] != buffer[i+j])
						{
							wr_err = ERR_VAL;
							break;
						}				
					}
					if(wr_err == ERR_OK)
					{
						FlagBegan = 1;
						PointBegan = i+j;		
						*retpoint = PointBegan;
					}
					
				}
				if(FlagBegan>0)  //ÓÐÊÕµ½³¤¶È
				{
					len = PointEnd - PointBegan;
					if(len < maxret )
					{
						j = PointBegan;			
						for(i=0;i<len;i++)
						{
							ret[i] = buffer[j];
							j++;
						}
						ret[i] = 0;
					}
					break;
				}			
			}
			if(i==0)
			{
				break;
			}
	  }	
		
		if(FlagBegan==0)
		{
			return 0;
		}
		else
		{
			return (PointBegan-strlen((char*)began));	
		}
}

//post ³õÊ¼»¯×¢²á½Ó¿Ú.×î¿ªÊ¼ÊÔµÄÒ»¸öÃüÁî
err_t make_http_InitSoket(unsigned  char *buffer)
{
	u16 i;
	u16 body_len;
	u16 len;
	unsigned  char *key_piont;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
//"Content-type: application/json;charset=utf8;\r\n"
//  sprintf((char*)LocalPoint, "\r\n{\"deviceNo\" : \"SJZ-YZBH01\",\r\n\"portNum\" : 4,\r\n\"retainPorts\" : [10,11,12,13],\r\n\"province\" : 1}");
	
	buffer[0]=0;
	strcat((char*)buffer,"POST "); 
	
	sprintf((char*)LocalPoint, "%s",(char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=sizeof("http://");i<len;i++)
	{
		if(LocalPoint[i]=='/')
		{
			key_piont =&LocalPoint[i];
			break;
		}	
	}
	strcat((char*)buffer,(char*)key_piont); 
	strcat((char*)buffer," HTTP/1.1\r\n"); 
	strcat((char*)buffer,"Host: "); 
	strcat((char*)buffer,(char*)HttpM_init.DnsNameStr); 
	strcat((char*)buffer,":"); 
	strcat((char*)buffer,(char*)HttpM_init.PortStr); 
	strcat((char*)buffer,"\r\n"); 
	strcat((char*)buffer,"Connection: keep-alive\r\n"); 		
	strcat((char*)buffer,"Accept: */*\r\n"); 		

	sprintf((char*)LocalPoint, (char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=0;i<len;i++)
	{
		if(LocalPoint[len-i]=='/')
		{
			LocalPoint[len-i] = 0;
			break;
		}	
	}
	strcat((char*)buffer,"Referer: "); 		
	strcat((char*)buffer,(char*)LocalPoint); 		
	strcat((char*)buffer,"\r\n"); 
	sprintf((char*)LocalPoint, "User-Agent: %s %s\r\n",(char*)device.Toolname,(char*)device.Version);
	strcat((char*)buffer,(char*)LocalPoint); 		
//	strcat((char*)buffer,"Accept-Encoding: gzip, deflate, sdch\r\n"); 		
//	strcat((char*)buffer,"Accept-Language: zh-CN,zh;q=0.8\r\n"); 		
	strcat((char*)buffer,"Content-type: application/json;charset=utf8;\r\n"); 		
	
//  sprintf((char*)LocalPoint, "\r\n{\"deviceNo\" : \"SJZ-YZBH01\",\r\n\"portNum\" : \"1\",\r\n\"retainPorts\" : \"12\",\r\n\"province\" : \"0\"}");
//  sprintf((char*)LocalPoint, "\r\n{\"deviceNo\" : \"SJZ-YZBH01\",\r\n\"portNum\" : 2,\r\n\"retainPorts\" : [10,11],\r\n\"province\" : 1}");
//  sprintf((char*)LocalPoint, "\r\n{\"deviceNo\" : \"SJZ-YZBH01\",\r\n\"portNum\" : 4,\r\n\"retainPorts\" : [10,11,12,13],\r\n\"province\" : 1}");
  sprintf((char*)LocalPoint, "{\"deviceNo\" : \"%s\"",ChargeM_init.SN);
  sprintf((char*)LocalPoint, "%s,\r\n\"portNum\" : %d,\r\n\"retainPorts\" : [%d,%d,%d,%d],\r\n\"province\" : %d}",
												LocalPoint,device.PortNum,device.PortId[0],device.PortId[1],device.PortId[2],device.PortId[3],device.province);
	body_len=strlen((char*)LocalPoint);  	//×Ö·û´®³¤¶È
	key_piont = &LocalPoint[body_len+1];
	u16toStr(body_len,key_piont);			//×ª10½øÖÆ×Ö·û´®
	strcat((char*)buffer,"Content-Length: "); 	
	strcat((char*)buffer,(char*)key_piont); 
	strcat((char*)buffer,"\r\n"); 
	strcat((char*)buffer,"\r\n"); //¿ªÊ¼ÕýÎÄ
	strcat((char*)buffer,(char*)LocalPoint); 
	
	strcat((char*)buffer,"\r\n"); 
	
	res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_write(&fnew,buffer,strlen((char*)buffer),&fnum);
	res_sd=f_close(&fnew);	
	
	return ERR_OK;
}
//postÇëÇó
err_t make_http_post(unsigned  char *buffer)
{
	u16 i;
	u16 body_len;
	u16 len;
	unsigned  char *key_piont;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;	
	buffer[0]=0;
	strcat((char*)buffer,"POST "); 
	
	sprintf((char*)LocalPoint, (char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=sizeof("http://");i<len;i++)
	{
		if(LocalPoint[i]=='/')
		{
			key_piont =&LocalPoint[i];
			break;
		}	
	}
	strcat((char*)buffer,(char*)key_piont); 
	strcat((char*)buffer," HTTP/1.1\r\n"); 
	strcat((char*)buffer,"Host: "); 
	strcat((char*)buffer,(char*)HttpM_init.DnsNameStr); 
	strcat((char*)buffer,":"); 
	strcat((char*)buffer,(char*)HttpM_init.PortStr); 
	strcat((char*)buffer,"\r\n"); 
	strcat((char*)buffer,"Connection: keep-alive\r\n"); 		
	strcat((char*)buffer,"Accept: */*\r\n"); 		

	sprintf((char*)LocalPoint, (char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=0;i<len;i++)
	{
		if(LocalPoint[len-i]=='/')
		{
			LocalPoint[len-i] = 0;
			break;
		}	
	}
//	strcat((char*)buffer,"Referer: "); 		
//	strcat((char*)buffer,(char*)LocalPoint); 		
//	strcat((char*)buffer,"\r\n"); 
	sprintf((char*)LocalPoint, "User-Agent: %s %s\r\n",(char*)device.Toolname,(char*)device.Version);
	strcat((char*)buffer,(char*)LocalPoint); 		
//	strcat((char*)buffer,"Accept-Encoding: gzip, deflate, sdch\r\n"); 		
//	strcat((char*)buffer,"Accept-Language: zh-CN,zh;q=0.8\r\n"); 		
	strcat((char*)buffer,"Content-type: application/json;charset=utf8;\r\n"); 		
	
//  sprintf((char*)LocalPoint, "\r\n{\"deviceNo\" : \"SJZ-YZBH01\",\r\n\"portNum\" : 4,\r\n\"retainPorts\" : [10,11,12,13],\r\n\"province\" : 1}");
//  sprintf((char*)LocalPoint, "{\"deviceNo\" : \"%s\"",ChargeM_init.SN);
//  sprintf((char*)LocalPoint, "%s,\r\n\"portNum\" : 4,\r\n\"retainPorts\" : [10,11,12,13],\r\n\"province\" : 1}",LocalPoint);
  sprintf((char*)LocalPoint, "%s",HttpM_init.HttpUrl->DestPath);
	body_len=strlen((char*)LocalPoint);  	//×Ö·û´®³¤¶È
	key_piont = &LocalPoint[body_len+1];
	u16toStr(body_len,key_piont);			//×ª10½øÖÆ×Ö·û´®
	strcat((char*)buffer,"Content-Length: "); 	
	strcat((char*)buffer,(char*)key_piont); 
	strcat((char*)buffer,"\r\n"); 
	strcat((char*)buffer,"\r\n"); //¿ªÊ¼ÕýÎÄ
	strcat((char*)buffer,(char*)LocalPoint); 
	
	strcat((char*)buffer,"\r\n"); 
	
	res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_write(&fnew,buffer,strlen((char*)buffer),&fnum);
	res_sd=f_close(&fnew);	
	
	return ERR_OK;
}
//getÇëÇó
err_t make_http_get(unsigned  char *buffer)
{
	u16 i;
	u16 len;
	unsigned  char *key_piont;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
//GET /sn/index.php?sn=123&n=asa HTTP/1.1
//Accept: */*
//Accept-Language: zh-cn
//host: localhost

//Content-Type: application/x-www-form-urlencoded
//Content-Length: 12
//Connection:close

//GET /Demo2/AServlet HTTP/1.1
//Host: localhost:8080
//Connection: keep-alive
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36
//Accept-Encoding: gzip, deflate, sdch
//Accept-Language: zh-CN,zh;q=0.8,en;q=0.6	
	buffer[0]=0;
	strcat((char*)buffer,"GET "); 
	
	sprintf((char*)LocalPoint,"%s",(char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=sizeof("http://");i<len;i++)
	{
		if(LocalPoint[i]=='/')
		{
			key_piont =&LocalPoint[i];
			break;
		}	
	}
	strcat((char*)buffer,(char*)key_piont); 
	strcat((char*)buffer," HTTP/1.1\r\n"); 
	strcat((char*)buffer,"Host: "); 
	strcat((char*)buffer,(char*)HttpM_init.DnsNameStr); 
	strcat((char*)buffer,":"); 
	strcat((char*)buffer,(char*)HttpM_init.PortStr); 
	strcat((char*)buffer,"\r\n"); 
	strcat((char*)buffer,"Connection: keep-alive\r\n"); 		
	strcat((char*)buffer,"Accept: */*\r\n"); 		

	sprintf((char*)LocalPoint,"%s",(char*)HttpM_init.HttpUrl->url);
	len = strlen((char*)LocalPoint)-1;
	for(i=0;i<len;i++)
	{
		if(LocalPoint[len-i]=='/')
		{
			LocalPoint[len-i] = 0;
			break;
		}	
	}
//	strcat((char*)buffer,"Referer: "); 		
//	strcat((char*)buffer,(char*)LocalPoint); 		
//	strcat((char*)buffer,"\r\n"); 
	sprintf((char*)LocalPoint, "User-Agent: %s %s\r\n",(char*)device.Toolname,(char*)device.Version);
	strcat((char*)buffer,(char*)LocalPoint); 		
//	strcat((char*)buffer,"Accept-Encoding: gzip, deflate, sdch\r\n"); 		
//	strcat((char*)buffer,"Accept-Language: zh-CN,zh;q=0.8\r\n"); 		
	strcat((char*)buffer,"\r\n"); 
	
	res_sd = f_open(&fnew, "logs/log_http.txt", FA_OPEN_ALWAYS | FA_WRITE); 	 		
	res_sd = f_lseek(&fnew, fnew.fsize);
	sprintf((char*)FatFile_URL.FileBufSlave,"time_sys:%d\r\n",time_sys);
	res_sd=f_write(&fnew,FatFile_URL.FileBufSlave,strlen((char*)FatFile_URL.FileBufSlave),&fnum);
	res_sd=f_write(&fnew,buffer,strlen((char*)buffer),&fnum);
	res_sd=f_close(&fnew);	
	
	return ERR_OK;
}
//
err_t LCD_task(void)
{	
	u8 i;
	u16 temp_x;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(LCDC.SW_state  == SW_NULL)
	{
		LCDC.SW_state = SW_ON;
		LCDC.Mode = 0;
		LCDC.SPTimeSet = 30000;
	  LCDC.TimeTimerSet =1000;
		
		for(i=0;i<DefPortNum;i++)
		{
			LCDC.LcdNumber[i] =(i%2)+1;
			if(i>=2)   
			{
				LCDC.LcdNumber[i]=0;  //Ã»ÓÐÆÁ
			}
		}
		
		for(i=0;i<DefPortNum;i++)
		{
			LCDC.TimeTimer[i] = time_sys;
			LCDC.SPTime[i] = time_sys;
			if(LCDC.LcdNumber[i]>0)
			{
			sprintf((char*)LocalPoint, "%s:%d",device.Toolname,(LCDC.LcdNumber[i]));
			tft_DisplayStr(320-16, 0, LocalPoint,BLACK, RED,LCDC.LcdNumber[i]);			
			}
		}
		
		if(device.SD_state !=FR_OK)
		{
			sprintf((char*)LocalPoint, "SD card error:%d",device.SD_state);
			tft_DisplayStr(0, 0, LocalPoint,BLACK, RED,3);						
		}
	}
	else
	{		
		if(LCDC.Mode==0)  //Õý³£Ä£Ê½
		{
			for(i=0;i<DefPortNum;i++)
			{
				if(time_sys-LCDC.TimeTimer[i]>LCDC.TimeTimerSet && (LCDC.LcdNumber[i]>0))
				{			
					LCDC.TimeTimer[i] +=LCDC.TimeTimerSet;
					if(device.PortPowerUseTime[i])
					{
						if(LCDC.SPID[i]<2)
						{
							device.PortPowerUseTime[i]++;
							LCDC.SPTime[i] = time_sys-LCDC.SPTimeSet;							
							res_sd = f_open(&fsrc, "area2/04.bmp", FA_OPEN_EXISTING | FA_READ); 	 		
							if(res_sd == FR_OK)
							{				
								res_sd = f_read (&fsrc, LCD_TxtBuffer[i], fsrc.fsize, &fnum);									
								LCD_TxtBuffer[i][2048]=0;
								LCD_TxtBuffer[i][2049]=0;
							}							
							res_sd = f_close(&fsrc); 	 		
							continue;
						}
						if(FatFile_AD.SW_state == SW_OFF)  //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						{
							FatFile_AD.SW_state = SW_ON;    //
							FatFile_AD.NetState = NET_FLASH_ON;
							
							FatFile_AD.f_area = 9;							
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/T%d.bmp",(device.PortPowerUseTime[i]%3600/600));
							bmp_print_unit(14,118,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/T%d.bmp",(device.PortPowerUseTime[i]%600/60));
							bmp_print_unit(14,139,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/T%d.bmp",(device.PortPowerUseTime[i]%60/10));
							bmp_print_unit(14,174,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/T%d.bmp",(device.PortPowerUseTime[i]%10));
							bmp_print_unit(14,195,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
//							display_flash_BMPE (14,118,3,((Uport_PowerUseTime[0]%3600/600)+'0'),3);//?? ?H
//							display_flash_BMPE (14,139,3,((Uport_PowerUseTime[0]%600/60)+'0'),3);//?? ?L
//							display_flash_BMPE (14,174,3,((Uport_PowerUseTime[0]%60/10)+'0'),3);//?? ?H
//							display_flash_BMPE (14,195,3,((Uport_PowerUseTime[0]%10)+'0'),3);//?? ?L							
							
							tft_1bitdeep_TXT (87, 0, LCD_TxtBuffer[i],POINT_COLOR, 0xffff,LCDC.LcdNumber[i]);							
							FatFile_AD.SW_state = SW_OFF; //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						}
					}		
				}
			}
			
			for(i=0;i<DefPortNum;i++)
			{
				if(time_sys-LCDC.SPTime[i]>LCDC.SPTimeSet && (LCDC.LcdNumber[i]>0))
				{			
					LCDC.SPTime[i] = time_sys;
					tft_Clear(0,0,320,240,WHITE,LCDC.LcdNumber[i]);		

					if(device.PortPowerUseTime[i])
					{
						if(FatFile_AD.SW_state == SW_OFF)  //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						{
							FatFile_AD.SW_state = SW_ON;    //
							FatFile_AD.NetState = NET_FLASH_ON;
							LCDC.SPID[i] =2;		
							FatFile_AD.f_area = 9;
	//						FatFile_AD.f_unm = LCDC.SPID[i]; 
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/P%d.bmp",LCDC.SPID[i]);
							bmp_print_unit(0,0,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
							
							FatFile_AD.SW_state = SW_OFF; //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						}
					}		
					else
					{
						if(FatFile_AD.SW_state == SW_OFF)  //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						{
							FatFile_AD.SW_state = SW_ON;    //
							FatFile_AD.NetState = NET_FLASH_ON;
							LCDC.SPID[i]++;		
							if(LCDC.SPID[i]>1)
							{
								LCDC.SPID[i] = 0;
							}
							FatFile_AD.f_area = 9;
							sprintf((char*)FatFile_AD.FileSourcePath, "area2/P%d.bmp",LCDC.SPID[i]);
							bmp_print_unit(0,0,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£									
						
							sprintf((char*)FatFile_AD.FileSourcePath, "%s",device.PortSourcePath[i]);
		//					tft_DisplayStr(270, 125, device_num,0x0000,0xffff,1);	
							if(LCDC.SPID[i]==0)
							{
								bmp_print_unit(80,(240-128)>>1,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
								sprintf((char*)LocalPoint, "%s:%d",device.Toolname,(LCDC.LcdNumber[i]));
								tft_DisplayStr(270, 110, LocalPoint,BLACK, WHITE,LCDC.LcdNumber[i]);	
							}
							else if(LCDC.SPID[i]==1)
							{
								bmp_print_unit(115,(240-128)>>1,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
								sprintf((char*)LocalPoint, "%s:%d",device.Toolname,(LCDC.LcdNumber[i]));
								tft_DisplayStr(15, 110, LocalPoint,BLACK, WHITE,LCDC.LcdNumber[i]);	
							}
							FatFile_AD.SW_state = SW_OFF; //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
						}
					}		
				}
			}
		}
		else if(LCDC.Mode==1)
		{
			for(i=0;i<DefPortNum;i++)
			{
				if(time_sys-LCDC.SPTime[i]>LCDC.SPTimeSet && (LCDC.LcdNumber[i]>0))
				{			
					LCDC.SPTime[i] = time_sys;					
					tft_Clear(160,0,160,240,RED,LCDC.LcdNumber[i]);				
					if(FatFile_AD.SW_state == SW_OFF)  //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
					{
						FatFile_AD.SW_state = SW_ON;    //
						FatFile_AD.NetState = NET_FLASH_ON;
						
						FatFile_AD.f_area = 9;	//from SD
//						FatFile_AD.f_area = 1;  //from flash 
						FatFile_AD.f_unm = LCDC.LcdNumber[i]-1; 
						sprintf((char*)FatFile_AD.FileSourcePath, "%s",device.PortSourcePath[i]);
					//extern void bmp_print_unit(u16 x,u16 y,struct FatFileTable BMP,u8 cs);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£
						bmp_print_unit(176,(240-128)>>1,FatFile_AD,LCDC.LcdNumber[i]);//ÏÔÊ¾Í¼Æ¬µ¥Ôª¡£		
						FatFile_AD.SW_state = SW_OFF; //¹Ø±ÕÊ±ÉêÇëÊ¹ÓÃ
					}				
					sprintf((char*)LocalPoint, "%s:%d",device.Toolname,(LCDC.LcdNumber[i]));
					tft_DisplayStr(320-16, 0, LocalPoint,BLACK, RED,LCDC.LcdNumber[i]);						
				}
			}
			temp_x =  320-16*2;
			for(i=0;i<DefPortNum;i++)
			{
				if((time_sys-LCDC.TimeTimer[i]>LCDC.TimeTimerSet))
				{
					LCDC.TimeTimer[i] += LCDC.TimeTimerSet;
					sprintf((char*)LocalPoint, "S:%d",device.PortPowerSetTime[i]);
					tft_DisplayStr(temp_x-32*i, 0, LocalPoint,BLACK, RED,3);
					sprintf((char*)LocalPoint, "U:%d",device.PortPowerUseTime[i]);
					tft_DisplayStr(temp_x-32*i-16, 0, LocalPoint,BLACK, RED,3);
					
					sprintf((char*)LocalPoint, "KV:%d",(fkey.KeyValue));
					tft_DisplayStr(320-16, 190, LocalPoint,BLACK, RED,(LCDC.LcdNumber[i]));
				}
			}
		}
	}
	return ERR_OK;
}
err_t charge_task(void)
{
//	unsigned  int i;
	
	
//	GPIO_EN_HV(EN_HV1_PORT, EN_HV1_PIN,ON);
//	GPIO_EN_HV(EN_HV2_PORT, EN_HV2_PIN,ON);
//	GPIO_EN_HV(EN_HV3_PORT, EN_HV3_PIN,ON);
//	GPIO_EN_HV(EN_HV4_PORT, EN_HV4_PIN,ON);
//	GPIO_EN_HV(EN_HV5_PORT, EN_HV5_PIN,ON);
//	GPIO_EN_HV(EN_HV6_PORT, EN_HV6_PIN,ON);

//	GPIO_EN_TPS54336(EN_TPS54336_1_PORT, EN_TPS54336_1_PIN,ON);
//	GPIO_EN_TPS54336(EN_TPS54336_2_PORT, EN_TPS54336_2_PIN,ON);
//	GPIO_EN_TPS54336(EN_TPS54336_3_PORT, EN_TPS54336_3_PIN,ON);
//	GPIO_EN_TPS54336(EN_TPS54336_4_PORT, EN_TPS54336_4_PIN,ON);
//	GPIO_EN_TPS54336(EN_TPS54336_5_PORT, EN_TPS54336_5_PIN,ON);
//	GPIO_EN_TPS54336(EN_TPS54336_6_PORT, EN_TPS54336_6_PIN,ON);
//	
//	GPIO_EN_FAN(F11_EN_FAN_PORT, F11_EN_FAN_PIN,ON);
//	GPIO_EN_9V(B1_EN1_9V_PORT, B1_EN1_9V_PIN,ON);
//	GPIO_EN_9V(B0_EN2_9V_PORT, B0_EN2_9V_PIN,ON);	
	
//	for(i=0;i<DefPortNum;i++)
	{
		if(time_sys-device.ChargeTimer>device.ChargeTimerSet)
		{
			device.ChargeTimer += device.ChargeTimerSet;
			Get_ADC_BaseLine();
			if(device.PortPowerUseTime[0]>0)
			{
				device.PortPowerUseTime[0]--;
				GPIO_EN_TPS54336(EN_TPS54336_1_PORT, EN_TPS54336_1_PIN,ON);
				GPIO_EN_TPS54336(EN_TPS54336_2_PORT, EN_TPS54336_2_PIN,ON);
				GPIO_EN_TPS54336(EN_TPS54336_3_PORT, EN_TPS54336_3_PIN,ON);
			}	
			else
			{
				GPIO_EN_TPS54336(EN_TPS54336_1_PORT, EN_TPS54336_1_PIN,OFF);
				GPIO_EN_TPS54336(EN_TPS54336_2_PORT, EN_TPS54336_2_PIN,OFF);
				GPIO_EN_TPS54336(EN_TPS54336_3_PORT, EN_TPS54336_3_PIN,OFF);
			}
			if(device.PortPowerUseTime[1]>0)
			{
				device.PortPowerUseTime[1]--;
				GPIO_EN_TPS54336(EN_TPS54336_4_PORT, EN_TPS54336_4_PIN,ON);
				GPIO_EN_TPS54336(EN_TPS54336_5_PORT, EN_TPS54336_5_PIN,ON);
				GPIO_EN_TPS54336(EN_TPS54336_6_PORT, EN_TPS54336_6_PIN,ON);		
			}	
			else
			{
				GPIO_EN_TPS54336(EN_TPS54336_4_PORT, EN_TPS54336_4_PIN,OFF);
				GPIO_EN_TPS54336(EN_TPS54336_5_PORT, EN_TPS54336_5_PIN,OFF);
				GPIO_EN_TPS54336(EN_TPS54336_6_PORT, EN_TPS54336_6_PIN,OFF);					
			}
			if(device.PortPowerUseTime[2]>0)
			{
				device.PortPowerUseTime[2]--;
			}	
			if(device.PortPowerUseTime[3]>0)
			{
				device.PortPowerUseTime[3]--;
			}	
		}
	}
	return ERR_OK;
}
//°´¼üÈÎÎñ
err_t key_task(void)
{
	unsigned  char i;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(fkey.SW_state == NULL)
	{
		fkey.SW_state = SW_OFF;
		fkey.NetState = NET_OFF;
		fkey.AddTimerSet = 100;
		fkey.OkTimerSet = 3000;
		fkey.RetTimerSet = 10000;
		fkey.PutInTimer = 0;
	}
	else
	{
		if((GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN) == KEY_ON))
		{
			if(fkey.SW_state==SW_OFF)
			{
				fkey.SW_state = SW_ON;
				fkey.PutInTimer = time_sys;
			}
			else if(fkey.SW_state==SW_ON)
			{
				fkey.SW_state = SW_ON_DONE;
			}
		}
		else
		{
			if(fkey.SW_state==SW_ON_DONE)
			{
				fkey.SW_state = SW_OFF;
				if(time_sys-fkey.PutInTimer>fkey.OkTimerSet)
				{
					if(fkey.KeyValue==1)       //¶Ïµç
					{
						for(i=0;i<DefPortNum;i++)
						{
							device.PortPowerSetTime[i] = 0;
							device.PortPowerUseTime[i] = device.PortPowerSetTime[i];
						}
					}
					else if(fkey.KeyValue==2)  //ÉÏ¶Ìµç
					{
						for(i=0;i<DefPortNum;i++)
						{
							device.PortPowerSetTime[i] = 60;
							device.PortPowerUseTime[i] = device.PortPowerSetTime[i];
						}
					}
					else if(fkey.KeyValue==3)	 //ÉÏ³¤µç
					{
						for(i=0;i<DefPortNum;i++)
						{
							device.PortPowerSetTime[i] = 0xffff;
							device.PortPowerUseTime[i] = device.PortPowerSetTime[i];
						}
					}
					else if(fkey.KeyValue==4)    //ÏÔÊ¾Éè±¸ÐÅÏ¢1
					{
						
					}
					else if(fkey.KeyValue==5)    //ÏÔÊ¾Éè±¸ÐÅÏ¢2
					{
						sprintf((char*)LocalPoint, "CCKJ CHARGE Version:%s\n",device.Version);
						Str_addto_Str(&DebugStr,LocalPoint);
						sprintf((char*)LocalPoint, "SELF_MAC: %d.%d.%d.%d.%d.%d\n",soket_lwip.mac[0],soket_lwip.mac[1],soket_lwip.mac[2],soket_lwip.mac[3],soket_lwip.mac[4],soket_lwip.mac[5]);
						Str_addto_Str(&DebugStr,LocalPoint);
						sprintf((char*)LocalPoint, "SELF_IP: %d.%d.%d.%d\n",(uint8_t)(gnetif.ip_addr.addr),(uint8_t)(gnetif.ip_addr.addr >> 8),(uint8_t)(gnetif.ip_addr.addr >> 16),(uint8_t)(gnetif.ip_addr.addr >> 24));
						Str_addto_Str(&DebugStr,LocalPoint);
						sprintf((char*)LocalPoint, "SOKET_IP: %d.%d.%d.%d.%d\n",soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
						Str_addto_Str(&DebugStr,LocalPoint);	
						sprintf((char*)LocalPoint, "HTTP_IP: %d.%d.%d.%d.%d\n",http_lwip.remoteip[0],http_lwip.remoteip[1],http_lwip.remoteip[2],http_lwip.remoteip[3],http_lwip.remote_port);
						Str_addto_Str(&DebugStr,LocalPoint);	
						sprintf((char*)LocalPoint, "UDPDEBUG_IP: %d.%d.%d.%d.%d\n",udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3],udpdebug_lwip.remote_port);
						Str_addto_Str(&DebugStr,LocalPoint);	
						tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					}
					else if(fkey.KeyValue==6)    //ÍøÂçÖØÆô
					{
						ETH_BSP_Config();		
						printf("PHY³õÊ¼»¯½áÊø\n");
						
						/* Initilaize the LwIP stack */
						lwip_default_ip_set();
						LwIP_Init();	
						dns_init();

						ChargeM_init.LastRxTime = time_sys;
						ChargeM_init.NetState = NET_OFF;
						ChargeM_init.SW_state = SW_OFF;
						HttpM_init.SW_state = SW_ON;			//¿ªÆôµÄHTTP
						HttpU_buffer[0].SW_state = SW_ON;			//¿ªÆôµÄSSIÈÎÎñ	
						HttpU_buffer[0].NetState = NET_ON;			//¿ªÆôµÄSSIÈÎÎñ	
						HttpM_init.HttpUrl->NetState = (unsigned char)NET_OFF_DONE;

						Str_addto_Str(&DebugStr,(unsigned char*)"service network restart\r\n");
						tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
						printf("service network restart\r\n");
					}
					else if(fkey.KeyValue==7)    //²Á³ý×ÊÔ´
					{
					 /* ¸ñÊ½»¯ */
						res_sd=f_mkfs("0:",0,0);							
						dwgl_FatInit();
					}
					else if(fkey.KeyValue==8)    //
					{}
					else if(fkey.KeyValue==9)    //²ÁAÀà
					{}
					else if(fkey.KeyValue>10&&fkey.KeyValue<20)    //·Ö±ðÉÏµç
					{						
						if((fkey.KeyValue-10<=DefPortNum)&&(time_sys-fkey.PutInTimer>fkey.RetTimerSet))
						{
							device.PortPowerSetTime[fkey.KeyValue-10-1] = (time_sys-fkey.PutInTimer-fkey.RetTimerSet)/1000*60;  //
							device.PortPowerUseTime[fkey.KeyValue-10-1] = device.PortPowerSetTime[fkey.KeyValue-10-1];
						}
					}
					else if(fkey.KeyValue%10 == 0)  //·µ»Ø
					{
						fkey.KeyValue = 0;
						LCDC.Mode = 0;					
						for(i=0;i<DefPortNum;i++)
						{
							LCDC.SPTime[i] = time_sys-LCDC.SPTimeSet;
						}
					}
				}
				else if(time_sys-fkey.PutInTimer>fkey.AddTimerSet)
				{
					if(LCDC.Mode==0)
					{
						LCDC.Mode = 1;
						for(i=0;i<DefPortNum;i++)
						{
							LCDC.SPTime[i] = time_sys-LCDC.SPTimeSet;
						}
					}
					else
					{
						fkey.KeyValue++;
					}
				}
			}
			else if(fkey.SW_state==SW_ON)
			{
				fkey.SW_state = SW_OFF;
			}				
		
		}	
	}
	return ERR_OK;
}

