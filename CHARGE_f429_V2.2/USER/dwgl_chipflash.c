//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#include "stm32f10x.h"
/***********************************************************************
�ļ����ƣ�dwgl_chipflash.C
��    �ܣ���ɶ�Ƭ�ϴ洢�Ĳ���
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺��������ͨ���ж������ض����ַ���ȷ��һ֡�����Ƿ�����ġ�
***********************************************************************/
 
//---------------------------------------------------------------------------------
u16 base_add;
unsigned char APP_EN;
unsigned char KEEP_EN = 0;
unsigned char PRO_State;
iapfun jump2app; 

void CHIP_PRO(void)
{
	unsigned int i,j;
	unsigned int f_size,f_addr,chip_addr;
	unsigned short int *p;
	FLASH_Status temp;
	
	FLASH2_GPIOSPI_Read (Addr_info2, &info2STR.head[0], sizeof(info2STR));  //ý�ݳ�ʼ��
	if(info2STR.item21_data[2] ==1)
	{
	FLASH2_GPIOSPI_Read (Addr_01min, str_buffer, 256);
	}
	else
	if(info2STR.item21_data[2] ==2)
	{
	FLASH2_GPIOSPI_Read (Addr_02min, str_buffer, 256);
	}
	else
	if(info2STR.item21_data[2] ==3)
	{
	FLASH2_GPIOSPI_Read (Addr_03min, str_buffer, 256);
	}
	else
	if(info2STR.item21_data[2] ==4)
	{
	FLASH2_GPIOSPI_Read (Addr_04min, str_buffer, 256);
	}
	else
	if(info2STR.item21_data[2] ==5)
	{
	FLASH2_GPIOSPI_Read (Addr_05min, str_buffer, 256);
	}
				f_addr = 0;
				f_size = 0;
	if(str_buffer[1]>13)
		{	
			str_buffer[1] = 13;	
		}
		
		for(i=0;i<str_buffer[1];i++)
		{
		if(str_buffer[0]!=frame_headerC)	
			{		break;	}			
				if(str_buffer[4+18*i+1]==info2STR.item21_data[3])//ȷ���ļ���
				{
				f_size = str_buffer[4+18*i+2];
				f_size <<= 8;
				f_size += str_buffer[4+18*i+3];
				f_size <<= 8;
				f_size += str_buffer[4+18*i+4];
				f_size <<= 8;
				f_size += str_buffer[4+18*i+5];	
					
				f_addr = str_buffer[4+18*i+14];
				f_addr <<= 8;
				f_addr += str_buffer[4+18*i+15];
				f_addr <<= 8;
				f_addr += str_buffer[4+18*i+16];
				f_addr <<= 8;
				f_addr += str_buffer[4+18*i+17];
				break;
				}
		}
				
	if(f_size>0)
		{			
     FLASH_Unlock();  //FLASH����
		 FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//?????

		p = (unsigned short int*)str_buffer;
		chip_addr = FLASH_APP1_ADDR;    //ƫ��64K
//		f_size >>=1;
		for(i=0;i<=f_size;)   //����ҳ=2K
			{
			while(1)
				{
				 temp=FLASH_ErasePage(chip_addr+i);     //???????
					if(temp==FLASH_COMPLETE)
					{break;}
				}
			i+=0x800;
			}

		i=0;
		while(f_size)
		{	
		 if((chip_addr&0xfffff800)==chip_addr)
		 {
//		 FLASH2_RAMSPI_Read (f_addr+2*i, str_buffer, 2048);   
 		 FLASH2_GPIOSPI_Read (f_addr+2*i, str_buffer, 2048);  //ý�ݳ�ʼ��
			j=0;
		 }		 
		 temp =FLASH_BUSY;
			while(temp!=FLASH_COMPLETE)
			{
     temp = FLASH_ProgramHalfWord(chip_addr,p[j]); //?????addr?????
			}
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//?????
		 
		i++;
		j++;
		chip_addr+=2;
		f_size -=2;
		}
		FLASH_Lock();    //??FLASH???????
		
		i = 1000;
		while(i--);		
	 }


// /* ����FLASH  */
// ��FLASH_Lock();
//   /*����FLASH*/
// ��FLASH_Unlock();
//   /* ������й����־λ */
// �� FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);        
//   /* ����FLASH ҳ*/
// ��for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
// ����  {
// ����    FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
// ����  }
//   /* д��FLASH  */
// ��Address = WRITE_START_ADDR;
// ��while((Address < WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
// ����  {
// ����    FLASHStatus = FLASH_ProgramWord(Address, Data);
// ����    Address = Address + 4;
// ����  }
// /* ����FLASH  */
// ��FLASH_Lock();

}


