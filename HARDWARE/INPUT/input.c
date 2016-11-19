#include "input.h"
#include "led.h"
#include "usart3.h"
#include "moto.h"


/*-----MOTO1---INPUT--PB6-----*/
/*-----MOTO2---INPUT--PA0-----*/
/*-----MOTO3---INPUT--PA1-----*/

/**********************************************
* describetion: 定时器5通道1输入捕获配置,1号电机的速度采集初始化
* param: arr: 自动重装值
*        psc: 时钟预分频数
* return: none
* author: 周学伟
* date : 2016-9-21
*************************************************/
void TIM4_Moto1_Input(u16 arr,u16 psc)
{	 
			TIM_ICInitTypeDef  TIM4_ICInitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  

			GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6 输入  下拉
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);						

			TIM_TimeBaseStructure.TIM_Period = arr; 
			TIM_TimeBaseStructure.TIM_Prescaler =psc; 	
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 

			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
			TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
			NVIC_Init(&NVIC_InitStructure);  
			TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);
			TIM_Cmd(TIM4,ENABLE ); 	
}

/**********************************************
* describetion: 1号电机的速度采集中断
* param: none
* return: none
* author: 周学伟
* date : 2016-9-21
*************************************************/ 
void TIM4_IRQHandler(void)
{ 
		if((Kinematics_Positive1.TIMCH1_CAPTURE_STA&0X80)==0)
		{	  
					if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
					{	    
								if(Kinematics_Positive1.TIMCH1_CAPTURE_STA&0X40)
								{
											if((Kinematics_Positive1.TIMCH1_CAPTURE_STA&0X3F)==0X3F)
											{
													Kinematics_Positive1.TIMCH1_CAPTURE_STA|=0X80;
													Kinematics_Positive1.TIMCH1_CAPTURE_VAL=0XFFFF;
											}else 
														Kinematics_Positive1.TIMCH1_CAPTURE_STA++;
								}	 
					}
					if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
					{	
								if(Kinematics_Positive1.TIMCH1_CAPTURE_STA&0X40)	
								{	  	
											Kinematics_Positive1.TIMCH1_CAPTURE_STA|=0X80;
											Kinematics_Positive1.TIMCH1_CAPTURE_VAL=TIM_GetCapture1(TIM4);
											TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获	
								}else  								
								{
											Kinematics_Positive1.TIMCH1_CAPTURE_STA=0;			
											Kinematics_Positive1.TIMCH1_CAPTURE_VAL=0;
											TIM_SetCounter(TIM4,0);
											Kinematics_Positive1.TIMCH1_CAPTURE_STA|=0X40;		
											TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
								}		    
					}			     	  					   
		}
				TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update);
}


/**********************************************
* describetion: 定时器5通道1输入捕获配置,2号电机的速度采集初始化
* param: arr: 自动重装值
*        psc: 时钟预分频数
* return: none
* author: 周学伟
* date : 2016-9-18
*************************************************/ 

void TIM5_Moto2_Input(u16 arr,u16 psc)
{	 
			TIM_ICInitTypeDef  TIM5_ICInitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  

			GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  下拉
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);						

			TIM_TimeBaseStructure.TIM_Period = arr; 
			TIM_TimeBaseStructure.TIM_Prescaler =psc; 	
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
			TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 

			TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	
			TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
			TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
			TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
			TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
			TIM_ICInit(TIM5, &TIM5_ICInitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
			NVIC_Init(&NVIC_InitStructure);  
			TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
			TIM_Cmd(TIM5,ENABLE ); 	
}


/**********************************************
* describetion: 3号电机的速度采集中断
* param: none
* return: none
* author: 周学伟
* date : 2016-9-20
*************************************************/ 
void TIM5_IRQHandler(void)
{ 
		if((Kinematics_Positive2.TIMCH1_CAPTURE_STA&0X80)==0)
		{	  
					if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
					{	    
								if(Kinematics_Positive2.TIMCH1_CAPTURE_STA&0X40)
								{
											if((Kinematics_Positive2.TIMCH1_CAPTURE_STA&0X3F)==0X3F)
											{
													Kinematics_Positive2.TIMCH1_CAPTURE_STA|=0X80;
													Kinematics_Positive2.TIMCH1_CAPTURE_VAL=0XFFFF;
											}else 
														Kinematics_Positive2.TIMCH1_CAPTURE_STA++;
								}	 
					}
					if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
					{	
								if(Kinematics_Positive2.TIMCH1_CAPTURE_STA&0X40)	
								{	  				
											Kinematics_Positive2.TIMCH1_CAPTURE_STA|=0X80;
											Kinematics_Positive2.TIMCH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
											TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
								}else  								
								{
											Kinematics_Positive2.TIMCH1_CAPTURE_STA=0;			
											Kinematics_Positive2.TIMCH1_CAPTURE_VAL=0;
											TIM_SetCounter(TIM5,0);
											Kinematics_Positive2.TIMCH1_CAPTURE_STA|=0X40;		
											TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
								}		    
					}			     	  					   
		}
				TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}



/**********************************************
* describetion: 定时器5通道1输入捕获配置,3号电机的速度采集初始化
* param: arr: 自动重装值
*        psc: 时钟预分频数
* return: none
* author: 周学伟
* date : 2016-9-22
*************************************************/ 

void TIM2_Moto3_Input(u16 arr,u16 psc)
{	 
			TIM_ICInitTypeDef  TIM2_ICInitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  

			GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1; 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA1 输入  下拉
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);						

			TIM_TimeBaseStructure.TIM_Period = arr; 
			TIM_TimeBaseStructure.TIM_Prescaler =psc; 	
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

			TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	
			TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
			TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
			TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
			TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
			TIM_ICInit(TIM2, &TIM2_ICInitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
			
			NVIC_Init(&NVIC_InitStructure);  
			TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);
			TIM_Cmd(TIM2,ENABLE ); 	
}
/**********************************************
* describetion: 3号电机的速度采集中断
* param: none
* return: none
* author: 周学伟
* date : 2016-9-20
*************************************************/ 
void TIM2_IRQHandler(void)
{ 
		if((Kinematics_Positive3.TIMCH1_CAPTURE_STA&0X80)==0)
		{	  
					if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
					{	    
								if(Kinematics_Positive3.TIMCH1_CAPTURE_STA&0X40)
								{
											if((Kinematics_Positive3.TIMCH1_CAPTURE_STA&0X3F)==0X3F)
											{
													Kinematics_Positive3.TIMCH1_CAPTURE_STA|=0X80;
													Kinematics_Positive3.TIMCH1_CAPTURE_VAL=0XFFFF;
											}else 
														Kinematics_Positive3.TIMCH1_CAPTURE_STA++;
								}	 
					}
					if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
					{	
								if(Kinematics_Positive3.TIMCH1_CAPTURE_STA&0X40)	
								{	  			
											Kinematics_Positive3.TIMCH1_CAPTURE_STA|=0X80;
											Kinematics_Positive3.TIMCH1_CAPTURE_VAL=TIM_GetCapture2(TIM2);
											TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获	
								}else  								
								{
											Kinematics_Positive3.TIMCH1_CAPTURE_STA=0;			
											Kinematics_Positive3.TIMCH1_CAPTURE_VAL=0;
											TIM_SetCounter(TIM2,0);
											Kinematics_Positive3.TIMCH1_CAPTURE_STA|=0X40;		
											TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
								}		    
					}			     	  					   
		}
				TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update);
}



