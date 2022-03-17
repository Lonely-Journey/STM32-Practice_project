#include "sys.h"
#include "usart.h"
#include "MG200.h"
#include "stdio.h"
#include "string.h"

//���ڽ���MG200ָ��ģ�鴫���������ݵı���
MG200DATA MG200data;

/*****************************************************
*��������  ����MG200ָ��ģ���Ӧ��GPIO�ڽ��г�ʼ������
*������    ��MG200_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           FING_PWR    -------   PB9    ָ��ģ��Ŀ���
*           FING_DETECT -------   PC13   �鿴������ָ����
********************************************************/
void MG200_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx���ýṹ��
	
	//ʱ��ʹ��   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	       //ʹ��PB�˿�ʱ��
	
	//GPIO����
	//PB9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;          				     //PB9 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //�����趨������ʼ��PB9 
	//PC13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;          				     //PC13 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 		               //��������
	GPIO_Init(GPIOC, &GPIO_InitStruct);					                 //�����趨������ʼ��PB9 

	//PB9����͵�ƽ    �ȹر�
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);                            //PB9 ����͵�ƽ
	//��������
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);                           //PC13 ����͵�ƽ
	
	//����2��ʼ��
	USART2_Init(115200);
}

/***********************************************************************
*��������  ����MG200ָ��ģ�鷢��ָ��
*������    ��MG200_SendCmd
*��������  ��u8 cmd  �� u8 parameter       //ָ����   ����
*��������ֵ��void
*����      ��
*           ���ݰ��ṹ����ʼ��+���͵�ַ+���յ�ַ+ָ����+����+Ԥ��+У���
*
*           STARTCODE      0x6C      //���ݰ���ʼ��
*           SENDADDR       0x63      //���ݰ����͵�ַ
*           RECEIVEADDR    0x62      //���ݰ����յ�ַ
*           RESERVE        0x00      //Ԥ���ֽ�,Ϊ�Ժ�MG200���̼��������õģ�������Ĭ��0����
*************************************************************************/
void MG200_SendCmd(u8 cmd ,u8 parameter)
{
	u8 checksum;    //У��ͱ���
	
	//����� = ����ʼ������������ݺͣ������ֵ����1���ֽڵĴ�С��ֻȡǰ8λ
	checksum = (SENDADDR + RECEIVEADDR + cmd + parameter + RESERVE) & 0xff;
	
	//���ս��ձ�־��1
	MG200data.End = 1;
	
	USART2_Send_Byte(STARTCODE);    //������ʼ��
	USART2_Send_Byte(SENDADDR);     //���ͷ��͵�ַ
	USART2_Send_Byte(RECEIVEADDR);  //���ͽ��յ�ַ
	USART2_Send_Byte(cmd);          //����ָ����
	USART2_Send_Byte(parameter);    //���Ͳ���
	USART2_Send_Byte(RESERVE);      //����Ԥ��λ
	USART2_Send_Byte(checksum);     //����У���

}

/***********************************************************************
*��������  ������MG200�����������ݰ�
*������    ��MG200_ReceiveAck
*��������  ��u8 cmd �� u8* result �� u8* parameter    //ָ����   ����ֵ   ����
*��������ֵ��u8 
*����      ��
*           DATA_OK             0x0   ���ݰ�û������
*						STARTCODE_ERROR     0x1   ���ݰ�����ʼ�����
*						SENDADDR_ERROR      0x2   ���ݰ��ķ��͵�ַ����
*						RECEIVEADDR_ERROR   0x3   ���ݰ��Ľ��յ�ַ����
*           ACKCODE_ERROR       0x4   ���ݰ���Ӧ�����
*						CHECKSUM_ERROR      0x5   ���ݰ���У��ʹ���
*************************************************************************/
u8 MG200_ReceiveAck(u8 cmd,u8 *result,u8 *parameter)
{
	u8 checksum;    //У��ͱ���
	
	while(MG200data.End);   //�ȴ��������
	
	//У�����ݰ��Ƿ���ȷ
	if(MG200data.StartCode != STARTCODE) return STARTCODE_ERROR;      //���ݰ�����ʼ��
	if(MG200data.SendAddr != 0x62)       return SENDADDR_ERROR;       //���ݰ��ķ��͵�ַУ��
	if(MG200data.ReceiveAddr != 0x63)    return RECEIVEADDR_ERROR;    //���ݰ��Ľ��յ�ַУ��
	if(MG200data.AckCode != cmd)         return ACKCODE_ERROR;        //���ݰ�Ӧ����У��  (Ӧ����==ָ����)
	checksum = (MG200data.SendAddr+MG200data.ReceiveAddr+MG200data.AckCode+MG200data.Result+MG200data.Parameter+MG200data.Reserve) & 0xff;
	if(MG200data.Checksum != checksum)   return CHECKSUM_ERROR;       //���ݰ�У���У��
	
	//ȷ�����ݰ�û���������ֻ���ȡ���ݰ���ķ���ֵ�Ͳ���������
	*result = MG200data.Result;
	*parameter = MG200data.Parameter;
	
	//�ṹ������
	memset(&MG200data,0,sizeof(MG200data));
	
	return DATA_OK;
}

