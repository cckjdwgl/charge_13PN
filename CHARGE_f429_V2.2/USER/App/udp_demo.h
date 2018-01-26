#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
#include "sys.h"
#include "netconf.h"
//#include "lwip_comm.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//UDP 测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
#define UDP1_RX_BUFSIZE		4096	//定义udp最大接收数据长度 
#define UDP1_TX_BUFSIZE		4096	//定义udp最大发送数据长度 
//#define UDP1_PORT			6080	//定义udp连接的端口 
struct udpdebug_msee 
{
	unsigned  char *method;      //方法
	unsigned  char *url;         //链接
	unsigned  char *httpv;       //HTTP版本
	unsigned  char *Host;        //主机名
	unsigned  char *Accept;      //接受的格式
	unsigned  char *type;        //消息体格式
	unsigned  int Length;       //消息体长度
	
	void *body;  //消息体
	unsigned  char SW_state;  			//连接状态开关
	unsigned  char NetState;      	//状态
	unsigned  int HeartTime;  				//心跳间隔
	unsigned  int LastRxTime;  				//最后一RX的时间
	unsigned  int LastTxTime;  				//最后一TX的时间
	
	unsigned  char *rxbuffer;      //接收的数据
	unsigned  char *txbuffer;      //发送的数据
	unsigned  int  RxLength;          //接收消息体长度
	unsigned  int  TxLength;          //发送消息体长度
	void *PCB;      //接的网络
};

extern struct udpdebug_msee UdpdebugM_init;

extern struct udp_pcb *dnsdebug_pcb;//定义一个UDP服务器控制块
extern struct udp_pcb *udpdebug_pcb;
extern unsigned  char udp1_rxbuf[UDP1_RX_BUFSIZE];	//UDP接收数据缓冲区 
extern unsigned  char udp1_txbuf[UDP1_TX_BUFSIZE];	//UDP发送数据内容

 
void udp_demo_test(void);
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port);
void udp_demo_senddata(struct udp_pcb *upcb,unsigned  char *buffer);
void udp_demo_connection_close(struct udp_pcb *upcb);

void dns_demo_test(void);
void get_input_dns(void *input);

#endif

