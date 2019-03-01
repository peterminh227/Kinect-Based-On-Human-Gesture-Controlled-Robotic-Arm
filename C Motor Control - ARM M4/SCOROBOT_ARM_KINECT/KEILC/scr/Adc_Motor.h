/**
  ******************************************************************************
  * @file    Adc_Motor.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Header for main.c module
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
#ifndef __ADC_MOTOR_H
#define __ADC_MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include "math.h"

  #define ADCx                     ADC3
  #define ADC_CHANNEL              ADC_Channel_1
  #define ADCx_CLK                 RCC_APB2Periph_ADC3
  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOA
  #define GPIO_PIN                 GPIO_Pin_1
  #define GPIO_PORT                GPIOA
  #define DMA_CHANNELx             DMA_Channel_2
  #define DMA_STREAMx              DMA2_Stream0
  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)
	extern __IO uint16_t uhADCxConvertedValue ;
void ADC_Config(void);
void Get_Value(void);


/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
