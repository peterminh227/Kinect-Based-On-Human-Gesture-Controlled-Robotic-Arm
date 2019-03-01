#include "Structure.h"
#include "Pid.h"
#include "Motor.h"
#include "Sensor.h"
#include "stdint.h"
#include "Sample_PID.h"
#include "USB_HID.h"
#include "caculator_setpoint.h"
#include <string.h>
int8_t clr_index;
uint8_t buffer1[40];
uint8_t Mode_Response;
int32_t duty_real;
void clear_debug(void)
{
for(clr_index=0;clr_index<64;clr_index++)
	{
	debugString[clr_index]=0;
	}
}
int32_t  tim_send_counter;
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
void send_debug_success(void)
{
if(tim_send_counter<250)
{

	{
	  Mode_Response=Busy_Res;
    tim_send_counter++;
	}
}
else if(tim_send_counter<400)
{ 
    {
			Mode_Response=Success_Res;
	    tim_send_counter++;
		}
}
else
{
tim_send_counter=0;

}

}
void Timer_Sample_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = 10000-1; // delay 10ms          
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	 //1mhz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	/* Immediate load of TIM2 Precaler values */
	TIM_PrescalerConfig(TIM5, 84-1, TIM_PSCReloadMode_Immediate);
	/* Clear TIM2 update pending flags */
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	/* TIM enable counter */
	TIM_Cmd(TIM5, ENABLE);	
	/* Enable TIM2 Update interrupts */
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	/* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void TIM5_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
			{ /*Sample Sensor of Scorobot*/
				Sample_Pid_BASE_ELBOW();
				Sample_Pid_Shoulder();
				//Cal_PID_Base_Position(1000);
				if(home_Request==1)
				{ 
				   Base_Motor_Speed(-875);
					Cal_PID_SHOULDER_Position(3300);
					TIM_PITCH_ROLL_GRIPPER->CCR1=2000;
					TIM_PITCH_ROLL_GRIPPER->CCR2=1400;
					TIM_PITCH_ROLL_GRIPPER->CCR3=600;
					if(Base_Home_Possition==1)
				    {
				    Base_Motor_Stop();
						PID_BASE_MOTOR.Current=0;
				    }
						if(Elbow_Home_Position==1)
				   {
						 Elbow_Motor_Stop();
						 PID_ELBOW_MOTOR.Current=0;
				   }
					 if(Shoulder_Home_Possition==1)
					 {
					   Shoulder_Motor_Stop();
						
						 {
						   if(Base_Home_Possition==1)
							 {
							 home_Request=0;
							Mode_Response=Home_Res;
				   
							 }
						 }
					 }
					 
				}

				/*End Home Request*/
				/* Receive data from PC and Run PID for base,Shoulder and elbow motor of scorobot*/
			
			  if(flag_process==1)
				 {
				  Cal_PID_SHOULDER_Position(CAL_SHOULDER.value_setpoint);
				  Cal_PID_ELBOW_Position(CAL_ELBOW.value_setpoint);
					Cal_PID_Base_Position(CAL_BASE.value_setpoint);
				  TIM_PITCH_ROLL_GRIPPER->CCR1=setpoint_pitch;
					TIM_PITCH_ROLL_GRIPPER->CCR2=1400;
					TIM_PITCH_ROLL_GRIPPER->CCR3=setpoint_gripper;
				 }
			
				/* End Process*/
			if(flag_Emegency_stop==1)
			{
				flag_process=0;
				flag_auto=0;
				flag_Receive_Data_Grid=0;
				flag_StopMotor_Base=0;
				flag_Reverse_Base=0;
				flag_Forward_Base =0;
				flag_Forward_Shoulder=0;
				flag_Reverse_Shoulder=0;
			  Base_Motor_Stop();
			  Shoulder_Motor_Stop();
			  Elbow_Motor_Stop();	
				Mode_Response=Stop_Res;
			}
			if(flag_Receive_Data_Grid==1)
			{
					//Cal_PID_SHOULDER_Position(setpoint_Shoulder);
				  Cal_PID_ELBOW_Position(setpoint_elbow);
					//Cal_PID_Base_Position(setpoint_base);
				  TIM_PITCH_ROLL_GRIPPER->CCR1=setpoint_pitch;
					TIM_PITCH_ROLL_GRIPPER->CCR2=setpoint_roll;
					TIM_PITCH_ROLL_GRIPPER->CCR3=setpoint_gripper;
				  Mode_Response=Test_Manual;
			}
			if(flag_auto==1)
			{
				Mode_Response=Test_Auto ;
				Cal_PID_SHOULDER_Position(CAL_SHOULDER.value_setpoint);
				  Cal_PID_ELBOW_Position(CAL_ELBOW.value_setpoint);
					Cal_PID_Base_Position(CAL_BASE.value_setpoint);
				  TIM_PITCH_ROLL_GRIPPER->CCR1=setpoint_pitch;
					TIM_PITCH_ROLL_GRIPPER->CCR2=1400;
					TIM_PITCH_ROLL_GRIPPER->CCR3=setpoint_gripper;
			}
			duty_real=Duty_Base*37;
			
		if(flag_Forward_Base)
		{
			if(PID_BASE_MOTOR.Current >14000)
			{
			Base_Motor_Stop();
			}
			else
			{
			Base_Motor_Forward(duty_real);
			}
		
		}
			if(flag_Reverse_Base)
				{
				if(PID_BASE_MOTOR.Current <=0)
				{
				Base_Motor_Stop();
				}
				else
				{
				Base_Motor_Reverse(duty_real);
			  }
				}
			
			if(flag_Forward_Shoulder)
			{
				if(PID_SHOULDER_MOTOR.Current >3250)
				{
				Shoulder_Motor_Stop();
				}
				else
				{
					Shoulder_Motor_Reverse(1591);
				}
			
			}
			if(flag_Reverse_Shoulder)
			{
				if(PID_SHOULDER_MOTOR.Current<1600)
				{
				Shoulder_Motor_Stop();
				}
				else
				{
				Shoulder_Motor_Forward(550);
				}
			
			}
			if(flag_StopMotor_Base)
			{
			Base_Motor_Stop();
			Shoulder_Motor_Stop();
			}
		}
			TIM_ClearFlag(TIM5,TIM_FLAG_Update);
}

