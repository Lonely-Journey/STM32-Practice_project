#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "spi.h"

int main(void)
{			
	SysTick_Init();	  	//��ʱ��ʼ��
  LED_Init();
	SPI2_Init();
	OLED_Init();
	
	OLED_HanziString(0,0,16,"��ͨ");
	
	while(1)
	{
		
	}	 
}




