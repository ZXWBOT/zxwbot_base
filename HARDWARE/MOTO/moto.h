#ifndef __MOTO_H
#define __MOTO_H	 
#include "sys.h"



typedef struct _kinematics_
{
        float Target_Speed;                    //�˶�ѧ�������ٶ�  m/s
				int Moto_Speed_Output;                 //������ת�� rpm/s   ʵ�ʵĵ��Ҫ���Լ��ٱ� 36
				float Input_Speed;                     //������������벶���ʵ���ٶ� m/s
				unsigned char   TIMCH1_CAPTURE_STA;	   //���벶��״̬		    				
				unsigned short	TIMCH1_CAPTURE_VAL;	   //���벶��ֵ
				float xyz_Target_value;               //ͨ����λ�������ֻ�ң����������xyz�����ϵ��ٶ�
				unsigned char D_Flag;
}_struct_Pos_;
extern _struct_Pos_ Kinematics_Positive1;
extern _struct_Pos_ Kinematics_Positive2;
extern _struct_Pos_ Kinematics_Positive3;

typedef union _Upload_speed_   //�˶�ѧ������ٶ� ����λ�����͵����ݰ�
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

//����������ı��
#define MOTO1  127
#define MOTO2  126
#define MOTO3  128

//ң�������ƻ����˵��������ٶ�
#define xspeed  0.4
#define yspeed  0.4
#define zspeed  0.5


void Control_Moto_Speed(unsigned char ip,short speed);
void Speed_Moto_Control(float vx,float vy,float vz);
void Input_Speed_Compture(void);
void Buletooth_Control(u8 Res);


		 				    
#endif