void Send_response_to_PC(int32_t Base_curent ,int32_t Base_set,int32_t Shoulder_current,int32_t Shoulder_set,int32_t Elbow_Curent,int32_t Elbow_set,int32_t set_pitch,int32_t set_roll,int32_t set_gripper)
{

	buffer1[0]=38;
	buffer1[1]=Mode_Response;
	buffer1[2]=(uint8_t)(Base_curent>>24);
	buffer1[3]=(uint8_t)(Base_curent>>16);
	buffer1[4]=(uint8_t)(Base_curent>>8);
	buffer1[5]=(uint8_t)(Base_curent>>0);
	
	buffer1[6]=(uint8_t)(Base_set>>24);
	buffer1[7]=(uint8_t)(Base_set>>16);
  buffer1[8]=(uint8_t)(Base_set>>8);
	buffer1[9]=(uint8_t)(Base_set>>0);
	
	buffer1[10]=(uint8_t)(Shoulder_current>>24);
	buffer1[11]=(uint8_t)(Shoulder_current>>16);
	buffer1[12]=(uint8_t)(Shoulder_current>>8);
	buffer1[13]=(uint8_t)(Shoulder_current>>0);
	
	buffer1[14]=(uint8_t)(Shoulder_set>>24);
	buffer1[15]=(uint8_t)(Shoulder_set>>16);
  buffer1[16]=(uint8_t)(Shoulder_set>>8);
	buffer1[17]=(uint8_t)(Shoulder_set>>0);
	
	buffer1[18]=(uint8_t)(Elbow_Curent>>24);
	buffer1[19]=(uint8_t)(Elbow_Curent>>16);
	buffer1[20]=(uint8_t)(Elbow_Curent>>8);
	buffer1[21]=(uint8_t)(Elbow_Curent>>0);
	
	buffer1[22]=(uint8_t)(Elbow_set>>24);
	buffer1[23]=(uint8_t)(Elbow_set>>16);
  buffer1[24]=(uint8_t)(Elbow_set>>8);
	buffer1[25]=(uint8_t)(Elbow_set>>0);
	
	buffer1[26]=(uint8_t)(set_pitch>>24);
	buffer1[27]=(uint8_t)(set_pitch>>16);
	buffer1[28]=(uint8_t)(set_pitch>>8);
	buffer1[29]=(uint8_t)(set_pitch>>0);
	
	buffer1[30]=(uint8_t)(set_roll>>24);
	buffer1[31]=(uint8_t)(set_roll>>16);
	buffer1[32]=(uint8_t)(set_roll>>8);
	buffer1[33]=(uint8_t)(set_roll>>0);
	
	buffer1[34]=(uint8_t)(set_gripper>>24);
	buffer1[35]=(uint8_t)(set_gripper>>16);
	buffer1[36]=(uint8_t)(set_gripper>>8);
	buffer1[37]=(uint8_t)(set_gripper>>0);
	
  buffer1[38] = '\n';

	
	USBD_HID_SendReport (&USB_OTG_dev, buffer1, HID_IN_PACKET);
}	