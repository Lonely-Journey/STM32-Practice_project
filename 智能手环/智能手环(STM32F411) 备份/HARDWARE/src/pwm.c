#include "stm32f4xx.h"   

/*****************************************************
*��������  ����ͨ�ö�ʱ��2���г�ʼ�����ò����PWM
*������    ��TIM2_PWM_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc,u16 ccr2
*����      ��ͨ��PWM������MOTOR����Ƶ��
*            PWM����� �� PB10   TIM2_CH3  ���ͨ��3
********************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc,u16 ccr3)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx���ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx���ýṹ��
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //ͨ�����ýṹ��
	
  //ʱ��ʹ��   GPIOB   TIM12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //ʹ��TIM2ʱ��
	
	/*GPIO������*/
	//PB10
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_10;              //ѡ��PB10
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����������
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //�������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//�ٶ�100MHz
	GPIO_Init(GPIOB,&GPIO_InitStruc);                   //�����趨������ʼ��PB15
	
	//TIM2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);  //PB10 ����ΪTIM2
	
	//TIMx����
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //�Զ���װ��ֵ(TIMx->ARR�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //��ʱ����Ƶ(TIMx->PSC�Ĵ���)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ(TIMx->CR1�Ĵ����ĵ�4λ)(ͨ�ö�ʱ��12ֻ�е�������)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ:tDTS=tCK_INT(TIMx->CR1�Ĵ����ĵ�8-9λ)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);            //��ʼ��TIM2
	
	//PWMģʽ����
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //����ΪPWMģʽ1(TIMx_CCMR1�Ĵ��������ģʽ�ĵ�4-6λ)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //ͨ��ѡ�����ģʽ(TIMx->CCER�Ĵ����ĵ�0λ)
	TIM_OCInitStruct.TIM_Pulse = ccr3;	                       //���ñȽϼĴ���(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //������Ч��ƽΪ�͵�ƽ(TIMx->CCER�Ĵ����ĵ�1λ)
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);                      //ͨ��3��ʼ������
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);          //ͨ��3�ȽϼĴ���Ӱ�ӼĴ���ʹ��
 
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM2, ENABLE);			                  // ʹ��TIM2���ؼĴ���ARR
	
	/*ʹ�ܶ�ʱ��2���� */
	TIM_Cmd(TIM2, ENABLE);   //ʹ�ܶ�ʱ��2
}










