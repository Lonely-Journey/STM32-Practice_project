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
#include "MG200.h"

int main(void)
{		
	u8 ID = 0 ,i;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	SysTick_Init();	  	 //��ʱ��ʼ��
	USART1_Init(115200); 
  MG200_Init();
	MG200_ON;
	delay_ms(100);
	MG200_Enroll();    //�Ǽ��û�ָ����Ϣ
	MG200_EraseAll();  //ɾ�������û�
	for(i=0;i<3;i++)
	MG200_Enroll();    //�Ǽ��û�ָ����Ϣ
	
	i = MG200_GetUserNum();
	printf("ָ���û�������%d\r\n",i);
	
	while(1)
	{
		ID =MG200_Match1Nl();
		if(ID != 0)
		{
			printf("ID:%d\r\n",ID);
			delay_ms(100);
			MG200_EraseOne(ID);
			
			delay_ms(100);
			i = MG200_GetUserNum();
			printf("ָ���û�������%d\r\n",i);
			
			ID = 0;
		}
	}	 
}




