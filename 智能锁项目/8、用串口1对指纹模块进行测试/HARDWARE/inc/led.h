#ifndef __LED_H
#define __LED_H	 

	#include "sys.h"

	#define LED1_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_3)    //��LED1
	#define LED1_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_3)      //��LED1
	#define LED2_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_2)    //��LED2
	#define LED2_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_2)      //��LED2

	void LED_Init(void);//��ʼ��
	void zhiwen(void);
	
#endif


