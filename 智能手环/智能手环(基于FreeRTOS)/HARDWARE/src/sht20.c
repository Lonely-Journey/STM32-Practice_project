#include "stm32f4xx.h"
#include "iic.h"
#include "sht20.h"
#include "math.h"

//��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}

/*****************************************************
*��������  ������ʪ�ȴ��������и�λ
*������    ��SHT20_RST
*��������  ��void
*��������ֵ��u8
*����      ��
********************************************************/
u8 SHT20_RST(void)
{
	u8 ack;
	
	//��ʼ�ź�
	IIC_Start();
	//����д��������
	IIC_Send_Byte(SHT20_WRITE);
	//�ȴ�Ӧ��
	ack = IIC_Receive_Ack();
	//���Ͳ���ָ��ʧ��
	if(ack == 1)
	{
		//ֹͣ�ź�
		IIC_Stop();
		return 1;
	}
	//���������λ����
	IIC_Send_Byte(SHT20_RST_CMD);
	//�ȴ�Ӧ��
	ack = IIC_Receive_Ack();
	//���͸�λָ��ʧ��
	if(ack == 1)
	{
		//ֹͣ�ź�
		IIC_Stop();
		return 2;
	}
	
	//ֹͣ�ź�
	IIC_Stop();
	
	//��λ�ɹ�
	return 0;
}

/*****************************************************
*��������  ������ʪ�ȴ��������г�ʼ��
*������    ��SHT20_Init
*��������  ��void
*��������ֵ��u8
*����      ��
********************************************************/
u8 SHT20_Init(void)
{
	//IIC���ų�ʼ��
	IIC_Init();
	
	//SHT20�����λ
	return SHT20_RST();
}

/*****************************************************
*��������  ����ȡSHT20���¶Ȼ�ʪ��ֵ
*������    ��SHT20_RH_T
*��������  ��u8 cmd
*��������ֵ��u16
*����      ��
********************************************************/
u16 SHT20_RH_T(u8 cmd)
{
	u8 ack;
	u8 val_1,val_2;
	u16 data;
	
	//��ʼ�ź�
	IIC_Start();
	//����д��������
	IIC_Send_Byte(SHT20_WRITE);
	//�ȴ�Ӧ��
	ack = IIC_Receive_Ack();
	//���Ͳ���ָ��ʧ��
	if(ack == 1)
	{
		//ֹͣ�ź�
		IIC_Stop();
		return 1;
	}
	//���Ͳ����¶Ȼ�ʪ������
	IIC_Send_Byte(cmd);
	//�ȴ�Ӧ��
	ack = IIC_Receive_Ack();
	//���Ͳ���ָ��ʧ��
	if(ack == 1)
	{
		//ֹͣ�ź�
		IIC_Stop();
		return 1;
	}
	
	//�ȴ����ݲ������
	delay_ms(85);       //����ʱ��ȥ����
	//��ʼ�ź�
	IIC_Start();
	//����д��������
	IIC_Send_Byte(SHT20_READ);
	//�ȴ�Ӧ��
	while(IIC_Receive_Ack());    //ֱ���������
	
	//��ȡ��������
	val_1 = IIC_Receive_Byte();      //���ո߰�λ
	IIC_Send_Ack(0);                 //��������
  val_2 = IIC_Receive_Byte(); 	   //���յͰ�λ
	IIC_Send_Ack(1);                 //���ٽ���
	
	//ֹͣ�ź�
	IIC_Stop();
	
	data = (val_1<<8)|val_2;
	
	return data;
}

/*****************************************************
*��������  ����ȡSHT20���¶Ⱥ�ʪ��ֵ
*������    ��SHT20_RH_T_data
*��������  ��viod
*��������ֵ��THR
*����      ��
********************************************************/
THR SHT20_RH_T_data(void)
{
	u16 T_val;
	u16 RH_val;
	THR trh_val;
	
	//ִ�л�ȡ�¶�
	T_val = SHT20_RH_T(SHT20_T_CMD);	
	//ִ�л�ȡʪ��
	RH_val = SHT20_RH_T(SHT20_RH_CMD);
	
	//ת�������ֵ
	//�¶ȣ�
	trh_val.t = -46.85 + 175.72 * ( T_val / pow(2,16));
	//ʪ�ȣ�
	trh_val.rh = -6 + 125 * (RH_val / pow(2,16));
	
	return trh_val;
}









 


