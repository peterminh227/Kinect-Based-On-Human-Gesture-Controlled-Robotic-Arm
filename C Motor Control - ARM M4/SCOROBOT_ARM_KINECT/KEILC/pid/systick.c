/**
  ******************************************************************************
  * @file    systick.c 
  ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include <stdio.h>


void TimingDelay_Decrement(void);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: SysTick_Handler
Decription: This function handles SysTick Handler.
Input: None
Output: None
*******************************************************************************/
void SysTick_Handler(void)
    {
    TimingDelay_Decrement();
    }
void Delay(__IO uint32_t nTime)
{
  uwTimingDelay=nTime;
	while(uwTimingDelay !=0);
}
/*******************************************************************************
Function name: SysTick_Init
Decription: This function initialize SysTick Timebase.
Input: None
Output: None
*******************************************************************************/
void SysTick_Init(uint32_t time)
{
    if (SysTick_Config(SystemCoreClock / time))
    { 
        /* Capture error */ 
        while (1);
    }
}

void TimingDelay_Decrement(void)
    {
            if(uwTimingDelay != 0)
              {
                uwTimingDelay--;
              }

    }

/******************* (C) COPYRIGHT 2011 STMicroelectronics - 2013 Robot Club BKHCM *****END OF FILE****/

