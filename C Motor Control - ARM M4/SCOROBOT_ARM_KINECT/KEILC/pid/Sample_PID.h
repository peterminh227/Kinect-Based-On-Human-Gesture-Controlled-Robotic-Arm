
#ifndef _SAMPLE_PID_H
#define _SAMPLE_PID_H
#include "stdint.h"
void Timer_Sample_Init(void);
void TIM5_IRQHandler(void);
extern void send_debug_success(void);
extern uint8_t buffer1[40];
extern void clear_debug(void);
extern uint8_t Mode_Response;
#define Home_Res  0x03
#define Busy_Res 0x04
#define Success_Res 0x05
#define Stop_Res 0x06
#define Test_Manual 0x07
#define Test_Auto   0x21
extern void Send_response_to_PC(
int32_t Base_curent ,
int32_t Base_set,
int32_t Shoulder_current,
int32_t Shoulder_set,
int32_t Elbow_Curent,
int32_t Elbow_set,
int32_t set_pitch,
int32_t set_roll,
int32_t set_gripper);
extern uint8_t Mode_Response;
extern int32_t duty_real;
#endif
