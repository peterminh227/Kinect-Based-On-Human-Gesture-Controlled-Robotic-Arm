#include "Structure.h"
#include "Pid.h"
#include "Motor.h"
#include "Sensor.h"
#include "stdint.h"
#include "Sample_PID.h"
#include "USB_HID.h"
#include "caculator_setpoint.h"
#include <string.h>
void Timer_Debug_Init(void)
{ RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = 10000-1; // delay 10ms          
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	 //1mhz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* Immediate load of TIM6 Precaler values */
	TIM_PrescalerConfig(TIM6, 84-1, TIM_PSCReloadMode_Immediate);
	/* Clear TIM6 update pending flags */
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	/* TIM enable counter */
	TIM_Cmd(TIM6, ENABLE);	
	/* Enable TIM6 Update interrupts */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	/* Enable the TIM6 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void TIM6_DAC_IRQHandler()
	{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)                   // If update flag is set
		{
			 Send_response_to_PC( 
			PID_BASE_MOTOR.Current,
			PID_BASE_MOTOR.SetPoint,
			PID_SHOULDER_MOTOR.Current,
			PID_SHOULDER_MOTOR.SetPoint,
			PID_ELBOW_MOTOR.Current,
			PID_ELBOW_MOTOR.SetPoint,
			setpoint_pitch,
			setpoint_roll,
			setpoint_gripper);
			if(flag_process==1)
			{
		send_debug_success();
			}
		}
    TIM_ClearFlag(TIM6,TIM_FLAG_Update);
}