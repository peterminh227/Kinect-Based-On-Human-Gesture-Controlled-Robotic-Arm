#ifndef _USB_HID_H
#define _USB_HID_H
#include "stm32f4xx.h"
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
extern int32_t kp,kd,ki,sp;
extern int8_t flag_receive;
extern int8_t flag_process;
extern int8_t home_Request;
extern int8_t flag_Emegency_stop;
extern int8_t flag_auto;
extern int8_t flag_Receive_Data_Grid;
extern int32_t setpoint_base,setpoint_Shoulder,setpoint_elbow,setpoint_pitch,setpoint_roll,setpoint_gripper;
extern int16_t Speed_Base,Speed_Shoulder,Speed_Elbow;
extern int16_t Duty_Base;
extern int8_t flag_Reverse_Base;
extern int8_t flag_StopMotor_Base;
extern int8_t flag_Forward_Base;
extern int8_t flag_Forward_Shoulder;
extern int8_t flag_Reverse_Shoulder;
#define nonPID_Forward_Base       0x25
#define nonPID_Reverse_Base				0x26
#define nonPID_StopMotor_Base     0x27
#define nonPID_Forward_Shouler    0x28
#define nonPID_Reverse_Shoulder   0x29
void USB_Init(void);
void Get_Data_from_PC(void);
void send_usb();
void ClearBuffer(void);
void Delayms(__IO uint32_t nCount);
extern uint8_t Buffer[HID_OUT_PACKET];
extern uint8_t InBuffer[64], OutBuffer[63],Buf[63];
extern char debugString[64];
extern uint8_t ToSendDataBuffer[64];
void debugInitialise(void);
void debugOut(char* debugString);
void copyDebugToSendBuffer(uint8_t* sendDataBuffer);
void Send_Debug(void);
#endif