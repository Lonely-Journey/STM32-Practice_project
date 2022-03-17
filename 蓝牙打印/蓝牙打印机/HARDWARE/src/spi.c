#include "sys.h"
#include "stdio.h"

/******************************************************************
*��������  ����SPI1ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI1�ļĴ�����������
*������    ��SPI1_Init
*��������  ��void
*��������ֵ��void
*����      ��
*           PA5   SPI1_SCK   ʱ����  ��������
*           PA6   SPI1_MISO  ������  ��������
*           PA7   SPI1_MOSI  �����  ��������
*******************************************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	SPI_InitTypeDef   SPI_InitStruct;      //SPIx���ýṹ��
	
	//ʱ��ʹ��   GPIOA   SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PA5��PA6��PA7
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PA5��PA6��PA7 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	             //����������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		               //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                              //�����趨������ʼ��PA5��PA6��PA7
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);            //�˿�����Ĵ��������(��������)
	
	/*SPI1�Ĵ�����ʼ������*/
	SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;   //SPI����Ϊ˫��˫��ȫ˫��,��NSS�ϵĵ�ƽΪ�ߵ�ƽ(SPI_CR1�Ĵ����ĵ�10��15λ)
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   //����Ϊ��SPI(SPI_CR1�Ĵ����ĵ�2��8λ)
	SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   //ʹ��8λ����֡��ʽ���з���/����(SPI_CR1�Ĵ����ĵ�11λ)
	SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      //����״̬ʱ�� SCK���ֵ͵�ƽ(SPI_CR1�Ĵ����ĵ�1λ)
	SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    //���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ(SPI_CR1�Ĵ����ĵ�0λ)
	SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      //����������豸����NSS�����ϵĵ�ƽ��SSIλ��ֵ����(SPI_CR1�Ĵ����ĵ�9λ)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;           //������Ԥ��ƵֵΪ4  (SPI_CR1�Ĵ����ĵ�3-5λ)
	SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ(�����׼:�����)(SPI_CR1�Ĵ����ĵ�7λ)
	SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 //CRCֵ����Ķ���ʽ:0x7�Ǹ�λֵ(SPI_CRCPR�Ĵ���)
	SPI_Init(SPI1, &SPI_InitStruct);                                          //��ʼ��SPI1
	
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����(SPI_CR1�Ĵ����ĵ�6λ)
}


/******************************************************************
*��������  ��SPI1����/����һ���ֽ����ݣ�8λ��
*������    ��SPI1_RS_Byte
*��������  ��u8 data
*��������ֵ��u8
*����      ��
*           PA5   SPI1_SCK   ʱ����
*           PA6   SPI1_MISO  ������
*           PA7   SPI1_MOSI  �����
*******************************************************************/
u8 SPI1_RS_Byte(u8 data)
{
  //�жϷ��ͻ������Ƿ�Ϊ��(SPI->SR�Ĵ����ĵ�1λ)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
	//��������
	SPI_I2S_SendData(SPI1,data);
  //�жϽ��ջ������Ƿ�Ϊ��(SPI->SR�Ĵ����ĵ�0λ)
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
	//��������
  data = SPI_I2S_ReceiveData(SPI1);
  
  return data;
}







