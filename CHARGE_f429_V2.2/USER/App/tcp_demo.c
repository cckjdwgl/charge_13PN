/**
  ******************************************************************************
  * @file    tcp_echoclient.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   tcp echoclient application using LwIP RAW API
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

/* Includes ------------------------------------------------------------------*/
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "memp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netconf.h"
#include "stm32f429_phy.h"
#include "tcp_demo.h"

#if LWIP_TCP
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t message_count=0;

//u8_t   data[100];



/* ECHO protocol states */
enum echoclient_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};


/* structure to be passed as argument to the tcp callbacks */
struct echoclient
{
  enum echoclient_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};

struct http_url HttpU_buffer[HttpU_BUFSIZE];
struct http_msee HttpM_init;
struct charge_msee ChargeM_init;

unsigned  char tcp1_rxbuf[TCP1_RX_BUFSIZE];	//TCP1接收数据缓冲区 
unsigned  char tcp1_txbuf[TCP1_TX_BUFSIZE];	//TCP1接收数据缓冲区 
unsigned  char tcp2_rxbuf[TCP2_RX_BUFSIZE];	//TCP2接收数据缓冲区 
unsigned  char tcp2_txbuf[TCP2_TX_BUFSIZE];	//TCP2接收数据缓冲区 

struct tcp_pcb *soket_pcb=NULL;
struct tcp_pcb *http_pcb=NULL;

/* Private function prototypes -----------------------------------------------*/
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

/* Private functions ---------------------------------------------------------*/


/**
* @brief  Connects to the TCP echo server
* @param  None
* @retval None
*/
struct tcp_pcb* tcp_echoclient_connect(void *lwip_dev)
{
  struct ip_addr DestIPaddr;
	struct tcp_pcb *pcb;
	__lwip_dev *lwip_addr;
	
	lwip_addr = (__lwip_dev *)lwip_dev;
	
  /* create new tcp pcb */
  pcb = tcp_new();
  
  if (pcb != NULL)
  {
    IP4_ADDR( &DestIPaddr, lwip_addr->remoteip[0],lwip_addr->remoteip[1],lwip_addr->remoteip[2],lwip_addr->remoteip[3]);
    
    /* connect to destination address/port */
		pcb->next = NULL;
    tcp_connect(pcb,&DestIPaddr,lwip_addr->remote_port,tcp_echoclient_connected);
  }
  else
  {
    /* deallocate the pcb */
    memp_free(MEMP_TCP_PCB, pcb);
#ifdef SERIAL_DEBUG
    printf("\n\r can not create tcp pcb");
#endif 
		Str_addto_Str(&DebugStr,"can not create tcp pcb\n");
		tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
  }
		return pcb;
}

/**
* @brief  Disconnects to the TCP echo server
* @param  None
* @retval None
*/
struct tcp_pcb* tcp_echoclient_disconnect(struct tcp_pcb *pcb)
{
	  struct echoclient *es = NULL;
//		struct tcp_pcb *tpcb;
//    struct tcp_pcb *next;
//  pcb = tcp_active_pcbs; //可以查正在活动的TCP 
//  TCP_MSL ; //再堆不够用的时间可以调小这个时间，来释放LAST_ACK卡死的内存
		es = (struct echoclient *)pcb->callback_arg;
	/* close connection */
  tcp_echoclient_connection_close(pcb,es);
#ifdef SERIAL_DEBUG
	printf("close TCP connection.state=%d\r\n",pcb->state);
#endif 
	
		return NULL;
}

