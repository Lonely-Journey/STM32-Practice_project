#include "sys.h"

/*****************************************************
*��������  ���Զ�ʱ��3���г�ʼ������
*������    ��TIM3_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx���ýṹ��
  NVIC_InitTypeDef NVIC_InitStruct;                //�ж����ýṹ��
  
  //TIMxʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*TIMx��ʼ������*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //����ֵ(TIMx_ARR�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //��Ƶ(TIMx_PSC�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��)
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����)
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);                 //��ʼ��TIM3
	
	/*NVIC����*/
  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;             //ѡ��TIM3�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct);                             //TIM3�жϳ�ʼ��
	
	//����TIMx�����ж�
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM3, ENABLE);			// ʹ��TIM6���ؼĴ���ARR
	
  //ʹ��TIMx
  TIM_Cmd(TIM3, DISABLE);
}









