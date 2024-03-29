#include "Pid.h"
#include "stdlib.h"
#include "Motor.h"
#include "Sensor.h"
#include "stdio.h"
#include "Structure.h"
#include "caculator_setpoint.h"
PID_Index PID_BASE_MOTOR;
PID_Index PID_SHOULDER_MOTOR;
PID_Index PID_ELBOW_MOTOR;


void PID_Init(void)
{
PID_BASE_MOTOR.e[0]=PID_BASE_MOTOR.e[1]=PID_BASE_MOTOR.e[2]=0;
PID_BASE_MOTOR.Kp= 0.412;//0.54//0.4;//0.34//0.24//0.29 --0.29
PID_BASE_MOTOR.Ki=0.385;//0.058//0.078//0.0088   //--0.298
PID_BASE_MOTOR.Kd=-0.00245;//-0.00068/-000198   //--0.00157
PID_SHOULDER_MOTOR.e[0]=PID_SHOULDER_MOTOR.e[1]=PID_SHOULDER_MOTOR.e[2]=0;
PID_SHOULDER_MOTOR.Kp=1700;//0.47//0.95--2000
PID_SHOULDER_MOTOR.Ki=0.7;//0.08884//0.25--0.9
PID_SHOULDER_MOTOR.Kd=-0.012;//0.000797//-0.095--0.015

PID_ELBOW_MOTOR.e[0]=PID_ELBOW_MOTOR.e[1]=PID_ELBOW_MOTOR.e[2]=0;
PID_ELBOW_MOTOR.Kp=0.39;
PID_ELBOW_MOTOR.Kd=-0.00189;
PID_ELBOW_MOTOR.Ki=1.07;	
}
void Sample_Pid_BASE_ELBOW(void)
{
  PID_BASE_MOTOR.Current+=encoder_Base_p;
  PID_ELBOW_MOTOR.Current+=encoder_Elbow_p;
  Encoder_Reset();
	PID_BASE_MOTOR.enable=1;
	PID_ELBOW_MOTOR.enable=1;
	
}
void Sample_Pid_Shoulder(void)
{
ADC_SoftwareStartConv(SHOULDER);
PID_SHOULDER_MOTOR.Current=	uhADCxConvertedValue;
PID_SHOULDER_MOTOR.enable=1;
}
void Cal_PID_Base_Position(int32_t p_set)
{
if (PID_BASE_MOTOR.enable)
{
PID_BASE_MOTOR.enable=0;
	  PID_BASE_MOTOR.SetPoint=p_set;
    PID_BASE_MOTOR.a0=(PID_BASE_MOTOR.Kp+PID_BASE_MOTOR.Ki*0.01/2+PID_BASE_MOTOR.Kd*100 );
		PID_BASE_MOTOR.a1 =(-PID_BASE_MOTOR.Kp+PID_BASE_MOTOR.Ki *0.01/2-2*PID_BASE_MOTOR.Kd*100 );
		PID_BASE_MOTOR.a2 =PID_BASE_MOTOR.Kd*100;
		PID_BASE_MOTOR.e[2]=p_set-PID_BASE_MOTOR.Current;
		PID_BASE_MOTOR.e[0]=PID_BASE_MOTOR.e[1];
		PID_BASE_MOTOR.e[1]=PID_BASE_MOTOR.e[2];
		PID_BASE_MOTOR.Pid_Result_Temp =PID_BASE_MOTOR.Pid_Result;
		PID_BASE_MOTOR.Pid_Result=PID_BASE_MOTOR.Pid_Result_Temp 
		+PID_BASE_MOTOR.a0*PID_BASE_MOTOR.e[2]
		+PID_BASE_MOTOR.a1*PID_BASE_MOTOR.e[1]
		+PID_BASE_MOTOR.a2*PID_BASE_MOTOR.e[0];
		if(PID_BASE_MOTOR.Pid_Result >MAX_SPEED_PWM) PID_BASE_MOTOR.Pid_Result=MAX_SPEED_PWM;
		if(PID_BASE_MOTOR.Pid_Result <-MAX_SPEED_PWM) PID_BASE_MOTOR.Pid_Result=-MAX_SPEED_PWM;
		if(abs(PID_BASE_MOTOR.e[2])<=1) PID_BASE_MOTOR.Pid_Result=0;	
			// Dat gia tri vao PWM	
		Base_Motor_Speed((int32_t )PID_BASE_MOTOR.Pid_Result);
	}
		else return;	
}


