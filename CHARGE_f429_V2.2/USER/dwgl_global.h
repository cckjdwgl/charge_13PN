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

#define DefPortNum      4  //电路板带的端口数
#define PCB_VERSION    PCB_VERSION_MC	//微充双层板
//cmd
#define frame_headerC				0x67
#define frame_headerD				0x68
#define frame_last					0x99
#define Version_FLAG1				('B')  //版本标志1
//#define port_num					0x02
#define cmd_set_addr				0xe2  //设充电控制板地址命令

#define Addr_01min					0x000000   //二维码地址始
#define Addr_01max					0x03ffff   //二维码地址末
#define Addr_02min					0x080000   //小图片地址始
#define Addr_02max					0x09FFFF   //小图片地址末
#define Addr_03min					0x100000   //大图片地址始
#define Addr_03max					0x2FFFFF   //大图片地址末
#define Addr_04min					0x400000   //大图片地址始
#define Addr_04max					0xBFFFFF   //大图片地址末
#define Addr_05min					0xE00000   //程序地址起
#define Addr_05max					0xEFFFFF   //程序地址末
#define Addr_info1					0xF00000   //信息地址1
#define Addr_info2					0xF10000   //信息地址2

#define GLOBLE_ADDR					0xE0
#define PC_ADDR						0xF0
#define Broadcast					0xFF

#define ADC_BUFFER_SIZE				320

#define ADC_LINE1					0x0c0  //低于些电压认为有手机;=0xc0波动为310mV,这个可能要做补偿
#define ADC_LINE2					0x029	//0x080  //电流波动范围 ;=0x80 波动为20mA
#define ADC_LINE3					0x01f	//0x060  //电流波动范围 ;=0x60 波动为15mA  电流小于此值表示没有使用
//#define ADC_LINE2					0x0100  //电流波动范围 ;=0x100 波动为40mA

#define UART1_RXBUFFER_SIZE			0x200
#define UART1_RX_MAX				0x1FF
#define UART3_RXBUFFER_SIZE			0x80
#define UART3_RX_MAX				0x7f

struct  Addr_info2STR {
	u8 head[2];
	u8 temp[3];
	u8 item1[2];		//广告控制项与项字长 
	u8 item1_data[4]; 
	u8 item2[2];		//屏保控制项与项字长      
	u8 item2_data[4]; 
	u8 item3[2];		//图片标记控制项与项字长      
	u8 item3_data[2]; 
	u8 item11[2];		//图片版本控制项与项字长      
	u8 item11_data[16]; 
	u8 item12[2];		//图片版本控制项与项字长      
	u8 item12_data[16]; 
	u8 item13[2];		//图片版本控制项与项字长      
	u8 item13_data[16]; 
	u8 item14[2];		//图片版本控制项与项字长      
	u8 item14_data[16]; 
	u8 item15[2];		//图片版本控制项与项字长      
	u8 item15_data[16]; 
	u8 item21[2];		//程序更新设定项
	u8 item21_data[4];
	u8 item81[2];		//设备号项
	u8 item81_data[16]; 
	u8 item31[2];		//B类程序版本项与项字长 
	u8 item31_data[16];
	u8 item32[2];		//A类程序版本项与项字长 
	u8 item32_data[16];
};	

struct  device_table {
	u8 head;
	u8 SW_state;  		//状态开关
	u8 SD_state;  		//SD状态
	unsigned  int ChargeTimer;  		//
	unsigned  int ChargeTimerSet;  	//最后一RX的时间
	u8 Toolname[16];
	u8 Version[16];
	u8 PortNum; 
	u8 PortId[DefPortNum];					//只用了前面两个保存端口号
	u8 PortState[DefPortNum];				//端口二维码是否下载完成
	u8 PortName[DefPortNum][16];		//
	u8 PortSourcePath[DefPortNum][128];		//资源地址
	u8 PortDestPath[DefPortNum][128];   //被保存到的路径
	u16 PortPowerSetTime[DefPortNum];		//
	u16 PortPowerUseTime[DefPortNum];		//
	u16 PortPowerShowTime[DefPortNum];	//
	u16 PortCurrent[DefPortNum][60];		//  mA
	u8 use;						//使用情况 bit0=1连接断； bit1=1地址与二维码充突； bit2=1，0号无二维码 ； bit3=1，1号无二维码 ；bit4=1，二维码有更新
	u8 TASK_state;		//任务情况对程序可以进行工作模式，调试模式
	u8 province;      //省份
};	  


