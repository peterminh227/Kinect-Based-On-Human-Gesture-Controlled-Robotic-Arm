#include "Pid.h"
#include "USB_HID.h"
#include <string.h>
#include "usbd_hid_core.h"
#include "usbd_req.h"
#include "main.h"
#include "Motor.h"
#include "SCOROBOT_INVERSE.h"
#include "caculator_setpoint.h"
#include "systick.h"
#include "Sensor.h"
#include "Sample_PID.h"
uint8_t Buffer[HID_OUT_PACKET];
uint8_t InBuffer[64], OutBuffer[63],Buf[63];
#define DEBUGBUFFERSIZE 128
#define Home 											0x04
#define Receive_Pulse 						0x05
#define Emergency_Stop 						0x07
#define Receive_data_from_Kinect 	0x06
#define Set_Speed          				0x02
#define Data_Grid_Receive 				0x20

uint8_t debugBufferStart;
uint8_t debugBufferEnd;
uint8_t debugBufferLevel;
int8_t home_Request;
uint8_t debugBuffer[DEBUGBUFFERSIZE];
char debugString[64];
 int32_t kp,kd,ki,sp;
uint8_t ToSendDataBuffer[64];
int32_t setpoint_base,setpoint_Shoulder,setpoint_elbow,setpoint_pitch,setpoint_roll,setpoint_gripper;
int16_t Duty_Base;
int16_t Speed_Base,Speed_Shoulder,Speed_Elbow;
float x_pos,y_pos,z_pos,grip;
int8_t flag_receive=0;
int8_t flag_process=0;
int8_t flag_auto=0;
int8_t flag_Emegency_stop=0;
int8_t flag_Receive_Data_Grid=0;
int8_t flag_Forward_Base=0;
int8_t flag_Reverse_Base=0;
int8_t flag_StopMotor_Base=0;
int8_t flag_Forward_Shoulder=0;
int8_t flag_Reverse_Shoulder=0;
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
void USB_Init(void)
{
	 USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
  USB_OTG_HS_CORE_ID,
#else
  USB_OTG_FS_CORE_ID,
#endif
  &USR_desc,
  &USBD_HID_cb,
  &USR_cb);
	
}


void Send_Debug(void)
{
	
	sprintf(debugString, "Scorobot already");
	debugOut(debugString);
	copyDebugToSendBuffer( &ToSendDataBuffer[0]);
	USBD_HID_SendReport (&USB_OTG_dev, ToSendDataBuffer, HID_IN_PACKET);
}

