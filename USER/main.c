#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "moto.h"
#include "usart2.h"
#include "time.h"
#include "input.h"
#include "usart3.h"

int main(void)
 {		
		delay_init();	
		LED_Init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		uart_init(115200);	
		Uart3_Init(115200);
		Bluetooth_Uart2_Init(9600);
		TIM3_PWM_Init(7200+1,0);
		Direction_Control();	
	 
	  TIM4_Moto1_Input(0XFFFF,72-1);
		TIM5_Moto2_Input(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
		TIM2_Moto3_Input(0XFFFF,72-1);
	 
		while(1)
		{
				Input_Speed_Compture(); //���벶���ٶ�ת���͵������˶�ѧ��������
		}	 
 }

