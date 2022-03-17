#ifndef _MG200_H
#define _MG200_H

	#include "sys.h"
	
	//MCU����MG200ָ��ģ������ݰ��ṹ��
	typedef struct MG200
	{
		u8 StartCode;   //��ʼ��
		u8 SendAddr;    //���͵�ַ
		u8 ReceiveAddr; //���յ�ַ
		u8 AckCode;     //Ӧ����
		u8 Result;      //����ֵ
		u8 Parameter;   //����
		u8 Reserve;     //Ԥ���ֽ�
		u8 Checksum;    //У���
		u8 End;         //������ɱ��
	}MG200DATA;
	
	//FING_PWR  
	#define   MG200_ON       GPIO_SetBits(GPIOB,GPIO_Pin_9)     //��
	#define   MG200_OFF      GPIO_ResetBits(GPIOB,GPIO_Pin_9)   //��
	//FING_DETECT
	#define   MG200_DETECT   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)    //�����ж�FING_DETECT�ĵ�ƽ״̬
	
	//���ݰ�ĳЩ�̶���ַ
	#define   STARTCODE      0x6C      //���ݰ���ʼ��
	#define   SENDADDR       0x63      //���ݰ����͵�ַ
	#define   RECEIVEADDR    0x62      //���ݰ����յ�ַ
	#define   RESERVE        0x00      //Ԥ���ֽ�,Ϊ�Ժ�MG200���̼��������õģ�������Ĭ��0����
	
	//�������ݰ�������ʾ
	#define  DATA_OK             0x0   //���ݰ�û������
	#define  STARTCODE_ERROR     0x1   //���ݰ�����ʼ�����
	#define  SENDADDR_ERROR      0x2   //���ݰ��ķ��͵�ַ����
	#define  RECEIVEADDR_ERROR   0x3   //���ݰ��Ľ��յ�ַ����
	#define  ACKCODE_ERROR       0x4   //���ݰ���Ӧ�����
	#define  CHECKSUM_ERROR      0x5   //���ݰ���У��ʹ���
	
	#define  EXTRACT_OK          0x0   //ָ�Ʋ����ɹ�
	#define  EXTRACT_ERROR       0x1   //ָ�Ʋ���ʧ��
	
	#define  ENROLL_OK           0x0    //ָ�ƵǼǳɹ�
	#define  ENROLL_ERROR        0x1    //ָ�ƵǼ�ʧ��
	
	#define  Match1Nl_ERROR      0x0    //ָ�Ʊȶ�ʧ��
	
	extern MG200DATA MG200data;
	
	void MG200_Init(void);
	void MG200_SendCmd(u8 cmd ,u8 parameter);
	u8 MG200_ReceiveAck(u8 cmd,u8 *result,u8 *parameter);
	u8 MG200_ExtractFingerprint(u8 parameter);
	u8 MG200_Enroll(void);
	u8 MG200_Match1Nl(void);
	void MG200_EraseAll(void);
	void MG200_EraseOne(u8 ID);
	u8 MG200_GetUserNum(void);
	
#endif





