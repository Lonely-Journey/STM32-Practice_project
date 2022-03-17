#ifndef _BS8116A_H
#define _BS8116A_H

	#include "sys.h"

	//BS8116A������ַ
	#define   BS81XX_Addr_Write   0xa0       //BS8116A��������ַ    д����
	#define   BS81XX_Addr_Read    0xa1       //BS8116A��������ַ    ������
	
	//�ڲ�����    ��
	#define   BS81XX_KeyStatus0   0x08       //BS8116A������Ĵ���KeyStatus0��ַ
	#define   BS81XX_KeyStatus1   0x09       //BS8116A������Ĵ���KeyStatus1��ַ
	
	//�ڲ�����    д
	//һ������ʹ��Ĭ��ֵ����ȥ����������
	//ע�⣺������BS8116Aд������ʱ�������0xb0��ʼ����д��22�������ֽڣ����һ���ֽ��Ǽ���͡�
	//ǰ�����ַ�����ݱ���Ĭ��(�鿴�ֲᷢĬ��ֵ)
	#define   K1_TH               0xb5      //Key1��������ֵ����
	#define   K2_TH               0xb6      //Key2��������ֵ����
	#define   K3_TH               0xb7      //Key3��������ֵ����
	#define   K4_TH               0xb8      //Key4��������ֵ����
	#define   K5_TH               0xb9      //Key5��������ֵ����
	#define   K6_TH               0xba      //Key6��������ֵ����
	#define   K7_TH               0xbb      //Key7��������ֵ����
	#define   K8_TH               0xbc      //Key8��������ֵ����
	#define   K9_TH               0xbd      //Key9��������ֵ����
	#define   K10_TH              0xbe      //Key10��������ֵ����
	#define   K11_TH              0xbf      //Key11��������ֵ����
	#define   K12_TH              0xf0      //Key12��������ֵ����
	#define   K13_TH              0xf1      //Key13��������ֵ����
	#define   K14_TH              0xf2      //Key14��������ֵ����
	#define   K15_TH              0xf3      //Key15��������ֵ����
	#define   K16_TH              0xf4      //Key16��������ֵ����
	
	//IICͨ�Ŵ����ʶ
	#define   BS81XX_ERR1        1      //������ַ���ݴ���ʧ��
	#define   BS81XX_ERR2        2      //�ֽڵ�ַ���ݴ���ʧ��
	
	#define   TK_IRQ       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)    //�����ж�TK_IRQ�ĵ�ƽ״̬
	
	void BS81xx_Init1(void);
	u8 BS81xx_ReadKey(u8 KeyStatus_arrd,u8 *data);
	u8 BS81xx_Key(void);
	
#endif













