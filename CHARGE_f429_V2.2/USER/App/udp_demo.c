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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//UDP ���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
struct udpdebug_msee UdpdebugM_init;
 
struct udp_pcb *dnsdebug_pcb=NULL;
struct udp_pcb *udpdebug_pcb=NULL;
//UDP�������ݻ�����
unsigned  char udp1_rxbuf[UDP1_RX_BUFSIZE];	//UDP�������ݻ����� 
//UDP������������
unsigned  char udp1_txbuf[UDP1_TX_BUFSIZE]="Explorer STM32F407 UDP demo send data\r\n";

//UDP ����ȫ��״̬��Ǳ���
//bit7:û���õ�
//bit6:0,û���յ�����;1,�յ�������.
//bit5:0,û��������;1,��������.
//bit4~0:����
u8 udp_demo_flag;

//UDP����
void udp_demo_test(void)
{
 	err_t err;
	struct ip_addr rmtipaddr;  	//Զ��ip��ַ
 	
//	u8 *tbuf;
// 	u8 key;
 	
	if(gnetif.ip_addr.addr == 0)
		{	return;	}
	if(udpdebug_lwip.remoteip[0]!=0||udpdebug_lwip.remoteip[1]!=0||udpdebug_lwip.remoteip[2]!=0||udpdebug_lwip.remoteip[3]!=0)  //��IP_ADDR_ANY
	{
		//ǰ����IP���ֺ�DHCP�õ���IPһ��
		udpdebug_lwip.remoteip[0] = (uint8_t)(gnetif.ip_addr.addr>>0);
		udpdebug_lwip.remoteip[1] = (uint8_t)(gnetif.ip_addr.addr>>8);
		udpdebug_lwip.remoteip[2] = (uint8_t)(gnetif.ip_addr.addr>>16);
	//	udpdebug_lwip.remoteip[3] = (uint8_t)(gnetif.ip_addr.addr>>14);
	}
		
	udpdebug_pcb=udp_new();
	if(udpdebug_pcb)//�����ɹ�
	{ 
		IP4_ADDR(&rmtipaddr,udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3]);
		
		err=udp_connect(udpdebug_pcb,&rmtipaddr,udpdebug_lwip.remote_port);//UDP�ͻ������ӵ�ָ��IP��ַ�Ͷ˿ںŵķ�����
		if(err==ERR_OK)
		{
//			err=udp_bind(udpdebug_pcb,IP_ADDR_ANY,UDP1_PORT);//�󶨱���IP��ַ��˿ں�
//			err=udp_bind(udpdebug_pcb,(ip_addr_t*)&gnetif.ip_addr.addr,UDP1_PORT);//�󶨱���IP��ַ��˿ں�
//			err=udp_bind(udpdebug_pcb,&gnetif.ip_addr,udpdebug_lwip.local_port);//�󶨱���IP��ַ��˿ں�
			err=udp_bind(udpdebug_pcb,IP_ADDR_ANY,udpdebug_lwip.local_port);//�󶨱���IP��ַ��˿ں� IP_ADDR_ANY����IPΪ����ֵ��ֻҪ�˿ڶԾ�OK
			if(err==ERR_OK)	//�����
			{
				udp_recv(udpdebug_pcb,udp_demo_recv,NULL);//ע����ջص����� 
//				LCD_ShowString(30,210,210,16,16,"STATUS:Connected   ");//�����������(UDP�Ƿǿɿ�����,���������ʾ����UDP�Ѿ�׼����)
				Str_addto_Str(&DebugStr,"connect to udpdebup\n");
				tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
				udp_demo_flag |= 1<<5;			//����Ѿ�������
			}
		}		
	}
} 

