#include "usart3.h"
#include "LED.h"
#include "moto.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/
/**********************************************
* describetion: USART3��ʼ������
* param: bound��������
* return: none
* author: ��ѧΰ
* date : 2016-9-22
*************************************************/
void Uart3_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_TX
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);  

		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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

		USART_Init(USART3, &USART_InitStructure); 
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART3, ENABLE);                   

}



//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}

union _Speed_
{
	unsigned char Serial_buf[16];     //���ջ���,���USART_REC_LEN���ֽ�.
	struct _L_R_speed_
	{
		float flag;
		float VX_Speed;
		float VY_Speed;
		float VZ_Speed;
	}Struct_Speed;
}Union_Reciver;


/**********************************************
* describetion: ���ڷ���һ���ֽں���
* param: none
* return: none
* author: ��ѧΰ
* date : 2016-9-22
*************************************************/
void USART3_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
			USART_SendData(USART3,b);
}

float send_data[4];
void shanwai_send_data1(uint8_t *value,uint32_t size )
{
	USART3_SendChar(0x03);
	USART3_SendChar(0xfc);
	while(size)
	{
		USART3_SendChar(*value);
		value++;
		size--;
	}
	USART3_SendChar(0xfc);
	USART3_SendChar(0x03);
}
/**********************************************
* describetion: USART3�жϷ����������ڽ�����λ�����͵��ٶ���Ϣ
* param: none
* return: none
* author: ��ѧΰ
* date : 2016-9-22
*************************************************/
u16 USART3_RX_STA=0; 
char Res;
float  VX_speed_ms = 0.0;  //�������˶�ѧ���⺯���ı���
float  VZ_speed_ms = 0.0;
void USART3_IRQHandler(void)                	
{
			if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
			{
						Res =USART_ReceiveData(USART3);	//��ȡ���յ�������     78 41 00 00 00 80 00 00 00 80 00 00 00 80 00 00
						Union_Reciver.Serial_buf[USART3_RX_STA] = Res;
						if(USART3_RX_STA == 16)
								USART3_RX_STA = 0;
						USART3_RX_STA++;
							
						if(Union_Reciver.Struct_Speed.flag == 15.5)//���֡ͷ
						{	
									LED0 = ~LED0;
									
									VX_speed_ms	= Union_Reciver.Struct_Speed.VX_Speed;
								  VZ_speed_ms	= Union_Reciver.Struct_Speed.VZ_Speed;
						}
			} 
}