//------------------------------------------------------
void Check_CHIP_PRO(void)   //���������
{
 	unsigned char temp = 0;
 	unsigned char  i= 0;
	APP_EN = 0;		
	FLASH2_GPIOSPI_Read (Addr_info2, &info2STR.head[0], sizeof(info2STR));  //ý�ݳ�ʼ��
	
	if((info2STR.item21[0]==21)&&(info2STR.item21[1]==4))  //
	{
		if(device.Version[0]==Version_FLAG1)
		{
			if(info2STR.item21_data[1]>=3||info2STR.item21_data[0]==0xE0)//���²���ִ������,�Ի�
			{
				info2STR.item21[0] = 21;
				info2STR.item21[1] = 4;
				for(i=0;i<4;i++)
				{
				info2STR.item21_data[i] = 0x00;
				}				
				
				info2STR.item15[0] = 15;
				info2STR.item15[1] = 0;
				for(i=0;i<16;i++)
				{
				info2STR.item15_data[i] = 0xff;
				}				
			  FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
				FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
				
			  FLASH_Unlock();  //FLASH����
			  FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//?????
				while(1)
					{
					 temp=FLASH_ErasePage(0x08010000);     //�����û�����
						if(temp==FLASH_COMPLETE)
						{break;}
					}
			  FLASH_Lock();    //??FLASH???????
				APP_EN = 0X00;
			}
			else
			if(info2STR.item21_data[0]==1)//��������
			{
				info2STR.item21_data[1]++; //
			  FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
				FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
				CHIP_PRO();  //�����µ�
				APP_EN = 0X01;
			}
			else
			if((((*(vu32*)(0X08010000))&0XFFF00000)==0x20000000) && (((*(vu32*)(0X08010004))&0XFFF00000)==0x08000000))   //��ջ�Ϸ�
			{
				APP_EN = 0X01;
			}
			else
			{
				APP_EN = 0X00;
			}
		}
		else   //�û�����
		{
			if(info2STR.item21_data[0]==1)//��������
			{
				info2STR.item21[0] = 21;
				info2STR.item21[1] = 4;
				for(i=0;i<4;i++)
				{
				info2STR.item21_data[i] = 0x00;
				}				
			  FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
				FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
			}
				APP_EN = 0X00;
		}		
	}
	else
	{
		if(device.Version[0]==Version_FLAG1)
		{
				info2STR.item21[0] = 21;
				info2STR.item21[1] = 4;
				for(i=0;i<4;i++)
				{
				info2STR.item21_data[i] = 0x00;
				}				
			  FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
				FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
			
			if((((*(vu32*)(0X08010000))&0XFFF00000)==0x20000000) && (((*(vu32*)(0X08010004))&0XFFF00000)==0x08000000))   //��ջ�Ϸ�
			{
				APP_EN = 0X01;
			}
			else
			{
				APP_EN = 0X00;
			}
		}
		else   //�û�����
		{
			if(info2STR.item21_data[0]==1)//��������
			{
				info2STR.item21[0] = 21;
				info2STR.item21[1] = 4;
				for(i=0;i<4;i++)
				{
				info2STR.item21_data[i] = 0x00;
				}				
			  FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
				FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
			}
				APP_EN = 0X00;
		}
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
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
	else
	{
		FLASH2_GPIOSPI_Read (Addr_info2, &info2STR.head[0], sizeof(info2STR));  //ý�ݳ�ʼ��
		if(info2STR.item15[1]!=0)
		{
		info2STR.item15[0] = 15;
		info2STR.item15[1] = 0;   //�汾д��
		FLASH2_GPIOSPI_SER(Addr_info2);  ////ÿ�β���4K
		FLASH2_GPIOSPI_Write(Addr_info2, &info2STR.head[0], sizeof(info2STR));
		}
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
 	unsigned char  temp= 0;
 	unsigned int   aa,bb;
	PRO_State = 0;
	
	FLASH2_GPIOSPI_Read (Addr_info2, &info2STR.head[0], sizeof(info2STR));  //ý�ݳ�ʼ��
	if(info2STR.item21_data[0]==1)//��������
	{
		PRO_State |= 0x01; 
	}
	if((info2STR.item15[1]<=16)&&(info2STR.item15[1]>0))//�о���
	{
		PRO_State |= 0x02; 
	}
		aa = ((*(vu32*)(0X08000000)));
		bb = ((*(vu32*)(0X08000004)));
		aa = ((*(vu32*)(0X08010000)));
		bb = ((*(vu32*)(0X08010004)));
	if((aa&0XFFF00000)==0x20000000&&(bb&0XFFF00000)==0x08000000)   //�г���
	{
		PRO_State |= 0x04; 
	}
	if(KEEP_EN==0xff)
	{
		PRO_State |= 0x08; 
	}
	if(info2STR.item21_data[1]>0)   //����ʧ��
	{
		temp = info2STR.item21_data[1]&0x0f;
		temp <<=4;
		PRO_State |= temp; 
	}
	
	UART_BUFFER[0] = 'B';
	UART_BUFFER[1] = 'L';
	UART_BUFFER[2] = '=';
	UART_BUFFER[3] = (((PRO_State)&0X01)+'0');
	UART_BUFFER[4] = (((PRO_State>>1)&0X01)+'0');
	UART_BUFFER[5] = (((PRO_State>>2)&0X01)+'0');
	UART_BUFFER[6] = (((PRO_State>>3)&0X01)+'0');
	UART_BUFFER[7] = (((PRO_State>>4)&0X07)+'0');
	UART_BUFFER[8] = 0;
	tft_DisplayStr(120, (240-8*8)/2, UART_BUFFER,POINT_COLOR, BACK_COLOR,3);
	if(info2STR.item3[0] == 3 && info2STR.item3[1]==2)//��3��Ч
		{
		}
		else
		if(device.Version[0]==Version_FLAG1)//
		{
			info2STR.item3_data[0] =  0x01;  //������ʾ
			info2STR.item3_data[1] =  0x01;
		}
		
			
	tft_cs_enable(3);
	LCD_WindowMax(289,0,289,239);	           //���ô���	dwgl	����1	
	LCD_WriteRAM_Prepare();  		//��ʼд��GRAM  
	aa = 240;
	bb = (time_sys);
	while(aa)
	{
		aa--;
		LCD_WE_RAM(bb);                             //��������ɫ һ����
	}			
	LCD_WindowMax(0,0,HDP,VDP);      //�ָ������С
	tft_cs_disable(3);
}
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
		LCD_SetCursor(len+X,wide+Y);           //���ù��λ��  
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