//UDP�ص�����
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port)
{
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	if(p!=NULL)	//���յ���Ϊ�յ�����ʱ
	{		
//		upcb->remote_ip=*addr; 				//��¼Զ��������IP��ַ
//		upcb->remote_port=port;  			//��¼Զ�������Ķ˿ں�
		udpdebug_lwip.remoteip[0]=(addr->addr)&0xff; 		//IADDR4
		udpdebug_lwip.remoteip[1]=(addr->addr>>8)&0xff; //IADDR3
		udpdebug_lwip.remoteip[2]=(addr->addr>>16)&0xff;//IADDR2
		udpdebug_lwip.remoteip[3]=(addr->addr>>24)&0xff;//IADDR1
		udp_demo_flag|=1<<6;	//��ǽ��յ�������
		
		UdpdebugM_init.RxLength = p->tot_len;		
		pbuf_copy_partial(p, udp1_rxbuf, p->tot_len, 0);
		memcpy((char*)LocalPoint, udp1_rxbuf,p->len);  //���ȡһ��pbuf������
		udp1_rxbuf[p->tot_len]=0;
		LocalPoint[p->len]=0;
		LocalPoint[32] = 0;    //���ȡpbuf��32B����
		sprintf((char*)LocalPoint, "%s\n",LocalPoint);
		Str_addto_Str(&DebugStr,LocalPoint);
		sprintf((char*)LocalPoint, "udpdebug_rec:%dB\n",p->tot_len);
		Str_addto_Str(&DebugStr,LocalPoint);
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);				
		sprintf((char*)LocalPoint, "udpdebug_rec:%dB\n",p->tot_len);
		udp_demo_senddata(udpdebug_pcb,LocalPoint);
		pbuf_free(p);//�ͷ��ڴ�				
	}else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//������ӶϿ�
	} 
} 
//UDP��������������
void udp_demo_senddata(struct udp_pcb *upcb,unsigned  char *buffer)
{
	struct pbuf *ptr;
	struct ip_addr rmtipaddr;  	//Զ��ip��ַ
	
	IP4_ADDR(&rmtipaddr,udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3]);
	
	if((upcb->remote_ip.addr!=rmtipaddr.addr)||(udp_demo_flag&(1<<5))==0)    //��ַ�����仯,UDP���رգ�����
	{
	udp_demo_connection_close(udpdebug_pcb); //�ȹر�
	udp_demo_test();//�����´�
	}
//	if((upcb->local_ip.addr ==IP_ADDR_ANY->addr) || (upcb->remote_ip.addr  == IP_ADDR_ANY->addr))
	if((upcb->remote_ip.addr == IP_ADDR_ANY->addr))
	{
		return;
	}
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)buffer),PBUF_POOL); //�����ڴ�
	if(ptr)
	{
		pbuf_take(ptr,(char*)buffer,strlen((char*)buffer)); //��tcp_demo_sendbuf�е����ݴ����pbuf�ṹ��
		udp_send(upcb,ptr);	//udp�������� 
		pbuf_free(ptr);//�ͷ��ڴ�
//		Str_addto_Str(&DebugStr,"connect to udpdebup\n");
//		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
	} 
} 
//�ر�UDP����
void udp_demo_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//�Ͽ�UDP���� 
	udp_demo_flag &= ~(1<<5);	//������ӶϿ�
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
//dns����
void dns_demo_test(void)
{
/* This is the aligned version of ip_addr_t,used as local variable, on the stack, etc. */
//struct ip_addr {
//  u32_t addr;
//};
struct ip_addr cipaddr_DNS;  //
	err_t dns_err;
const char hostname1[]="baidu.com";
const char hostname2[]="admin.freecharge168.com";  //���Ի���
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;

  /* host name already in octet notation? set ip addr and return ERR_OK */
  cipaddr_DNS.addr = ipaddr_addr(hostname1);   //��������IP��ַ,�硰192.168.1.1��
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

  cipaddr_DNS.addr = ipaddr_addr(hostname2);   //��������IP��ַ,�硰192.168.1.1��
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
  cipaddr_DNS.addr = ipaddr_addr(input);   //��������IP��ַ,�硰192.168.1.1��
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



