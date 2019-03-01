#include "stm32f4xx.h"
#include "stdio.h"
#include "stdint.h"
#include "Matrix_Operator.h"
#include "Structure.h"
#include "Motor.h"
#define Sinoid_pAmplitude 8000
#define Sinoid_Period 5
#define Sinoid_Counter_Max Sinoid_Period*200
volatile unsigned char Position_Model = 0, lamda_Model = 1;
double const inside_Sine = 2*3.14159265358979323*0.005/Sinoid_Period;
double Set_Frequency_[3]={0,0,0};
double Error_value_[5]={0,0,0,0,0};
long Set_Position[4]={0,0,0,0};
long Position_[8]={0,0,0,0,0,0,0,0};
double out[8]={0,0,0,0,0,0,0,0};
double out_temp=0,out_real=0;
uint16_t out_int;
double s0,s1,s2,s3,t0,t1,t2,r1,r2,r3;
double phi[6][6], theta[6][6], theta_temp[6][6], L[6][6], P[6][6], phiT[6][6]; 
double temp[6][6], temp2[6][6], Kalman[6][6], denominator[6][6], numerator[6][6], Temp_mul;
double lamda_predict = 100, lamda = 0.98, phiTPphi = 0, trace_P = 0;// The higher lamda the more stable, the lower the faster from 0.95 to 1
double epsilon = 0; 

void Estimation_Algorithm(void)
{
//phi(k)=[-y[l-1]-y(k-2)-y(k-3) u(k-1) u(k-2) u(k-3)]T
	phi[0][0]=-Position_[1];//-pre_Position
	phi[1][0]=-Position_[2];//-pre_pre_Position;
	phi[2][0]=-Position_[3];//-pre_pre_pre_Position;
	phi[3][0]=out[1];//pre_out;
	phi[4][0]=out[2];//pre_pre_out;
	phi[5][0]=out[3];//pre_pre_pre_out;
//tinh ma tran chuyen vi
	Tranpose_Matrix(phi,6,1,phiT);
/*
//	epsilon(k)=Position(k)-phiT(k)*theta(k-1)
*/
	//tinh temp=phiT(k)*theta(k-1)
	Multiply_Matrix(phiT,1,6,theta,6,1,temp);
	epsilon=Position_[0]-temp[0][0];
	/******************************************
	*                 P(k-1)*phi(k)
	*   L(k)=-----------------------------
	*					lamda + phiT(k)*P(k-1)*phi(k)
	******************************************/
	//numerator=(P(k-1)*phi(k))
	Multiply_Matrix(P,6,6,phi,6,1,numerator);
	//temp = phiT(k)*P(k-1)
	Multiply_Matrix(phiT,1,6,P,6,6,temp);
	//denominator=phiT(k)*P(k-1)*phi(k)
	Multiply_Matrix(temp,1,6,phi,6,1,denominator);
	phiTPphi = denominator[0][0];
	//(1 + phiT(k)*P(k-1)*phi(k))
	denominator[0][0] = denominator[0][0] + 1;
	//tinh L
	Divide_Matrix_Vector(numerator,6,1,denominator[0][0],L);
	/*********************************************
	* 						  [1- phiT(k-1)*L(k)]*esilon(k)^2
  *	          {1- --------------------------------- neu lamda(k)>lamda(k)min
	*														 tocdothaydoi
	* lamda(k)=|
	* 					{ lamda(k) min neu lamda(k)< lamda(k)min
	*
	*********************************************/
		  lamda = epsilon * epsilon;
			lamda = lamda / (1 + phiTPphi);
			lamda = lamda / lamda_predict;
			lamda = 1 - lamda;
			if(lamda>1){lamda = 1;}
			if(lamda<0.95){lamda = 0.95;}
	/*************************************************************		
	*P(k) = (P(k-1) - //lamda
	*
  *						1			|						(P(k-1)*phi(k)*phiT(k)*P(k-1))   |
  *	  P(k)=  ---  *	| P(k-1) - -------------------------------   |
  *					lamda   |						(lamda + phiT(k)*P(k-1)*phi(k))) |
  *			1		 |                          |
	*		----  *| P(k-1)-L(k)*phi(k)*P(k-1)|  
	*		lamda  |													|
	*		
	**************************************************************/
			Multiply_Matrix(L,6,1,temp,1,6,temp2);//temp = phiT(k)*P(k-1)
			Sub_Matrix(P,temp2,6,6,temp);
			Divide_Matrix_Vector(temp,6,6,lamda,P);
/*************************************************
			theta(k) = theta(k-1) + L(k)*epsilon(k)
**************************************************/	
			theta_temp[3][0] = theta[3][0];
			Multiply_Matrix_Vector(L,6,1,epsilon,temp);
			Add_Matrix(theta,temp,6,1,theta);
		/*Recursion*/	
       Position_[7]=Position_[6];
			 Position_[6]=Position_[5];
			 Position_[5]=Position_[4];
			 Position_[4]=Position_[3];
			 Position_[3]=Position_[2];
			 Position_[2]=Position_[1];
			 Position_[1]=Position_[0];
			 
			 out[7]=out[6];
			 out[6]=out[5];
			 out[5]=out[4];
			 out[4]=out[3];
			 out[3]=out[2];
			 out[2]=out[1];
			 out[1]=out[0];
			 
			 Set_Position[3]=Set_Position[2];
			 Set_Position[2]=Set_Position[1];
			 Set_Position[1]=Set_Position[0];
			 
			 Error_value_[2]=Error_value_[1];
			 Error_value_[1]=Error_value_[0];
			 
			 trace_P = P[0][0] + P[1][1]+ P[2][2]+ P[3][3]+ P[4][4]+ P[5][5];
			 
			 if (theta[3][0] == 0) {theta[3][0] = theta_temp[3][0];}
}

