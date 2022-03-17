#include "sys.h"
#include "BS8116A.h"
#include "iic.h"
#include "stdio.h"

//�����ʱ��������BS8116Aæ�׶��õ�
void BS81XXWait(void)
{
   u8 t = 150;
   while(t--);
}

/***************************************************************
*��������  ����BS81xx����Ҫ��GPIO�ڽ��г�ʼ������(���ⲿ�ж�)
*������    ��BS81xx_Init1
*��������  ��void
*��������ֵ��void
*����      ��
*          ʱ����  IIC_SCL  PB6   �������
*          ������  IIC_SDA  PB7   ��©���
*          ������  TK_IRQ   PB8   ��������    ���Լ����ⲿ�ж�
*****************************************************************/
void BS81xx_Init1(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx���ýṹ��
	
	/*GPIOx����*/
  //GPIOxʱ��ʹ��  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //GPIOx����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	       //PB8�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��PB8
	//����
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);                //����͵�ƽ����
	
	IIC_Init();
}

/***************************************************************
*��������  ��ͨ��IICͨ�Ŷ�ȡBS8116A������Ĵ���KeyStatus��ֵ
*������    ��BS81xx_ReadKey
*��������  ��u8 KeyStatus_arrd    //BS8116A������Ĵ���KeyStatus��ַ
*��������ֵ��u8
*����      ��
*          ʱ����  IIC_SCL  PB6   �������
*          ������  IIC_SDA  PB7   ��©���
*          ������  TK_IRQ   PB8   ��������    ���Լ����ⲿ�ж�
*
*          1       BS81XX_ERR1        //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*          2       BS81XX_ERR2        //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*          0xa0    BS81XX_Addr_Write  //������ַ+д���� 
*          0xa1    BS81XX_Addr_Read   //������ַ+������
*
*   �ر�ע�⣡����
*      		  �����IICͨ����һ���ر�ÿ�η���8bit���ݸ�BS8116Aʱ��BS8116A
*    		�����Լ�����ʱ���ߣ�������������æ�����Ǳ������æ���˲��ܼ�������
*    		һ��8bit�����ݣ�����ᷢ��ͨ��ʧ��(�ײ��)������ֱ�Ӹ���С��ʱ����
*    		�����æ�׶�(��ֱ��д��һ��С��ʱ����BS81XXWait())
*
*   �����Ҫ�˽⣬�������Լ��������ϣ�����BS8116A��ؼ���֧�����Ͻ���
*********************************************************************/
u8 BS81xx_ReadKey(u8 KeyStatus_arrd,u8 *data)
{
	u8 ack=0;
	
	//����������ʼ�ź�
  IIC_Start();
	//��������������ַ(д)
  IIC_Send_Byte(BS81XX_Addr_Write);
	//���մӻ�Ӧ���ź�
	ack = IIC_Receive_Ack();
	//�ж�Ӧ��
	if(ack != 0)
	{
		IIC_Stop();
		return BS81XX_ERR1;
	}
	BS81XXWait();
	//��������Ĵ���KeyStatus��ַ(�ֵ�ַ)
  IIC_Send_Byte(KeyStatus_arrd);
	//���մӻ�Ӧ���ź�
	ack = IIC_Receive_Ack();
	//�ж�Ӧ��
	if(ack != 0)
	{
		IIC_Stop();
		return BS81XX_ERR2;
	}
	BS81XXWait();
	//����������ʼ�ź�
  IIC_Start();
  //��������������ַ(��)
  IIC_Send_Byte(BS81XX_Addr_Read);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return BS81XX_ERR1;
  }
	BS81XXWait();
	//������ʼ��������
  *data = IIC_Receive_Byte();
  //��������Ӧ���ź�    //��Ӧ���ź�
  IIC_Send_Ack(1);
  //��������ֹͣ�ź�
  IIC_Stop();
  
  return ack;
}

/***************************************************************
*��������  ����BS8116A������Ĵ���KeyStatus��ֵ���а���ֵת��
*������    ��BS81xx_Key
*��������  ��void
*��������ֵ��u8
*����      ��
*          ʱ����  IIC_SCL  PB6   �������
*          ������  IIC_SDA  PB7   ��©���
*          ������  TK_IRQ   PB8   ��������    ���Լ����ⲿ�ж�
*
*          1       BS81XX_ERR1        //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*          2       BS81XX_ERR2        //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*          0xa0    BS81XX_Addr_Write  //������ַ+д���� 
*          0xa1    BS81XX_Addr_Read   //������ַ+������
*
*          0x08    BS81XX_KeyStatus0  //BS8116A������Ĵ���KeyStatus0��ַ
*          0x09    BS81XX_KeyStatus1  //BS8116A������Ĵ���KeyStatus1��ַ
*********************************************************************/
u8 BS81xx_Key(void)
{
	u8 data_h,data_l;
	static u8 key_flay = 1;
	u16 key = 0;
	u8 temp = 0;
	
	if(key_flay && (!TK_IRQ))
	{
		//��ȡ����Ĵ���KeyStatusֵ(ÿһ��λ����һ������)
		BS81xx_ReadKey(BS81XX_KeyStatus1,&data_h);
		BS81xx_ReadKey(BS81XX_KeyStatus0,&data_l);
		
		//�ϳ�16λ����
		key = ((u16)data_h<<8) + (u16)data_l;
		
		key_flay = 0;
		
		switch(key)
		{
			case 0x0001: temp = '1'; break;
			case 0x0002: temp = '2'; break;
			case 0x0004: temp = '3'; break;
			case 0x0008: temp = '4'; break;
			case 0x0010: temp = '5'; break;
			case 0x0020: temp = '6'; break;
			case 0x0040: temp = '7'; break;
			case 0x0080: temp = '8'; break;
			case 0x0100: temp = '9'; break;
			case 0x0200: temp = '*'; break;
			case 0x0400: temp = '0'; break;
			case 0x0800: temp = '#'; break;
		}
	}
	if((!key_flay)&& TK_IRQ) key_flay = 1;
	
	return temp;
}






