#include "stm32f4xx.h"  

/*****************************************************
*��������  ���Զ�ʱ��9���г�ʼ������
*������    ��TIM9_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM9_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);            //��ʼ��TIM9
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //����ʱ��9�����ж�
	
	//TIM10�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;    //TIM9�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //��ռ���ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //�����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM9, ENABLE);			            // ʹ��TIM9���ؼĴ���ARR
	
	TIM_Cmd(TIM9,ENABLE); //ʹ�ܶ�ʱ��9
}

/*****************************************************
*��������  ���Զ�ʱ��10���г�ʼ������
*������    ��TIM10_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM10_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);            //��ʼ��TIM10
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE); //����ʱ��10�����ж�
	
	//TIM10�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;    //TIM10�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //��ռ���ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //�����ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM10, ENABLE);			            // ʹ��TIM10���ؼĴ���ARR
	
	TIM_Cmd(TIM10,ENABLE); //ʹ�ܶ�ʱ��10
}

/*****************************************************
*��������  ���Զ�ʱ��4���г�ʼ������
*������    ��TIM4_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);            //��ʼ��TIM4
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	
	//TIM10�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;             //TIM4�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //��ռ���ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //�����ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM4, ENABLE);			                 // ʹ��TIM4���ؼĴ���ARR
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
}

/*****************************************************
*��������  ���Զ�ʱ��4���г�ʼ������
*������    ��TIM3_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)(ע�⣺������ʱ��û�����)	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);            //��ʼ��TIM3
	//TIMx�ж�ѡ��ʹ��
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	
	//TIM10�ж�����-----NVIC ����
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;             //TIM3�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //��ռ���ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //�����ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM3, ENABLE);			                 // ʹ��TIM3���ؼĴ���ARR
	
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
}


