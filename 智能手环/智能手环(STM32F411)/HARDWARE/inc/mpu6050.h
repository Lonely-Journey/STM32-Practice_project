#ifndef _MPU6050_H
#define _MPU6050_H

  #include "stm32f4xx.h"
	
	typedef struct
	{
		float	x_val;
		float y_val;
		float z_val;
	}ADD_SPEED;

	//MPU6050������ַ���ڲ��Ĵ�����ַ
  //��֪����Ĵ�����ַ���ѯ�ֲ᣺<<MPU-6000 & MPU-6050 �Ĵ�����������(���İ�)>>
  #define  MPU6050_ADDR   0x68  //MPU6050������ַ
  #define	 SMPLRT_DIV		  0x19	//�����ǲ����ʷ�Ƶ�Ĵ����������ǲ����ʣ�����ֵ��0x07(125Hz)
  #define	 CONFIG			    0x1A	//���üĴ�������ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
  #define  FIFO_EN        0x23  // FIFO ʹ�ܼĴ�����һ��С���̲���
  #define	 GYRO_CONFIG		0x1B	//���������üĴ������������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
  #define	 ACCEL_CONFIG	  0x1C	//���ٶȴ��������üĴ��������ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
  #define	 ACCEL_XOUT_H	  0x3B  //X���ٶȲ���ֵ��λ�Ĵ���
  #define	 ACCEL_XOUT_L	  0x3C  //X���ٶȲ���ֵ��λ�Ĵ���
  #define  ACCEL_YOUT_H	  0x3D  //Y���ٶȲ���ֵ��λ�Ĵ���
  #define	 ACCEL_YOUT_L	  0x3E  //Y���ٶȲ���ֵ��λ�Ĵ���
  #define	 ACCEL_ZOUT_H	  0x3F  //Z���ٶȲ���ֵ��λ�Ĵ���
  #define	 ACCEL_ZOUT_L	  0x40  //Z���ٶȲ���ֵ��λ�Ĵ���
  #define	 TEMP_OUT_H		  0x41  //�¶Ȳ���ֵ��λ�Ĵ���
  #define  TEMP_OUT_L		  0x42  //�¶Ȳ���ֵ��λ�Ĵ���
  #define	 GYRO_XOUT_H		0x43  //X���ٶȲ���ֵ��λ�Ĵ���
  #define	 GYRO_XOUT_L		0x44	//X���ٶȲ���ֵ��λ�Ĵ���
  #define	 GYRO_YOUT_H		0x45  //Y���ٶȲ���ֵ��λ�Ĵ���
  #define	 GYRO_YOUT_L		0x46  //Y���ٶȲ���ֵ��λ�Ĵ���
  #define	 GYRO_ZOUT_H		0x47  //Z���ٶȲ���ֵ��λ�Ĵ���
  #define	 GYRO_ZOUT_L		0x48  //Z���ٶȲ���ֵ��λ�Ĵ���
  #define	 PWR_MGMT_1		  0x6B	//��Դ����Ĵ���1����Դ��������ֵ��0x00(��������)
  #define	 WHO_AM_I		    0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
	
	u8 IIC_Write(u8 addr,u8 reg,u8 len,u8 * data);
	u8 IIC_Read(u8 addr,u8 reg,u8 len,u8 *buf);
	void MPU6050_Init(void);
	short MPU6050_Receive_Byte(u8 REG_Address);
	ADD_SPEED MPU6050_ReadSpeed(void);
	
#endif