void Cal_PID_ELBOW_Position(int32_t p_set)
{
if (PID_ELBOW_MOTOR.enable)
{
PID_ELBOW_MOTOR.enable=0;
	PID_ELBOW_MOTOR.SetPoint=p_set;
    PID_ELBOW_MOTOR.a0=(PID_ELBOW_MOTOR.Kp+PID_ELBOW_MOTOR.Ki*0.01/2+PID_ELBOW_MOTOR.Kd*100 );
		PID_ELBOW_MOTOR.a1 =(-PID_ELBOW_MOTOR.Kp+PID_ELBOW_MOTOR.Ki *0.01/2-2*PID_ELBOW_MOTOR.Kd*100 );
		PID_ELBOW_MOTOR.a2 =PID_ELBOW_MOTOR.Kd*100;
		PID_ELBOW_MOTOR.e[2]=p_set-PID_ELBOW_MOTOR.Current;
		PID_ELBOW_MOTOR.e[0]=PID_ELBOW_MOTOR.e[1];
		PID_ELBOW_MOTOR.e[1]=PID_ELBOW_MOTOR.e[2];
		PID_ELBOW_MOTOR.Pid_Result_Temp =PID_ELBOW_MOTOR.Pid_Result;
		PID_ELBOW_MOTOR.Pid_Result=PID_ELBOW_MOTOR.Pid_Result_Temp 
		+PID_ELBOW_MOTOR.a0*PID_ELBOW_MOTOR.e[2]
		+PID_ELBOW_MOTOR.a1*PID_ELBOW_MOTOR.e[1]
		+PID_ELBOW_MOTOR.a2*PID_ELBOW_MOTOR.e[0];
		if(PID_ELBOW_MOTOR.Pid_Result >MAX_SPEED_PWM) PID_ELBOW_MOTOR.Pid_Result=MAX_SPEED_PWM;
		if(PID_ELBOW_MOTOR.Pid_Result <-MAX_SPEED_PWM) PID_ELBOW_MOTOR.Pid_Result=-MAX_SPEED_PWM;
		if(abs(PID_ELBOW_MOTOR.e[2])<=1)
		{
			PID_ELBOW_MOTOR.Pid_Result=400;
		
		}
			// Dat gia tri vao PWM	
		Elbow_Motor_Speed((int32_t )PID_ELBOW_MOTOR.Pid_Result);
	}
		else return;	
}



void Cal_PID_SHOULDER_Position(int16_t p_set)
{
if (PID_SHOULDER_MOTOR.enable)
{   
    if(p_set <1200)
		{
			PID_SHOULDER_MOTOR.SetPoint=PID_SHOULDER_MOTOR.Current;
		}
		else
		{
  PID_SHOULDER_MOTOR.SetPoint=p_set;
		}
	PID_SHOULDER_MOTOR.enable=0;
    PID_SHOULDER_MOTOR.a0=(PID_SHOULDER_MOTOR.Kp+PID_SHOULDER_MOTOR.Ki*0.01/2+PID_SHOULDER_MOTOR.Kd*100 );
		PID_SHOULDER_MOTOR.a1 =(-PID_SHOULDER_MOTOR.Kp+PID_SHOULDER_MOTOR.Ki *0.01/2-2*PID_SHOULDER_MOTOR.Kd*100 );
		PID_SHOULDER_MOTOR.a2 =PID_SHOULDER_MOTOR.Kd*100;
		PID_SHOULDER_MOTOR.e[2]=PID_SHOULDER_MOTOR.SetPoint-PID_SHOULDER_MOTOR.Current;
		PID_SHOULDER_MOTOR.e[0]=PID_SHOULDER_MOTOR.e[1];
		PID_SHOULDER_MOTOR.e[1]=PID_SHOULDER_MOTOR.e[2];
		PID_SHOULDER_MOTOR.Pid_Result_Temp =PID_SHOULDER_MOTOR.Pid_Result;
		PID_SHOULDER_MOTOR.Pid_Result=PID_SHOULDER_MOTOR.Pid_Result_Temp 
		+PID_SHOULDER_MOTOR.a0*PID_SHOULDER_MOTOR.e[2]
		+PID_SHOULDER_MOTOR.a1*PID_SHOULDER_MOTOR.e[1]
		+PID_SHOULDER_MOTOR.a2*PID_SHOULDER_MOTOR.e[0];
		if(PID_SHOULDER_MOTOR.Pid_Result >MAX_SPEED_PWM) PID_SHOULDER_MOTOR.Pid_Result=MAX_SPEED_PWM;
		if(PID_SHOULDER_MOTOR.Pid_Result <-MAX_SPEED_PWM) PID_SHOULDER_MOTOR.Pid_Result=-MAX_SPEED_PWM;
		if(abs(PID_SHOULDER_MOTOR.e[2])<=20) PID_SHOULDER_MOTOR.Pid_Result=0;	
			// Dat gia tri vao PWM	
		Shoulder_Motor_Speed((int32_t )PID_SHOULDER_MOTOR.Pid_Result);
	}
		else return;	
}
