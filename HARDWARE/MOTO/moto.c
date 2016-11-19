#include "moto.h"
#include "delay.h"
#include "math.h"
#include "usart.h"
#include "led.h"
#include "usart3.h"
#include "time.h"

/**********************************************
* describetion: 控制电机运转函数
* param: ip:所控制电机的ip
*        speed:电机的速度，单位转每秒,（没有经过减速器）
* return: none
* author: 周学伟
* date : 2016-9-16
* 电机的控制器设置最大速度上限为72转每秒,占空比和转速的比率=100
*************************************************/
void Control_Moto_Speed(unsigned char ip,short speed)  //speed : rpm/s
{
		switch(ip)
		{
				case MOTO1: if(speed > 0){
												MOTO1_DIRECTION = 1;Kinematics_Positive1.D_Flag=1;}
										else if(speed < 0){
												MOTO1_DIRECTION = 0;Kinematics_Positive1.D_Flag=0;speed = -speed;}
										TIM_SetCompare1(TIM3,speed*100);  
										break;
				case MOTO2: if(speed > 0){
												MOTO2_DIRECTION = 1;Kinematics_Positive2.D_Flag=1;}
										else if(speed < 0){
												MOTO2_DIRECTION = 0;Kinematics_Positive2.D_Flag=0;speed = -speed;}
										TIM_SetCompare2(TIM3,speed*100); 
										break;
				case MOTO3: if(speed > 0){
												MOTO3_DIRECTION = 1;Kinematics_Positive3.D_Flag=1;}
										else if(speed < 0){
												MOTO3_DIRECTION = 0;Kinematics_Positive3.D_Flag=0;speed = -speed;}
										TIM_SetCompare3(TIM3,speed*100); 
										break;
		}
}

/**********************************************
* describetion: 速度滑动数组滤波函数
* param: ip:所控制电机的ip
*        speed:电机的速度，单位转每秒,（没有经过减速器）
* return: none
* author: 周学伟
* date : 2016-10-3
*************************************************/
#define VALUE_MARRE 20
float Add_moto1_Value,Add_moto2_Value,Add_moto3_Value;
float Marrer_Data_1[VALUE_MARRE+1],Marrer_Data_2[VALUE_MARRE+1],Marrer_Data_3[VALUE_MARRE+1];
void The_Filter_Function(float current1,float current2,float current3)
{
		unsigned char i;
		Marrer_Data_1[VALUE_MARRE] = current1;
		Marrer_Data_2[VALUE_MARRE] = current2;
		Marrer_Data_3[VALUE_MARRE] = current3;

    for(i=0;i<VALUE_MARRE;i++)
    {
        Marrer_Data_1[i] = Marrer_Data_1[i+1];
				Marrer_Data_2[i] = Marrer_Data_2[i+1];
				Marrer_Data_3[i] = Marrer_Data_3[i+1];
        
        Add_moto1_Value  += Marrer_Data_1[i];
				Add_moto2_Value  += Marrer_Data_2[i];
				Add_moto3_Value  += Marrer_Data_3[i];
    }
    Kinematics_Positive1.Input_Speed  = Add_moto1_Value  / VALUE_MARRE;
		Kinematics_Positive2.Input_Speed  = Add_moto2_Value  / VALUE_MARRE;
		Kinematics_Positive3.Input_Speed  = Add_moto3_Value  / VALUE_MARRE;
    
    Add_moto1_Value = 0;
		Add_moto2_Value = 0;
		Add_moto3_Value = 0;
}



_struct_Pos_ Kinematics_Positive1 = {0,0,0,0,0,0};
_struct_Pos_ Kinematics_Positive2 = {0,0,0,0,0,0};
_struct_Pos_ Kinematics_Positive3 = {0,0,0,0,0,0};

#define VX_VALUE           0.5f   //(0.5f)
#define VY_VALUE           0.72f  //(sqrt(3)/2.0)      
#define L_value            0.219f // 0.2/角速度校准参数(0.915) 

