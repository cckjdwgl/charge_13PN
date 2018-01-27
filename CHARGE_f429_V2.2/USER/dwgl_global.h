/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dwgl_global_H
#define __dwgl_global_H



/* Includes ------------------------------------------------------------------*/
#include "memp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "netconf.h"
#include "tcp_demo.h"
#include "udp_demo.h" 

#include "stm32f4xx.h"
#include "stm32f429_phy.h"
#include "dwgl_bsp_gpio.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/key/bsp_key.h"
#include "dwgl_WB_LCD.h"
#include "dwgl_tft.h"
//#include "dwgl_spi.h"
#include "bsp_spi_flash.h"
#include "./adc/bsp_adc.h"
#include "HzLib_65k.h"
#include "dwgl_tim.h"
#include "dwgl_function.h"



//#include "ff.h"
//typedef unsigned   char    u8;
//typedef signed     char    s8;
//typedef unsigned   short   u16;
//typedef signed     short   s16;
//typedef unsigned   long    u32;
//typedef signed     long    s32;

#define PCB_VERSION_AC    10	//
#define PCB_VERSION_BC    11	//
#define PCB_VERSION_QC    12	//
#define PCB_VERSION_MC    13	//

#define DefPortNum      4  //��·����Ķ˿���
#define PCB_VERSION    PCB_VERSION_MC	//΢��˫���
//cmd
#define frame_headerC				0x67
#define frame_headerD				0x68
#define frame_last					0x99
#define Version_FLAG1				('B')  //�汾��־1
//#define port_num					0x02
#define cmd_set_addr				0xe2  //������ư��ַ����

#define Addr_01min					0x000000   //��ά���ַʼ
#define Addr_01max					0x03ffff   //��ά���ַĩ
#define Addr_02min					0x080000   //СͼƬ��ַʼ
#define Addr_02max					0x09FFFF   //СͼƬ��ַĩ
#define Addr_03min					0x100000   //��ͼƬ��ַʼ
#define Addr_03max					0x2FFFFF   //��ͼƬ��ַĩ
#define Addr_04min					0x400000   //��ͼƬ��ַʼ
#define Addr_04max					0xBFFFFF   //��ͼƬ��ַĩ
#define Addr_05min					0xE00000   //�����ַ��
#define Addr_05max					0xEFFFFF   //�����ַĩ
#define Addr_info1					0xF00000   //��Ϣ��ַ1
#define Addr_info2					0xF10000   //��Ϣ��ַ2

#define GLOBLE_ADDR					0xE0
#define PC_ADDR						0xF0
#define Broadcast					0xFF

#define ADC_BUFFER_SIZE				320

#define ADC_LINE1					0x0c0  //����Щ��ѹ��Ϊ���ֻ�;=0xc0����Ϊ310mV,�������Ҫ������
#define ADC_LINE2					0x029	//0x080  //����������Χ ;=0x80 ����Ϊ20mA
#define ADC_LINE3					0x01f	//0x060  //����������Χ ;=0x60 ����Ϊ15mA  ����С�ڴ�ֵ��ʾû��ʹ��
//#define ADC_LINE2					0x0100  //����������Χ ;=0x100 ����Ϊ40mA

#define UART1_RXBUFFER_SIZE			0x200
#define UART1_RX_MAX				0x1FF
#define UART3_RXBUFFER_SIZE			0x80
#define UART3_RX_MAX				0x7f

struct  Addr_info2STR {
	u8 head[2];
	u8 temp[3];
	u8 item1[2];		//�������������ֳ� 
	u8 item1_data[4]; 
	u8 item2[2];		//���������������ֳ�      
	u8 item2_data[4]; 
	u8 item3[2];		//ͼƬ��ǿ����������ֳ�      
	u8 item3_data[2]; 
	u8 item11[2];		//ͼƬ�汾�����������ֳ�      
	u8 item11_data[16]; 
	u8 item12[2];		//ͼƬ�汾�����������ֳ�      
	u8 item12_data[16]; 
	u8 item13[2];		//ͼƬ�汾�����������ֳ�      
	u8 item13_data[16]; 
	u8 item14[2];		//ͼƬ�汾�����������ֳ�      
	u8 item14_data[16]; 
	u8 item15[2];		//ͼƬ�汾�����������ֳ�      
	u8 item15_data[16]; 
	u8 item21[2];		//��������趨��
	u8 item21_data[4];
	u8 item81[2];		//�豸����
	u8 item81_data[16]; 
	u8 item31[2];		//B�����汾�������ֳ� 
	u8 item31_data[16];
	u8 item32[2];		//A�����汾�������ֳ� 
	u8 item32_data[16];
};	

