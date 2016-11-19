#ifndef __USART3_H
#define __USART3_H	 

#include "sys.h"
#include "stdio.h"	

void Uart3_Init(u32 bound);
void USART3_SendChar(unsigned char b);
void shanwai_send_data1(uint8_t *value,uint32_t size );
		 				    
#endif