#define RADIUS_value       0.386  
#define REDUCTION_RATIO    36 //减速比
/**********************************************
* describetion: 底盘运动学正解函数
* param: vx: x方向的速度
*        vy: y方向的速度
*        vz：z轴的速度
* return: none
* author: 周学伟
* date : 2016-9-16
*************************************************/
void Speed_Moto_Control(float vx,float vy,float vz)
{
        Kinematics_Positive1.Target_Speed    = (-VX_VALUE*vy + VY_VALUE*vx + L_value*vz);//正解函数，具体推算过程请查看我的博客
        Kinematics_Positive2.Target_Speed    = (-VX_VALUE*vy - VY_VALUE*vx + L_value*vz);
        Kinematics_Positive3.Target_Speed    = (vy + L_value*vz);
	
				Kinematics_Positive1.Moto_Speed_Output  = -(Kinematics_Positive1.Target_Speed  * REDUCTION_RATIO / RADIUS_value);   //单位m/s
        Kinematics_Positive2.Moto_Speed_Output  = -(Kinematics_Positive2.Target_Speed  * REDUCTION_RATIO / RADIUS_value);
        Kinematics_Positive3.Moto_Speed_Output  = -(Kinematics_Positive3.Target_Speed  * REDUCTION_RATIO / RADIUS_value);
	
				Control_Moto_Speed(MOTO1,Kinematics_Positive1.Moto_Speed_Output);
				Control_Moto_Speed(MOTO2,Kinematics_Positive2.Moto_Speed_Output);
				Control_Moto_Speed(MOTO3,Kinematics_Positive3.Moto_Speed_Output);
}

/************************************************
* describetion: 蓝牙控制函数
* param: Res: 蓝牙传输的控制码
* return: none
* author: 周学伟
* date : 2016-9-18
*************************************************/ 
unsigned char Remote_control = 0;  //蓝牙遥控器模式开关量
extern u8 Buletooth_Res;   //遥控器指令码


