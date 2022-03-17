#include "sys.h"

/*****************************************************
*��������  ����LED��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��LED_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            LED1     PB3
*            LED2     PB2   
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx���ýṹ��
	
	/*ʧ��JTAG PB3��PB4*/
  //AFIOʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //��ӳ��SWJ,��JTAGʧ�ܣ�ʹPB3��PB4��Ϊ��ͨGPIO��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//ʱ��ʹ��   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB�˿�ʱ��
	
	//GPIO����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;				     //PB2/3 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PA1

	//GPIO����ߵ�ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_2);						       //PB2/3 �����
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}




