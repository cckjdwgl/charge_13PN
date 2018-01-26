/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Network connection configuration
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
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netconf.h"
#include <stdio.h>
#include "stm32f429_phy.h"

/* Private typedef -----------------------------------------------------------*/
#define MAX_DHCP_TRIES        4

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif gnetif;  //网卡1
uint32_t TCPTimer = 0;
uint32_t ARPTimer = 0;
uint32_t LinkTimer = 0;
uint32_t IPaddress = 0;

 __lwip_dev self_default_lwip;	//lwip控制结构体
 __lwip_dev soket_lwip;	//lwip控制结构体
 __lwip_dev http_lwip;	//lwip控制结构体
 __lwip_dev dnsdebug_lwip;	//lwip控制结构体
 __lwip_dev udpdebug_lwip;	//lwip控制结构体

#ifdef USE_DHCP
#include "Bsp/led/bsp_led.h" 
uint32_t DHCPfineTimer = 0;
uint32_t DHCPcoarseTimer = 0;
__IO uint8_t DHCP_state;
#endif
extern __IO uint32_t  EthStatus;

/* Private functions ---------------------------------------------------------*/
void LwIP_DHCP_Process_Handle(void);
/**
* @brief  Initializes the lwIP stack
* @param  None
* @retval None
*/

void lwip_default_ip_set(void)
{
//	u32 sn0_temp;
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	sn0+=*(vu32*)(0x1FFF7A14);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	sn0+=*(vu32*)(0x1FFF7A18);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	sn0+=(sn0>>16);//获取STM32的唯一ID的前24位作为MAC地址后三字节
//------------------soket_lwip------------------  
//默认远端IP为:192.168.1.100
//	soket:112.124.34.151:8000   20171012	
//	soket_lwip.remoteip[0]=192;	
//	soket_lwip.remoteip[1]=168;
//	soket_lwip.remoteip[2]=0;
//	soket_lwip.remoteip[3]=100;	
//	soket_lwip.remote_port = 6000;	
	soket_lwip.remoteip[0]=112;	
	soket_lwip.remoteip[1]=124;
	soket_lwip.remoteip[2]=34;
	soket_lwip.remoteip[3]=151;	
	soket_lwip.remote_port = 8000;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	soket_lwip.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	soket_lwip.mac[1]=0;
	soket_lwip.mac[2]=0;
	soket_lwip.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	soket_lwip.mac[4]=(sn0>>8)&0XFFF;;
	soket_lwip.mac[5]=sn0&0XFF; 
	//默认本地IP为:192.168.0.200
	soket_lwip.ip[0]=192;	
	soket_lwip.ip[1]=168;
	soket_lwip.ip[2]=0;
	soket_lwip.ip[3]=200;
	//默认子网掩码:255.255.255.0
	soket_lwip.netmask[0]=255;	
	soket_lwip.netmask[1]=255;
	soket_lwip.netmask[2]=255;
	soket_lwip.netmask[3]=0;
	//默认网关:192.168.1.1
	soket_lwip.gateway[0]=192;	
	soket_lwip.gateway[1]=168;
	soket_lwip.gateway[2]=0;
	soket_lwip.gateway[3]=1;	
	
//------------------http_lwip------------------  
	//默认远端IP为:192.168.1.100
	http_lwip.remoteip[0]=192;	
	http_lwip.remoteip[1]=168;
	http_lwip.remoteip[2]=0;
	http_lwip.remoteip[3]=100;	
	http_lwip.remote_port = 8000;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	http_lwip.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	http_lwip.mac[1]=0;
	http_lwip.mac[2]=0;
	http_lwip.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	http_lwip.mac[4]=(sn0>>8)&0XFFF;;
	http_lwip.mac[5]=sn0&0XFF; 
	//默认本地IP为:192.168.0.200
	http_lwip.ip[0]=192;	
	http_lwip.ip[1]=168;
	http_lwip.ip[2]=0;
	http_lwip.ip[3]=200;
	//默认子网掩码:255.255.255.0
	http_lwip.netmask[0]=255;	
	http_lwip.netmask[1]=255;
	http_lwip.netmask[2]=255;
	http_lwip.netmask[3]=0;
	//默认网关:192.168.1.1
	http_lwip.gateway[0]=192;	
	http_lwip.gateway[1]=168;
	http_lwip.gateway[2]=0;
	http_lwip.gateway[3]=1;	
	
//------------------dnsdebug_lwip------------------  
	//默认远端IP为:192.168.1.100
	dnsdebug_lwip.remoteip[0]=192;	
	dnsdebug_lwip.remoteip[1]=168;
	dnsdebug_lwip.remoteip[2]=0;
	dnsdebug_lwip.remoteip[3]=100;	
	dnsdebug_lwip.remote_port = 8000;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	dnsdebug_lwip.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	dnsdebug_lwip.mac[1]=0;
	dnsdebug_lwip.mac[2]=0;
	dnsdebug_lwip.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	dnsdebug_lwip.mac[4]=(sn0>>8)&0XFFF;;
	dnsdebug_lwip.mac[5]=sn0&0XFF; 
	//默认本地IP为:192.168.0.200
	dnsdebug_lwip.ip[0]=192;	
	dnsdebug_lwip.ip[1]=168;
	dnsdebug_lwip.ip[2]=0;
	dnsdebug_lwip.ip[3]=200;
	//默认子网掩码:255.255.255.0
	dnsdebug_lwip.netmask[0]=255;	
	dnsdebug_lwip.netmask[1]=255;
	dnsdebug_lwip.netmask[2]=255;
	dnsdebug_lwip.netmask[3]=0;
	//默认网关:192.168.1.1
	dnsdebug_lwip.gateway[0]=192;	
	dnsdebug_lwip.gateway[1]=168;
	dnsdebug_lwip.gateway[2]=0;
	dnsdebug_lwip.gateway[3]=1;		
	
//------------------udpdebug_lwip------------------  
	//默认远端IP为:192.168.1.100
	udpdebug_lwip.remoteip[0]=0;	
	udpdebug_lwip.remoteip[1]=0;
	udpdebug_lwip.remoteip[2]=0;
	udpdebug_lwip.remoteip[3]=0;	
	udpdebug_lwip.remote_port = 32768;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	udpdebug_lwip.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	udpdebug_lwip.mac[1]=0;
	udpdebug_lwip.mac[2]=0;
	udpdebug_lwip.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	udpdebug_lwip.mac[4]=(sn0>>8)&0XFFF;;
	udpdebug_lwip.mac[5]=sn0&0XFF; 
	//默认本地IP为:192.168.0.200
	udpdebug_lwip.ip[0]=0;	
	udpdebug_lwip.ip[1]=0;
	udpdebug_lwip.ip[2]=0;
	udpdebug_lwip.ip[3]=0;
	udpdebug_lwip.local_port  =32767;
	//默认子网掩码:255.255.255.0
	udpdebug_lwip.netmask[0]=255;	
	udpdebug_lwip.netmask[1]=255;
	udpdebug_lwip.netmask[2]=255;
	udpdebug_lwip.netmask[3]=0;
	//默认网关:192.168.1.1
	udpdebug_lwip.gateway[0]=192;	
	udpdebug_lwip.gateway[1]=168;
	udpdebug_lwip.gateway[2]=0;
	udpdebug_lwip.gateway[3]=1;	
} 

