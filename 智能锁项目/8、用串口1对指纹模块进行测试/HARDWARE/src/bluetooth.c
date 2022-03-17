#include "sys.h"

/*****************************************************
*��������  ����������Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Bluetooth_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           BLUE_PWRC    PB12     1����  	    0����
*     			BLUE_KEY		 PA8			1������	ģʽ  0������ͨ��
********************************************************/
void Bluetooth_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	       //ʹ��PA�˿�ʱ��

	//PB12
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;				               //PB12 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PB12
	//PA8
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;				               //PA8 �˿�����
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                 //�����趨������ʼ��PA8
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);						                 //PB1 �����
}















