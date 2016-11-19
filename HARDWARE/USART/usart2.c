#include "usart2.h"
#include "usart.h"
#include "moto.h"

/*-----USART2_TX-----PA2-----*/
/*-----USART2_RX-----PA3-----*/ 
  
void Bluetooth_Uart2_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART1_TX
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//∏°ø’ ‰»Î
		GPIO_Init(GPIOA, &GPIO_InitStructure);//≥ı ºªØGPIOA.10  

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);	

		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

		USART_Init(USART2, &USART_InitStructure); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);                   

}
u8 Buletooth_Res;
void USART2_IRQHandler(void)                	
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{
				Buletooth_Res =USART_ReceiveData(USART2);	
    } 
} 

