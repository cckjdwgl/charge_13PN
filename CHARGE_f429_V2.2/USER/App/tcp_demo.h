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

//tcp1���ڳ�������TCP2����HTTP��
#define TCP1_RX_BUFSIZE		4096	//����udp���������ݳ���   
#define TCP1_TX_BUFSIZE		4096	//����udp��������ݳ��� 

#define TCP2_RX_BUFSIZE		4096	//����udp���������ݳ��� 
#define TCP2_TX_BUFSIZE		4096	//����udp��������ݳ��� 

#define HttpU_BUFSIZE		  16	//HTTP_URL��SIZE

typedef err_t (*http_sent_fn)(unsigned  char *buffer);

struct charge_msee 
{
	unsigned  short ID;              //IDʶ���
	unsigned  short LEN;             //����
	unsigned  short NUMBER;          //��ˮ��
	unsigned  short encryption;      //���ܷ�ʽ
	unsigned  char longitude[8];     //����
	unsigned  char latitude[8];      //γ��
	
	void *body;  //��Ϣ��
	unsigned  char SN[16];      	//�豸��
	unsigned  char TOKEN[4];	  	//У����� 
	unsigned  char MD5[16];	    	//MD5ֵ	
	unsigned  char port1[8];      //�˿�1
	unsigned  char port2[8];      //�˿�2
	
	unsigned  char DnsNameStr[256];  	//����
	unsigned  char DnsIP[4];    		 	//��ǰ��IP
	unsigned  short int Port;    		 	//��ǰ��Զ�̶˿�
	unsigned  char PortStr[8];  	    //��ǰ��Զ�̶˿�
	unsigned  char SW_state;  		//����״̬����
	unsigned  char NetState;      //״̬
	unsigned  int HeartTime;  				//�������
	unsigned  int LastRxTime;  				//���һRX��ʱ��
	unsigned  int LastTxTime;  				//���һTX��ʱ��
	
	unsigned  char *rxbuffer;         //���յ�����
	unsigned  char *txbuffer;         //���͵�����
	unsigned  int  RxLength;          //������Ϣ�峤��
	unsigned  int  TxLength;          //������Ϣ�峤��
	void *PCB;      //�ӵ�����
};
struct http_msee 
{
	unsigned  char *method;      //����
	unsigned  char *url;         //����
	unsigned  char *httpv;       //HTTP�汾
	unsigned  char *Host;        //������
	unsigned  char *Accept;      //���ܵĸ�ʽ
	
	void *body;    //��Ϣ��	
	struct http_url *HttpUrl;  				//URL�ṹ��
	unsigned  char DnsNameStr[256];  	//����
	unsigned  char DnsIP[4];    		 	//��ǰ��IP
	unsigned  short int Port;    		 	//��ǰ��Զ�̶˿�
	unsigned  char PortStr[8];  	    //��ǰ��Զ�̶˿�
	unsigned  char SW_state;  				//����״̬����
	unsigned  char NetState;      		//״̬
	unsigned  int HeartTime;  				//�������
	unsigned  int LastRxTime;  				//���һRX��ʱ��
	unsigned  int LastTxTime;  				//���һTX��ʱ��
	unsigned  char *rxbuffer;      		//���յ�����
	unsigned  char *txbuffer;      		//���͵�����
	unsigned  int  RxLength;          //������Ϣ�峤��
	unsigned  int  TxLength;          //������Ϣ�峤��
	void *PCB;      //�ӵ�����
};
struct http_url 
{
	void *next;  //��һ��
	unsigned  char url[256];         //����
	unsigned  char DestPath[256];    //Ŀ�ĵ�ַ
	unsigned  char type[4];       	 //��Ϣ���� ��SSI����ʼʹ��,URL��Դ���������ɾ��.
	unsigned  int  PresentLength;    //��ǰ��Ϣ�峤��
	unsigned  int  TotLength;        //����Ϣ�峤��
	
	void *body;  //��Ϣ��
	unsigned  char SW_state;  		//����״̬����
	unsigned  char NetState;      //HTTP�����״̬
	unsigned  int HeartTime;  				//�������
	unsigned  int LastRxTime;  				//���һRX��ʱ��
	unsigned  int LastTxTime;  				//���һTX��ʱ��
	http_sent_fn sent;  //�ӵ�����
};

extern struct http_url HttpU_buffer[HttpU_BUFSIZE];
extern struct http_msee HttpM_init;
extern struct charge_msee ChargeM_init;

extern unsigned  char tcp1_rxbuf[TCP1_RX_BUFSIZE];	//TCP1�������ݻ����� 
extern unsigned  char tcp1_txbuf[TCP1_TX_BUFSIZE];	//TCP1�������ݻ����� 

extern unsigned  char tcp2_rxbuf[TCP2_RX_BUFSIZE];	//TCP2�������ݻ����� 
extern unsigned  char tcp2_txbuf[TCP2_TX_BUFSIZE];	//TCP2�������ݻ����� 

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
