#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

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
    //�ж�Ҫִ�е�����
    str[i] = USART1->DR;
		i++;
  }
	//�����ж�        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//�����־
    USART1->SR;
    USART1->DR;
		
		str[i] = '\0';
		
		USART2_Send_Str(str);
		
		i = 0;
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

/******************************************�ⲿ�ж�******************************************/
/******************************************************************
*��������  ���ⲿ�ж�4������
*������    ��EXTI0_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void EXTI4_IRQHandler(void)
{
	//�ж��ⲿ�ж���0�Ƿ���Ĵ���   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
  {
    delay_ms(10);
		//�˲�
    if(!(GPIOE->IDR & (1<<4)))
    {
      EXTI->PR |= (1<<4);
      GPIOB->ODR ^= (1<<5);
    }
  }
}

/******************************************************************
*��������  ������ⲿ�жϷ���ʼ��
*������    ��EXTI9_5_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
             ����˿�
*******************************************************************/
void EXTI9_5_IRQHandler(void)
{
	//�ж��ⲿ�ж���5�Ƿ���Ĵ���   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
  {
    EXTI->PR |= (1<<5);
		GPIOB->ODR ^= (1<<5);
  }
}

/******************************************ADC�ж�******************************************/
/******************************************************************
*��������  ��ADC3�жϷ�����
*������    ��ADC3_IRQHandler
*��������  ��void
*��������ֵ��void
*����      ��
*******************************************************************/
void ADC3_IRQHandler(void)
{
	u16 data;
	
	//�Ƿ�ת�����
	if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)
	{
		//��ȡת���������(ADC_DR�Ĵ���)
		data = ADC_GetConversionValue(ADC3);
		printf("data:%d\r\n",data);
	}
	//�����־
	ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
	//��������ͨ��ת������(ADC_CR2�Ĵ�����22λ)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}



/******************************************��ʱ���ж�******************************************/
/******************************************
*��������  ����ʱ��6���жϷ�����
*������    ��TIM6_DAC_IRQHandler ��
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM6_IRQHandler(void)
{
	//���TIM6�����жϷ������
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  { 
    //�ж��¼�
    GPIOE->ODR ^= (1<<5);
  }
	//���TIMx�����жϱ�־ 
  TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}

/******************************************
*��������  ����ʱ��7���жϷ�����
*������    ��TIM7_IRQHandler ��
*��������  ����
*��������ֵ����
*��������  ��
*********************************************/
void TIM7_IRQHandler(void)
{
  static u8 temp = 0;
  
	//�ж��Ƿ�ʱ��7�ж�
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		//�����¼�
		if(temp == 0)
    {
      if(TIM3->CCR2 == 0) temp = 1;
      else TIM3->CCR2 -=20;
    }
    if(temp == 1)
    {
      if(TIM3->CCR2 == 1000) temp = 0;
      else TIM3->CCR2 +=20;
    }
	}
	//���TIMx�����жϱ�־ 
  TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}

/*********************************************
*��������  ����ʱ��4�жϺ���
*������    ��TIM1_BRK_TIM9_IRQHandler ��
*��������  ��void     
*��������ֵ��void
*��������  ��
* 					 �ж��Ƕ�ʱ9��ͨ��1���ظ������־
* 							���־λ
* 							��CCR1
*
* 					 �ж��Ƕ�ʱ9��ͨ��2�Ĳ����־
* 							���־λ
*               ��CCR2
*********************************************/
void TIM4_IRQHandler(void)
{
	static u16 data_2;
	u16 data_1;
	u16 p_val;
	
	if(TIM_GetITStatus(TIM4,TIM_FLAG_CC2) != RESET)
	{
		//�����ͨ��IC2�Ļ��־λ
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC2);
		//�����¼�
		//CCR2
		data_2 = TIM_GetCapture2(TIM4);	
	}
	
	if(TIM_GetITStatus(TIM4,TIM_FLAG_CC1) != RESET)
	{
		//���ͨ��1�Ĳ����־λ
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1);
		//���ͨ��1�ظ������־λ
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1OF);
		//�����¼�
		//��¼ֵCCR1
		data_1 = TIM_GetCapture1(TIM4);
		//�������ں�ռ�ձ�
		p_val = (float)data_2/data_1*100;
		
		printf("���ڣ�%d   �ߵ�ƽ��%d   ռ�ձȣ�%d%%\r\n",data_1,data_2,p_val);	
	}
}



