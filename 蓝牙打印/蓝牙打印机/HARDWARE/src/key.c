#include "sys.h"
#include "delay.h"
#include "key.h"

/*****************************************************
*��������  ����KEY��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��KEY_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            KEY     PB8     �͵�ƽ���£��ߵ�ƽ̧��
*                                 -------------���ģʽ
********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx���ýṹ��
	
	/*GPIOx����*/
  //GPIOxʱ��ʹ��  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  //GPIOx����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	       //PB8�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	 //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��PB8  
}


/*****************************************************
*��������  ������ɨ�躯��
*������    ��KEY_Scan
*��������  ��void
*��������ֵ��u8
*����      ��
*            KEY      PB8     �͵�ƽ���£��ߵ�ƽ̧��
*                                 -------------���ģʽ
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay && (!KEY))
  {
    delay_ms(10);
    if(!KEY)
    {
      key_flay = 0;
			temp = 1;
    }
  }
	
  if(KEY) key_flay = 1;
	
  return temp;
}












