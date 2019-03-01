#include "Structure.h"
#include "Pid.h"
#include "Motor.h"
#include "Sensor.h"
#include "stdint.h"
#include "Sample_PID.h"
#include "USB_HID.h"
#include "caculator_setpoint.h"
CAL_SETPOINT CAL_BASE;
CAL_SETPOINT CAL_SHOULDER;
CAL_SETPOINT CAL_ELBOW;
#define period 250
#define period2 200
#define period3 100
#define period_1 0.004
#define period_2 0.005
#define period_3 0.01
float max_speed ;
int16_t tim_index;
int8_t flag_cal;
int32_t value_setpoint;
int32_t setpoint_previous;
float map;
float a1,a2,a3,v01,v02,v03,x1,x2,x3;
float b1,b2,b3;
//float period;
void Timer_Caculator_Init(void)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = 10000-1; // delay 10ms          
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	 //1mhz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* Immediate load of TIM2 Precaler values */
	TIM_PrescalerConfig(TIM2, 84-1, TIM_PSCReloadMode_Immediate);
	/* Clear TIM2 update pending flags */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);	
 Enable_interrupt_timer2(); 
}
void Enable_interrupt_timer2(void)
{
/* Enable TIM2 Update interrupts */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void Disable_interrupt_timer2(void)
{
/* Enable TIM2 Update interrupts */
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	/* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void TIM2_IRQHandler(void)
{
if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
{
increase_tim();

}
TIM_ClearFlag(TIM2,TIM_FLAG_Update);
}

void Cal_Setpoint(int32_t set_point)
{
if((flag_process)||(flag_auto))
{

  
	if(set_point ==setpoint_previous)
	{
	value_setpoint=set_point;
	}
	if(set_point >setpoint_previous)
{
    max_speed = (float)((set_point- setpoint_previous)/period);
	if(tim_index<=(period/3))
	{
	  a1=(float)(max_speed*3/period);
		v01=0;
		x1=(float)(setpoint_previous+0.5*a1*tim_index*tim_index);
		value_setpoint=x1;
		
	}
	else if(tim_index<=(2*period/3))
	{
	  a2=0;v02=max_speed;
		x2=(float)(x1+v02*(tim_index-period/3));
		value_setpoint=x2;
	}
	else if(tim_index<=period)
	{
	  a3=(float)(max_speed*3/period);
		v03=max_speed;
		x3=(float)(x2+0.5*a3*(tim_index-2*period/3)*(tim_index-2*period/3)+v03*(tim_index-2*period/3));
		value_setpoint=x3;
	}
	else if(tim_index>period)
	{
	tim_index=period;
		value_setpoint=x3;
		//TIM_Cmd(TIM2, DISABLE);	
		//Disable_interrupt_timer2();
		//TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	}
	//Cal_PID_ELBOW_Position(value_setpoint);
	// flag_process=1;
	//setpoint_previous=set_point;
	}
else
{
 max_speed =(float)((setpoint_previous-set_point)/period);
	if(tim_index<=(period/3))
	{
	  a1=(float)(max_speed*3/period);
		v01=0;
		x1=(float)(setpoint_previous-0.5*a1*tim_index*tim_index);
		value_setpoint=x1;
	}
	else if(tim_index<=(2*period/3))
	{
	  a2=0;v02=max_speed;
		x2=(float)(x1-v02*(tim_index-period/3));
		value_setpoint=x2;
	}
	else if(tim_index<=period)
	{
	  a3=(float)(max_speed*3/period);
		v03=max_speed;
		x3=(float)(x2-0.5*a3*(tim_index-2*period/3)*(tim_index-2*period/3)-v03*(tim_index-2*period/3));
		value_setpoint=x3;
	}
	else if(tim_index>period)
	{
	  tim_index=period;
		value_setpoint=x3;
		
		
	}

}
   
}
}
void Cal_SetPoint_Base(int32_t set_point)
{
  if((flag_process)||(flag_auto))
{
      if(set_point ==CAL_BASE.setpoint_previous)
	     {
		     CAL_BASE.value_setpoint=set_point;
	     }
	  if(set_point >CAL_BASE.setpoint_previous)
     {
	CAL_BASE.maxspeed=(float)((set_point-CAL_BASE.setpoint_previous)/period3);
	if(CAL_BASE.timer_counter<=(period3 /3))
	      {
	  CAL_BASE.a1=(float)(CAL_BASE.maxspeed*3/period3 );
		CAL_BASE.v01=0;
		CAL_BASE.x1=(float)(CAL_BASE.setpoint_previous+0.5*CAL_BASE.a1*CAL_BASE.timer_counter*CAL_BASE.timer_counter);
		CAL_BASE.value_setpoint=CAL_BASE.x1;
		
	     }
	else if(CAL_BASE.timer_counter<=(2*period3 /3))
	     {
	  CAL_BASE.a2=0;
		CAL_BASE.v02=CAL_BASE.maxspeed;
		CAL_BASE.x2=(float)(CAL_BASE.x1+CAL_BASE.v02*(CAL_BASE.timer_counter-period3/3));
		CAL_BASE.value_setpoint=CAL_BASE.x2;
	     }
	else if(CAL_BASE.timer_counter<=period3)
	     {
	  CAL_BASE.a3=(float)(-CAL_BASE.maxspeed*3/period3 );
		CAL_BASE.v03=CAL_BASE.maxspeed;
		CAL_BASE.x3=(float)(CAL_BASE.x2+0.5*CAL_BASE.a3*(CAL_BASE.timer_counter-2*period3/3)*(CAL_BASE.timer_counter-2*period3/3)+CAL_BASE.v03*(CAL_BASE.timer_counter-2*period3/3));
		CAL_BASE.value_setpoint=CAL_BASE.x3;
	     }
	else if(CAL_BASE.timer_counter>period3 )
	     {
	CAL_BASE.timer_counter=period3 ;
	CAL_BASE.value_setpoint=CAL_BASE.x3;
	     }
	      }
else
{
 CAL_BASE.maxspeed =(float)((CAL_BASE.setpoint_previous-set_point)/period3 );
	if(CAL_BASE.timer_counter<=(period3 /3))
	{
	  CAL_BASE.a1=(float)(CAL_BASE.maxspeed*3/period3 );
		CAL_BASE.v01=0;
		CAL_BASE.x1=(float)(CAL_BASE.setpoint_previous-0.5*CAL_BASE.a1*CAL_BASE.timer_counter*CAL_BASE.timer_counter);
		CAL_BASE.value_setpoint=CAL_BASE.x1;
	}
	else if(CAL_BASE.timer_counter<=(2*period3 /3))
	{
	  CAL_BASE.a2=0;
		CAL_BASE.v02=CAL_BASE.maxspeed;
		CAL_BASE.x2=(float)(CAL_BASE.x1-CAL_BASE.v02*(CAL_BASE.timer_counter-period3 /3));
		CAL_BASE.value_setpoint=CAL_BASE.x2;
	}
	else if(CAL_BASE.timer_counter<=period3 )
	{
	  CAL_BASE.a3=(float)(-CAL_BASE.maxspeed*3/period3);
		CAL_BASE.v03=CAL_BASE.maxspeed;
		CAL_BASE.x3=(float)(CAL_BASE.x2+0.5*CAL_BASE.a3*(CAL_BASE.timer_counter-2*period3/3)*(CAL_BASE.timer_counter-2*period3/3)-CAL_BASE.v03*(CAL_BASE.timer_counter-2*period3/3));
		CAL_BASE.value_setpoint=CAL_BASE.x3;
	}
	else if(CAL_BASE.timer_counter>period3)
	    {
	   CAL_BASE.timer_counter=period3;
		CAL_BASE.value_setpoint=CAL_BASE.x3;

	    }
    }
  }
}
void Cal_Base_Linear(int32_t set_point)
{
if((flag_process)||(flag_auto))
{
      if(set_point ==CAL_BASE.setpoint_previous)
	     {
		     CAL_BASE.value_setpoint=set_point;
	     }
	  if(set_point >CAL_BASE.setpoint_previous)
     {
	CAL_BASE.maxspeed=(float)((set_point-CAL_BASE.setpoint_previous)*period_3);
	if(CAL_BASE.timer_counter<=(period3 ))
	      {
		CAL_BASE.x1=(float)(CAL_BASE.setpoint_previous+CAL_BASE.maxspeed*CAL_BASE.timer_counter);
		CAL_BASE.value_setpoint=CAL_BASE.x1;
	      }
	else if(CAL_BASE.timer_counter>period3 )
	     {
	CAL_BASE.timer_counter=period3 ;
	CAL_BASE.value_setpoint=CAL_BASE.x1;
	     }
	      }
else
      {
 CAL_BASE.maxspeed =(float)((CAL_BASE.setpoint_previous-set_point)*period_3 );
	if(CAL_BASE.timer_counter<=(period3))
	{
		CAL_BASE.x1=(float)(CAL_BASE.setpoint_previous-CAL_BASE.maxspeed*CAL_BASE.timer_counter);
		CAL_BASE.value_setpoint=CAL_BASE.x1;
	}
	else if(CAL_BASE.timer_counter>period3)
	    {
	  CAL_BASE.timer_counter=period3;
		CAL_BASE.value_setpoint=CAL_BASE.x1;

	    }
    }
  }

}
void khongche_shoulder(void)
{
if(CAL_SHOULDER.value_setpoint<1200)
{
	CAL_SHOULDER.value_setpoint=PID_SHOULDER_MOTOR.Current;
}
if(CAL_SHOULDER.setpoint_previous<1200)
{
CAL_SHOULDER.setpoint_previous=PID_SHOULDER_MOTOR.Current;

}
}
void Cal_SetPoint_Shoulder(int32_t set_point)
{
    
			
{
if((flag_process)||(flag_auto))
	{
      if(set_point ==CAL_SHOULDER.setpoint_previous)
	     {
		     CAL_SHOULDER.value_setpoint=set_point;
	     }
	  if(set_point >CAL_SHOULDER.setpoint_previous)
     {
	CAL_SHOULDER.maxspeed=(float)((set_point-CAL_SHOULDER.setpoint_previous)*1.5*period_1);
	if(CAL_SHOULDER.timer_counter<=((float)(period*0.333333)))
	      {
	  CAL_SHOULDER.a1=(float)(CAL_SHOULDER.maxspeed*3*period_1);
		CAL_SHOULDER.v01=0;
		CAL_SHOULDER.x1=(float)(CAL_SHOULDER.setpoint_previous+0.5*CAL_SHOULDER.a1*CAL_SHOULDER.timer_counter*CAL_SHOULDER.timer_counter);
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x1;
		
	     }
	else if(CAL_SHOULDER.timer_counter<=((float)(period*0.666667)))
	     {
	  CAL_SHOULDER.a2=0;
		CAL_SHOULDER.v02=CAL_SHOULDER.maxspeed;
		CAL_SHOULDER.x2=(float)(CAL_SHOULDER.x1+CAL_SHOULDER.v02*(CAL_SHOULDER.timer_counter-period*0.333333));
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x2;
	     }
	else if(CAL_SHOULDER.timer_counter<=period)
	     {
	  CAL_SHOULDER.a3=(float)(-CAL_SHOULDER.maxspeed*3*period_1);
		CAL_SHOULDER.v03=CAL_SHOULDER.maxspeed;
		CAL_SHOULDER.x3=(float)(CAL_SHOULDER.x2+0.5*CAL_SHOULDER.a3*(CAL_SHOULDER.timer_counter-period*0.666667)*(CAL_SHOULDER.timer_counter-period*0.666667)+CAL_SHOULDER.v03*(CAL_SHOULDER.timer_counter-period*0.666667));
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x3;
	     }
	else if(CAL_SHOULDER.timer_counter>period)
	     {
	CAL_SHOULDER.timer_counter=period;
	CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x3;
	     }
	      }
else
{
 CAL_SHOULDER.maxspeed =(float)((CAL_SHOULDER.setpoint_previous-set_point)*1.5*period_1);
	if(CAL_SHOULDER.timer_counter<=((float)(period*0.333333)))
	{
	  CAL_SHOULDER.a1=(float)(CAL_SHOULDER.maxspeed*3*period_1);
		CAL_SHOULDER.v01=0;
		CAL_SHOULDER.x1=(float)(CAL_SHOULDER.setpoint_previous-0.5*CAL_SHOULDER.a1*CAL_SHOULDER.timer_counter*CAL_SHOULDER.timer_counter);
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x1;
	}
	else if(CAL_SHOULDER.timer_counter<=((float)(period*0.666667)))
	{
	  CAL_SHOULDER.a2=0;
		CAL_SHOULDER.v02=CAL_SHOULDER.maxspeed;
		CAL_SHOULDER.x2=(float)(CAL_SHOULDER.x1-CAL_SHOULDER.v02*(CAL_SHOULDER.timer_counter-period*0.333333));
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x2;
	}
	else if(CAL_SHOULDER.timer_counter<=period)
	{
	  CAL_SHOULDER.a3=(float)(CAL_SHOULDER.maxspeed*3*period_1);
		CAL_SHOULDER.v03=CAL_SHOULDER.maxspeed;
		CAL_SHOULDER.x3=(float)(CAL_SHOULDER.x2+0.5*CAL_SHOULDER.a3*(CAL_SHOULDER.timer_counter-period*0.666667)*(CAL_SHOULDER.timer_counter-period*0.666667)-CAL_SHOULDER.v03*(CAL_SHOULDER.timer_counter-period*0.666667));
		CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x3;
	}
	else if(CAL_SHOULDER.timer_counter>period)
	    {
	   CAL_SHOULDER.timer_counter=period;
		 CAL_SHOULDER.value_setpoint=CAL_SHOULDER.x3;

	    }
    }
  }
}
}
void Cal_SetPoint_Elbow(int32_t set_point)
{
   if((flag_process)||(flag_auto))
{  map=(float)(200*0.333333333);
      if(set_point ==CAL_ELBOW.setpoint_previous)
	     {
		     CAL_ELBOW.value_setpoint=set_point;
	     }
	  if(set_point >CAL_ELBOW.setpoint_previous)
     {
	CAL_ELBOW.maxspeed=(float)((set_point-CAL_ELBOW.setpoint_previous)*1.5*period_2);
	if(CAL_ELBOW.timer_counter<=((float)(period2*0.333333)))
	      {
	  CAL_ELBOW.a1=(float)(CAL_ELBOW.maxspeed*3*period_2);
		CAL_ELBOW.v01=0;
		CAL_ELBOW.x1=(float)(CAL_ELBOW.setpoint_previous+0.5*CAL_ELBOW.a1*CAL_ELBOW.timer_counter*CAL_ELBOW.timer_counter);
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x1;
		
	     }
	else if(CAL_ELBOW.timer_counter<=((float)(period2*0.666667)))
	     {
	  CAL_ELBOW.a2=0;
		CAL_ELBOW.v02=CAL_ELBOW.maxspeed;
		CAL_ELBOW.x2=(float)(CAL_ELBOW.x1+CAL_ELBOW.v02*(CAL_ELBOW.timer_counter-period2*0.3333));
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x2;
	     }
	else if(CAL_ELBOW.timer_counter<=period2)
	     {
	  CAL_ELBOW.a3=(float)(-CAL_ELBOW.maxspeed*3*period_2);
		CAL_ELBOW.v03=CAL_ELBOW.maxspeed;
		CAL_ELBOW.x3=(float)(CAL_ELBOW.x2+0.5*CAL_ELBOW.a3*(CAL_ELBOW.timer_counter-period2*0.666667)*(CAL_ELBOW.timer_counter-period2*0.666667)+CAL_ELBOW.v03*(CAL_ELBOW.timer_counter-period2*0.666667));
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x3;
	     }
	else if(CAL_ELBOW.timer_counter>period2)
	     {
	CAL_ELBOW.timer_counter=period2;
	CAL_ELBOW.value_setpoint=CAL_ELBOW.x3;
	     }
	      }
else
{
 CAL_ELBOW.maxspeed =(float)((CAL_ELBOW.setpoint_previous-set_point)*1.5*period_2);
	if(CAL_ELBOW.timer_counter<=((float)(period2*0.333333)))
	{
	  CAL_ELBOW.a1=(float)(CAL_ELBOW.maxspeed*3*period_2);
		CAL_ELBOW.v01=0;
		CAL_ELBOW.x1=(float)(CAL_ELBOW.setpoint_previous-0.5*CAL_ELBOW.a1*CAL_ELBOW.timer_counter*CAL_ELBOW.timer_counter);
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x1;
	}
	else if(CAL_ELBOW.timer_counter<=((float)(period2*0.666667)))
	{
	  CAL_ELBOW.a2=0;
		CAL_ELBOW.v02=CAL_ELBOW.maxspeed;
		CAL_ELBOW.x2=(float)(CAL_ELBOW.x1-CAL_ELBOW.v02*(CAL_ELBOW.timer_counter-period2*0.333333));
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x2;
	}
	else if(CAL_ELBOW.timer_counter<=period2)
	{
	  CAL_ELBOW.a3=(float)(CAL_ELBOW.maxspeed*3*period_2);
		CAL_ELBOW.v03=CAL_ELBOW.maxspeed;
		CAL_ELBOW.x3=(float)(CAL_ELBOW.x2+0.5*CAL_ELBOW.a3*(CAL_ELBOW.timer_counter-period2*0.666667)*(CAL_ELBOW.timer_counter-period2*0.666667)-CAL_ELBOW.v03*(CAL_ELBOW.timer_counter-period2*0.666667));
		CAL_ELBOW.value_setpoint=CAL_ELBOW.x3;
	}
	else if(CAL_ELBOW.timer_counter>period2)
	    {
	   CAL_ELBOW.timer_counter=period2;
		 CAL_ELBOW.value_setpoint=CAL_ELBOW.x3;

	    }
    }
  }
}


void increase_tim(void)
{
  tim_index++;
	CAL_BASE.timer_counter++;
	CAL_SHOULDER.timer_counter++;
	CAL_ELBOW.timer_counter++;
flag_cal=1;
}