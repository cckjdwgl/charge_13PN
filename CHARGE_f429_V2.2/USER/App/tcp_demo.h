/**
  ******************************************************************************
  * @file    tcp_echoclient.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   Header file for tcp_echoclient.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#ifndef __TCP_ECHOCLIENT_H__
#define __TCP_ECHOCLIENT_H__

#include "stm32f4xx.h"

//tcp1用于长连连；TCP2用于HTTP等
#define TCP1_RX_BUFSIZE		4096	//定义udp最大接收数据长度   
#define TCP1_TX_BUFSIZE		4096	//定义udp最大发送数据长度 

#define TCP2_RX_BUFSIZE		4096	//定义udp最大接收数据长度 
#define TCP2_TX_BUFSIZE		4096	//定义udp最大发送数据长度 

#define HttpU_BUFSIZE		  16	//HTTP_URL的SIZE

typedef err_t (*http_sent_fn)(unsigned  char *buffer);

struct charge_msee 
{
	unsigned  short ID;              //ID识别号
	unsigned  short LEN;             //包长
	unsigned  short NUMBER;          //流水号
	unsigned  short encryption;      //加密方式
	unsigned  char longitude[8];     //经度
	unsigned  char latitude[8];      //纬度
	
	void *body;  //消息体
	unsigned  char SN[16];      	//设备号
	unsigned  char TOKEN[4];	  	//校验口令 
	unsigned  char MD5[16];	    	//MD5值	
	unsigned  char port1[8];      //端口1
	unsigned  char port2[8];      //端口2
	
	unsigned  char DnsNameStr[256];  	//链接
	unsigned  char DnsIP[4];    		 	//当前的IP
	unsigned  short int Port;    		 	//当前的远程端口
	unsigned  char PortStr[8];  	    //当前的远程端口
	unsigned  char SW_state;  		//连接状态开关
	unsigned  char NetState;      //状态
	unsigned  int HeartTime;  				//心跳间隔
	unsigned  int LastRxTime;  				//最后一RX的时间
	unsigned  int LastTxTime;  				//最后一TX的时间
	
	unsigned  char *rxbuffer;         //接收的数据
	unsigned  char *txbuffer;         //发送的数据
	unsigned  int  RxLength;          //接收消息体长度
	unsigned  int  TxLength;          //发送消息体长度
	void *PCB;      //接的网络
};
struct http_msee 
{
	unsigned  char *method;      //方法
	unsigned  char *url;         //链接
	unsigned  char *httpv;       //HTTP版本
	unsigned  char *Host;        //主机名
	unsigned  char *Accept;      //接受的格式
	
	void *body;    //消息体	
	struct http_url *HttpUrl;  				//URL结构体
	unsigned  char DnsNameStr[256];  	//链接
	unsigned  char DnsIP[4];    		 	//当前的IP
	unsigned  short int Port;    		 	//当前的远程端口
	unsigned  char PortStr[8];  	    //当前的远程端口
	unsigned  char SW_state;  				//连接状态开关
	unsigned  char NetState;      		//状态
	unsigned  int HeartTime;  				//心跳间隔
	unsigned  int LastRxTime;  				//最后一RX的时间
	unsigned  int LastTxTime;  				//最后一TX的时间
	unsigned  char *rxbuffer;      		//接收的数据
	unsigned  char *txbuffer;      		//发送的数据
	unsigned  int  RxLength;          //接收消息体长度
	unsigned  int  TxLength;          //发送消息体长度
	void *PCB;      //接的网络
};
struct http_url 
{
	void *next;  //下一个
	unsigned  char url[256];         //链接
	unsigned  char DestPath[256];    //目的地址
	unsigned  char type[4];       	 //消息类型 有SSI初化始使用,URL资源连接用完就删除.
	unsigned  int  PresentLength;    //当前消息体长度
	unsigned  int  TotLength;        //总消息体长度
	
	void *body;  //消息体
	unsigned  char SW_state;  		//连接状态开关
	unsigned  char NetState;      //HTTP的完成状态
	unsigned  int HeartTime;  				//心跳间隔
	unsigned  int LastRxTime;  				//最后一RX的时间
	unsigned  int LastTxTime;  				//最后一TX的时间
	http_sent_fn sent;  //接的网络
};

extern struct http_url HttpU_buffer[HttpU_BUFSIZE];
extern struct http_msee HttpM_init;
extern struct charge_msee ChargeM_init;

extern unsigned  char tcp1_rxbuf[TCP1_RX_BUFSIZE];	//TCP1接收数据缓冲区 
extern unsigned  char tcp1_txbuf[TCP1_TX_BUFSIZE];	//TCP1接收数据缓冲区 

extern unsigned  char tcp2_rxbuf[TCP2_RX_BUFSIZE];	//TCP2接收数据缓冲区 
extern unsigned  char tcp2_txbuf[TCP2_TX_BUFSIZE];	//TCP2接收数据缓冲区 

extern struct tcp_pcb *soket_pcb;
extern struct tcp_pcb *http_pcb;

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
struct tcp_pcb* tcp_echoclient_connect(void *lwip_addr);
struct tcp_pcb* tcp_echoclient_disconnect(struct tcp_pcb *pcb);
#endif /* __TCP_ECHOCLIENT_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