void LwIP_Init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
  mem_init();

  /* Initializes the memory pools defined by MEMP_NUM_x.*/
  memp_init();

#ifdef USE_DHCP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#else
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif  
  
  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
  struct ip_addr *netmask, struct ip_addr *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))

  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface.*/
  netif_set_default(&gnetif);

  if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
  { 
    /* Set Ethernet link flag */
    gnetif.flags |= NETIF_FLAG_LINK_UP;

    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
#ifdef USE_DHCP
    DHCP_state = DHCP_START;
#else
#ifdef SERIAL_DEBUG
		printf("\n  Static IP address   \n");
		printf("IP: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
		printf("NETMASK: %d.%d.%d.%d\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
		printf("Gateway: %d.%d.%d.%d\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* SERIAL_DEBUG */
#endif /* USE_DHCP */
  }
  else
  {
    /*  When the netif link is down this function must be called.*/
    netif_set_down(&gnetif);
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
#ifdef SERIAL_DEBUG
		printf("\n  Network Cable is  \n");
		printf("    not connected   \n");
#endif /* SERIAL_DEBUG */
  }
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ETH_link_callback);
}

/**
* @brief  Called when a frame is received
* @param  None
* @retval None
*/
void LwIP_Pkt_Handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
  ethernetif_input(&gnetif);
}

/**
* @brief  LwIP periodic tasks
* @param  localtime the current LocalTime value
* @retval None
*/
void LwIP_Periodic_Handle(__IO uint32_t localtime)
{
#if LWIP_TCP
  /* TCP periodic process every 250 ms */
  if (localtime - TCPTimer >= TCP_TMR_INTERVAL)
  {
    TCPTimer =  localtime;
    tcp_tmr();
  }
#endif

  /* ARP periodic process every 5s */
  if ((localtime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  localtime;
    etharp_tmr();
  }

	/* Check link status periodically */
	if ((localtime - LinkTimer) >= LINK_TIMER_INTERVAL) {
		ETH_CheckLinkStatus(ETHERNET_PHY_ADDRESS);
		LinkTimer=localtime;
	}
	
#ifdef USE_DHCP
  /* Fine DHCP periodic process every 500ms */
  if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  localtime;
    dhcp_fine_tmr();
    if ((DHCP_state != DHCP_ADDRESS_ASSIGNED) && 
        (DHCP_state != DHCP_TIMEOUT) &&
          (DHCP_state != DHCP_LINK_DOWN))
    {
#ifdef SERIAL_DEBUG
			LED_TOGGLE;
			printf("\nFine DHCP periodic process every 500ms\n");
#endif /* SERIAL_DEBUG */

      /* process DHCP state machine */
      LwIP_DHCP_Process_Handle();    
    }
  }

  /* DHCP Coarse periodic process every 60s */
  if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
  {
    DHCPcoarseTimer =  localtime;
    dhcp_coarse_tmr();
  }
  
#endif
}

