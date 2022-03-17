#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"
#include "BS8116A.h"
#include "MG200.h"
#include "spi.h"
#include "MFRC522.h"

/**********************************************
**RCC522��������
**RCC522��ʼ��
**Ѱ��
**if(Ѱ�� == yes) ����ײ
**ѡ��
**��֤��Ƭ
**����Ƭ/д��Ƭ
**������Ƭ����
**********************************************/
u8 CardId[4];  				//����
u8 CardType[2];//������
unsigned char RFID[16];			//���RFID 
u8 writeDataBuf[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0,1,2,3,4,0xff,0xff,0xff}; //д�������

unsigned char total=0; 
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //������-��ʼ����--�׿��ĳ�������--
u8 MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //����1������ ��ʽ(A���� ����λ  B���� )
u8 MIMA_2[16]={88,88,88,88,88,88};//����1��A����


void print_info(unsigned char *p,int cnt)
{
  int i;
  for(i=0;i<cnt;i++)
  {
		if(!i)
			printf("0x%02X ",p[i]);
		else
			printf("%02X ",p[i]);
  }
  printf("\r\n");
}

/**********************************************
**�������ܣ�Ѱ��---������ײ ---��ѡ��

**����˵��: 
pTagType[OUT]����Ƭ���ʹ���(���������)
**                0x4400 = Mifare_UltraLight
**                0x0400 = Mifare_One(S50)
**                0x0200 = Mifare_One(S70)
**                0x0800 = Mifare_Pro(X)
**                0x4403 = Mifare_DESFire
pSnr[OUT]:��Ƭ���кţ�4�ֽ�(���������)

**����ֵ���ɹ�����MI_OK
**���ߣ�weihao Mo
**********************************************/
u8 Identificate_Card(u8 *pTagType,u8 *pSnr)
{
  u8 status=MI_ERR;
	u8 STA=0;

   if(MFRC_PcdRequest(PICC_REQALL,pTagType)==MI_OK)//Ѱ���ɹ�
   {
      STA |=0x01;
      if(MFRC_PcdAnticoll(pSnr)==MI_OK)/*����ײ�ɹ�*/
      {
				STA |=0x02;
		
        if(MFRC_PcdSelect(pSnr)==MI_OK)//ѡ���ɹ�
        {
					STA |=0x04;
        }
      }
   }
	 if(STA==0x07)
		 status=MI_OK;
	 
  return status;
}

/*******************************************************************************************************************
*���ܣ������ݵ�ָ����
*������
*u8   auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
*u8   addr      �����ݴ�ŵĵ�ַ��ÿ��������0��1��2���Ǵ�����ݡ�3�Ǵ�����롣
*һ���0��1��2 ��4��5��6��8��9��10
*u8 *Src_Key    ��ԭ���� 
*u8 *data       ������������
*u8 *pSnr       ������
*
*����һ���ʽ��u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //д��Ľ�
*********************************************************************************************************************/
u8 ReadCardData(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *data,u8 *pSnr)
{
  int status;
	
  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr);  
  /*2. ��֤������*/
  if(status==MI_OK)
  {
		printf("������:");
		print_info(CardType,2);//��ӡ����
		printf("����:");
		print_info(pSnr,4);//��ӡ����		
    status=MFRC_PcdAuthState(auth_mode,addr/4*4+3,KEY,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
    printf("��֤������ɹ�\r\n");
  }
  /*3. ��������*/
  if(status==MI_OK)
  {
    status=MFRC_PcdRead(addr,data); //�ӵ�addr���������ֵ��
  }
  if(status==MI_OK)
  {
    printf("���ݶ����ɹ�!\r\n");
  }
  return status;
}

/*********************************************************************************************************************
*���ܣ�д���ݵ�ָ����
*������
*			u8   auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
*			u8   addr      �����ݴ�ŵĵ�ַ��ÿ��������0��1��2���Ǵ�����ݡ�3�Ǵ�����롣
*			һ���0��1��2 ��4��5��6��8��9��10----  (���Ե�ַ��)
*			u8 *Src_Key    ��ԭ���� 
*			u8 *data       ����д�������
*			u8 *pSnr       ������
*
*����һ���ʽ��u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //д��Ľ�
*****************************************************************************************************************************/
u8 WriteCardData(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *data,u8 *pSnr)
{
  int status=MI_ERR;

  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. ��֤������*/
  if(status==MI_OK)
  {
    status =MFRC_PcdAuthState(0x60,addr/4*4+3,KEY,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
  }
  if(status==MI_OK)
  {
    printf("������֤�ɹ�!\r\n");
  }
  /*3. д���ݵ���*/
  if(status==MI_OK)
  {
    status = MFRC_PcdWrite(addr,data); //д���ݵ���addr�飬data�������ֵ��
  }
  if(status==MI_OK)
  {
    printf("����д��OK!\r\n");
  }
  return status;
}

/**********************************************
*�������ܣ��޸Ŀ�����
*
*����˵��: 
* 		u8   auth_mode ����֤��������͡�����ΪPICC_AUTHENT1A��PICC_AUTHENT1B��
*			u8   addr      �������ŵĵ�ַ�����������ÿ�������ĵ�3���顣��һ�������Ƚ����⡣��
*			һ���3��7��11.....����
*			u8 *Src_Key    ��ԭ���� 
*			u8 *New_Key    ��������
*			u8 *pSnr       ������
*
*			�����ʽ��
*			u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //��֤A����B����ĸ�ʽ
*			u8 MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //�޸�������ĸ�ʽ(A���� ����λ  B���� )
*
*			����ֵ���ɹ�����MI_OK
**********************************************/
int Card_Passworld(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *New_Key,u8 *pSnr)
{
  int status;

  /*1. Ѱ��--����ײ--ѡ��*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. ��֤������*/
  if(status==MI_OK)
  {
    status = MFRC_PcdAuthState(auth_mode,addr,Src_Key,pSnr);   //��֤��Ƭ���� �ββ�������֤��ʽ�����ַ�����룬�����к�
  }
  /*3. д���ݵ���*/
  if(status==MI_OK)
  {
    status = MFRC_PcdWrite(addr,New_Key); //д���ݵ���addr�飬New_Keyд�������ֵ��
  }
  return status;
}

int main(void)
{		
	u8 readDataBuf[16];	
	
	SysTick_Init();	  	 //��ʱ��ʼ��
	USART1_Init(115200);
	LED_Init();
	SPI2_Init();
	MFRC_Init();
	
	while(1)
	{
		delay_ms(1000);
		//ע�⣺��0����û���޸ĵ�
		if(ReadCardData(PICC_AUTHENT1A,10,KEY,readDataBuf,CardId)==MI_OK)
		print_info(readDataBuf,16);
	}
}




