#include "udp_demo.h" 
//#include "delay.h"
//#include "usart.h"
//#include "led.h"
//#include "key.h"
//#include "lcd.h"
//#include "malloc.h"
#include "stdio.h"
#include "string.h" 

#include "netconf.h"

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
struct udpdebug_msee UdpdebugM_init;
 
struct udp_pcb *dnsdebug_pcb=NULL;
struct udp_pcb *udpdebug_pcb=NULL;
//UDP接收数据缓冲区
unsigned  char udp1_rxbuf[UDP1_RX_BUFSIZE];	//UDP接收数据缓冲区 
//UDP发送数据内容
unsigned  char udp1_txbuf[UDP1_TX_BUFSIZE]="Explorer STM32F407 UDP demo send data\r\n";

//UDP 测试全局状态标记变量
//bit7:没有用到
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有连接上;1,连接上了.
//bit4~0:保留
u8 udp_demo_flag;

//UDP测试
void udp_demo_test(void)
{
 	err_t err;
	struct ip_addr rmtipaddr;  	//远端ip地址
 	
//	u8 *tbuf;
// 	u8 key;
 	
	if(gnetif.ip_addr.addr == 0)
		{	return;	}
	if(udpdebug_lwip.remoteip[0]!=0||udpdebug_lwip.remoteip[1]!=0||udpdebug_lwip.remoteip[2]!=0||udpdebug_lwip.remoteip[3]!=0)  //非IP_ADDR_ANY
	{
		//前三个IP保持和DHCP得到的IP一致
		udpdebug_lwip.remoteip[0] = (uint8_t)(gnetif.ip_addr.addr>>0);
		udpdebug_lwip.remoteip[1] = (uint8_t)(gnetif.ip_addr.addr>>8);
		udpdebug_lwip.remoteip[2] = (uint8_t)(gnetif.ip_addr.addr>>16);
	//	udpdebug_lwip.remoteip[3] = (uint8_t)(gnetif.ip_addr.addr>>14);
	}
		
	udpdebug_pcb=udp_new();
	if(udpdebug_pcb)//创建成功
	{ 
		IP4_ADDR(&rmtipaddr,udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3]);
		
		err=udp_connect(udpdebug_pcb,&rmtipaddr,udpdebug_lwip.remote_port);//UDP客户端连接到指定IP地址和端口号的服务器
		if(err==ERR_OK)
		{
//			err=udp_bind(udpdebug_pcb,IP_ADDR_ANY,UDP1_PORT);//绑定本地IP地址与端口号
//			err=udp_bind(udpdebug_pcb,(ip_addr_t*)&gnetif.ip_addr.addr,UDP1_PORT);//绑定本地IP地址与端口号
//			err=udp_bind(udpdebug_pcb,&gnetif.ip_addr,udpdebug_lwip.local_port);//绑定本地IP地址与端口号
			err=udp_bind(udpdebug_pcb,IP_ADDR_ANY,udpdebug_lwip.local_port);//绑定本地IP地址与端口号 IP_ADDR_ANY本地IP为任意值。只要端口对就OK
			if(err==ERR_OK)	//绑定完成
			{
				udp_recv(udpdebug_pcb,udp_demo_recv,NULL);//注册接收回调函数 
//				LCD_ShowString(30,210,210,16,16,"STATUS:Connected   ");//标记连接上了(UDP是非可靠连接,这里仅仅表示本地UDP已经准备好)
				Str_addto_Str(&DebugStr,"connect to udpdebup\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				udp_demo_flag |= 1<<5;			//标记已经连接上
			}
		}		
	}
} 

//UDP回调函数
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port)
{
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(p!=NULL)	//接收到不为空的数据时
	{		
//		upcb->remote_ip=*addr; 				//记录远程主机的IP地址
//		upcb->remote_port=port;  			//记录远程主机的端口号
		udpdebug_lwip.remoteip[0]=(addr->addr)&0xff; 		//IADDR4
		udpdebug_lwip.remoteip[1]=(addr->addr>>8)&0xff; //IADDR3
		udpdebug_lwip.remoteip[2]=(addr->addr>>16)&0xff;//IADDR2
		udpdebug_lwip.remoteip[3]=(addr->addr>>24)&0xff;//IADDR1
		udp_demo_flag|=1<<6;	//标记接收到数据了
		
		UdpdebugM_init.RxLength = p->tot_len;		
		pbuf_copy_partial(p, udp1_rxbuf, p->tot_len, 0);
		memcpy((char*)LocalPoint, udp1_rxbuf,p->len);  //最多取一个pbuf的内容
		udp1_rxbuf[p->tot_len]=0;
		LocalPoint[p->len]=0;
		LocalPoint[32] = 0;    //最多取pbuf的32B内容
		sprintf((char*)LocalPoint, "%s\n",LocalPoint);
		Str_addto_Str(&DebugStr,LocalPoint);
		sprintf((char*)LocalPoint, "udpdebug_rec:%dB\n",p->tot_len);
		Str_addto_Str(&DebugStr,LocalPoint);
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);				
		sprintf((char*)LocalPoint, "udpdebug_rec:%dB\n",p->tot_len);
		udp_demo_senddata(udpdebug_pcb,LocalPoint);
		pbuf_free(p);//释放内存				
	}else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//标记连接断开
	} 
} 
//UDP服务器发送数据
void udp_demo_senddata(struct udp_pcb *upcb,unsigned  char *buffer)
{
	struct pbuf *ptr;
	struct ip_addr rmtipaddr;  	//远端ip地址
	
	IP4_ADDR(&rmtipaddr,udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3]);
	
	if((upcb->remote_ip.addr!=rmtipaddr.addr)||(udp_demo_flag&(1<<5))==0)    //地址发生变化,UDP被关闭，重连
	{
	udp_demo_connection_close(udpdebug_pcb); //先关闭
	udp_demo_test();//再重新打开
	}
