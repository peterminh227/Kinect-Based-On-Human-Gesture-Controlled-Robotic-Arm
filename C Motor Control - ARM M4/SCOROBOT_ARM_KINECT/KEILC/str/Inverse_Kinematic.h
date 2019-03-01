#ifndef	__INVERSE_KINEMATIC_H
#define __INVERSE_KINEMATIC_H

extern float Grip_angle_R;
extern float base_angle_r;
extern float shl_angle_r;
extern float shl_angle_d ;
extern float elb_angle_r;
extern float elb_angle_d ;
extern float elb_angle_dn;
extern float wri_angle_d;

float radians(float deg);
float Deg(float Radian);
void Set_Arm(float x,float y,float z,float grip_angle_d);


#endif