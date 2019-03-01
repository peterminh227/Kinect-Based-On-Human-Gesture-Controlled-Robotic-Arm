
#ifndef _SYSTICK_H
#define _SYSTICK_H
#include "Stm32f4xx.h"
static __IO uint32_t uwTimingDelay;
 void Delay(__IO uint32_t nTime);
void SysTick_Handler(void);
void Delay(__IO uint32_t nTime);
void SysTick_Init(uint32_t time);
void TimingDelay_Decrement(void);
#endif