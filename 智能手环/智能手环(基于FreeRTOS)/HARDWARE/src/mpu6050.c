#include "stm32f4xx.h"
#include "iic.h"
#include "mpu6050.h"
#include "OLED.h"
#include "stdio.h"

//��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}

/***************************************************************
*��������  ��ģ��IIC��������ĳ���ӻ�д����ֽ����ݵ�ĳ���Ĵ�����
*������    ��IIC_Write
*��������  ��u8 addr, u8 reg, u8 len, u8 * data
*��������ֵ��u8
*����      ��
*             addr :  ĳ���ӻ���������ַ
*             reg  :  �ӻ���ĳ���Ĵ�����ַ
***************************************************************/
u8 IIC_Write(u8 addr,u8 reg,u8 len,u8 * data)
{
  u8 ack;
  u8 i;
  
  //����������ʼ�ź�
  IIC_Start();
  //ע��
  //   I2C_Direction_Transmitter �� д
  //   I2C_Direction_Receiver    �� ��
  //��������������ַ(д)  
  IIC_Send_Byte((addr<<1)|I2C_Direction_Transmitter);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return 1;    //��1�׶δ��󣺷���������ַ����
  }
  //���ͼĴ�����ַ
  IIC_Send_Byte(reg);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return 2;    //��2�׶δ��󣺷��ͼĴ�����ַ����
  }
  //��ʼд������
  for (i = 0; i < len; i++)
  {
    IIC_Send_Byte(data[i]);
    //���մӻ�Ӧ���ź�
    ack = IIC_Receive_Ack();
    //�ж�Ӧ��
    if(ack != 0)
    {
      IIC_Stop();
      return 3;   //��3�׶δ��󣺷������ݹ��̴���
    }
  }
  //��������ֹͣ�ź�
  IIC_Stop();
  
  return 0;  //���д��
}

/******************************************************************
*��������  ��ģ��IIC��������ĳ���ӻ���ĳ���Ĵ������ȡ����ֽ�����
*������    ��IIC_Read
*��������  ��u8 addr, u8 reg, u8 len, u8 *buf
*��������ֵ��u8
*����      ��
*             addr :  ĳ���ӻ���������ַ
*             reg  :  �ӻ���ĳ���Ĵ�����ַ
*******************************************************************/
u8 IIC_Read(u8 addr,u8 reg,u8 len,u8 *buf)
{
  u8 ack;
  
  //����������ʼ�ź�
  IIC_Start();
  //ע��
  //   I2C_Direction_Transmitter �� д
  //   I2C_Direction_Receiver    �� ��
  //��������������ַ(д)  
  IIC_Send_Byte((addr<<1)|I2C_Direction_Transmitter);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return 1;    //��1�׶δ��󣺷���������ַ����
  }
  //���ͼĴ�����ַ
  IIC_Send_Byte(reg);
  //���մӻ�Ӧ���ź�
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return 2;    //��2�׶δ��󣺷��ͼĴ�����ַ����
  }
  //����������ʼ�ź�
  IIC_Start();
  //ע��
  //   I2C_Direction_Transmitter �� д
  //   I2C_Direction_Receiver    �� ��
  //��������������ַ(��)
  IIC_Send_Byte((addr<<1)|I2C_Direction_Receiver);
  //��������Ӧ��
  ack = IIC_Receive_Ack();
  //�ж�Ӧ��
  if(ack != 0)
  {
    IIC_Stop();
    return 3;    //��3�׶δ��󣺷���������ַ����
  }
  //��ʼ��ȡ����
  while (len)
  {
    *buf = IIC_Receive_Byte();
  
    if (len == 1)
        IIC_Send_Ack(1);   //��Ӧ��(������)
    else
        IIC_Send_Ack(0);   //Ӧ��  (��������)
    buf++;
    len--;
  }
  
  //��������ֹͣ�ź�
  IIC_Stop();
  
  return 0;   //��ȡ���
}

