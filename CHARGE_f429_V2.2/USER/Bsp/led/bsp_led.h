#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#ifndef ON
#define ON  0
#endif

#ifndef OFF
#define OFF 1
#endif

/* 带参宏，可以像内联函数一样使用 */
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
					

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
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
