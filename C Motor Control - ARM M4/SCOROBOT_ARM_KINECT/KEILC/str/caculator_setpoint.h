#ifndef __CACULATOR_SETPOINT_H
#define __CACULATOR_SETPOINT_H
#include "stdint.h"
extern float map;
extern int16_t tim_index;
extern int8_t flag_cal;
extern int32_t value_setpoint;
extern float a1,a2,a3,v01,v02,v03,x1,x2,x3;
extern float max_speed;
extern int32_t setpoint_previous;
void Timer_Caculator_Init(void);
void Enable_interrupt_timer2(void);
void Disable_interrupt_timer2(void);
void TIM2_IRQHandler(void);
void Cal_Setpoint(int32_t set_point);
void Cal_SetPoint_Base(int32_t set_point);
void increase_tim(void);
void Cal_SetPoint_Shoulder(int32_t set_point);
void Cal_SetPoint_Elbow(int32_t set_point);
void Cal_Base_Linear(int32_t SetPoint);
void khongche_shoulder(void);
typedef struct
{
  int32_t value_setpoint;
	float a1,a2,a3,v01,v02,v03,x1,x2,x3;
	float maxspeed ;
	int32_t setpoint_previous;
	int32_t timer_counter;

}CAL_SETPOINT;
extern CAL_SETPOINT CAL_BASE;
extern CAL_SETPOINT CAL_SHOULDER;
extern CAL_SETPOINT CAL_ELBOW;
#endif