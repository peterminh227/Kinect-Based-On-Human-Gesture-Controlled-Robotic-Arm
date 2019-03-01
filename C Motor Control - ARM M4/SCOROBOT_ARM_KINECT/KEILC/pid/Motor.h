/*
*  @file Motor.h
*
*
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__MOTOR_H
#define __MOTOR_H
#include "stdint.h"
#include "stm32f4xx.h"
/* Private define ------------------------------------------------------------*/
#define RCC_TIM_SHOULDER_ELBOW_BASE    							RCC_APB1Periph_TIM3
#define RCC_PWM_SHOULDER_ELBOW_BASE_PORT 						RCC_AHB1Periph_GPIOC
#define TIM_SHOULDER_ELBOW_BASE 										TIM3
#define SHOULDER_ELBOW_BASE_PORT 										GPIOC
#define GPIO_PIN_PWM_SHOULDER 											GPIO_Pin_7
#define GPIO_PIN_PWM_ELBOW 													GPIO_Pin_8
#define GPIO_PIN_PWM_BASE 													GPIO_Pin_6
#define PIN_BASE_SOURCE 														GPIO_PinSource6
#define PIN_SHOULDER_SOURCE 												GPIO_PinSource7
#define PIN_ELBOW_SOURCE 														GPIO_PinSource8
#define GPIO_AF_TIM_SHOULDER_ELBOW_BASE 						GPIO_AF_TIM3


#define MAX_SPEED_PWM   3700
#define MIN_SPEED_PWM   0   

/////////////////////////////////////////////////////////////////////////////////
#define RCC_TIM_PITCH_ROLL_GRIPPER									RCC_APB1Periph_TIM4
#define RCC_PWM_PITCH_ROLL_GRIPPER_PORT 						RCC_AHB1Periph_GPIOD
#define TIM_PITCH_ROLL_GRIPPER											TIM4
#define PITCH_ROLL_GRIPPER_PORT 										GPIOD
#define GPIO_PIN_PWM_PITCH 													GPIO_Pin_12
#define GPIO_PIN_PWM_ROLL 													GPIO_Pin_13
#define GPIO_PIN_PWM_GRIPPER 												GPIO_Pin_14
#define PITCH_PINSOURCE															GPIO_PinSource12
#define ROLL_PINSOURCE															GPIO_PinSource13
#define GRIPPER_PINSOURCE													  GPIO_PinSource14
#define GPIO_AF_TIM_PITCH_ROLL_GRIPPER						  GPIO_AF_TIM4

///////////////////////////////////////////////////////////////////////////////////
#define GPIO_PIN_DIR_SHOULDER 											GPIO_Pin_2
#define GPIO_PIN_DIR_ELBOW 													GPIO_Pin_3
#define GPIO_PIN_DIR_BASE 													GPIO_Pin_9

void Configure_Timer_Base_Shoulder_Elbow(void);
void Confiure_Direction_Pin_Base_Shoulder_Elbow(void);
void Configure_Tim_Pitch_Roll_Gripper(void);
void Motor_Init(void);
void Base_Motor_Forward(uint16_t speed);
void Base_Motor_Reverse(uint16_t speed);
void Base_Motor_Stop(void);
void Base_Motor_Speed(int32_t speed);
void Shoulder_Motor_Forward(uint16_t speed);
void Shoulder_Motor_Reverse(uint16_t speed);
void Shoulder_Motor_Stop(void);
void Shoulder_Motor_Speed(int32_t speed);
void Elbow_Motor_Forward(uint16_t speed);
void Elbow_Motor_Reverse(uint16_t speed);
void Elbow_Motor_Stop(void);
void Elbow_Motor_Speed(int32_t speed);
extern int8_t flag_stop_elbow;
#endif
