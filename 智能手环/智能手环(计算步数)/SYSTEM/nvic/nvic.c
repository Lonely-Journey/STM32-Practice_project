#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"
#include "led.h"


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
  static u8 i = 0;
  static u8 str[200];
  
  //�ж�����ʲô��ʽ�����ж�
	//��������ж�    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //ִ���ж�����
    str[i] = USART_ReceiveData(USART1); //��ȡһ���ֽڵ�����(USART1->DR)
    i++;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
    
    str[i] = '\0';
    printf("�㷢�͵������ǣ�%s\r\n",str);
    i = 0;
  }
}


/******************************************ADC******************************************/
/******************************************************************
*��������  ��ADC�жϷ�����
*������    ��ADC_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void ADC_IRQHandler(void)
{
  u16 data;
  if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
  { 
    //��ȡת���������(ADC1->DR)
		data = ADC_GetConversionValue(ADC1);
    printf("data:%d\r\n",data);
  }
	//�����־
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  //��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC1);
}

/******************************************TIMx��ʱ��******************************************/
/******************************************
*��������  ����ʱ��9���жϷ�����
*������    ��TIM1_BRK_TIM9_IRQHandler
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM1_BRK_TIM9_IRQHandler(void)
{
	//�ж��Ƿ�ʱ��9�ж�
	if(TIM_GetITStatus(TIM9,TIM_FLAG_Update)==SET)
	{ 
		//�����¼�
		GPIOA->ODR ^= 1 << 7;
		//����жϱ�־λ
		TIM_ClearITPendingBit(TIM9,TIM_FLAG_Update);
	}
}

/******************************************
*��������  ����ʱ��10���жϷ�����
*������    ��TIM1_UP_TIM10_IRQHandler
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM1_UP_TIM10_IRQHandler(void)
{
	//�ж��Ƿ�ʱ��10�ж�
	if(TIM_GetITStatus(TIM10,TIM_FLAG_Update)==SET)
	{ 
		//�����¼�
		GPIOA->ODR ^= 1 << 7;
		//����жϱ�־λ
		TIM_ClearITPendingBit(TIM10,TIM_FLAG_Update);
	}
}