struct  device_table {
	u8 head;
	u8 SW_state;  		//״̬����
	u8 SD_state;  		//SD״̬
	unsigned  int ChargeTimer;  		//
	unsigned  int ChargeTimerSet;  	//���һRX��ʱ��
	u8 Toolname[16];
	u8 Version[16];
	u8 PortNum; 
	u8 PortId[DefPortNum];					//ֻ����ǰ����������˿ں�
	u8 PortState[DefPortNum];				//�˿ڶ�ά���Ƿ��������
	u8 PortName[DefPortNum][16];		//
	u8 PortSourcePath[DefPortNum][128];		//��Դ��ַ
	u8 PortDestPath[DefPortNum][128];   //�����浽��·��
	u16 PortPowerSetTime[DefPortNum];		//
	u16 PortPowerUseTime[DefPortNum];		//
	u16 PortPowerShowTime[DefPortNum];	//
	u16 PortCurrent[DefPortNum][60];		//  mA
	u8 use;						//ʹ����� bit0=1���Ӷϣ� bit1=1��ַ���ά���ͻ�� bit2=1��0���޶�ά�� �� bit3=1��1���޶�ά�� ��bit4=1����ά���и���
	u8 TASK_state;		//��������Գ�����Խ��й���ģʽ������ģʽ
	u8 province;      //ʡ��
};	  


//str_buffer[0]=	//ͷ0X67
//str_buffer[?]=	//LEN
//str_buffer[?]=	//�ļ���u8
//str_buffer[?]=	//�ļ���u8
//str_buffer[?]=	//�ļ���u64
//str_buffer[?]=	//�ļ���Сu32
//str_buffer[?]=	//�ļ����ַu32
//str_buffer[?]=	//check
//str_buffer[?]=	//β0X99

#define LCD1_INDEX			(0)
#define LCD2_INDEX			(1)

struct  LCDREG {
	unsigned char Head;					 //ͷ
	unsigned char SW_state;  		 //����״̬����
	unsigned char NetState;      //״̬
	unsigned char Mode;  				 //LCD ģʽ
	unsigned char LcdNumber[DefPortNum];  //LCD ����
	unsigned int TestTimerSet;  					//�����Ļ����趨ֵ
	unsigned int TestTimer[DefPortNum];   //
	unsigned int TimeTimerSet;  					//��ʱ��ʾ����趨ֵ
	unsigned int TimeTimer[DefPortNum];   //��ʱ��ʾ���
	unsigned char  SPSwitch;			        //��������
	unsigned int SPTimeSet;		           	//�������        
	unsigned int SPTime[DefPortNum];		  //�����������
	unsigned char  SPID[DefPortNum];			//����ID
	unsigned char  ADSwitch;				      //��濪��
	unsigned char  ADNum;				          //������
	unsigned int ADTimeSet;		          	//�����        
	unsigned int ADTime[DefPortNum];		  //���������
	unsigned char  ADID[DefPortNum];			//���ͼƬID
	
	unsigned char POWEROFFTIME[DefPortNum];		//�ϵ�ʱ��
};	 

struct  KEYREG {
	unsigned char SW_state;  		 //����״̬����
	unsigned char NetState;      //״̬
	unsigned char KeyValue;  		//����ֵ��
	unsigned int PutInTimer;  	//����ʱ��
	unsigned int OkTimerSet;   	//ȷ����ʱ��
	unsigned int AddTimerSet;   //�Ӻż�ʱ��
	unsigned int RetTimerSet;   //���ؼ�ʱ��
};	 