void Buletooth_Control(u8 Res)
{
		if(Res == 'h')
				Remote_control = 1;//遥控模式开
		if(Res == 'i')  
				Remote_control = 0;//遥控模式关
		if(Remote_control == 1)
		{
				switch(Res)
				{
					case 'a':  Speed_Moto_Control(xspeed,0.0,0.0);   break;
					case 'b':  Speed_Moto_Control(-xspeed,0.0,0.0);  break;
					case 'c':  Speed_Moto_Control(0.0,yspeed,0.0);   break;
					case 'd':  Speed_Moto_Control(0.0,-yspeed,0.0);  break;
					case 'e':  Speed_Moto_Control(0.0,0.0,zspeed);   break;
					case 'f':  Speed_Moto_Control(0.0,0.0,-zspeed);  break;
					case 'g':  Speed_Moto_Control(0.0,0.0,0.0);      break;
				}
		}
}
/**********************************************
* describetion: 输入捕获速度转化和底盘逆运动学解析函数
* param:  none
* return: none
* author: 周学伟
* date : 2016-9-21
* note: 实际电机的
*************************************************/
extern float send_data[4];
#define   DISTANCE   0.0107  //实际电机转动一圈的位移
Struct_Union movement_cul;
unsigned char i=0,k;
unsigned moveit_flag = 0;   //程序执行次数
extern float  VX_speed_ms;  //给定到运动学正解函数的变量，全局速度信息，通过上位机订阅的cmd_val话题下的数据
extern float  VZ_speed_ms;
void Input_Speed_Compture()
{		
		if((Kinematics_Positive1.TIMCH1_CAPTURE_STA&0x80))
		{	
						Kinematics_Positive1.Input_Speed = (((Kinematics_Positive1.TIMCH1_CAPTURE_STA&0X3F)*65535)+Kinematics_Positive1.TIMCH1_CAPTURE_VAL)*0.000001;
						Kinematics_Positive1.Input_Speed =  DISTANCE/(Kinematics_Positive1.Input_Speed*4.0);
						(Kinematics_Positive1.D_Flag > 0)?(Kinematics_Positive1.Input_Speed = Kinematics_Positive1.Input_Speed):(Kinematics_Positive1.Input_Speed = -Kinematics_Positive1.Input_Speed);
						Kinematics_Positive1.TIMCH1_CAPTURE_STA = 0;   //清零工作 只有清零后才能进入速度捕获函数		
		}
		
		if(Kinematics_Positive2.TIMCH1_CAPTURE_STA&0x80)
		{
						Kinematics_Positive2.Input_Speed = (((Kinematics_Positive2.TIMCH1_CAPTURE_STA&0X3F)*65535)+Kinematics_Positive2.TIMCH1_CAPTURE_VAL)*0.000001;
						Kinematics_Positive2.Input_Speed =  DISTANCE/(Kinematics_Positive2.Input_Speed*4.0);
						(Kinematics_Positive2.D_Flag > 0)?(Kinematics_Positive2.Input_Speed = Kinematics_Positive2.Input_Speed):(Kinematics_Positive2.Input_Speed = -Kinematics_Positive2.Input_Speed);
						Kinematics_Positive2.TIMCH1_CAPTURE_STA = 0;  
		}
		
		if(Kinematics_Positive3.TIMCH1_CAPTURE_STA&0x80)
		{
						Kinematics_Positive3.Input_Speed = (((Kinematics_Positive3.TIMCH1_CAPTURE_STA&0X3F)*65535)+Kinematics_Positive3.TIMCH1_CAPTURE_VAL)*0.000001;
						Kinematics_Positive3.Input_Speed = DISTANCE/(Kinematics_Positive3.Input_Speed*4.0);
						(Kinematics_Positive3.D_Flag > 0)?(Kinematics_Positive3.Input_Speed = Kinematics_Positive3.Input_Speed):(Kinematics_Positive3.Input_Speed = -Kinematics_Positive3.Input_Speed);
						Kinematics_Positive3.TIMCH1_CAPTURE_STA = 0;   
		}
		//因为速度为0时，驱动器没有脉冲输出，所以逆解的速度无法得到更新，所以出此下策
		(Kinematics_Positive1.Moto_Speed_Output == 0)?(Kinematics_Positive1.Input_Speed = 0.0):(k=0); 
		(Kinematics_Positive2.Moto_Speed_Output == 0)?(Kinematics_Positive2.Input_Speed = 0.0):(k=0); 
		(Kinematics_Positive3.Moto_Speed_Output == 0)?(Kinematics_Positive3.Input_Speed = 0.0):(k=0); 		

		//将传送到上位机的速度数据滤波   如果增加滤波后需要对机器人的角速度和线速度警醒校准
		//The_Filter_Function(Kinematics_Positive1.Input_Speed,Kinematics_Positive2.Input_Speed,Kinematics_Positive3.Input_Speed);

		movement_cul.upload_speed.Header  = 15.5;//填充0上位机传送的速度数据
		movement_cul.upload_speed.Y_speed = -(2.0*Kinematics_Positive3.Input_Speed-Kinematics_Positive1.Input_Speed-Kinematics_Positive2.Input_Speed)/3.0;
		movement_cul.upload_speed.X_speed = -(Kinematics_Positive1.Input_Speed - Kinematics_Positive2.Input_Speed)/1.442;
		movement_cul.upload_speed.Z_speed = -(Kinematics_Positive1.Input_Speed + Kinematics_Positive2.Input_Speed + Kinematics_Positive3.Input_Speed)/(L_value*3);

		for(i=0;i<16;i++)
				 USART3_SendChar(movement_cul.buffer[i]);  //向上位机传送速度数据包
		
		
		Buletooth_Control(Buletooth_Res);
		if(Remote_control == 0)
				Speed_Moto_Control(VX_speed_ms,0.0,VZ_speed_ms);
		
		send_data[0] = movement_cul.upload_speed.X_speed*100.0;  //填充查看波形的数据包数据，数据太小，曲线不明显所以放大100
		send_data[1] = movement_cul.upload_speed.Y_speed*100.0;
		send_data[2] = movement_cul.upload_speed.Z_speed*100.0;
		//shanwai_send_data1((uint8_t*)&send_data,sizeof(send_data));
		
}

