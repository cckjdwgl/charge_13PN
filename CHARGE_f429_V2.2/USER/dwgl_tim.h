//*******************************************
//dwgl for stm32f1XX
//V1.1 20160401
//*******************************************
#ifndef  __DWGL_TIM_H
#define  __DWGL_TIM_H
#include "stm32f4xx.h"

extern unsigned  int LocalTime; /* this variable is used to create a time reference incremented by 10ms */
/* Private function prototypes -----------------------------------------------*/
extern void TIM3_Config(uint16_t period,uint16_t prescaler);
#endif




