#include "stm32f4xx.h"

/*****************************************************
*��������  ����MOTOR��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��MOTOR_Init
*��������  ��void
*��������ֵ��void
*����      ��
*             MOTOR ------ PB10
*            �ߵ粻�𣬵͵���-----���ģʽ
********************************************************/
void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;
  
  //CPIOx��ʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  /*GPIOx�˿�����*/
  //PA7
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_10;         //ѡ������7
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_2MHz;    //�ٶ�Ϊ2MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);
  
  /*����������ݼĴ���*/
  //�Ƚ�MOTOR���ر�
  GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}





