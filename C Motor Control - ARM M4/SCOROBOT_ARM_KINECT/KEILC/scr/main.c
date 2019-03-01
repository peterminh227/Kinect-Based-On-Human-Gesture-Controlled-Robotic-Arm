/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Demonstracni program pro 7. dil serialu k STM32F4 Discovery kitu
 * Vyuziti kitu jako USB Custom HID zaøízení
 * Upravy ver. 1.0; (C) 2012 Mard, mcu.cz */



/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "USB_HID.h"
#include "Motor.h"
#include "Pid.h"
#include "Sample_PID.h"
#include "Sensor.h"
#include "Structure.h"
#include "systick.h"
#include "math.h"
#include "Inverse_Kinematic.h"
#include "caculator_setpoint.h"
#include "Debug_Timer.h"
//#include "SCOROBOT_INVERSE.h"
int8_t tim,a;
int8_t state;
int32_t ADC_Curent,s=0,sub=0;
int32_t time_delay;
float value;
/** @addtogroup STM32F4-Discovery_USB_HID
  * @{
  */
// String for creating debug messages

void Caculator_Shoulder(int16_t setpoint);
void Init_HardWare(void)
{
		USB_Init();
	  Motor_Init();
	  PID_Init();
	  Timer_Sample_Init();
	  Encoder_Init();
	  ADC_Config();
		Microswitches_Init();
    SysTick_Init(1000);	
    Timer_Caculator_Init();
	  Timer_Debug_Init();
}
  


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  Init_HardWare();

		
	while(1)
{ 
	//khongche_shoulder();
	{

		
		Cal_Setpoint(setpoint_base);
   // Cal_SetPoint_Base(setpoint_base);
		Cal_Base_Linear(setpoint_base);
		Cal_SetPoint_Shoulder(setpoint_Shoulder);
		Cal_SetPoint_Elbow(setpoint_elbow);
}
}
}
	

/**
  * @brief  send data received to PC.
  * @param  None.
  * @return None.
  */
