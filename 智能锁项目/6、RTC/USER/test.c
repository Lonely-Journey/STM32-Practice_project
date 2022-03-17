#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"

int main(void)
{		
	RTC_DATES time = {2019,10,10,0,12,20,20};  //�������޷����õ�
  RTC_DATES rtc;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	SysTick_Init();	  	 //��ʱ��ʼ��
	USART1_Init(115200); 
	//USART3_Init(115200);
	OLED_Init();
  MyRTC_Init(time);
	
	OLED_HanziString(0,0,16,"��ͨ");
  
	while(1)
	{
    rtc = RTC_Get();
    printf("%d��%d��%d�� ����%d\r\n",rtc.year,rtc.month,rtc.day,rtc.week);
    printf("%d : %d : %d\r\n",rtc.hour,rtc.min,rtc.sec);
    delay_ms(1000);
  } 
}




