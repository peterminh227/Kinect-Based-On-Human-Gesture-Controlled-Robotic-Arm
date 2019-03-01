#include "math.h"
#include "SCOROBOT_INVERSE.h"
#include "Motor.h"
#define pi  3,14159
#define d1 148
#define a2 270
#define a3 162
#define d5 130

float theta1_d,theta2_d_n,theta2_d_p,theta3_d,theta3_d_n,theta3_d_p,theta4_d_n,theta4_d_p,theta234_d;
float X1,X2,X3,Y1,Y2,Y3;
float theta234_r,theta1_r,theta2_r_p,theta2_r_n,theta3_r,theta4_r_n,theta4_r_p;
	float cos_theta3,theta3_r_p,theta3_r_n,sin_theta3_p,sin_theta3_n;
float Degrees_to_Radians(float Deg)
{
	float Radian;
	Radian=Deg*0.0174533;
	return Radian;
}
float Radians_to_Degrees(float Rad)
{
  float Deg;
	Deg=(float)(Rad*57.2958);
	return Deg;
}

void Caculator_Inverse(float x,float y,float z,float grip_angle)
{

	theta234_r=Degrees_to_Radians(grip_angle);
	theta1_r=atan2(x,y);
	theta1_d=Radians_to_Degrees(theta1_r);
	X1=sqrt((x*x)+(y*y));
	Y1=z;
	X2=X1-d5*cos(theta234_r);
	Y2=Y1+d5*sin(theta234_r);
	X3=X2;
	Y3=Y2-d1;
	cos_theta3=((X3*X3+Y3*Y3-a2*a2-a3*a3)/(2*a2*a3));
	sin_theta3_p=(float)sqrt(1-cos_theta3*cos_theta3);
	sin_theta3_n=(float)(-sqrt(1-(cos_theta3*cos_theta3)));
	theta3_r_p=atan2(sin_theta3_p,cos_theta3);
	theta3_r_n=atan2(sin_theta3_n,cos_theta3);
	theta3_d_p=Radians_to_Degrees(theta3_r_p);
	theta3_d_n=Radians_to_Degrees(theta3_r_n);
	theta2_r_p=-atan2(Y3,X3)-atan2(a3*sin(theta3_r_p),a2+cos(theta3_r_p));
	theta2_r_n=-atan2(Y3,X3)-atan2(a3*sin(theta3_r_n),a2+cos(theta3_r_n));
	theta2_d_p=Radians_to_Degrees(theta2_r_p);
	theta2_d_n=Radians_to_Degrees(theta2_r_n);
	theta4_d_p=grip_angle-theta2_d_p-theta3_d_p;
	theta4_d_n=grip_angle-theta2_d_n-theta3_d_n;
}
