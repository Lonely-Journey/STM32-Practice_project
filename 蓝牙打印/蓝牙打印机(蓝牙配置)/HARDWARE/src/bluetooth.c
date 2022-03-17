#include "sys.h"

/*****************************************************
*��������  ����������Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Bluetooth_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           BLUE_PWR    PB1      1����  	    0����
*     			BLUE_KEY		PB9			 1������	ģʽ  0������ͨ��
********************************************************/
void Bluetooth_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB�˿�ʱ��

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_9;				     //PB1��PB9 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PB1��PB9
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);						                 //PB1 �����
}















