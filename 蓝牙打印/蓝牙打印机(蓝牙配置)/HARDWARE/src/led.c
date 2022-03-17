//#include "sys.h"
#include "stm32f10x.h" 



/*****************************************************
*��������  ����LED��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��LED_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            LED     PA1
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				     //PA1 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					       //�����趨������ʼ��PA1
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						         //PA1 �����
}