/**
  * @brief Function called when TCP connection established
  * @param tpcb: pointer on the connection contol block
  * @param err: when connection correctly established err should be ERR_OK 
  * @retval err_t: returned error 
  */
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct echoclient *es = NULL;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
  if (err == ERR_OK)   
  {
    /* allocate structure es to maintain tcp connection informations */
    es = (struct echoclient *)mem_malloc(sizeof(struct echoclient));
    if (es != NULL)
    {
      es->state = ES_CONNECTED;
      es->pcb = tpcb;
      
      sprintf((char*)LocalPoint, "TCP connect ok\r\n");
//      sprintf((char*)LocalPoint, "POST http://admin.freecharge168.com/heart_log HTTP/1.1\r\nUser-Agent: Fiddler\r\nContent-type: application/json;charset=utf8;\r\nHost: admin.freecharge168.com\r\nContent-Length: 124\r\n");
//      sprintf((char*)LocalPoint, "%s\r\n{\"terminal\" : \"zhzq-wangjign-text\",\r\n\"software_version\" : \"1.0.0\",\r\n\"client_time\" : \"2017-04-07 11:18:47\",\r\n\"traffic\" : \"0\"}", LocalPoint);
			printf((char*)LocalPoint);
			{
			/* pass newly allocated es structure as argument to tpcb */
			tcp_arg(tpcb, es);

			/* initialize LwIP tcp_recv callback function */ 
			tcp_recv(tpcb, tcp_echoclient_recv);

			/* initialize LwIP tcp_sent callback function */
			tcp_sent(tpcb, tcp_echoclient_sent);

			/* initialize LwIP tcp_poll callback function */
			tcp_poll(tpcb, tcp_echoclient_poll, 1);
			} 
	//		if(tpcb->1)    //TCP1 ->soket
			if((tpcb->remote_ip.addr ==soket_pcb->remote_ip.addr) &&(tpcb->remote_port ==soket_pcb->remote_port))    //
			{			
			/* allocate pbuf */
				es->p_tx = NULL;
			}
	//		if(tpcb->1)    //TCP2 ->http
			else if((tpcb->remote_ip.addr==http_pcb->remote_ip.addr) &&(tpcb->remote_port ==http_pcb->remote_port))    //
			{
	//				make_http_post(tcp2_txbuf);
					HttpM_init.HttpUrl->sent(tcp2_txbuf); 
	//				dwgl_hex_send(tpcb, tcp2_txbuf,strlen((char*)tcp2_txbuf));			
					udp_demo_senddata(udpdebug_pcb,tcp2_txbuf);
					sprintf((char*)LocalPoint, "HTTP send:%d\r\n",time_sys);
					Str_addto_Str(&DebugStr,LocalPoint);
					tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
					printf("HTTP send:%d:%s\r\n",time_sys,tcp2_txbuf);
				
				/* allocate pbuf */
	//      es->p_tx = NULL;
				es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)tcp2_txbuf) , PBUF_POOL);
				if (es->p_tx)
				{       
					/* copy data to pbuf */
				 pbuf_take(es->p_tx, (char*)tcp2_txbuf, strlen((char*)tcp2_txbuf));
					
					/* send data */
					tcp_echoclient_send(tpcb,es); 
				}
				else
				{
					printf("HTTP send:%d:ERR_MEM\r\n",time_sys);
					return ERR_MEM;  
				}
			}
			
    }
    else
    {
      /* close connection */
      tcp_echoclient_connection_close(tpcb, es);
      
      /* return memory allocation error */
      return ERR_MEM;  
    }
  }
  else
  {
    /* close connection */
    tcp_echoclient_connection_close(tpcb, es);
  }
  return err;
}
    
/**
  * @brief tcp_receiv callback
  * @param arg: argument to be passed to receive callback 
  * @param tpcb: tcp connection control block 
  * @param err: receive error code 
  * @retval err_t: retuned error  
  */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ 
	char *recdata=0;
	struct pbuf *p_temp;
  struct echoclient *es;
  err_t ret_err;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
  LWIP_ASSERT("arg != NULL",arg != NULL);
  
  es = (struct echoclient *)arg;
	
  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p_tx == NULL)
    {
       /* we're done sending, close connection */
       tcp_echoclient_connection_close(tpcb, es);
    }
    else
    {    
      /* send remaining data*/
      tcp_echoclient_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }   
  /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    pbuf_free(p);

    ret_err = err;
  }
  else if(es->state == ES_CONNECTED)
  {
    /* increment message count */
    message_count++;     
	
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);  
		
#ifdef SERIAL_DEBUG		
		printf("tcp_rec:%dB: ",p->tot_len);		
		p_temp = p;
		while(p_temp)
		{
			recdata=(char *)malloc(p_temp->len*sizeof(char));
			if(recdata!=NULL)
			{
				memcpy(recdata,p_temp->payload,p_temp->len);
				printf("%s",recdata);
			}
			free(recdata);
			p_temp = p_temp->next;
		}
		printf("\r\n");
