#include "stm32f4xx.h"  

/*****************************************************
*��������  ����LED��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��LED_Init
*��������  ��void
*��������ֵ��void
*����      ��
*             LED ------ PA7
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
********************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruc;
  
  //CPIOx��ʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  /*GPIOx�˿�����*/
  //PA7
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_7;         //ѡ������7
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //ͨ�����
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_2MHz;    //�ٶ�Ϊ2MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);
  
  /*����������ݼĴ���*/
  //�Ƚ�LED���ر�
  GPIO_SetBits(GPIOA,GPIO_Pin_7);
}





