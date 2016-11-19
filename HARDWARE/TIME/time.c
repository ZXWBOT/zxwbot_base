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
* describetion: PWM初始化函数 不分频。PWM频率=72000000/7200=10Khz
* param:  arr:设置在下一个更新事件装入活动的自动重装载寄存器周期的值
*         psc:设置用来作为TIMx时钟频率除数的预分频值 
* return: none
* author: 周学伟
* date : 2016-9-30
*************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;


			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

			GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3完全冲映射

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			 //初始化TIM3
			TIM_TimeBaseStructure.TIM_Period = arr;  
			TIM_TimeBaseStructure.TIM_Prescaler =psc;  
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高

			TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
			TIM_OC3Init(TIM3, &TIM_OCInitStructure); 

			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
			TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  

			TIM_Cmd(TIM3, ENABLE);  //使能TIM2
	
}
/**********************************************
* describetion: 电机方向控制初始化函数
* param:  none
* author: 周学伟
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











