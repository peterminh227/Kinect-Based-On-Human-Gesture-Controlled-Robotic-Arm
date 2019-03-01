#include "Sensor.h"
#include "Structure.h"
#include "stdio.h"
#include "stdint.h"
#include "Motor.h"
int32_t encoder_Base_v=0,encoder_Base_p=0;
int32_t encoder_Elbow_v=0,encoder_Elbow_p=0;
__IO uint16_t uhADCxConvertedValue = 0;
int32_t Elbow_Home_Position;
int8_t Base_Home_Possition;
int8_t Shoulder_Home_Possition;
void Encoder_Init(void)
{
/*Enable GPIOB Clock*/
	RCC_AHB1PeriphClockCmd(RCC_ENCODER_PORT, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Configure PB10,PB11,PB12,PB13 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//QUAN TRONG
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_ENC_BASE_PHASE_A|GPIO_ENC_BASE_PHASE_B|GPIO_ENC_ELBOW_PHASE_A|GPIO_ENC_ELBOW_PHASE_B;
	GPIO_Init(GPIO_ENC_PORT , &GPIO_InitStructure);
	/* Connect EXTI Line10 to PB10,11,12,13 pin */
	SYSCFG_EXTILineConfig(GPIO_ENCODER_PORTSOURCE	,ENC_BASE_PHASE_A_PINSOURCE);
	SYSCFG_EXTILineConfig(GPIO_ENCODER_PORTSOURCE	,ENC_BASE_PHASE_B_PINSOURCE);
	SYSCFG_EXTILineConfig(GPIO_ENCODER_PORTSOURCE	,ENC_ELBOW_PHASE_A_PINSOURCE);
	SYSCFG_EXTILineConfig(GPIO_ENCODER_PORTSOURCE	,ENC_ELBOW_PHASE_B_PINSOURCE);
	/* Configure EXTI Line10 */
	/* Configure EXTI Line11 */
	/* Configure EXTI Line12 */
	/* Configure EXTI Line13 */
	EXTI_InitStructure.EXTI_Line = ENC_BASE_PHASE_A_Line|ENC_BASE_PHASE_B_Line|ENC_ELBOW_PHASE_A_Line |ENC_ELBOW_PHASE_B_Line ;
	/* Interrupt mode */
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	/* Enable interrupt */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* Enable and set EXTI Line15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
 {
	if(EXTI_GetITStatus(ENC_BASE_PHASE_A_Line) != RESET)
	{   
     /* Clear the EXTI line 15 pending bit */

		EXTI_ClearITPendingBit(ENC_BASE_PHASE_A_Line);
		if(((GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_BASE_PHASE_A))^(GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_BASE_PHASE_B)))==0)
		{
				encoder_Base_v++;
				encoder_Base_p++;
		 }
		else
		{
				encoder_Base_v--;
				encoder_Base_p--;
		}
	}
	 //INTERUPT CHANEL B OF MOTOR 1
	 if(EXTI_GetITStatus(ENC_BASE_PHASE_B_Line) != RESET)
	{   
     /* Clear the EXTI line 15 pending bit */
    EXTI_ClearITPendingBit(ENC_BASE_PHASE_B_Line);
		if(((GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_BASE_PHASE_B))^(GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_BASE_PHASE_A)))==0)
		{
				encoder_Base_v--;
				encoder_Base_p--;
		}
		else
		{
				encoder_Base_v++;
				encoder_Base_p++;
		}
	}
	 
	if(EXTI_GetITStatus(ENC_ELBOW_PHASE_A_Line) != RESET)
	{   
     /* Clear the EXTI line 15 pending bit */

		EXTI_ClearITPendingBit(ENC_ELBOW_PHASE_A_Line);
		if(((GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_ELBOW_PHASE_A))^(GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_ELBOW_PHASE_B)))==0)
		{
				encoder_Elbow_v++;
				encoder_Elbow_p++;
		 }
		else
		{
				encoder_Elbow_v--;
				encoder_Elbow_p--;
		}
	}
	 //INTERUPT CHANEL B OF MOTOR 1
	 if(EXTI_GetITStatus(ENC_ELBOW_PHASE_B_Line) != RESET)
	{   
     /* Clear the EXTI line 15 pending bit */
    EXTI_ClearITPendingBit(ENC_ELBOW_PHASE_B_Line);
		if(((GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_ELBOW_PHASE_B 	))^(GPIO_ReadInputDataBit(GPIO_ENC_PORT,GPIO_ENC_ELBOW_PHASE_A 	)))==0)
		{
				encoder_Elbow_v--;
				encoder_Elbow_p--;
		}
		else
		{
				encoder_Elbow_v++;
				encoder_Elbow_p++;
		}
	}

 }
 
 void Encoder_Reset(void)
 {
				encoder_Base_v=0;
				encoder_Base_p=0;
				encoder_Elbow_v=0;
				encoder_Elbow_p=0;
 }
 void ADC_Config(void)
 {
 /* Enable ADCx, DMA and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(SHOULDER_CHANNEL_GPIO_CLK , ENABLE);  
  RCC_APB2PeriphClockCmd(SHOULDER_CLK , ENABLE);
  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = SHOULDER_DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SHOULDER_DR_ADDRESS ;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCxConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(SHOULDER_DMA_STREAMx  , &DMA_InitStructure);
  DMA_Cmd(SHOULDER_DMA_STREAMx, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = SHOULDER_GPIO_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(SHOULDER_PORT , &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(SHOULDER, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration *************************************/
  ADC_RegularChannelConfig(SHOULDER, SHOULDER_CHANEL , 1, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(SHOULDER, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(SHOULDER, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(SHOULDER, ENABLE);
	
	ADC_SoftwareStartConv(SHOULDER);

 
 }
 void Microswitches_Init(void)
 {
 /*Enable GPIOB Clock*/
	RCC_AHB1PeriphClockCmd(Rcc_Microswitches_Port	, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Configure PB10,PB11,PB12,PB13 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//QUAN TRONG
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Microswitches_Base|GPIO_Microswitches_Shoulder|GPIO_Microswitches_Elbow;
	GPIO_Init(GPIO_Microswitches_Port , &GPIO_InitStructure);
	/* Connect EXTI Line5 to PB5,6,7 */
	SYSCFG_EXTILineConfig(GPIO_Microswitches_PortSource	,Microswitches_Base_PinSource );
	SYSCFG_EXTILineConfig(GPIO_Microswitches_PortSource	,Microswitches_Shoulder_PinSource );
	SYSCFG_EXTILineConfig(GPIO_Microswitches_PortSource	,Microswitches_Elbow_PinSource );

	/* Configure EXTI Line5 */
	/* Configure EXTI Line6 */
	/* Configure EXTI Line7 */

	EXTI_InitStructure.EXTI_Line = Microswitches_Base_Line |Microswitches_Shoulder_Line |Microswitches_Elbow_Line  ;
	/* Interrupt mode */
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising  edge */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling  
;  
	/* Enable interrupt */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* Enable and set EXTI Line9_5 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 }
 
 void EXTI9_5_IRQHandler(void)
 {
 if(EXTI_GetITStatus(Microswitches_Base_Line) != RESET)
 {    
	    Base_Home_Possition=1;
			Base_Motor_Stop();
		 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
	 {
		 Base_Home_Possition=1;
	 }
	 else
	 {
		 Base_Home_Possition=0;
	 }	
	 EXTI_ClearITPendingBit(Microswitches_Base_Line);
 }
 
 if(EXTI_GetITStatus(Microswitches_Shoulder_Line ) != RESET)
 {     
	 	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	 {
		 Shoulder_Home_Possition=1;
	 }
	 else
	 {
		 Shoulder_Home_Possition=0;
	 }
			Base_Motor_Stop();
			Shoulder_Motor_Stop(); 
	    Elbow_Motor_Stop();
	 EXTI_ClearITPendingBit(Microswitches_Shoulder_Line);
 }
 
  if(EXTI_GetITStatus(Microswitches_Elbow_Line ) != RESET)
 {
			//Base_Motor_Stop();
			//Shoulder_Motor_Stop(); 
	    //Elbow_Motor_Stop();
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
	 {
		 Elbow_Home_Position=1;
	 }
	 else
	 {
		 Elbow_Home_Position=0;
	 }
	 
	 EXTI_ClearITPendingBit(Microswitches_Elbow_Line);
 }
 }