#define NET_NULL   		0
#define NET_ON  			1
#define NET_ON_DONE 	2
#define NET_OFF 			3
#define NET_OFF_DONE 	4
#define NET_DNS_OK 	  5
#define NET_HTTP_OK 	6		 //����Ӧ
#define NET_SOCKET_OK 7    //����Ӧ
#define NET_UDP_OK 	  8
#define NET_FAT_ON 	  9
#define NET_FLASH_ON 	  10
#define NET_FLASH_AREA 	11
#define NET_FLASH_HEAD 	11
#define NET_FLASH_OK 	  12

#define SW_NULL  		0
#define SW_ON  			1
#define SW_ON_DONE  2
#define SW_OFF 			3
#define SW_OFF_DONE 4

extern struct device_table device;
extern struct Addr_info2STR info2STR;
extern struct LCDREG LCDC;
extern struct KEYREG fkey;
extern u16 step;
extern u16 time_s;
extern u16 testcmd1_time;
extern u16 testcmd3_time;
extern u32 time_s_temp;
extern u32 time_sys;
extern u16 global_u16temp;
extern u16 *global_u16p;

extern volatile unsigned char touch_flag;
extern u32 time_uart1;
extern u32 time_uart3;
extern u8 str_buffer[4100];			//����ʾ���ڴ�
extern u8 LCD_TxtBuffer[2][2050];	//����ʾ���ڴ�
extern u8 Dport_State[6];
extern u8 checking_port[2];

extern u32 file_addr;				  //д�ļ�ʱ��ַ
extern u32 Rfile_addr;				//���ļ�ʱ��ַ
extern u8 file_hook;
extern u8 file_wr;
extern u8 file_id;					//��ǰ��д�ļ���ID
extern u32 NextFileAddr;
extern u32 check_time;

extern u8 UART_BUFFER[128];
extern u8 UART1_TXBUFFER[128];
extern u8 UART1_RXBUFFER[UART1_RXBUFFER_SIZE];
extern u16 UART1_RXBUFFE_HEAD;		//��Ч���ݵĵ�һ��
extern u16 UART1_RXBUFFE_LAST;		//��Ч���ݵ����һ��
extern u8 UART2_TXBUFFER[128];
extern u8 UART2_RXBUFFER[128];
extern u8 UART3_TXBUFFER[128];
extern u8 UART3_RXBUFFER[UART3_RXBUFFER_SIZE];
extern u16 UART3_RXBUFFE_HEAD;		//��Ч���ݵĵ�һ��
extern u16 UART3_RXBUFFE_LAST;		//��Ч���ݵ����һ��
extern u8 SPI_BUFFER[128];
extern u16 ADC_BUFFER[ADC_BUFFER_SIZE];
extern u8 AINx_ADCch[18];
extern u16 ADC_Base0[18];			//ADC��ֵ̬
extern u8 device_num[20];
#ifndef BOOTLOADER_SOURCECODE//ZHZQ_CHANGE
extern 	u16 AD_count[64];   //������
extern 	u8 charge_speed[2];   //����ٶ�
#endif

extern 	u8  SF_REG;
extern 	u8  GAIN_REG;
extern 	u8  KEEP_EN;


#define USB_POWER_ON		(0)
#define USB_POWER_OFF		(1)
#define USB_ALL_INDEX		(0xff)


#define LED_TURN_ON			(0)
#define LED_TURN_OFF		(1)
#define LED_TURN_NEGATION	(2)	//make the LED state from ON to OFF, or from OFF to ON

#define LED_INDEX				(0)
#define LED1_INDEX			(1)
#define LED2_INDEX			(2)
#define LED_ALL_INDEX		(0xff)

extern void NVIC_Configuration(void);
#define USE_DHCP 

	extern unsigned  int time_sys;
	extern unsigned  int time_ms;
	extern unsigned  int send_times;
	extern unsigned  char   data_b[2048];

#endif
	 
