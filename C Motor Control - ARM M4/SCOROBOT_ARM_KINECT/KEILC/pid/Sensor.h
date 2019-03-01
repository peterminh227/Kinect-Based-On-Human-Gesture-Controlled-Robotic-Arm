#ifndef	__SENSOR_H
#define __SENSOR_H
#include "stdint.h"
#include "stm32f4xx.h"
/* Define Encoder Sensor*/
#define RCC_ENCODER_PORT 											RCC_AHB1Periph_GPIOB
#define GPIO_ENC_PORT 												GPIOB
#define GPIO_ENC_BASE_PHASE_A 								GPIO_Pin_11
#define GPIO_ENC_BASE_PHASE_B 								GPIO_Pin_12
#define GPIO_ENC_ELBOW_PHASE_A 								GPIO_Pin_13
#define GPIO_ENC_ELBOW_PHASE_B 								GPIO_Pin_14
#define GPIO_ENCODER_PORTSOURCE							 	EXTI_PortSourceGPIOB
#define ENC_BASE_PHASE_A_PINSOURCE 						EXTI_PinSource11
#define ENC_BASE_PHASE_B_PINSOURCE 						EXTI_PinSource12
#define ENC_ELBOW_PHASE_A_PINSOURCE 					EXTI_PinSource13
#define ENC_ELBOW_PHASE_B_PINSOURCE 					EXTI_PinSource14
#define ENC_BASE_PHASE_A_Line 								EXTI_Line11
#define ENC_BASE_PHASE_B_Line 								EXTI_Line12
#define ENC_ELBOW_PHASE_A_Line 								EXTI_Line13
#define ENC_ELBOW_PHASE_B_Line 								EXTI_Line14
extern int32_t Elbow_Home_Position;
extern int8_t Base_Home_Possition;
extern int8_t Shoulder_Home_Possition;
void Encoder_Init(void);
void EXTI15_10_IRQHandler(void);
void Encoder_Reset(void);
/*
Define ADC

*/
#define SHOULDER 											ADC3
#define SHOULDER_CHANEL 							ADC_Channel_1
#define SHOULDER_CLK                	RCC_APB2Periph_ADC3
#define SHOULDER_CHANNEL_GPIO_CLK    	RCC_AHB1Periph_GPIOA
#define SHOULDER_GPIO_PIN             GPIO_Pin_1
#define SHOULDER_PORT                	GPIOA
#define SHOULDER_DMA_CHANNELx         DMA_Channel_2
#define SHOULDER_DMA_STREAMx          DMA2_Stream0
#define SHOULDER_DR_ADDRESS          ((uint32_t)0x4001224C)

extern __IO uint16_t uhADCxConvertedValue ;
extern int32_t encoder_Base_v,encoder_Base_p;
extern int32_t encoder_Elbow_v,encoder_Elbow_p;
void ADC_Config(void);




//define Microswitches interupt
#define Rcc_Microswitches_Port										RCC_AHB1Periph_GPIOB
#define GPIO_Microswitches_Port 									GPIOB
#define GPIO_Microswitches_Base 									GPIO_Pin_5
#define GPIO_Microswitches_Shoulder								GPIO_Pin_6
#define GPIO_Microswitches_Elbow									GPIO_Pin_7
#define GPIO_Microswitches_PortSource							EXTI_PortSourceGPIOB
#define Microswitches_Base_PinSource 							EXTI_PinSource5
#define Microswitches_Shoulder_PinSource 					EXTI_PinSource6
#define Microswitches_Elbow_PinSource  					  EXTI_PinSource7
#define Microswitches_Base_Line 									EXTI_Line5
#define Microswitches_Shoulder_Line 							EXTI_Line6
#define Microswitches_Elbow_Line 									EXTI_Line7
void Microswitches_Init(void);
void EXTI9_5_IRQHandler(void);
#endif