void Get_Data_from_PC(void)
{
	uint32_t Kp_temp=0,Ki_temp=0,Kd_temp=0,Speed_ADC=0;
	int32_t setpoint_base_temp,setpoint_Shoulder_temp,setpoint_elbow_temp,setpoint_pitch_temp,setpoint_roll_temp,setpoint_gripper_temp;
	int16_t SetPoint;
	int32_t speed_base_temp,speed_shoulder_temp,speed_Elbow_temp;
	int16_t Duty_Base_temp;
  uint32_t i=0;
		Delayms(0xFFFF);
	
		while (i<64)
		  {
			  Buf[i]=OutBuffer[i];
			  i++;
		  }
////////////////////////////////////////////////////////////////////
///////////////       SET SPEED FOR MOTOR         //////////////////
////////////////////////////////////////////////////////////////////	
			if(Buf[0]==Set_Speed)
			{
			speed_base_temp= Buf[4]<<24;
			speed_base_temp|=Buf[3]<<16;
			speed_base_temp|=Buf[2]<<8;	
			speed_base_temp|=Buf[1];	
      Speed_Base=speed_base_temp;
//////////////////////////////////////////
			speed_shoulder_temp=Buf[8]<<24;
			speed_shoulder_temp|=Buf[7]<<16;
      speed_shoulder_temp|=	Buf[6]<<8;
			speed_shoulder_temp|=Buf[5];
				Speed_Shoulder=speed_shoulder_temp;
//////////////////////////////////////////
			speed_Elbow_temp=Buf[12]<<24;
			speed_Elbow_temp|=Buf[11]<<16;
			speed_Elbow_temp|=Buf[10]<<8;
			speed_Elbow_temp|=Buf[9];
				Speed_Elbow=speed_Elbow_temp;
			}
			
			
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%         END SET SPEED FOR MOTOR      %%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||			
////////////////////////////////////////////////////////////////////
///////////////       HOME REQUEST/RESPONSE       //////////////////
////////////////////////////////////////////////////////////////////	
			

			if(Buf[0]==Home)
			{
				home_Request=1;

		//		if(Elbow_Home_Position==1)
		//		{
		//		sprintf(debugString, "Home Success");
		//		debugOut(debugString);
	  //    copyDebugToSendBuffer( &ToSendDataBuffer[0]);
	  //    USBD_HID_SendReport (&USB_OTG_dev, ToSendDataBuffer, HID_IN_PACKET);
		//		}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%         END HOME REQUEST/RESONSE     %%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
			}
	
////////////////////////////////////////////////////////////////////
///////////////       MODE MANUAL       ////////////////////////////
////////////////////////////////////////////////////////////////////			
			if(OutBuffer[0]==0x05)
			{
				flag_process=1;
				tim_index=0;
				CAL_BASE.timer_counter=0;
				CAL_SHOULDER.timer_counter=0;
				CAL_BASE.setpoint_previous=PID_BASE_MOTOR.Current;
				CAL_SHOULDER.setpoint_previous=PID_SHOULDER_MOTOR.Current;
				CAL_ELBOW.timer_counter=0;
				CAL_ELBOW.setpoint_previous=PID_ELBOW_MOTOR.Current;
				setpoint_previous=PID_BASE_MOTOR.Current;
			  setpoint_base_temp =Buf[4]<<24;
				setpoint_base_temp|=Buf[3]<<16;
				setpoint_base_temp|=Buf[2]<<8;
				setpoint_base_temp|=Buf[1];
				setpoint_base=setpoint_base_temp;
			
			////////////////////////////////////////////////////////////////
				setpoint_Shoulder_temp =Buf[8]<<24;
				setpoint_Shoulder_temp|=Buf[7]<<16;
				setpoint_Shoulder_temp|=Buf[6]<<8;
				setpoint_Shoulder_temp|=Buf[5];
				setpoint_Shoulder=setpoint_Shoulder_temp;

				/////////////////////////////////////////////////////////////
				setpoint_elbow_temp = Buf[12]<<24;
				setpoint_elbow_temp|=Buf[11]<<16;
				setpoint_elbow_temp|=Buf[10]<<8;
				setpoint_elbow_temp|=Buf[9];
				setpoint_elbow=setpoint_elbow_temp;

				//////////////////////////////////////////////////////////////
				setpoint_pitch_temp =Buf[16]<<24;
				setpoint_pitch_temp|=Buf[15]<<16;
				setpoint_pitch_temp|=Buf[14]<<8;
				setpoint_pitch_temp|=Buf[13];
				setpoint_pitch=setpoint_pitch_temp;

				/////////////////////////////////////////////////////////////
				setpoint_roll_temp =Buf[20]<<24;
				setpoint_roll_temp|=Buf[19]<<16;
				setpoint_roll_temp|=Buf[18]<<8;
				setpoint_roll_temp|=Buf[17];
				setpoint_roll=setpoint_roll_temp;

				////////////////////////////////////////////////////////////

				setpoint_gripper_temp =Buf[24]<<24;
				setpoint_gripper_temp|=Buf[23]<<16;
				setpoint_gripper_temp|=Buf[22]<<8;
				setpoint_gripper_temp|=Buf[21];
				setpoint_gripper=setpoint_gripper_temp;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%         END MODE MANUAL     %%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||	
			}

			
/////////////////////////////////////////////////////////////////////
////////////////////       MODE AUTO       //////////////////////////
/////////////////////////////////////////////////////////////////////
				if(Buf[0]==0x06)
			{	
				flag_auto=1;
			 // flag_process=1;
				tim_index=0;
				CAL_BASE.timer_counter=0;
				CAL_SHOULDER.timer_counter=0;
				CAL_BASE.setpoint_previous=PID_BASE_MOTOR.Current;
				CAL_SHOULDER.setpoint_previous=PID_SHOULDER_MOTOR.Current;
				CAL_ELBOW.timer_counter=0;
				CAL_ELBOW.setpoint_previous=PID_ELBOW_MOTOR.Current;
				setpoint_previous=PID_BASE_MOTOR.Current;
			  setpoint_base_temp =Buf[4]<<24;
				setpoint_base_temp|=Buf[3]<<16;
				setpoint_base_temp|=Buf[2]<<8;
				setpoint_base_temp|=Buf[1];
				setpoint_base=setpoint_base_temp;
			
			////////////////////////////////////////////////////////////////
				setpoint_Shoulder_temp =Buf[8]<<24;
				setpoint_Shoulder_temp|=Buf[7]<<16;
				setpoint_Shoulder_temp|=Buf[6]<<8;
				setpoint_Shoulder_temp|=Buf[5];
				setpoint_Shoulder=setpoint_Shoulder_temp;

				/////////////////////////////////////////////////////////////
				setpoint_elbow_temp = Buf[12]<<24;
				setpoint_elbow_temp|=Buf[11]<<16;
				setpoint_elbow_temp|=Buf[10]<<8;
				setpoint_elbow_temp|=Buf[9];
				setpoint_elbow=setpoint_elbow_temp;

				//////////////////////////////////////////////////////////////
				setpoint_pitch_temp =Buf[16]<<24;
				setpoint_pitch_temp|=Buf[15]<<16;
				setpoint_pitch_temp|=Buf[14]<<8;
				setpoint_pitch_temp|=Buf[13];
				setpoint_pitch=setpoint_pitch_temp;

				/////////////////////////////////////////////////////////////
				setpoint_roll_temp =Buf[20]<<24;
				setpoint_roll_temp|=Buf[19]<<16;
				setpoint_roll_temp|=Buf[18]<<8;
				setpoint_roll_temp|=Buf[17];
				setpoint_roll=setpoint_roll_temp;

				////////////////////////////////////////////////////////////

				setpoint_gripper_temp =Buf[24]<<24;
				setpoint_gripper_temp|=Buf[23]<<16;
				setpoint_gripper_temp|=Buf[22]<<8;
				setpoint_gripper_temp|=Buf[21];
				setpoint_gripper=setpoint_gripper_temp;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%         END MODE AUTO      %%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||	
			}

			
//////////////////////////////////////////////////
/////////%      EMERGENCY STOP       %////////////
//////////////////////////////////////////////////			
						if(Buf[0]==Emergency_Stop)
						{
						
					 flag_Emegency_stop=1;
						}
						else
						{
						flag_Emegency_stop=0;
						}
//////////////////////////////////////////////////////////////////
/////////////////       END_EMERGENCY_STOP       /////////////////		
//////////////////////////////////////////////////////////////////
/****************************************************************/						
//////////////////////////////////////////////////////////////////
/////////////////%      Data Grid Receive     %///////////////////
//////////////////////////////////////////////////////////////////
			if(OutBuffer[0]==Data_Grid_Receive)
			{
				flag_Receive_Data_Grid =1;
				
			  setpoint_base_temp =Buf[4]<<24;
				setpoint_base_temp|=Buf[3]<<16;
				setpoint_base_temp|=Buf[2]<<8;
				setpoint_base_temp|=Buf[1];
				setpoint_base=setpoint_base_temp;
			
			////////////////////////////////////////////////////////////////
				setpoint_Shoulder_temp =Buf[8]<<24;
				setpoint_Shoulder_temp|=Buf[7]<<16;
				setpoint_Shoulder_temp|=Buf[6]<<8;
				setpoint_Shoulder_temp|=Buf[5];
				setpoint_Shoulder=setpoint_Shoulder_temp;

				/////////////////////////////////////////////////////////////
				setpoint_elbow_temp = Buf[12]<<24;
				setpoint_elbow_temp|=Buf[11]<<16;
				setpoint_elbow_temp|=Buf[10]<<8;
				setpoint_elbow_temp|=Buf[9];
				setpoint_elbow=setpoint_elbow_temp;

				//////////////////////////////////////////////////////////////
				setpoint_pitch_temp =Buf[16]<<24;
				setpoint_pitch_temp|=Buf[15]<<16;
				setpoint_pitch_temp|=Buf[14]<<8;
				setpoint_pitch_temp|=Buf[13];
				setpoint_pitch=setpoint_pitch_temp;

				/////////////////////////////////////////////////////////////
				setpoint_roll_temp =Buf[20]<<24;
				setpoint_roll_temp|=Buf[19]<<16;
				setpoint_roll_temp|=Buf[18]<<8;
				setpoint_roll_temp|=Buf[17];
				setpoint_roll=setpoint_roll_temp;

				////////////////////////////////////////////////////////////

				setpoint_gripper_temp =Buf[24]<<24;
				setpoint_gripper_temp|=Buf[23]<<16;
				setpoint_gripper_temp|=Buf[22]<<8;
				setpoint_gripper_temp|=Buf[21];
				setpoint_gripper=setpoint_gripper_temp;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%         END Data Grid Receive      %%%%%%%%%%%%%%%%%%||
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%||	
			}
			
			if(OutBuffer[0]==nonPID_Forward_Base )
			{
				flag_Forward_Base =1;
				flag_Reverse_Base=0;
				flag_StopMotor_Base=0;
			  Duty_Base_temp =Buf[4]<<24;
				Duty_Base_temp|=Buf[3]<<16;
				Duty_Base_temp|=Buf[2]<<8;
				Duty_Base_temp|=Buf[1];
				Duty_Base=Duty_Base_temp;
		  }
				if(OutBuffer[0]==nonPID_Reverse_Base )
			{
				flag_Reverse_Base=1;
				flag_Forward_Base =0;
				flag_StopMotor_Base=0;
			  Duty_Base_temp =Buf[4]<<24;
				Duty_Base_temp|=Buf[3]<<16;
				Duty_Base_temp|=Buf[2]<<8;
				Duty_Base_temp|=Buf[1];
				Duty_Base=Duty_Base_temp;
		  }
		if(OutBuffer[0]==nonPID_StopMotor_Base )
			{
				flag_StopMotor_Base=1;
				flag_Reverse_Base=0;
				flag_Forward_Base =0;
				flag_Forward_Shoulder=0;
				flag_Reverse_Shoulder=0;
			  Duty_Base_temp =Buf[4]<<24;
				Duty_Base_temp|=Buf[3]<<16;
				Duty_Base_temp|=Buf[2]<<8;
				Duty_Base_temp|=Buf[1];
				Duty_Base=Duty_Base_temp;
		  }
			if(OutBuffer[0]==nonPID_Forward_Shouler)
			{
			flag_Forward_Shoulder=1;
			flag_Reverse_Shoulder=0;
			flag_StopMotor_Base=0;
			}
			if(OutBuffer[0]==nonPID_Reverse_Shoulder )
			{
			flag_Reverse_Shoulder=1;
			flag_Forward_Shoulder=0;
			flag_StopMotor_Base=0;
			}
			
	
	}
