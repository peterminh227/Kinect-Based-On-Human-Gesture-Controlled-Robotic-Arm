/*
*Motor.c
* Author Hongvu
*/
#include "Motor.h"
#include "stm32f4xx.h"
#include "Structure.h"
uint32_t Prescaler_Base_Shoulder_Elbow;
uint32_t Prescaler_Pitch_Roll_Gripper;
int8_t flag_stop;
int8_t flag_stop_elbow;
void Motor_Init(void)
{
  Configure_Timer_Base_Shoulder_Elbow();
	Confiure_Direction_Pin_Base_Shoulder_Elbow();
	Configure_Tim_Pitch_Roll_Gripper();
}
void Configure_Timer_Base_Shoulder_Elbow(void)
{
/* TIM SHOULDER_ELBOW_BASE clock enable */
  RCC_APB1PeriphClockCmd(RCC_TIM_SHOULDER_ELBOW_BASE  , ENABLE);
 /* SHOULDER_ELBOW_BASE_PORT clock enable */
  RCC_AHB1PeriphClockCmd(RCC_PWM_SHOULDER_ELBOW_BASE_PORT, ENABLE);

	//PWM_left_motor ,PWM_right_motor
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_BASE|GPIO_PIN_PWM_SHOULDER|GPIO_PIN_PWM_ELBOW;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(SHOULDER_ELBOW_BASE_PORT, &GPIO_InitStructure); 
  GPIO_PinAFConfig(SHOULDER_ELBOW_BASE_PORT, PIN_BASE_SOURCE, GPIO_AF_TIM_SHOULDER_ELBOW_BASE);
  GPIO_PinAFConfig(SHOULDER_ELBOW_BASE_PORT, PIN_SHOULDER_SOURCE, GPIO_AF_TIM_SHOULDER_ELBOW_BASE);
	GPIO_PinAFConfig(SHOULDER_ELBOW_BASE_PORT, PIN_ELBOW_SOURCE, GPIO_AF_TIM_SHOULDER_ELBOW_BASE);
	
	
	/* -----------------------------------------------------------------------
    
		TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 84 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /84 MHz) - 1
                                              
    To get TIM3 output clock at 21 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1
           = 3999

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   
   /* Time base configuration */
	 Prescaler_Base_Shoulder_Elbow =(uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;
   TIM_TimeBaseStructure.TIM_Period = 4199;//2099
   TIM_TimeBaseStructure.TIM_Prescaler = Prescaler_Base_Shoulder_Elbow;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseInit(TIM_SHOULDER_ELBOW_BASE, &TIM_TimeBaseStructure);
	 
	 /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse =0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// Channel 1
  TIM_OC1Init(TIM_SHOULDER_ELBOW_BASE, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM_SHOULDER_ELBOW_BASE, TIM_OCPreload_Enable);
  // Channel 2
  TIM_OC2Init(TIM_SHOULDER_ELBOW_BASE, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM_SHOULDER_ELBOW_BASE, TIM_OCPreload_Enable);
	// Channel3
  TIM_OC3Init(TIM_SHOULDER_ELBOW_BASE, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM_SHOULDER_ELBOW_BASE, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM_SHOULDER_ELBOW_BASE, ENABLE);
	TIM_Cmd(TIM_SHOULDER_ELBOW_BASE, ENABLE);

}
void Confiure_Direction_Pin_Base_Shoulder_Elbow(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_DIR_BASE;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_PIN_DIR_ELBOW|GPIO_PIN_DIR_SHOULDER;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//PITCH_ROLL_GRIPPER
void Configure_Tim_Pitch_Roll_Gripper(void)
{
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_TIM_PITCH_ROLL_GRIPPER, ENABLE);
  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_PWM_PITCH_ROLL_GRIPPER_PORT, ENABLE);
  /*config Pin for RC Servo*/
	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM_PITCH | GPIO_PIN_PWM_ROLL| GPIO_PIN_PWM_GRIPPER;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(PITCH_ROLL_GRIPPER_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(PITCH_ROLL_GRIPPER_PORT, PITCH_PINSOURCE, GPIO_AF_TIM_PITCH_ROLL_GRIPPER);
  
	GPIO_PinAFConfig(PITCH_ROLL_GRIPPER_PORT, ROLL_PINSOURCE, GPIO_AF_TIM_PITCH_ROLL_GRIPPER);
	
	GPIO_PinAFConfig(PITCH_ROLL_GRIPPER_PORT, GRIPPER_PINSOURCE	, GPIO_AF_TIM_PITCH_ROLL_GRIPPER);
	
	/* -----------------------------------------------------------------------
    
	TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM4 counter clock at 20 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /20 MHz) - 1
                                              
    To get TIM4 output clock at 50 Hz, the period (ARR)) is computed as follows:
       ARR = (TIM4 counter clock / TIM4 output clock) - 1
           = 399999

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   
   /* Time base configuration */
	 Prescaler_Pitch_Roll_Gripper =(uint16_t) ((SystemCoreClock / 2)/1000000) - 1;
   TIM_TimeBaseStructure.TIM_Period = 19999;//399999
   TIM_TimeBaseStructure.TIM_Prescaler = Prescaler_Pitch_Roll_Gripper;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM_PITCH_ROLL_GRIPPER, &TIM_TimeBaseStructure);
	  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse =0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
// Channel 1
  TIM_OC1Init(TIM_PITCH_ROLL_GRIPPER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM_PITCH_ROLL_GRIPPER, TIM_OCPreload_Enable);

	// Channel2
  TIM_OC2Init(TIM_PITCH_ROLL_GRIPPER, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM_PITCH_ROLL_GRIPPER, TIM_OCPreload_Enable);	
	
		// Channel3
  TIM_OC3Init(TIM_PITCH_ROLL_GRIPPER, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM_PITCH_ROLL_GRIPPER, TIM_OCPreload_Enable);	
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

}
void Base_Motor_Forward(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR1=speed;
		GPIO_WriteBit(GPIOC,GPIO_Pin_9,(BitAction) 1);
}
void Base_Motor_Reverse(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR1=speed;
		GPIO_WriteBit(GPIOC,GPIO_Pin_9,(BitAction) 0);
}

