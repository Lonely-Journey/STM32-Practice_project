#include "sys.h"
#include "printer.h"
#include "motor.h"

u8 p_flag = 1;//��ӡˢ�����ݱ�־����

/*****************************************************
*��������  ���Դ�ӡͷ��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Printer_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

	//GPIOA
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	//PA8/11/12 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		              //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		              //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                    //�����趨������ʼ��PA8/11/12
	
	//GPIOB
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //PB10/11/12/13/14/15 �˿�����
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                    //�����趨������ʼ��PB10/11/12/13/14/15
}


/*****************************************************
*��������  �����ӡͷ����8λ����
*������    ��Printer_Write_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Write_Byte(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(data & 0x80) DI_H;
		else DI_L;
		CLK_H;
		data <<= 1;
	}
}

/*****************************************************
*��������  ���Դ�ӡͷ����߼���
*������    ��Printer_Left
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Left(void)
{
	STB1_H ;
	STB2_H ;
	STB3_H ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*��������  ���Դ�ӡͷ���ұ߼���
*������    ��Printer_Right
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Right(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_H ;
	STB5_H ;
	STB6_H ;
}

/*****************************************************
*��������  ���رմ�ӡͷ����
*������    ��Printer_OFF
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_OFF(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*��������  ����ӡ����
*������    ��Printer_Stripe
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Stripe(void)
{
	u8 i =0;
	for(i=0; i<48;i++)
	{
		Printer_Write_Byte(0xf0);
	}
}

/**************************************************************
*��������  ����ӡһ��ͼƬ
*������    ��Printer_Picture
*��������  ��u8 *tu
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_Picture(u8 *tu)
{
	u8 i;
	static u16 line = 0;
	
	if(p_flag == 1)//�ж�����������ݱ�־λ //��λ�����ж�����1����ʼֵ����Ϊ1��������һ�����ݾ���������ʱ��ǰ׼������
	{
		p_flag = 0;   //����������ݱ�־λ ��0           //Ϊ����һ�����ж�����1
		for (i=0; i<48; i++)  //дͼƬ��һ������ // 48�ֽ�
		{
			Printer_Write_Byte(*(tu+i+line*48));//ͼƬȡģ���򣺴����ң����ϵ��£�һ��48�ֽڣ�����ÿһ������ƫ��48
		}
		line++;
	}
	
	if (line >= 473)  //ͼƬ��473 �к�0~472    ����Ҫ��472�д�ӡ�����Ե�473ʱ�ر�
	{
		line = 0; //�к����㣬��һ�δ�ӡ��0�п�ʼ
		MOTOR_OFF;
		TIM_Cmd(TIM3, DISABLE);
		Printer_OFF();
	}
}

/**************************************************************
*��������  ����ӡһ��24*24�ַ���
*������    ��Printer_ShowStr
*��������  ��u8 *str
*��������ֵ��void
*����      ��
*            
****************************************************************/













