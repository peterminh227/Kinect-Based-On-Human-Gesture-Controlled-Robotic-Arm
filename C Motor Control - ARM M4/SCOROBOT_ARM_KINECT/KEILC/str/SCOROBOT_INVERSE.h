#ifndef	__SCOROBOT_INVERSE_H
#define __SCOROBOT_INVERSE_H

extern float theta1_d,theta2_d,theta3_d,theta3_d_n,theta3_d_p,theta4_d,theta234_d;
extern float X1,X2,X3,Y1,Y2,Y3;
extern float theta234_r,theta1_r,theta2_r,theta3_r,theta4_r;
extern 	float cos_theta3,theta3_r_p,theta3_r_n,sin_theta3_p,sin_theta3_n;
float Degrees_to_Radians(float Deg);
float Radians_to_Degrees(float Rad);
void Caculator_Inverse(float x,float y,float z,float grip_angle);

#endif