#endif	
		sprintf((char*)LocalPoint, "tcp_rec:%dB\r\n",p->tot_len);
		udp_demo_senddata(udpdebug_pcb,LocalPoint);

//		if(tpcb->1)    //TCP1 ->soket
		if((tpcb->remote_ip.addr ==soket_pcb->remote_ip.addr) &&(tpcb->remote_port ==soket_pcb->remote_port))    //
		{
			ChargeM_init.RxLength = p->tot_len;
			pbuf_copy_partial(p, tcp1_rxbuf, p->tot_len, 0);
			memcpy((char*)LocalPoint, tcp1_rxbuf,p->len);  //最多取一个pbuf的内容
			tcp1_rxbuf[p->tot_len]=0;
			LocalPoint[p->len]=0;
			LocalPoint[32] = 0;    //最多取pbuf的32B内容
			sprintf((char*)LocalPoint, "%s\n",LocalPoint);
			Str_addto_Str(&DebugStr,LocalPoint);
			sprintf((char*)LocalPoint, "soket_tcp_rec:%dB\n",p->tot_len);
			Str_addto_Str(&DebugStr,LocalPoint);
//			Str_addto_Str(&DebugStr,"soket_tcp_rec:");
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		}
		/* free received pbuf*/
//		if(tpcb->1)    //TCP2 ->http
		if((tpcb->remote_ip.addr==http_pcb->remote_ip.addr) &&(tpcb->remote_port ==http_pcb->remote_port))    //
		{
			HttpM_init.RxLength = p->tot_len;
			pbuf_copy_partial(p, tcp2_rxbuf, p->tot_len, 0);
			memcpy((char*)LocalPoint, tcp2_rxbuf,p->len);  //最多取一个pbuf的内容
			tcp2_rxbuf[p->tot_len]=0;
			LocalPoint[p->len]=0;
			LocalPoint[32] = 0;    //最多取pbuf的32B内容
			sprintf((char*)LocalPoint, "%s\n",LocalPoint);
			Str_addto_Str(&DebugStr,LocalPoint);
			sprintf((char*)LocalPoint, "http_tcp_rec:%dB\n",p->tot_len);
			Str_addto_Str(&DebugStr,LocalPoint);
//			Str_addto_Str(&DebugStr,"http_tcp_rec:");
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
		}		
			
		/* free received pbuf*/
    pbuf_free(p);
    ret_err = ERR_OK;
  }

  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    
		recdata=(char *)malloc(p->len*sizeof(char));
		if(recdata!=NULL)
		{
			memcpy(recdata,p->payload,p->len);
			printf("tcp_rec<<%s",recdata);
		}
		free(recdata);

    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief function used to send data
  * @param  tpcb: tcp control block
  * @param  es: pointer on structure of type echoclient containing info on data 
  *             to be sent
  * @retval None 
  */
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
 
  while ((wr_err == ERR_OK) &&
         (es->p_tx != NULL) && 
         (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {
    
    /* get pointer on pbuf from es structure */
    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    
    if (wr_err == ERR_OK)
    { 
      /* continue with next pbuf in chain (if any) */
      es->p_tx = ptr->next;
      
      if(es->p_tx != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p_tx);
      }
      
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
     es->p_tx = ptr;
   }
   else
   {		 
     /* other problem ?? */
   }
  }
}

/**
  * @brief  This function implements the tcp_poll callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: tcp connection control block
  * @retval err_t: error code
  */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct echoclient *es;

  es = (struct echoclient*)arg;
  if (es != NULL)
  {
    if (es->p_tx != NULL)
    {
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_echoclient_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /* close tcp connection */
        tcp_echoclient_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data) 
  * @param  arg: pointer on argument passed to callback
  * @param  tcp_pcb: tcp connection control block
  * @param  len: length of data sent 
  * @retval err_t: returned error code
  */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct echoclient *es;

  LWIP_UNUSED_ARG(len);

  es = (struct echoclient *)arg;
  
  if(es->p_tx != NULL)
  {
    /* still got pbufs to send */
    tcp_echoclient_send(tpcb, es);
  }

  return ERR_OK;
}

/**
  * @brief This function is used to close the tcp connection with server
  * @param tpcb: tcp connection control block
  * @param es: pointer on echoclient structure
  * @retval None
  */
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es )
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);

  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
  
}

#endif /* LWIP_TCP */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
