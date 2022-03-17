#include "sys.h"
#include "voic.h"
#include "delay.h"

/*****************************************************
*��������  ����voic����ģ���Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Voic_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            VO_BUSY     PB1   //����
*            VO_DATA     PB0   //���
********************************************************/
void Voic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB�˿�ʱ��
	
	//GPIO����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;				               //PB0 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PB0
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				               //PB1 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		       //��������
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PB0
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

/*****************************************************
*��������  ����voic����ģ�鷢������λ1
*������    ��Voic_SendHight
*��������  ��void
*��������ֵ��void
*����      ��
*            VO_BUSY     PB1   //����
*            VO_DATA     PB0   //���
********************************************************/
void Voic_SendHight(void)
{
	VO_DATA_H;
	delay_us(1500);
	VO_DATA_L;
	delay_us(500);
}

/*****************************************************
*��������  ����voic����ģ�鷢������λ0
*������    ��Voic_SendLow
*��������  ��void
*��������ֵ��void
*����      ��
*            VO_BUSY     PB1   //����
*            VO_DATA     PB0   //���
********************************************************/
void Voic_SendLow(void)
{
	VO_DATA_H;
	delay_us(500);
	VO_DATA_L;
	delay_us(1500);
}

/*****************************************************
*��������  ����voic����ģ�鷢������
*������    ��Voic_SendData
*��������  ��u8 data
*��������ֵ��void
*����      ��
*            VO_BUSY     PB1   //����
*            VO_DATA     PB0   //���
********************************************************/
void Voic_SendData(u8 data)
{
	u8 i;
	
	VO_DATA_H;
	delay_ms(8);
	VO_DATA_L;
	delay_ms(1);
	
	for(i=0;i<8;i++)
	{
		if(data & 0x80) Voic_SendHight();
		else  Voic_SendLow();
		
		data<<=1;
	}
}




