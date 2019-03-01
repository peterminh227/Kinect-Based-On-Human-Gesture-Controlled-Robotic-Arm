#ifndef __MAIN_H
#define __MAIN_H
#include "stdint.h"
typedef struct
{
double phi[6][6],theta[6][6],theta_temp[6][6];
double L[6][6],P[6][6],phiT[6][6];
double temp[6][6],temp2[6][6],Kalman[6][6],denominator[6][6],numerator[6][6];
double lamda,lamda_predict,phiTPphi,trace_P;
double epsilon;
uint8_t Sinoid_Counter;
double Set_Frequency_[3];
double Frequency_[4],Frequency_Debug;
double Error_value_[5];
long Set_Position_[8];
double out[8];
double out_temp,out_real;
unsigned int out_int;
double s0,s1,s2,s3,t0,t1,t2,r1,r2,r3;	

}STR_index;
#endif