/*****************************************************************************
*��������  ��ʹ��0x51ָ������MG200ָ��ģ����ץȡָ��ͼ��ͬʱ��ȡ������
*������    ��MG200_ExtractFingerprint
*��������  ��u8 cmd �� u8* result �� u8* parameter    //ָ����   ����ֵ   ����
*��������ֵ��u8 
*����      ��
*             EXTRACT_OK          0x0   //ָ�Ʋ����ɹ�
*             EXTRACT_ERROR       0x1   //ָ�Ʋ���ʧ��
*********************************************************************************/
u8 MG200_ExtractFingerprint(u8 parameter)
{
	u8 result,ackparam,ack;
	
	//����0x51ָ���ȡָ��
	MG200_SendCmd(0x51,parameter);
	//����MG200�����ݰ�
	ack = MG200_ReceiveAck(0x51,&result,&ackparam);
	if(ack != 0) return EXTRACT_ERROR;
	/*********************************************************
	*�����Ӧ��ţ�
	*		00h : ץȡָ��ͼ����������ȡ�ɹ�
	*		B1h : ָ�ƹ�С(small finger)
	*		B2h : ��ָ�� (no finger)
	*		B3h : ָ��λ�ù��ڿ�����Ҫ����ָ���ұ�������left finger��
	*		B4h : ָ��λ�ù��ڿ��ң���Ҫ����ָ�����������right finger��
	*		B5h : ָ��λ�ù��ڿ��ϣ���Ҫ����ָ���±�������up finger��
	*		B6h : ָ��λ�ù��ڿ��£���Ҫ����ָ���ϱ����� (down finger��
	*		B7h : ʪ��ָ ��ָ�Ʊ����������ˮ�չ��ࣩ (wet finger)
	*		B8h : ������ָ(dry finger)
	*		C0h: ������ȡ����
	*		C2h: ������ȡ���� (ͼ��������)
	***********************************************************/
	switch(result)
	{
		case 0x00 : printf("ץȡָ��ͼ����������ȡ�ɹ�\r\n"); return EXTRACT_OK;
		case 0xB1 : printf("small finger               \r\n"); return EXTRACT_ERROR;
		case 0xB2 : printf("no finger                  \r\n"); return EXTRACT_ERROR;
		case 0xB3 : printf("left finger                \r\n"); return EXTRACT_ERROR;
		case 0xB4 : printf("right finger               \r\n"); return EXTRACT_ERROR;
		case 0xB5 : printf("up finger                  \r\n"); return EXTRACT_ERROR;
		case 0xB6 : printf("down finger                \r\n"); return EXTRACT_ERROR;
		case 0xB7 : printf("wet finger                 \r\n"); return EXTRACT_ERROR;
		case 0xB8 : printf("dry finger                 \r\n"); return EXTRACT_ERROR;
		case 0xC0 : printf("������ȡ����                \r\n"); return EXTRACT_ERROR;
		case 0xC2 : printf("������ȡ���� (ͼ��������)   \r\n"); return EXTRACT_ERROR;
		default   : printf("ץȡָ��ͼ����������ȡ����\r\n");  return EXTRACT_ERROR; 
	}
}

/*****************************************************************************
*��������  ��ʹ��0x7Fָ������MG200ָ��ģ����û�ָ�ƽ��еǼ�
*������    ��MG200_Enroll
*��������  ��void
*��������ֵ��u8 
*����      ��
*          ENROLL_OK          0x0      ָ�ƵǼǳɹ�
*          ENROLL_ERROR       0x1      ָ�ƵǼ�ʧ��
*********************************************************************************/
u8 MG200_Enroll(void)
{
	u8 result,ackparam,ack;
	
	//�ɼ�ָ����Ϣ�����ǲ������β���ָ������
	/**********************��һ��ָ��¼��**********************/
	printf("��һ��¼�룬�������ָ\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x00);
	while(ack != 0)
	{
		printf("����¼��....\r\n");
		//��ָ������̧��
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x00);
	}
	//�ȴ�̧����ָ
	while(MG200_DETECT);
	/**********************�ڶ���ָ��¼��**********************/
	printf("�ڶ���¼�룬�������ָ\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x01);
	while(ack != 0)
	{
		printf("����¼��....\r\n");
		//��ָ������̧��
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x01);
	}
	//�ȴ�̧����ָ
	while(MG200_DETECT);
	/**********************�ڶ���ָ��¼��**********************/
	printf("������¼�룬�������ָ\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x02);
	while(ack != 0)
	{
		printf("����¼��....\r\n");
		//��ָ������̧��
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x02);
	}
	//�ȴ�̧����ָ
	while(MG200_DETECT);
	
	//����ָ����Ϣ���еǼ�
	MG200_SendCmd(0x7F,0x00);
	//���շ��ص����ݰ�
	ack = MG200_ReceiveAck(0x7F,&result,&ackparam);
	if(ack != 0) 
	{
		printf("���ݰ�������...\r\n");
		return ENROLL_ERROR;
	}
	
	switch(result)
	{
		case 0x00:printf("ע��ɹ�                                       \r\n"); return ENROLL_OK;
		case 0x83:printf("ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���\r\n"); return ENROLL_ERROR;
		case 0x91:printf("ע��ʧ��(�û���������)                          \r\n"); return ENROLL_ERROR;
		case 0x93:printf("�Ѿ�ע��� ID                                  \r\n"); return ENROLL_ERROR;
		case 0x94:printf("ָ����ȡ���� < 3                               \r\n"); return ENROLL_ERROR;
		default  : return ENROLL_ERROR; 
	}
}