//	if((upcb->local_ip.addr ==IP_ADDR_ANY->addr) || (upcb->remote_ip.addr  == IP_ADDR_ANY->addr))
	if((upcb->remote_ip.addr == IP_ADDR_ANY->addr))
	{
		return;
	}
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)buffer),PBUF_POOL); //申请内存
	if(ptr)
	{
		pbuf_take(ptr,(char*)buffer,strlen((char*)buffer)); //将tcp_demo_sendbuf中的数据打包进pbuf结构中
		udp_send(upcb,ptr);	//udp发送数据 
		pbuf_free(ptr);//释放内存
//		Str_addto_Str(&DebugStr,"connect to udpdebup\n");
//		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
	} 
} 
//关闭UDP连接
void udp_demo_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//断开UDP连接 
	udp_demo_flag &= ~(1<<5);	//标记连接断开
}

void my_found(const char *name, struct ip_addr *ipaddr, void *arg)
{
	uint8_t ip[4];
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	
	ip[0] = ipaddr->addr>>24; 
	ip[1] = ipaddr->addr>>16;
	ip[2] = ipaddr->addr>>8; 
	ip[3] = ipaddr->addr; 
	
	printf("DNS: %s:%d.%d.%d.%d\n",name,ip[3], ip[2], ip[1], ip[0]);
	sprintf((char*)LocalPoint, "DNS: %s:%d.%d.%d.%d\n",name,ip[3], ip[2], ip[1], ip[0]);
	Str_addto_Str(&DebugStr,LocalPoint);
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);

	if ((strcmp(name, (char *)HttpM_init.DnsNameStr) == 0))
	{
			HttpM_init.NetState  = NET_DNS_OK;  //dns OK
			HttpM_init.DnsIP[0]=ip[3];	
			HttpM_init.DnsIP[1]=ip[2];	
			HttpM_init.DnsIP[2]=ip[1];	
			HttpM_init.DnsIP[3]=ip[0];	
	}
	if ((strcmp(name, (char *)ChargeM_init.DnsNameStr) == 0))
	{
			ChargeM_init.NetState  = NET_DNS_OK;  //dns OK
			ChargeM_init.DnsIP[0]=ip[3];	
			ChargeM_init.DnsIP[1]=ip[2];	
			ChargeM_init.DnsIP[2]=ip[1];	
			ChargeM_init.DnsIP[3]=ip[0];	
	}

}
//dns测试
void dns_demo_test(void)
{
/* This is the aligned version of ip_addr_t,used as local variable, on the stack, etc. */
//struct ip_addr {
//  u32_t addr;
//};
struct ip_addr cipaddr_DNS;  //
	err_t dns_err;
const char hostname1[]="baidu.com";
const char hostname2[]="admin.freecharge168.com";  //测试环境
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;

  /* host name already in octet notation? set ip addr and return ERR_OK */
  cipaddr_DNS.addr = ipaddr_addr(hostname1);   //本来就是IP地址,如“192.168.1.1”
  if (cipaddr_DNS.addr != IPADDR_NONE) 
	{
		my_found(hostname1,&cipaddr_DNS,NULL);
	}
	else
	{
		dns_err=dns_gethostbyname(hostname1,&cipaddr_DNS,my_found,NULL);	
		if(dns_err ==ERR_MEM) 
		{
			sprintf((char*)LocalPoint, "DNS: %s: Out of memory error\n",hostname1);
			Str_addto_Str(&DebugStr,LocalPoint);
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		}
		else if(dns_err ==ERR_OK)
		{
			my_found(hostname1,&cipaddr_DNS,NULL);
		}
	}

  cipaddr_DNS.addr = ipaddr_addr(hostname2);   //本来就是IP地址,如“192.168.1.1”
  if (cipaddr_DNS.addr != IPADDR_NONE) 
	{
		my_found(hostname2,&cipaddr_DNS,NULL);
	}
	else
	{
		dns_err=dns_gethostbyname(hostname2,&cipaddr_DNS,my_found,NULL);	
		if(dns_err ==ERR_MEM) 
		{
			sprintf((char*)LocalPoint, "DNS: %s: Out of memory error\n",hostname2);
			Str_addto_Str(&DebugStr,LocalPoint);
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		}
		else if(dns_err ==ERR_OK)
		{
			my_found(hostname2,&cipaddr_DNS,NULL);
		}
	}
}

void get_input_dns(void *input)
{
	struct ip_addr cipaddr_DNS;  //
	err_t dns_err;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;		
  cipaddr_DNS.addr = ipaddr_addr(input);   //本来就是IP地址,如“192.168.1.1”
  if (cipaddr_DNS.addr != IPADDR_NONE) 
	{
		my_found(input,&cipaddr_DNS,NULL);
	}
	else
	{
		dns_err=dns_gethostbyname(input,&cipaddr_DNS,my_found,NULL);	
		if(dns_err ==ERR_MEM) 
		{
			sprintf((char*)LocalPoint, "DNS: %s: Out of memory error\n",(char*)input);
			Str_addto_Str(&DebugStr,LocalPoint);
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		}
		else if(dns_err ==ERR_OK)
		{
			my_found(input,&cipaddr_DNS,NULL);
		}
	}

}



