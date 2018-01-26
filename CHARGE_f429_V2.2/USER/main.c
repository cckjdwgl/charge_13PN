/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   TCP Client����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t EthLinkStatus;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
//	u32 temp;
//	uint8_t flag=0;
	unsigned char *LocalPoint;
//	unsigned int LocalPointSize;
	
	LocalPoint = data_b;
	/* ��ʼ��LED */
	BSP_GPIO_Config();
	LED_GPIO_Config();
	/* ��ʼ������ */
	Key_GPIO_Config();
	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();
	BSP_ADC_Init();
	SPI_FLASH_Init();
	
	LCD_Init_BSP();
	NVIC_Configuration();
	/* ��ʼ��ϵͳ�δ�ʱ�� */	
	SysTick_Init();
	
	TIM3_Config(999,899);//10ms��ʱ��
	printf("��̫��ͨ��ʵ������\n");
	
	LCD_Init();
	LCD_Init1();
	
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	dwgl_FatInit();
	
	time_ms=time_sys;
	LED_ON;
	while(time_sys-time_ms<100);
	time_ms=time_sys;
	LED_OFF;
	while(time_sys-time_ms<100);
	time_ms=time_sys;
	LED_ON;
	while(time_sys-time_ms<100);
	time_ms=time_sys;
	LED_OFF;
	while(time_sys-time_ms<100);
	time_ms=time_sys;
	LED_ON;
	while(time_sys-time_ms<100);
	time_ms=time_sys;
	LED_OFF;
	while(time_sys-time_ms<100);
	time_ms=time_sys;

	make_device_mess();
	make_soket_mess();
	make_http_mess();
	make_udpdebug_mess();
/* Configure ethernet (GPIOs, clocks, MAC, DMA) */
  ETH_BSP_Config();		
  printf("PHY��ʼ������\n");
	
  /* Initilaize the LwIP stack */
	lwip_default_ip_set();
  LwIP_Init();	
	dns_init();
  
  /* IP��ַ�Ͷ˿ڿ���netconf.h�ļ��޸ģ�����ʹ��DHCP�����Զ���ȡIP
	(��Ҫ·����֧��)*/
  printf("����IP�Ͷ˿�: %d.%d.%d.%d\n",soket_lwip.ip[0],soket_lwip.ip[1],soket_lwip.ip[2],soket_lwip.ip[3]);
  printf("Զ��IP�Ͷ˿�: %d.%d.%d.%d:%d\n",soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
	
	sprintf((char*)LocalPoint, "CCKJ CHARGE Version:%s\n",device.Version);
	Str_addto_Str(&DebugStr,LocalPoint);
	sprintf((char*)LocalPoint, "SELF_MAC: %d.%d.%d.%d.%d.%d\n",soket_lwip.mac[0],soket_lwip.mac[1],soket_lwip.mac[2],soket_lwip.mac[3],soket_lwip.mac[4],soket_lwip.mac[5]);
	Str_addto_Str(&DebugStr,LocalPoint);
	sprintf((char*)LocalPoint, "SELF_IP: %d.%d.%d.%d\n",soket_lwip.ip[0],soket_lwip.ip[1],soket_lwip.ip[2],soket_lwip.ip[3]);
	Str_addto_Str(&DebugStr,LocalPoint);
	sprintf((char*)LocalPoint, "SOKET_IP: %d.%d.%d.%d.%d\n",soket_lwip.remoteip[0],soket_lwip.remoteip[1],soket_lwip.remoteip[2],soket_lwip.remoteip[3],soket_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);	
	sprintf((char*)LocalPoint, "HTTP_IP: %d.%d.%d.%d.%d\n",http_lwip.remoteip[0],http_lwip.remoteip[1],http_lwip.remoteip[2],http_lwip.remoteip[3],http_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);	
	sprintf((char*)LocalPoint, "UDPDEBUG_IP: %d.%d.%d.%d.%d\n",udpdebug_lwip.remoteip[0],udpdebug_lwip.remoteip[1],udpdebug_lwip.remoteip[2],udpdebug_lwip.remoteip[3],udpdebug_lwip.remote_port);
	Str_addto_Str(&DebugStr,LocalPoint);	
	tft_DisplayStr_debug(tft_debug_x,tft_debug_y, &DebugStr,WHITE, BLACK,3);

//	tcp_echoclient_disconnect(soket_pcb);
  dns_demo_test();
//	udp_demo_test();
	
	time_ms=time_sys;
	LED_ON;
	while(time_sys-time_ms<500);
	time_ms=time_sys;
	LED_OFF;
	while(time_sys-time_ms<500);
	time_ms=time_sys;
	LED_ON;
	while(time_sys-time_ms<500);
	time_ms=time_sys;
	LED_OFF;
	while(time_sys-time_ms<500);
	
	time_ms=time_sys;	
		
	while(1)
	{
		/* check if any packet received */
    if (ETH_CheckFrameReceived())
    { 
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
		
		
		if(((time_sys-time_ms)>5000))
		{
			time_ms =time_sys;
			LED_TOGGLE;
//			if(DHCP_state !=DHCP_ADDRESS_ASSIGNED)
//			{
//				/* Configure ethernet (GPIOs, clocks, MAC, DMA) */
//					ETH_BSP_Config();		
//					printf("PHY��ʼ������\n");					
//					/* Initilaize the LwIP stack */
//					lwip_default_ip_set();
//					LwIP_Init();	
//					dns_init();
//			}			
		}
		tcpl_task(soket_pcb);
		tcp2_task(http_pcb);
		udp1_task(udpdebug_pcb);
		LCD_task();
		charge_task();
		key_task();
		
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