void Model_Position(void)
{
	/* MODEL 0			POT = 0% 								Tqd < 0.0001		0.005 seconds    
	epsilon = 0.826085 ~ 1                  POT = 0%                     POT = e^[(-epsilon * pi) / sqrt(1 - epsilon^2)]
	Wn = 4000 ~ 4000                        Tqd < 0.001								 	Tqd = 4 / (epsilon * Wn)
	Gd =
          z^2 + 4.372e-08 z + 5.245e-20
  ---------------------------------------------
  z^3 - 4.122e-09 z^2 + 4.248e-18 z + 3.622e-41
	*/ 
	if (Position_Model == 0){
		#ifdef Integrator_Roburst
		/*r1=b2-b1/b1
		r2=b3-b2/b1
		r2=-b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1+1)/b1
		s1=(a2m-a2+a1)/b1
		s2=(a3m-a3+a2)/b1
		s3=a3/b1*/
		r1 = (theta[4][0]-theta[3][0])/ theta[3][0];
		r2 = (theta[5][0]-theta[4][0]) / theta[3][0];
		r3 = -theta[5][0] / theta[3][0];
		t0 = 0.999999952160871 / theta[3][0];
		t1 = 4.37168138727063e-08 / theta[3][0];
		t2 = 5.24488179668768e-20 / theta[3][0];
		s0 = (-4.12230724487712e-09 - theta[0][0] + 1) / theta[3][0];
		s1 = (4.24835425529119e-18 - theta[1][0] + theta[0][0]) / theta[3][0];
		s2 = (3.62206672972073e-41 - theta[2][0] + theta[1][0]) / theta[3][0];
		s3 = theta[2][0] / theta[3][0];
		#else
		/*r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1*/
		r1 = theta[4][0] / theta[3][0];
		r2 = theta[5][0] / theta[3][0];
		t0 = 0.999999952160871 / theta[3][0];
		t1 = 4.37168138727063e-08 / theta[3][0];
		t2 = 5.24488179668768e-20 / theta[3][0];
		s0 = (-4.12230724487712e-09 - theta[0][0]) / theta[3][0];
		s1 = (4.24835425529119e-18 - theta[1][0]) / theta[3][0];
		s2 = (3.62206672972073e-41 - theta[2][0]) / theta[3][0];
		#endif
	}
	/* MODEL 1			POT = 0% 						Tqd < 2		0.005 seconds   
	epsilon = 0.826085 ~ 1              POT = 0%                POT = e^[(-epsilon * pi) / sqrt(1 - epsilon^2)]
	Wn = 1600 ~ 1600                    Tqd < 0.0025							Tqd = 4 / (epsilon * Wn)
       0.9967 z^2 + 0.002642 z + 1.389e-09
  ---------------------------------------------
  z^3 - 0.0006709 z^2 + 1.125e-07 z - 7.117e-27
	*/ 
	if (Position_Model == 1){
		#ifdef Integrator_Roburst
		/*r1=b2-b1/b1
		r2=b3-b2/b1
		r2=-b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1+1)/b1
		s1=(a2m-a2+a1)/b1
		s2=(a3m-a3+a2)/b1
		s3=a3/b1*/
		r1 = (theta[4][0]-theta[3][0])/ theta[3][0];
		r2 = (theta[5][0]-theta[4][0]) / theta[3][0];
		r3 = -theta[5][0] / theta[3][0];
		t0 = 0.996686788860220 / theta[3][0];
		t1 = 0.00264239702982444 / theta[3][0];
		t2 = 1.38932314468172e-09 / theta[3][0];
		s0 = (-0.000670925255805015 - theta[0][0] + 1) / theta[3][0];
		s1 = (1.12535174719261e-07 - theta[1][0] + theta[0][0]) / theta[3][0];
		s2 = (-7.11704102524349e-27 - theta[2][0] + theta[1][0]) / theta[3][0];
		s3 = theta[2][0] / theta[3][0];
		#else
		/*r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1*/
		r1 = theta[4][0] / theta[3][0];
		r2 = theta[5][0] / theta[3][0];
		t0 = 0.996686788860220 / theta[3][0];
		t1 = 0.00264239702982444 / theta[3][0];
		t2 = 1.38932314468172e-09 / theta[3][0];
		s0 = (-0.000670925255805015 - theta[0][0]) / theta[3][0];
		s1 = (1.12535174719261e-07 - theta[1][0]) / theta[3][0];
		s2 = (-7.11704102524349e-27 - theta[2][0]) / theta[3][0];
		#endif
		/*
		r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1
		*/
	}
		/* MODEL 2			POT = 0% 						Tqd < 2		0.005 seconds   
	epsilon = 0.826085 ~ 1              POT = 0%                POT = e^[(-epsilon * pi) / sqrt(1 - epsilon^2)]
	Wn = 800 ~ 800                   	  Tqd < 0.005							Tqd = 4 / (epsilon * Wn)
      0.9005 z^2 + 0.06319 z + 4.142e-06
  -------------------------------------------
  z^3 - 0.03663 z^2 + 0.0003355 z - 3.145e-22
	*/ 
	if (Position_Model == 2){
		#ifdef Integrator_Roburst
		/*r1=b2-b1/b1
		r2=b3-b2/b1
		r2=-b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1+1)/b1
		s1=(a2m-a2+a1)/b1
		s2=(a3m-a3+a2)/b1
		s3=a3/b1*/
		r1 = (theta[4][0]-theta[3][0])/ theta[3][0];
		r2 = (theta[5][0]-theta[4][0]) / theta[3][0];
		r3 = -theta[5][0] / theta[3][0];
		t0 = 0.900507640604406 / theta[3][0];
		t1 = 0.0631924027321025 / theta[3][0];
		t2 = 4.14151392472197e-06 / theta[3][0];
		s0 = (-0.0366312777774684 - theta[0][0] + 1) / theta[3][0];
		s1 = (0.000335462627902516 - theta[1][0] + theta[0][0]) / theta[3][0];
		s2 = (-3.14463895119926e-22 - theta[2][0] + theta[1][0]) / theta[3][0];
		s3 = theta[2][0] / theta[3][0];
		#else
		/*r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1*/
		r1 = theta[4][0] / theta[3][0];
		r2 = theta[5][0] / theta[3][0];
		t0 = 0.900507640604406 / theta[3][0];
		t1 = 0.0631924027321025 / theta[3][0];
		t2 = 4.14151392472197e-06 / theta[3][0];
		s0 = (-0.0366312777774684 - theta[0][0]) / theta[3][0];
		s1 = (0.000335462627902516 - theta[1][0]) / theta[3][0];
		s2 = (-3.14463895119926e-22 - theta[2][0]) / theta[3][0];
		#endif
		/*
		r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1
		*/
	}
		/* MODEL 3			POT = 0% 						Tqd < 2		0.005 seconds   
	epsilon = 0.826085 ~ 1              POT = 0%                POT = e^[(-epsilon * pi) / sqrt(1 - epsilon^2)]
	Wn = 400 ~ 400                    Tqd < 0.01							Tqd = 4 / (epsilon * Wn)
     0.5656 z^2 + 0.1818 z + 0.0002261
  ----------------------------------------
  z^3 - 0.2707 z^2 + 0.01832 z - 3.775e-11
	*/ 
	if (Position_Model == 3){
		#ifdef Integrator_Roburst
		/*r1=b2-b1/b1
		r2=b3-b2/b1
		r2=-b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1+1)/b1
		s1=(a2m-a2+a1)/b1
		s2=(a3m-a3+a2)/b1
		s3=a3/b1*/
		r1 = (theta[4][0]-theta[3][0])/ theta[3][0];
		r2 = (theta[5][0]-theta[4][0]) / theta[3][0];
		r3 = -theta[5][0] / theta[3][0];
		t0 = 0.565590448844686 / theta[3][0];
		t1 = 0.181828503413316 / theta[3][0];
		t2 = 0.000226118616496226 / theta[3][0];
		s0 = (-0.270670568534380 - theta[0][0] + 1) / theta[3][0];
		s1 = (0.0183156394466279 - theta[1][0] + theta[0][0]) / theta[3][0];
		s2 = (-3.77513450014425e-11 - theta[2][0] + theta[1][0]) / theta[3][0];
		s3 = theta[2][0] / theta[3][0];
		#else
		/*r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1*/
		r1 = theta[4][0] / theta[3][0];
		r2 = theta[5][0] / theta[3][0];
		t0 = 0.001254 / theta[3][0];
		t1 = 0.00378 / theta[3][0];
		t2 = 0.0006895 / theta[3][0];
		s0 = (- 2.178 - theta[0][0]) / theta[3][0];
		s1 = (1.484 - theta[1][0]) / theta[3][0];
		s2 = (- 0.3012 - theta[2][0]) / theta[3][0];
		#endif
		/*
		r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1
		*/
	}
}