#ifdef USE_DHCP
/**
* @brief  LwIP_DHCP_Process_Handle
* @param  None
* @retval None
*/
void LwIP_DHCP_Process_Handle(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
  switch (DHCP_state)
  {
    case DHCP_START:
    {
      DHCP_state = DHCP_WAIT_ADDRESS;
      dhcp_start(&gnetif);
      /* IP address should be set to 0 
         every time we want to assign a new DHCP address */
      IPaddress = 0;
#ifdef SERIAL_DEBUG
			printf("\n     Looking for    \n");
			printf("     DHCP server    \n");
			printf("     please wait... \n");
#endif /* SERIAL_DEBUG */
#ifdef TFT_DEBUG
//			sprintf((char*)LocalPoint, "DEST_IP: %d.%d.%d.%d.%d\n",DEST_IP_ADDR0, DEST_IP_ADDR1,DEST_IP_ADDR2, DEST_IP_ADDR3,DEST_PORT);
			Str_addto_Str(&DebugStr,"     Looking for    \n");	
			Str_addto_Str(&DebugStr,"     DHCP server    \n");	
			Str_addto_Str(&DebugStr,"     please wait... \n");	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
#endif /* SERIAL_DEBUG */
    }
    break;
    
    case DHCP_WAIT_ADDRESS:
    {
      /* Read the new IP address */
      IPaddress = gnetif.ip_addr.addr;

      if (IPaddress!=0) 
      {
        DHCP_state = DHCP_ADDRESS_ASSIGNED;	

        /* Stop DHCP */
        dhcp_stop(&gnetif);

#ifdef SERIAL_DEBUG
      	printf("\n  IP address assigned \n");
				printf("    by a DHCP server   \n");
		    printf("IP: %d.%d.%d.%d\n",(uint8_t)(IPaddress),(uint8_t)(IPaddress >> 8),
				                       (uint8_t)(IPaddress >> 16),(uint8_t)(IPaddress >> 24));
				printf("NETMASK: %d.%d.%d.%d\n",(uint8_t)(gnetif.netmask.addr),(uint8_t)(gnetif.netmask.addr>>8),(uint8_t)(gnetif.netmask.addr>>16),(uint8_t)(gnetif.netmask.addr>>24));
				printf("Gateway: %d.%d.%d.%d\n",(uint8_t)(gnetif.gw.addr),(uint8_t)(gnetif.gw.addr>>8),(uint8_t)(gnetif.gw.addr>>16),(uint8_t)(gnetif.gw.addr>>24));
        LED_ON;
#endif /* SERIAL_DEBUG */
#ifdef TFT_DEBUG
			Str_addto_Str(&DebugStr,"IP address assigned \n");	
			Str_addto_Str(&DebugStr,"by a DHCP server\n");	
			sprintf((char*)LocalPoint, "SELF_IP: %d.%d.%d.%d\n",(uint8_t)(IPaddress),(uint8_t)(IPaddress >> 8),(uint8_t)(IPaddress >> 16),(uint8_t)(IPaddress >> 24));
			Str_addto_Str(&DebugStr,LocalPoint);	
			sprintf((char*)LocalPoint, "NETMASK: %d.%d.%d.%d\n",(uint8_t)(gnetif.netmask.addr),(uint8_t)(gnetif.netmask.addr>>8),(uint8_t)(gnetif.netmask.addr>>16),(uint8_t)(gnetif.netmask.addr>>24));
			Str_addto_Str(&DebugStr,LocalPoint);	
			sprintf((char*)LocalPoint, "Gateway: %d.%d.%d.%d\n",(uint8_t)(gnetif.gw.addr),(uint8_t)(gnetif.gw.addr>>8),(uint8_t)(gnetif.gw.addr>>16),(uint8_t)(gnetif.gw.addr>>24));
			Str_addto_Str(&DebugStr,LocalPoint);	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y,&DebugStr,WHITE, BLACK,3);
#endif /* SERIAL_DEBUG */
      }
      else
      {
        /* DHCP timeout */
        if (gnetif.dhcp->tries > MAX_DHCP_TRIES)
        {
          DHCP_state = DHCP_TIMEOUT;

          /* Stop DHCP */
          dhcp_stop(&gnetif);

          /* Static address used */
          IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
          IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
          IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
          netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);

#ifdef SERIAL_DEBUG
          printf("\n    DHCP timeout    \n");
          printf("  Static IP address   \n");
		      printf("IP: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
					printf("NETMASK: %d.%d.%d.%d\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
					printf("Gateway: %d.%d.%d.%d\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
          LED_ON;
#endif /* SERIAL_DEBUG */
#ifdef TFT_DEBUG
			Str_addto_Str(&DebugStr,"DHCP timeout\n");	
			Str_addto_Str(&DebugStr,"Static IP address\n");	
			tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);
#endif /* SERIAL_DEBUG */
        }
      }
    }
    break;
  default: break;
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