//str_buffer[0]=	//头0X67
//str_buffer[?]=	//LEN
//str_buffer[?]=	//文件区u8
//str_buffer[?]=	//文件号u8
//str_buffer[?]=	//文件名u64
//str_buffer[?]=	//文件大小u32
//str_buffer[?]=	//文件起地址u32
//str_buffer[?]=	//check
//str_buffer[?]=	//尾0X99

#define LCD1_INDEX			(0)
#define LCD2_INDEX			(1)

struct  LCDREG {
	unsigned char Head;					 //头
	unsigned char SW_state;  		 //连接状态开关
	unsigned char NetState;      //状态
	unsigned char Mode;  				 //LCD 模式
	unsigned char LcdNumber[DefPortNum];  //LCD 屏号
	unsigned int TestTimerSet;  					//检查屏幕间隔设定值
	unsigned int TestTimer[DefPortNum];   //
	unsigned int TimeTimerSet;  					//计时显示间隔设定值
	unsigned int TimeTimer[DefPortNum];   //计时显示间隔
	unsigned char  SPSwitch;			        //屏保开关
	unsigned int SPTimeSet;		           	//屏保间隔        
	unsigned int SPTime[DefPortNum];		  //屏保间隔计数
	unsigned char  SPID[DefPortNum];			//屏保ID
	unsigned char  ADSwitch;				      //广告开关
	unsigned char  ADNum;				          //广告个数
	unsigned int ADTimeSet;		          	//广告间隔        
	unsigned int ADTime[DefPortNum];		  //广告间隔计数
	unsigned char  ADID[DefPortNum];			//广告图片ID
	
	unsigned char POWEROFFTIME[DefPortNum];		//断电时间
};	 

struct  KEYREG {
	unsigned char SW_state;  		 //连接状态开关
	unsigned char NetState;      //状态
	unsigned char KeyValue;  		//按键值。
	unsigned int PutInTimer;  	//按下时刻
	unsigned int OkTimerSet;   	//确定键时间
	unsigned int AddTimerSet;   //加号键时间
	unsigned int RetTimerSet;   //反回键时间
};	 

#define NET_NULL   		0
#define NET_ON  			1
#define NET_ON_DONE 	2
#define NET_OFF 			3
#define NET_OFF_DONE 	4
#define NET_DNS_OK 	  5
#define NET_HTTP_OK 	6		 //正响应
#define NET_SOCKET_OK 7    //正响应
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
extern u8 str_buffer[4100];			//做显示的内存
extern u8 LCD_TxtBuffer[2][2050];	//做显示的内存
extern u8 Dport_State[6];
extern u8 checking_port[2];

extern u32 file_addr;				  //写文件时地址
extern u32 Rfile_addr;				//读文件时地址
extern u8 file_hook;
extern u8 file_wr;
extern u8 file_id;					//当前正写文件的ID
extern u32 NextFileAddr;
extern u32 check_time;

extern u8 UART_BUFFER[128];
extern u8 UART1_TXBUFFER[128];
extern u8 UART1_RXBUFFER[UART1_RXBUFFER_SIZE];
extern u16 UART1_RXBUFFE_HEAD;		//有效内容的第一个
extern u16 UART1_RXBUFFE_LAST;		//有效内容的最后一个
extern u8 UART2_TXBUFFER[128];
extern u8 UART2_RXBUFFER[128];
extern u8 UART3_TXBUFFER[128];
extern u8 UART3_RXBUFFER[UART3_RXBUFFER_SIZE];
extern u16 UART3_RXBUFFE_HEAD;		//有效内容的第一个
extern u16 UART3_RXBUFFE_LAST;		//有效内容的最后一个
extern u8 SPI_BUFFER[128];
extern u16 ADC_BUFFER[ADC_BUFFER_SIZE];
extern u8 AINx_ADCch[18];
extern u16 ADC_Base0[18];			//ADC静态值
extern u8 device_num[20];
#ifndef BOOTLOADER_SOURCECODE//ZHZQ_CHANGE
extern 	u16 AD_count[64];   //广告计数
extern 	u8 charge_speed[2];   //充电速度
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
	 
