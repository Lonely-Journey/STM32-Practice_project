#include "sys.h"
#include "motor.h"

/*****************************************************
*��������  ���Բ��������Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Motor_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��
	
	/*ʧ��JTAG PB3��PB4*/
  //AFIOʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //��ӳ��SWJ,��JTAGʧ�ܣ�ʹPB3��PB4��Ϊ��ͨGPIO��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
  //GPIOxʱ��ʹ��  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//GPIOx����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	  //PB3/4/5/6�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		                        //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		                        //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                              //�����趨������ʼ��PB3/4/5/6
}



/*****************************************************
*��������  �����Ƶ��ת��(��ת����ֽ)
*������    ��Motor_Run
*��������  ��void
*��������ֵ��void
*����      ��
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Run(void)
{
	static u8 cont = 0;
	switch(cont)
	{
		//��һ������
		case 0: M_IN1_H;  M_IN2_L;  M_IN3_L;  M_IN4_H; break;
		//�ڶ�������
		case 1: M_IN1_L;  M_IN2_H;  M_IN3_L;  M_IN4_H; break;
		//����������
		case 2: M_IN1_L;  M_IN2_H;  M_IN3_H;  M_IN4_L; break;  
		//���ĸ�����
		case 3: M_IN1_H;  M_IN2_L;  M_IN3_H;  M_IN4_L; break;     
	}
	cont++;
	if(cont >= 4) cont = 0;
}

/*****************************************************
*��������  �����Ƶ��ת��(��ת����ֽ)
*������    ��Motor_Run
*��������  ��void
*��������ֵ��void
*����      ��
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Run2(void)
{
	static u8 cont = 0;
	switch(cont)
	{
		//��һ������
		case 0: M_IN1_H;  M_IN2_L;  M_IN3_H;  M_IN4_L; break; 
		//�ڶ�������
		case 1: M_IN1_L;  M_IN2_H;  M_IN3_H;  M_IN4_L; break; 
		//����������
		case 2: M_IN1_L;  M_IN2_H;  M_IN3_L;  M_IN4_H; break;
		//���ĸ�����
		case 3: M_IN1_H;  M_IN2_L;  M_IN3_L;  M_IN4_H; break;    
	}
	cont++;
	if(cont >= 4) cont = 0;
}



