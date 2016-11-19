#include "time.h"
#include "usart2.h"
#include "moto.h"


/*-----MOTO1---PWM-OUTPUT----PC6--*/
/*-----MOTO2---PWM-OUTPUT----PC7--*/
/*-----MOTO3---PWM-OUTPUT----PC8--*/

/*-----MOTO1---INPUT---------PB6--*/
/*-----MOTO2---INPUT---------PA0--*/
/*-----MOTO3---INPUT---------PA1--*/

/*-----MOTO1---direction-----PC0--*/
/*-----MOTO2---direction-----PC1--*/
/*-----MOTO3---direction-----PC2--*/





/**********************************************
* describetion: PWM��ʼ������ ����Ƶ��PWMƵ��=72000000/7200=10Khz
* param:  arr:��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
*         psc:����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
* return: none
* author: ��ѧΰ
* date : 2016-9-30
*************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;


			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

			GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3��ȫ��ӳ��

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			 //��ʼ��TIM3
			TIM_TimeBaseStructure.TIM_Period = arr;  
			TIM_TimeBaseStructure.TIM_Prescaler =psc;  
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�

			TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
			TIM_OC3Init(TIM3, &TIM_OCInitStructure); 

			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
			TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  

			TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM2
	
}
/**********************************************
* describetion: ���������Ƴ�ʼ������
* param:  none
* author: ��ѧΰ
* date : 2016-10-1
*************************************************/
void Direction_Control()
{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_0;				 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
			GPIO_Init(GPIOC, &GPIO_InitStructure);					 
	
			GPIO_SetBits(GPIOC,GPIO_Pin_1);
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
}











