#ifndef __TIME_H
#define __TIME_H	 

#include "sys.h"

void TIM3_PWM_Init(u16 arr,u16 psc);
void Direction_Control(void);

#define MOTO1_DIRECTION PCout(0)
#define MOTO2_DIRECTION PCout(1)
#define MOTO3_DIRECTION PCout(2)

		 				    
#endif


