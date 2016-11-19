#ifndef __MOTO_H
#define __MOTO_H	 
#include "sys.h"



typedef struct _kinematics_
{
        float Target_Speed;                    //运动学正解后的速度  m/s
				int Moto_Speed_Output;                 //给定的转速 rpm/s   实际的电机要乘以减速比 36
				float Input_Speed;                     //电机控制器输入捕获的实际速度 m/s
				unsigned char   TIMCH1_CAPTURE_STA;	   //输入捕获状态		    				
				unsigned short	TIMCH1_CAPTURE_VAL;	   //输入捕获值
				float xyz_Target_value;               //通过上位机或者手机遥控器给定的xyz方向上的速度
				unsigned char D_Flag;
}_struct_Pos_;
extern _struct_Pos_ Kinematics_Positive1;
extern _struct_Pos_ Kinematics_Positive2;
extern _struct_Pos_ Kinematics_Positive3;

typedef union _Upload_speed_   //运动学逆解后的速度 向上位机传送的数据包
{
		unsigned char buffer[16];
		struct _Speed_data_
		{
				float Header;
				float X_speed;
				float Y_speed;
				float Z_speed;
		}upload_speed;
}Struct_Union;

extern Struct_Union movement_cul;

//电机驱动器的编号
#define MOTO1  127
#define MOTO2  126
#define MOTO3  128

//遥控器控制机器人的最大输出速度
#define xspeed  0.4
#define yspeed  0.4
#define zspeed  0.5


void Control_Moto_Speed(unsigned char ip,short speed);
void Speed_Moto_Control(float vx,float vy,float vz);
void Input_Speed_Compture(void);
void Buletooth_Control(u8 Res);


		 				    
#endif