void MRC_Design(void)
{
    Error_value_[0] =Set_Position[0] - Position_[0];
		Model_Position();
	#ifdef Integrator_Roburst
		/*r1=b2-b1/b1
		r2=b3-b2/b1
		r3=-b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1+1)/b1
		s1=(a2m-a2+a1)/b1
		s2=(a3m-a3+a2)/b1
		s3=a3/b1*/
		out[0] = - (r1 * out[1]) - (r2 * out[2]) - (r3 * out[3])
					+ (t0 * Set_Position[0]) + (t1 * Set_Position[1]) + (t2 * Set_Position[2])
					- (s0 * Position_[0]) - (s1 * Position_[1])- (s2 * Position_[2]) - (s3 * Position_[3]);
		#else
		/*r1=b2/b1
		r2=b3/b1
		t0=b1m/b1
		t1=b2m/b1
		t2=b3m/b1
		s0=(a1m-a1)/b1
		s1=(a2m-a2)/b1
		s2=(a3m-a3)/b1*/
		out[0] = - (r1 * out[1]) - (r2 * out[2])
					+ (t0 * Set_Position[0]) + (t1 * Set_Position[1]) + (t2 * Set_Position[2])
					- (s0 * Position_[0]) - (s1 * Position_[1])- (s2 * Position_[2]);
		#endif
		out_real = out[0];
		#ifdef Track
		#endif

		if (out[0]>MAX_SPEED_PWM) {out[0]=MAX_SPEED_PWM;}
		if (out[0]<-MAX_SPEED_PWM) {out[0]=-MAX_SPEED_PWM;}
Base_Motor_Speed( (int32_t)out[0]);
}
void Initial_Condition_Position(void) 
{	
	theta[0][0]= -1;			//a1
	theta[1][0]= 0;				//a2
	theta[2][0]= 0;				//a3
	theta[3][0]= 1;				//b1
	theta[4][0]= 0;				//b2
	theta[5][0]= 0;				//b3
	theta_temp[3][0]= 1;				//b1
	// Init P matrix
//	P[0][0] = 1000;
//	P[1][1] = 1000;
//	P[2][2] = 1000;
//	P[3][3] = 1000;
//	P[4][4] = 1000;
//	P[5][5] = 1000;
//	trace_P = 6000;	
	
	P[0][0] = 1;
	P[1][1] = 1;
	P[2][2] = 1;
	P[3][3] = 1;
	P[4][4] = 1;
	P[5][5] = 1;
	trace_P = 6;
}

// Random number generator
void RNG_Config(void) 
{
 /* Enable RNG clock source */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
  /* RNG Peripheral enable */
  RNG_Cmd(ENABLE);
}