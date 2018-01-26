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
 
#define UDP1_RX_BUFSIZE		4096	//����udp���������ݳ��� 
#define UDP1_TX_BUFSIZE		4096	//����udp��������ݳ��� 
//#define UDP1_PORT			6080	//����udp���ӵĶ˿� 
struct udpdebug_msee 
{
	unsigned  char *method;      //����
	unsigned  char *url;         //����
	unsigned  char *httpv;       //HTTP�汾
	unsigned  char *Host;        //������
	unsigned  char *Accept;      //���ܵĸ�ʽ
	unsigned  char *type;        //��Ϣ���ʽ
	unsigned  int Length;       //��Ϣ�峤��
	
	void *body;  //��Ϣ��
	unsigned  char SW_state;  			//����״̬����
	unsigned  char NetState;      	//״̬
	unsigned  int HeartTime;  				//�������
	unsigned  int LastRxTime;  				//���һRX��ʱ��
	unsigned  int LastTxTime;  				//���һTX��ʱ��
	
	unsigned  char *rxbuffer;      //���յ�����
	unsigned  char *txbuffer;      //���͵�����
	unsigned  int  RxLength;          //������Ϣ�峤��
	unsigned  int  TxLength;          //������Ϣ�峤��
	void *PCB;      //�ӵ�����
};

extern struct udpdebug_msee UdpdebugM_init;

extern struct udp_pcb *dnsdebug_pcb;//����һ��UDP���������ƿ�
extern struct udp_pcb *udpdebug_pcb;
extern unsigned  char udp1_rxbuf[UDP1_RX_BUFSIZE];	//UDP�������ݻ����� 
extern unsigned  char udp1_txbuf[UDP1_TX_BUFSIZE];	//UDP������������

 
void udp_demo_test(void);
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p,struct ip_addr *addr,u16_t port);
void udp_demo_senddata(struct udp_pcb *upcb,unsigned  char *buffer);
void udp_demo_connection_close(struct udp_pcb *upcb);

void dns_demo_test(void);
void get_input_dns(void *input);

#endif