void Caculator_Shoulder(int16_t setpoint)
{
	int8_t i;
	for (i=0;i<1;i++)
	{
	Sample_Pid_Shoulder();
	//if ((setpoint-PID_SHOULDER_MOTOR.Current)<20 && (setpoint-PID_SHOULDER_MOTOR.Current)>-20)
		//{
	//	Shoulder_Motor_Stop();
		//}
		//else
		{
	if(PID_SHOULDER_MOTOR.enable==1)
	{
		
		s=PID_SHOULDER_MOTOR.Current;
		if(0<(setpoint-PID_SHOULDER_MOTOR.Current))
		{   
			if((setpoint-PID_SHOULDER_MOTOR.Current)<650)
			{

	       	if(PID_SHOULDER_MOTOR.Current==setpoint)
		       {
							Shoulder_Motor_Stop();
					 }
	        else
	      {
	  if(	1300<PID_SHOULDER_MOTOR.Current<=1400) state=0;
		else if(	1400<PID_SHOULDER_MOTOR.Current<=1500) state=1;
		else if(	1500<PID_SHOULDER_MOTOR.Current<=1600) state=2;
		else if(	1600<PID_SHOULDER_MOTOR.Current<=1700) state=3;
		else if(	1700<PID_SHOULDER_MOTOR.Current<=1800) state=4;
		else if(	1800<PID_SHOULDER_MOTOR.Current<=1900) state=5;
		else if(	1900<PID_SHOULDER_MOTOR.Current<=2000) state=6;
		else if(	2000<PID_SHOULDER_MOTOR.Current<=2100) state=7;
		else if(	2100<PID_SHOULDER_MOTOR.Current<=2200) state=8;
		else if(	2200<PID_SHOULDER_MOTOR.Current<=2300) state=9;
		else if(	2300<PID_SHOULDER_MOTOR.Current<=2400) state=10;
		else if(	2400<PID_SHOULDER_MOTOR.Current<=2500) state=11;
					else if(	2500<PID_SHOULDER_MOTOR.Current<=2600) state=12;
					else if(	2600<PID_SHOULDER_MOTOR.Current<=2700) state=13;
					else if(	2700<PID_SHOULDER_MOTOR.Current<=2800) state=14;
					else if(	2800<PID_SHOULDER_MOTOR.Current<=2900) state=15;
					else if(	2900<PID_SHOULDER_MOTOR.Current<=3000) state=16;
		switch (state)
		{
			case 0:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 1:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2900);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 2:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 3:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 4:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 5:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 6:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 7:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 8:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 9:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 10:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 11:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 12:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 13:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 14:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 15:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 16:
			{
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2100);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						default:
						{
						Shoulder_Motor_Stop();
							break;
						}
	     	}
    	}
		}
			if((setpoint-PID_SHOULDER_MOTOR.Current)>650)
		{
		      if(PID_SHOULDER_MOTOR.Current==setpoint)
		       {
							Shoulder_Motor_Stop();
					 }
	        else
	         {
	  if(	1800<PID_SHOULDER_MOTOR.Current<=1900) state=17;
	else if(	1800<PID_SHOULDER_MOTOR.Current<=1900) state=18;	
  else if(	1900<PID_SHOULDER_MOTOR.Current<=2000) state=19;			
  else if(	2000<PID_SHOULDER_MOTOR.Current<=2100) state=20;
else if(	2100<PID_SHOULDER_MOTOR.Current<=2200) state=21;	
else if(	2100<PID_SHOULDER_MOTOR.Current<=2100) state=22;						 
					 }
				switch (state)
		        {
			case 17:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
						 case 18:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
						 case 19:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
						 case 20:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
						 						 case 21:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
						 
													 case 22:
			       {
										tim=1;
										time_delay=(setpoint-PID_SHOULDER_MOTOR.Current)*0.95;
										Shoulder_Motor_Reverse(2150);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			        }
			default:
						{
						Shoulder_Motor_Stop();
							break;
						}
			 }
		 }
	 }
	}
		/*
    
	 */
		if((setpoint-PID_SHOULDER_MOTOR.Current)<0)
		{
		// forward
		if(PID_SHOULDER_MOTOR.Current==setpoint)
		{
    Shoulder_Motor_Stop();
		}
		else
		{
	  if(	1300<PID_SHOULDER_MOTOR.Current<=1400) state=0;
		else if(	1400<PID_SHOULDER_MOTOR.Current<=1500) state=1;
		else if(	1500<PID_SHOULDER_MOTOR.Current<=1600) state=2;
		else if(	1600<PID_SHOULDER_MOTOR.Current<=1700) state=3;
		else if(	1700<PID_SHOULDER_MOTOR.Current<=1800) state=4;
		else if(	1800<PID_SHOULDER_MOTOR.Current<=1900) state=5;
		else if(	1900<PID_SHOULDER_MOTOR.Current<=2000) state=6;
		 else if(	2000<PID_SHOULDER_MOTOR.Current<=2100) state=7;
		 else if(	2100<PID_SHOULDER_MOTOR.Current<=2200) state=8;
		 else if(	2200<PID_SHOULDER_MOTOR.Current<=2300) state=9;
		 else if(	2300<PID_SHOULDER_MOTOR.Current<=2400) state=10;
		 else if(	2400<PID_SHOULDER_MOTOR.Current<=2500) state=11;
		 else if(	2500<PID_SHOULDER_MOTOR.Current<=2600) state=12;
		 else if(	2600<PID_SHOULDER_MOTOR.Current<=2700) state=13;
		 else if(	2700<PID_SHOULDER_MOTOR.Current<=2800) state=14;
		 else if(	2800<PID_SHOULDER_MOTOR.Current<=2900) state=15;
			else if(	2900<PID_SHOULDER_MOTOR.Current<=3000) state=16;
		}
		switch (state)
		{
			case 0:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 1:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 2:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 3:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 4:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						case 5:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
						case 6:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 7:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 8:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 9:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 10:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 11:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
										case 12:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
			
										case 13:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(600);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
										case 14:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(500);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
										case 15:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(500);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
									case 16:
			{
										tim=1;
										time_delay=(-setpoint+PID_SHOULDER_MOTOR.Current)*0.5;
										Shoulder_Motor_Forward(500);
										Delay(time_delay);
									  tim=0;
										Shoulder_Motor_Stop();
				break;
			}
						default:
						{
						Shoulder_Motor_Stop();
							break;
						}
	     	}
	 
			}
	}
}
}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
