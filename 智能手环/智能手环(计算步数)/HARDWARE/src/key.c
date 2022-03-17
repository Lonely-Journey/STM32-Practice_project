#include "stm32f4xx.h" 
#include "key.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}



/*****************************************************
*��������  ����KEY��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��KEY_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           KEY  PA0    �ߵ�ƽ����
*                  ����ģʽ
*                  ��������
********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;    //GPIOx���ýṹ��
  
  //ʱ��ʹ��  GPIOA  GPIOE 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//PA0
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PA0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;     //ͨ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //��������
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);             //�����趨������ʼ��PA0
}

/*****************************************************
*��������  ������ɨ�躯��
*������    ��KEY_Scan
*��������  ��void
*��������ֵ��u8
*����      ��
*           KEY1  PA0    �ߵ�ƽ����
*                  ����ģʽ
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay&&KEY)
  {
    delay_ms(10);
    if(KEY)
    {
      key_flay = 0;
      temp = 1;
    }
  }
  
  if(!KEY)
  {
    key_flay = 1;
  }
	
  return temp;
}