/***********************************************************************************
*��������  ����MPU6050���г�ʼ��
*������    ��MPU6050_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            ��ʼ�����裺
*                      1����ʼ�� IIC �ӿ�
*                      2����λ MPU6050
*                      3�����ý��ٶȴ������������ǣ��ͼ��ٶȴ������������̷�Χ
*                      4��������������
*                      5������ϵͳʱ��Դ��ʹ�ܽ��ٶȴ������ͼ��ٶȴ�����
*           
*           ��ַ��
*                MPU6050_ADDR     0x68     MPU6050������ַ 
*                SMPLRT_DIV       0x19	   �����ǲ����ʷ�Ƶ�Ĵ���
*                PWR_MGMT_1       0x6B	   ��Դ����Ĵ���1
*                CONFIG           0x1A	   ���üĴ���
*                GYRO_CONFIG      0x1B	   ���������üĴ���
*                ACCEL_CONFIG     0x1C	   ���ٶȴ��������üĴ���
***************************************************************************************/
void MPU6050_Init(void)
{
  //Ҫ��MPU6050�ڲ��Ĵ������õ�ֵ
  u8 buff[6] = {0x80,0x01,0x07,0x04,0x18,0x00};
  
  //IIC�ӿڳ�ʼ��
  IIC_Init();
  
  //��ʱ3ms������iic��·����(�ɲ���)
  delay_ms(3);
  
  //MPU6050��λ
  IIC_Write(MPU6050_ADDR,PWR_MGMT_1, 1,&buff[0]);   //��λ
  delay_ms(100);//��ʱ5ms������MPU6050��λδ���
  
  //MPU6050�ڲ��Ĵ�������
  IIC_Write(MPU6050_ADDR,PWR_MGMT_1, 1,&buff[1]);   //�������״̬  ʱ��Դ��PLL��ʹ�� X ��������Ϊ�ο�   һ�㶼ѡ�����
  IIC_Write(MPU6050_ADDR,SMPLRT_DIV, 1,&buff[2]);   //����Ƶ�� ��125Hz
  IIC_Write(MPU6050_ADDR,CONFIG, 1,&buff[3]);       //���ٶȴ�����������21Hz �ӳ�8.5ms Fs=1Khz �����ǣ�����20Hz  8.3ms Fs=1Khz 
  IIC_Write(MPU6050_ADDR,GYRO_CONFIG, 1,&buff[4]);  //�����ǵ������̷�Χ���� 2000�� /S
  IIC_Write(MPU6050_ADDR,ACCEL_CONFIG, 1,&buff[5]); //���ٶȴ������������̷�Χ���� 2g
}

/***********************************************************
*��������  ����MPU605���ĳ���Ĵ�����ȡһ��16λ�õ�����
*������    ��MPU6050_Receive_Byte
*��������  ��u8 REG_Address
*��������ֵ��short
*����      ��
*            ���������ר����Ի�ȡ���ٶȲ���ֵ�����ٶȲ���ֵ
*        ���¶Ȳ���ֵ(��ʵ��û��)�ģ���Ϊ����ֵ��λ�Ĵ����Ͳ�
*        ��ֵ��λ�Ĵ�����ַ�������ģ�֧�ּĴ�������
*
*        MPU6050_ADDR   0x68  MPU6050������ַ
***********************************************************/
short MPU6050_Receive_Byte(u8 REG_Address)
{
  u8 Measure[2];   //����װ����ֵ��λ�͵�λ��
  
  //��ȡ����ֵ��λ�͵�λ
  IIC_Read(MPU6050_ADDR,REG_Address,2,Measure);
  
  //��8λ������ϳ�16λ���ݷ���ȥ
  return ((Measure[0] << 8) | Measure[1]);
}


/***********************************************************
*��������  ����MPU605��Ļ�ȡ��ǰ�ļ��ٶ�
*������    ��MPU6050_Receive_Byte
*��������  ��u8 REG_Address
*��������ֵ��short
*����      ��
*        MPU6050_ADDR   0x68  MPU6050������ַ
*     	 ACCEL_XOUT_H	  0x3B  //X���ٶȲ���ֵ��λ�Ĵ���
*        ACCEL_XOUT_L	  0x3C  //X���ٶȲ���ֵ��λ�Ĵ���
*        ACCEL_YOUT_H	  0x3D  //Y���ٶȲ���ֵ��λ�Ĵ���
*        ACCEL_YOUT_L	  0x3E  //Y���ٶȲ���ֵ��λ�Ĵ���
*        ACCEL_ZOUT_H	  0x3F  //Z���ٶȲ���ֵ��λ�Ĵ���
*        ACCEL_ZOUT_L	  0x40  //Z���ٶȲ���ֵ��λ�Ĵ���
***********************************************************/
ADD_SPEED MPU6050_ReadSpeed(void)
{
	ADD_SPEED xyz_val;
	
	short x_data;
	short y_data;
	short z_data;
	
	/*X�᷽��*/
	x_data  = MPU6050_Receive_Byte(ACCEL_XOUT_H);
	/*Y�᷽��*/
	y_data  = MPU6050_Receive_Byte(ACCEL_YOUT_H);
	/*Z�᷽��*/
	z_data  = MPU6050_Receive_Byte(ACCEL_ZOUT_H);
	
	/*����ת���ɼ��ٶ�*/
	xyz_val.x_val = ((float)x_data / (65536/4)) * 10;
	xyz_val.y_val = ((float)y_data / (65536/4)) * 10;
	xyz_val.z_val = ((float)z_data / (65536/4)) * 10;
	
	return xyz_val;
}






