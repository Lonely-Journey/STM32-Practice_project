#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "key.h"
#include "usart.h"
#include "bluetooth.h"
#include "time.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"
#include "stdio.h"
#include "matrix.h"
#include "nvic.h"
#include "stdio.h"
#include "string.h"

/**************************************************************
*��ע���
*         1��W25Q64��0x0b0000~0x1CF82C���ڴ洢�ֿ�
*            ��ʼ���飺0b   ��11��      ������0    ��0������
*                  ҳ��0    ��0ҳ       �ֽڣ�00   ��0���ֽ�
*            �������飺1C   ��28��      ������F    ��15������
*                  ҳ��8    ��8ҳ       �ֽڣ�2C   ��44���ֽ�
***************************************************************/

int main(void)
{			
	u8 key;
	
	SysTick_Init();	  	//��ʱ��ʼ��
	LED_Init();
	KEY_Init();
	USART1_Init(115200);
	USART2_Init(38400);
	Bluetooth_Init();
//	BLUE_PWR_ON;
//	BLUE_KEY_CMD;
	Printer_Init();
	Motor_Init();
	TIM3_Init(8000-1,71);
	W25Q64_Init();
		
	
	while(1)
	{	
		key = KEY_Scan();
		if(key==1)
		{
			GPIOA->ODR ^= 1<<1;
			Printer1_String24(buff);
			MOTOR_OFF;
			Printer_OFF();
		}
	}	 
}




