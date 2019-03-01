/*
*
*
*/
#ifndef _PID_H
#define _PID_H
#include "Structure.h"

typedef struct
{
float Kp;
	float Ki;
	float Kd;
	float e[3];
	float Pid_Result;
	float Pid_Result_Temp;
	float a0,a1,a2;
	int32_t SetPoint;
	int32_t Current;
	int16_t enable;

}PID_Index;
extern PID_Index PID_BASE_MOTOR;
extern PID_Index PID_SHOULDER_MOTOR;
extern PID_Index PID_ELBOW_MOTOR;
void PID_Init(void);
void Sample_Pid_BASE_ELBOW(void);
void Sample_Pid_Shoulder(void);
void Cal_PID_ELBOW_Position(int32_t p_set);
void Cal_PID_SHOULDER_Position(int16_t p_set);
void Cal_PID_Base_Position(int32_t p_set);
#endif