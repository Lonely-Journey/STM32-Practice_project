#include "sys.h"
#include "motor.h"
#include "delay.h"

/*****************************************************
*��������  ����motor�����Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Motor_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            LOCK+     PA4
*            LOCK-     PA5   
********************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	       //ʹ��PA�˿�ʱ��
	
	//GPIO����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				     //PA4/5 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                 //�����趨������ʼ��PA4/5
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


/*****************************************************
*��������  ��motor�����ת��תʵ�ֿ��ź͹���
*������    ��Motor_DoorSwitch
*��������  ��u8 Switch
*��������ֵ��void
*����      ��
*            LOCK+     PA4
*            LOCK-     PA5   
*         
*           MySwitch==0  ����
*           MySwitch==1  ����
********************************************************/
void Motor_DoorSwitch(u8 MySwitch)
{
	switch(MySwitch)
	{
		case 0:LOCK1_L; LOCK2_H; break;   //��
		case 1:LOCK1_H; LOCK2_L; break;   //��
	}
	delay_ms(250);
	LOCK1_L;
  LOCK2_L;
}


/*****************************************************
*��������  ��ģ���Ž�����
*������    ��Motor_DoorOpen
*��������  ��u8 Switch
*��������ֵ��void
*����      ��
*            LOCK+     PA4
*            LOCK-     PA5   
*         
*           MySwitch==0  ����
*           MySwitch==1  ����
********************************************************/
void Motor_DoorOpen(u8 s)
{
  Motor_DoorSwitch(1);
	delay_ms(s*1000);
	Motor_DoorSwitch(0);
}