/*****************************************************************************
*��������  ��ʹ��0x71ָ������MG200ָ��ģ����е�ǰָ����ģ���ڴ洢���û�ָ�ƽ��бȶ�
*������    ��MG200_Match1Nl
*��������  ��void
*��������ֵ��u8 
*����      ��
*               Match1Nl_ERROR       0x0       ָ�Ʊȶ�ʧ��
*********************************************************************************/
u8 MG200_Match1Nl(void)
{
	u8 result,ackparam,ack;
	
	printf("�������ָ\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x00);
	while(ack != 0)
	{
		printf("���°���ָ....\r\n");
		//��ָ������̧��
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x00);
	}
	//�ȴ���ָ̧��
	while(MG200_DETECT);
	
	//��ʼ��ָ����Ϣ���бȶ�
	MG200_SendCmd(0x71,0x00);
	//���շ��ص����ݰ�
	ack = MG200_ReceiveAck(0x71,&result,&ackparam);
	if(ack != 0) 
	{
		printf("���ݰ�������...\r\n");
		return Match1Nl_ERROR;
	}

	switch(result)
	{
		case 0x00: printf("�ȶԳɹ�  \r\n"); return ackparam;         //����ID��
		case 0x92: printf("û�и�ָ��\r\n"); return Match1Nl_ERROR;   //�ȶ�ʧ��break;
		default:   printf("ʧ��      \r\n"); return Match1Nl_ERROR;   //�ȶ�ʧ��
	}
}

/*****************************************************************************
*��������  ��ʹ��0x54ָ����ɾ��MG200ָ��ģ���������û�ָ����Ϣ
*������    ��MG200_EraseAll
*��������  ��void
*��������ֵ��void  
*����      ��
*********************************************************************************/
void MG200_EraseAll(void)
{
	u8 result,ackparam,ack;
	
	//��ʼɾ��������Ϣ
	MG200_SendCmd(0x54,0x00);
	//���շ��ص����ݰ�
	ack = MG200_ReceiveAck(0x54,&result,&ackparam);
	if(ack != 0) 
	{
		printf("���ݰ�������...\r\n");
		return;
	}
	
	switch(result)
	{
		case 0x00:printf("������Ϣɾ���ɹ�\r\n");break;
		case 0x90:printf("ɾ��ʧ�ܣ�ע������Ϊ 0��\r\n");break;
		default:printf("ɾ��ʧ��\r\n");break;
	}
	
}

/*****************************************************************************
*��������  ��ʹ��0x73ָ����ɾ��MG200ָ��ģ����ɾ��ָ���û���ָ����Ϣ
*������    ��MG200_EraseOne
*��������  ��u8 ID
*��������ֵ��void  
*����      ��
*********************************************************************************/
void MG200_EraseOne(u8 ID)
{
	u8 result,ackparam,ack;
	
	MG200_SendCmd(0x73,ID);
	//���շ��ص����ݰ�
	ack = MG200_ReceiveAck(0x73,&result,&ackparam);
	if(ack != 0) 
	{
		printf("���ݰ�������...\r\n");
		return;
	}
	switch(result)
	{
		case 0x00:printf("ɾ���ɹ�    \r\n");break;
		case 0x83:printf("��������    \r\n");break;
		case 0x90:printf("δע����û�\r\n");break;
		case 0xFF:printf("δע����û�\r\n");break;
    default:printf("ɾ��ʧ��      \r\n");break;
	}
}

/*****************************************************************************
*��������  ��ʹ��0x55ָ����ɾ��MG200ָ��ģ����ɾ��ָ���û���ָ����Ϣ
*������    ��MG200_GetUserNum
*��������  ��u8 ID
*��������ֵ��void  
*����      ��
*********************************************************************************/
u8 MG200_GetUserNum(void)
{
	u8 result,ackparam,ack;
	
	MG200_SendCmd(0x55,0x00);
	//���շ��ص����ݰ�
	ack = MG200_ReceiveAck(0x55,&result,&ackparam);
	if(ack != 0) 
	{
		printf("���ݰ�������...\r\n");
		return 0;
	}
	
	return ackparam;
}





