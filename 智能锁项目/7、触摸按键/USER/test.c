#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"
#include "BS8116A.h"

int main(void)
{		
	u8 key;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	SysTick_Init();	  	 //��ʱ��ʼ��
	USART1_Init(115200); 
  LED_Init();
	BS81xx_Init1();
	
	while(1)
	{
		key = BS81xx_Key();
		
		if(key)
		{
			printf("%c",key);
		}
	}	 
}