void send_usb()
{
	USBD_HID_SendReport (&USB_OTG_dev, debugBuffer, 64);
}
void SendStrToUSB(uint8_t* str)
{
	int i=0;
	while (*str)
	{
		InBuffer[i]=*str;
		i++;
		str++;
	}
	USBD_HID_SendReport(&USB_OTG_dev, InBuffer, i);
}

void ClearBuffer(void)
{
	int i=0;
	while (i<64)
	{
		//Buf[i]=0;
		OutBuffer[i]=0;
		i++;
	}
}
void Delayms(__IO uint32_t nCount)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * nCount / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}
void debugInitialise(void)
{
debugBufferStart=0;
debugBufferEnd=0;
debugBufferLevel=0;
}
// Send debug text to the debug log
void debugOut(char* debugString)
{

	uint16_t charNumber;
	
	// Is there space in the debug buffer?
	if (debugBufferLevel + strlen(debugString) >= DEBUGBUFFERSIZE - 2)
	{
		// Buffer does not have enough space... silently drop the debug string
	}
	else
	{	
		// Buffer is not full, write the bytes and update the end pointer
		for (charNumber = 0; charNumber < strlen(debugString); charNumber++)
		{
			debugBuffer[debugBufferEnd] = debugString[charNumber];
			debugBufferEnd = (debugBufferEnd + 1) % DEBUGBUFFERSIZE;
			
			// Increment the buffer level indicator
			debugBufferLevel++;
		}
		
		// Add a return and new line to the end of the string
		debugBuffer[debugBufferEnd] = '\r';
		debugBufferEnd = (debugBufferEnd + 1) % DEBUGBUFFERSIZE;
		debugBufferLevel++;
		debugBuffer[debugBufferEnd] = '\n';
		debugBufferEnd = (debugBufferEnd + 1) % DEBUGBUFFERSIZE;
		debugBufferLevel++;
	}

}
// Copy 63 bytes of the debug buffer to the USB send buffer
// The first byte is the number of characters transferred
void copyDebugToSendBuffer(uint8_t* sendDataBuffer)
{

	uint8_t bytesToSend = 0;
	uint8_t byteCounter;
	
	// Determine the number of bytes to send
	if (debugBufferLevel > 63) bytesToSend = 63;
		else bytesToSend = debugBufferLevel;
	
	// Place the number of sent bytes in byte[0] of the send buffer
	sendDataBuffer[0] = bytesToSend - 1;
	
	if (debugBufferLevel != 0)
	{
		for (byteCounter = 1; byteCounter < bytesToSend; byteCounter++)
		{
			// Send the next byte to the send buffer
			sendDataBuffer[byteCounter] = debugBuffer[debugBufferStart];
			
			// Update the cyclic buffer pointer
			debugBufferStart = (debugBufferStart + 1) % DEBUGBUFFERSIZE;
			
			// Decrement the buffer level indicator
			debugBufferLevel--;
		}
	}

}	
