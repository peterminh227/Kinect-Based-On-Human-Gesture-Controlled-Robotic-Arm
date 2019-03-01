#ifndef __STRUCTURE_H
#define __STRUCTURE_H

#include "stm32f4xx.h"
extern GPIO_InitTypeDef GPIO_InitStructure;
extern NVIC_InitTypeDef NVIC_InitStructure;
extern USART_InitTypeDef USART_InitStructure;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
extern EXTI_InitTypeDef   EXTI_InitStructure;
extern SPI_InitTypeDef   SPI_InitStructure;
extern I2C_InitTypeDef  I2C_InitStructure;
extern DMA_InitTypeDef       DMA_InitStructure;
extern ADC_InitTypeDef       ADC_InitStructure;
extern ADC_CommonInitTypeDef ADC_CommonInitStructure;
#endif
