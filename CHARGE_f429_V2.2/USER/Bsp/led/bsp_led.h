#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#ifndef ON
#define ON  0
#endif

#ifndef OFF
#define OFF 1
#endif

/* ���κ꣬��������������һ��ʹ�� */
#define R_LED(a)	if (a)	\
					GPIO_SetBits(R_LED_PORT,R_LED_PIN);\
					else		\
					GPIO_ResetBits(R_LED_PORT,R_LED_PIN)

#define L_LED(a)	if (a)	\
					GPIO_SetBits(L_LED_PORT,L_LED_PIN);\
					else		\
					GPIO_ResetBits(L_LED_PORT,L_LED_PIN)

#define LED(a)	if (a)	\
					GPIO_SetBits(LED_PORT,LED_PIN);\
					else		\
					GPIO_ResetBits(LED_PORT,LED_PIN)
					

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define R_LED_TOGGLE		digitalToggle(R_LED_PORT,R_LED_PIN)
#define R_LED_OFF		    digitalLo(R_LED_PORT,R_LED_PIN)
#define R_LED_ON			  digitalHi(R_LED_PORT,R_LED_PIN)

#define L_LED_TOGGLE		digitalToggle(L_LED_PORT,L_LED_PIN)
#define L_LED_OFF		    digitalLo(L_LED_PORT,L_LED_PIN)
#define L_LED_ON			  digitalHi(L_LED_PORT,L_LED_PIN)

#define LED_TOGGLE		digitalToggle(LED_PORT,LED_PIN)
#define LED_OFF		    digitalLo(LED_PORT,LED_PIN)
#define LED_ON			  digitalHi(LED_PORT,LED_PIN)



void LED_GPIO_Config(void);

#endif /* __LED_H */