void Base_Motor_Stop(void)
{
		TIM_SHOULDER_ELBOW_BASE->CCR1=0;
		GPIO_WriteBit(GPIOC,GPIO_Pin_9,(BitAction) 1);
	  flag_stop=1;
}

void Base_Motor_Speed(int32_t speed)
{
    if (speed >= 0)
        Base_Motor_Forward((uint16_t)speed);
    else
        Base_Motor_Reverse((uint16_t)(-speed));
}
void Shoulder_Motor_Forward(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR2=speed;
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction) 1);
}

void Shoulder_Motor_Reverse(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR2=speed;
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction) 0);
}

void Shoulder_Motor_Stop(void)
{
    TIM_SHOULDER_ELBOW_BASE->CCR2=0;
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction) 1);
}

void Shoulder_Motor_Speed(int32_t speed)
{
    if (speed >= 0)
       Shoulder_Motor_Reverse ((uint16_t)speed);
    else
        Shoulder_Motor_Forward((uint16_t)(-speed));
}

void Elbow_Motor_Forward(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR3=speed;
		GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction) 1);
}

void Elbow_Motor_Reverse(uint16_t speed)
{
		TIM_SHOULDER_ELBOW_BASE->CCR3=speed;
		GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction) 0);
}

void Elbow_Motor_Stop(void)
{
	  flag_stop_elbow=1;
    TIM_SHOULDER_ELBOW_BASE->CCR3=0;
		GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction) 1);
}

void Elbow_Motor_Speed(int32_t speed)
{
    if (speed >= 0)
        Elbow_Motor_Reverse((uint16_t)speed);
    else
      Elbow_Motor_Forward((uint16_t)(-speed));
		
}

