#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"

/******************************************�����ж�******************************************/
/******************************************************************
*��������  ������1���жϷ�����
*������    ��USART1_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void USART1_IRQHandler(void)
{
  u8 data;
  static u32 addr = 0x0b0000;
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //�ж�Ҫִ�е�����
    data = USART1->DR;
    W25Q64_Page_Write(addr,1,&data);
    addr++;
    LED_ON;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
    
    LED_OFF;
  }
}

/******************************************************************
*��������  ������2���жϷ�����
*������    ��USART2_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void USART2_IRQHandler(void)
{
  static u8 i = 0;
  static u8 str[200];
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
  {
    //�ж�Ҫִ�е�����
    str[i] = USART2->DR;
		i++;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART2->SR;
    USART2->DR;
		
		str[i] = '\0';
    printf("�㷢�͵������ǣ�%s\r\n",str);
    i = 0;
  }
}

/******************************************��ʱ���ж�******************************************/
/******************************************
*��������  ����ʱ��3���жϷ�����
*������    ��TIM3_IRQHandler 
*��������  ��void
*��������ֵ��void
*��������  ��
*********************************************/
void TIM3_IRQHandler(void)
{
	static u32 i = 0;
  //���TIM3�����жϷ������
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    //���TIMx�����жϱ�־ 
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
    //�ж��¼�
    Motor_Run();
		
		if (i  == 1)
		{
			LAT_H;
			LAT_L;
			LAT_H;	//�����������
			Printer_Left();
			p_flag = 1;//��ӡ��־��1��������һ������
		}
		else if(i == 2)
		{
			Printer_Right();
		}
		i++;
		if(i == 3)
		{
			i =1;
		}
  }
}



