#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"

u8 buff[500] = {'\r','\n',0};

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
  static u8 i;
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //�ж�Ҫִ�е�����
    buff[i] = USART1->DR;
		i++;
    LED_ON;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
    
		buff[i]   = '\r';
		buff[i+1] = '\n';
    i = 0;
		LED_OFF;
  }
}

/******************************************************************
*��������  ������2���жϷ�����(��������)
*������    ��USART2_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
//void USART2_IRQHandler(void)
//{
//  static u8 i = 0;
//  static u8 str[200];
//  
//  //�ж�����ʲô��ʽ�����ж�
//	//��������ж�    SET=1   RESET=0
//  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
//  {
//    //�ж�Ҫִ�е�����
//    str[i] = USART2->DR;
//		i++;
//  }
//	//�����ж�        SET=1   RESET=0
//  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
//  {
//		//�����־
//    USART2->SR;
//    USART2->DR;
//		
//		str[i] = '\0';
//    printf("�㷢�͵������ǣ�%s\r\n",str);
//    i = 0;
//  }
//}

/******************************************************************
*��������  ������1���жϷ�����
*������    ��USART1_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void USART2_IRQHandler(void)
{
  static u8 i;
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
  {
    //�ж�Ҫִ�е�����
    buff[i] = USART2->DR;
		i++;
    LED_ON;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART2->SR;
    USART2->DR;
    
		buff[i]   = '\r';
		buff[i+1] = '\n';
    i = 0;
		LED_OFF;
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
    Motor_Run();//�������ת��
		if (i % 2 == 0)
		{
			Printer_Left(); //�ȴ�ӡ���
		}
		else
		{
			Printer_Right(); //���ӡ�ұ�
		}
		i++;
		if(i >= 3)     //800ms
		{
			i = 0;
			p_flag = 1;
		}
  }
}



