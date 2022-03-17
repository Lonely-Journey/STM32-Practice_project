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
#include "spi.h"
#include "MFRC522.h"

void print_info(unsigned char *p,int cnt)
{
  int i;
  for(i=0;i<cnt;i++)
  {
		if(!i)
			printf("0x%02X ",p[i]);
		else
			printf("%02X ",p[i]);
  }
  printf("\r\n");
}

int main(void)
{		
	u8 card_id[4];  				//����
	unsigned char CT[2];    //������
	
	SysTick_Init();	  	 //��ʱ��ʼ��
	USART1_Init(115200);
	LED_Init();
	SPI2_Init();
	MFRC_Init();
	
	while(1)
	{
   if(MFRC_PcdRequest(PICC_REQALL,CT)==MI_OK)//Ѱ���ɹ�
    {
       
      if(MFRC_PcdAnticoll(card_id)==MI_OK)/*����ײ�ɹ�*/
      {
        printf("������:");
				print_info(CT,2);//��ӡ����
				printf("����:");
				print_info(card_id,4);//��ӡ����
				
        if(MFRC_PcdSelect(card_id)==MI_OK)//ѡ���ɹ�
        {
					printf("��Ƭ��ȡ�ɹ�\r\n");
					delay_ms(1000);
        }
      }
    }
	}
}




