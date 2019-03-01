#include "math.h"
#include "Inverse_Kinematic.h"
#include "Motor.h"
#define pi  3,14159
#define BASE_HIGHT 148
#define HUMERUS 270
#define ULNA    162
#define GRIPPER 130

float hum_sq=HUMERUS*HUMERUS;
float uln_sq=ULNA*ULNA ;
float Grip_angle_R;
float base_angle_r;
float base_angle_d;
float shl_angle_r;
float shl_angle_d ;
float elb_angle_r;
float elb_angle_d ;
float elb_angle_dn;
float wri_angle_d;
float wri_servopulse;
int32_t basepuse;
int32_t elbowpuse;
float radians(float deg)
{
float Radian;
	Radian=deg*0.0174533;
	return Radian;

}
float Deg(float Radian)
{
float Deg;
	Deg=(float)(Radian*57.2958);
	return Deg;
}
/*****************************************************************************/
//
//
//													//
//												//
//								=======
//							//
//						//
//					||
//					||
/*****************************************************************************/
void Set_Arm(float x,float y,float z,float grip_angle_d)
{  
	float rdist ,grip_off_z,grip_off_y,wrist_z,wrist_y,s_w,s_w_sqrt,a1,a2;

	//grip angle in radians for use in calculations
   Grip_angle_R=radians(grip_angle_d);
	 /* Base angle and radial distance from x,y coordinates */
	 base_angle_r = atan2( x, y );
	  base_angle_d=Deg( base_angle_r);
	//Distance from base coodinate to gripper point in x,y plane
	 rdist = sqrt(( x * x ) + ( y * y ));
	/* rdist is y coordinate for the arm */
	/* Grip offsets calculated based on grip angle */
	 grip_off_z = ( sin( Grip_angle_R )) * GRIPPER;
   grip_off_y = ( cos( Grip_angle_R )) * GRIPPER;
	/* Wrist position */
	 wrist_z = ( z - grip_off_z ) - BASE_HIGHT;
   wrist_y = rdist - grip_off_y;
	/* Shoulder to wrist distance ( AKA sw ) */
	 s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
   s_w_sqrt = sqrt( s_w );
	
	/* s_w angle to ground */
	 a1 = atan2( wrist_z, wrist_y );
	/* s_w angle to humerus */
	 a2=acos((( hum_sq - uln_sq ) + s_w ) / ( 2 * HUMERUS * s_w_sqrt ));
	 /* shoulder angle */
	  shl_angle_r = a1 + a2;
	  shl_angle_d = Deg( shl_angle_r ); 
	 /* elbow angle */
	 elb_angle_r = acos(( hum_sq + uln_sq - s_w ) / ( 2 * HUMERUS * ULNA ));
	 elb_angle_d = Deg( elb_angle_r );
	 elb_angle_dn =  elb_angle_d-90+Deg( shl_angle_r );
	/* wrist angle */
   wri_angle_d =  180+grip_angle_d - elb_angle_d  - shl_angle_d;
	  wri_servopulse = 1400 + ( wri_angle_d  * 11 );
	 TIM_PITCH_ROLL_GRIPPER->CCR1=wri_servopulse;
	 elbowpuse=(int32_t)(elb_angle_dn*212.78);
	 basepuse=(int32_t)(base_angle_d*188.89);
	 TIM_PITCH_ROLL_GRIPPER->CCR2=1200;
	 TIM_PITCH_ROLL_GRIPPER->CCR3=2000;
	 Cal_PID_ELBOW_Position(elbowpuse);
	 Cal_PID_Base_Position(basepuse);
}
