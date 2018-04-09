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
#ifndef __dwgl_function_H
#define __dwgl_function_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//#include "lwip/debug.h"
//#include "lwip/stats.h"
//#include "lwip/tcp.h"
//#include "memp.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "netconf.h"
//#include "stm32f429_phy.h"

#define wave_area_XL    0
#define wave_area_XR    320
#define wave_area_YU    50
#define wave_area_YD    180

extern void make_device_mess(void);
extern void u16toStr(u16 u_data,u8 *p);	
extern void u16toStr_unit(u16 u_data,u8 *p,u8 unit);	
extern void Version_display(u16 x,u8 *p);

extern void Device_Info(void);
extern void Device_Rst(void);
extern void File_Requst(u8 FN);
extern void File_Tx(u8 FN);
extern void File_Recall(u8 FN);
extern void File_Erase(u8 FN);
extern void Flash_Erase(unsigned int addr,unsigned int size);
extern void RGB888_565(unsigned int addr);

extern void LCD_TEST(u8 cs);
extern void State_Message(unsigned int x, unsigned int y, u16 UpColor, u16 DownColor,u16 ChargeColor);

extern u8 Frame_check_cmd1(void);
//uart1
extern void uart1_cmd (void);
extern void cmd_Erase_Flash(void);
extern void cmd_Read_Flash(void);
extern void cmd_Write_Flash(void);
extern void cmd_Get_State(void);
extern void cmd_Set_State(void);
extern void cmd_Get_ADC(void);
extern void cmd_Save_ADC(void);
extern void cmd_RGB888_565(void);
extern void cmd_RGB_clear(void);
extern void cmd_CHIP_PRO(void);
extern void cmd_PRO_Version(void);
extern void cmd_Hub_Rst(void);			//NC
extern void cmd_Device_Info(void);
extern void cmd_ShakeHands(void);
extern void cmd_Power_on(void);
extern void cmd_Power_off(void);
extern void cmd_File_Requst(void);
extern void cmd_File_Tx(void);
extern void cmd_File_Recall(void);
extern void cmd_File_Erase(void);
extern void cmd_Port_Info(void);
extern void cmd_Device_num(void);
extern void cmd_Device_Check(void);
extern void cmd_MediaCtrl(void);
extern void cmd_Set_MediaV(void);
extern void cmd_Get_MediaV(void);
extern void cmd_Set_Version(void);
extern void cmd_Get_Version(void);
#ifndef BOOTLOADER_SOURCECODE//ZHZQ_CHANGE
extern void cmd_Get_AD_count(void);
extern void cmd_Get_charge_speed(void);
#endif

//充电
extern void Get_ADC_BaseLine(void);
extern void DisplayADC_BL(unsigned int x, unsigned int y, u16 *s,u16 PenColor, u16 BackColor,u8 cs);
extern void FiletoBuffer_ID(u8 area,u8 id,u8 *p);//以ID放式调读文件到BUFFER。

typedef void (*VOID_FUNC_START_ROUTINE)(void);
#define VOID_FUNC_COUNT			(11)

extern void usb_power_ctrl(u8 usb_port, u8 new_state);
extern void led_power_ctrl(u8 usb_port, u8 new_state);
extern void usb_mutually_exclusive_power_on(u8 lcd_index);
extern void rewrite_ADC_BaseLine_flash_data(void);
//extern void Dport_Charge_State(u8 lcd_index);
extern void port_Charge_State(u8 lcd_index);
extern void get_ADC1_3_data(u16 * ADC_data);

//	extern unsigned  int time_ms;
extern err_t dwgl_ms_send(struct tcp_pcb *tpcb, void *arg,err_t err);
extern err_t dwgl_ms_send1(struct tcp_pcb *tpcb, void *arg,err_t err);
extern void make_soket_mess(void);
extern void make_http_mess(void);
extern void make_udpdebug_mess(void);
extern err_t make_soket_10000(unsigned  char *buffer);
extern err_t make_soket_10001(unsigned  char *buffer);
extern err_t cmd_soket_10003(unsigned  char *cmd,unsigned  char *ret);    //cmd10003分析
extern err_t make_soket_10004(unsigned  char *buffer,unsigned  char *cmd);
extern err_t cmd_soket_10005(unsigned  char *cmd,unsigned  char *ret);    //cmd10005分析
extern err_t make_soket_10006(unsigned  char *buffer,unsigned  char *cmd);
extern err_t make_soket_10009(unsigned  char *buffer,unsigned  char *ret);
extern err_t make_soket_10011(unsigned  char *buffer,unsigned  char *ret);
extern err_t make_soket_rxok(unsigned  char *buffer);   //接收成功

extern err_t make_http_InitSoket(unsigned  char *buffer);
extern err_t make_http_post(unsigned  char *buffer);
extern err_t make_http_get(unsigned  char *buffer);
extern err_t http_url_select(struct http_msee *HttpM);
extern err_t make_area1_get(unsigned  char *buffer);
extern err_t make_area2_get(unsigned  char *buffer);
extern err_t make_area4_get(unsigned  char *buffer);
extern err_t make_area5_get(unsigned  char *buffer);
extern err_t do_area1_get(unsigned  char *flag);  //ret=0,无更新；ret=1,有更新。flag=0,无溢出；flag=1,有溢出。
extern err_t done_area1_get(unsigned  char *buffer);
extern err_t get_AD_number(unsigned  char *buffer);
extern err_t get_AD_dir(unsigned  char *buffer,unsigned  char *retdir);
extern unsigned short cut_between_strs(unsigned  char *buffer,unsigned  char *began,unsigned  char *end, unsigned  char *ret,unsigned short maxret,unsigned short *retpoint);
extern unsigned short cut_between_strs_inverted(unsigned  char *buffer,unsigned  char *began,unsigned  char *end, unsigned  char *ret,unsigned short maxret,unsigned short *retpoint);
extern unsigned short find_strs(unsigned  char *buffer,unsigned  char *began,unsigned short maxret,unsigned short *retpoint);

extern err_t tcpl_cmd(struct tcp_pcb *tpcb);
extern err_t tcp2_cmd(struct tcp_pcb *tpcb);  //
extern err_t udp1_cmd(struct udp_pcb *upcb);
extern err_t tcpl_task(struct tcp_pcb *tpcb);
extern err_t tcp2_task(struct tcp_pcb *tpcb);
extern err_t udp1_task(struct udp_pcb *upcb);
extern err_t LCD_task(void);
extern err_t charge_task(void);   //充电任务
extern err_t key_task(void);   //充电任务

#endif
	 
