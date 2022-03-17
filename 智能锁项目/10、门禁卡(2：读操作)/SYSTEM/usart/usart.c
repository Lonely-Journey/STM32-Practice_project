#include "sys.h"
#include "usart.h"
#include "stdio.h"

/*****************************************************
*��������  ����USART1��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��USART1_Init
*��������  ��u32 baud
*��������ֵ��void
*����      ��
*           PA9     TX     ---------���
*           PA10    RX     ---------����
********************************************************/
void USART1_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
  USART_InitTypeDef USART_InitStruct;    //USARTx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(�ж�)���ýṹ��
  
  //ʱ��ʹ��   GPIOA   USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 
  
  /*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PA9   USART1_TX ----- ���ô������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;				     //PA9 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //����������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //�����趨������ʼ��PA9
  //PA10  USART1_RX ----- ���ô�������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;				     //PA10 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //��������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //�����趨������ʼ��PA10
  
  /*USART ��ʼ������*/
  //USART1����
  USART_InitStruct.USART_BaudRate      = baud;                 //����������(USART->BRR�Ĵ���)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ:һ����ʼλ�� 8������λ�� n��ֹͣλ��(USART->CR1�Ĵ����ĵ�12λ)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //һ��ֹͣλ(USART->CR2�Ĵ����ĵ�12��13λ)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //����żУ��(USART->CR1�Ĵ����ĵ�10λ)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������(USART->CR3�Ĵ����ĵ�9λ)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ(USART->CR1�Ĵ����ĵ�2��3λ)
  USART_Init(USART1,&USART_InitStruct);                        //��ʼ��USART1
	
	/*NVIC����*/
	//ѡ���жϷ�ʽ��ʹ�ܸ÷�ʽ�ж�
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //��ȡ���ݼĴ�����Ϊ��(�������)�ж� (USART->CR1�ĵ�5λ) 
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     //��⵽������·(����)�ж� (USART->CR1�ĵ�4λ)

  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;           //ѡ�񴮿�1�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct); //����1�жϳ�ʼ��
  
  //ʹ�ܴ���1
  USART_Cmd(USART1, ENABLE);
}

/*****************************************************
*��������  ������1����һ���ֽڵ����ݣ�u8��
*������    ��USART1_Send_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
*           PA9     TX     ---------���
*           PA10    RX     ---------����
********************************************************/
void USART1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//�ȴ����ͽ���(USART1->SR�ĵ�6λ)
	USART_SendData(USART1, data);                           //����һ���ֽڵ�����(USART1->DR)
}

/*****************************************************
*��������  ������1����һ���ֽڵ����ݣ�u8��
*������    ��USART1_Receive_Byte
*��������  ��void
*��������ֵ��u8
*����      ��
********************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=SET){};//�ȴ����ͽ���(USART1->SR�ĵ�5λ)
  data = USART_ReceiveData(USART1);                         //��ȡһ���ֽڵ�����(USART1->DR)
  
  return data;
}

/******************************************************************
*��������  ������1����һ���ַ���
*������    ��USART1_Send_Str
*��������  ��u8 *str
*��������ֵ��void
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
void USART1_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART1_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*��������  ������1����һ���ַ���
*������    ��USART1_Receive_Str
*��������  ��u8 *str
*��������ֵ��void 
*����      ��
            PA9   TX    ���
            PA10  RX    ����
*******************************************************************/
void USART1_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART1_Receive_Byte();
    if(data == '#')   //�� # Ϊ������־
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}


/*****************************************************
*��������  ����USART3��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��USART3_Init
*��������  ��u32 baud
*��������ֵ��void
*����      ��
*           PB10     TX     ---------���
*           PB11     RX     ---------����
********************************************************/
void USART3_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
  USART_InitTypeDef USART_InitStruct;    //USARTx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(�ж�)���ýṹ��
  
  //ʱ��ʹ��   GPIOB   USART3
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 
  
  /*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PB10   USART3_TX ----- ���ô������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;				     //PB10 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //����������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                  //�����趨������ʼ��PB10
  //PB11  USART3_RX ----- ���ô�������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;				     //PB11 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //��������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                  //�����趨������ʼ��PB11
  
  /*USART ��ʼ������*/
  //USART3����
  USART_InitStruct.USART_BaudRate      = baud;                 //����������(USART->BRR�Ĵ���)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ:һ����ʼλ�� 8������λ�� n��ֹͣλ��(USART->CR1�Ĵ����ĵ�12λ)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //һ��ֹͣλ(USART->CR2�Ĵ����ĵ�12��13λ)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //����żУ��(USART->CR1�Ĵ����ĵ�10λ)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������(USART->CR3�Ĵ����ĵ�9λ)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ(USART->CR1�Ĵ����ĵ�2��3λ)
  USART_Init(USART3,&USART_InitStruct);                        //��ʼ��USART3
	
	/*NVIC����*/
	//ѡ���жϷ�ʽ��ʹ�ܸ÷�ʽ�ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);     //��ȡ���ݼĴ�����Ϊ��(�������)�ж� (USART->CR1�ĵ�5λ) 
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);     //��⵽������·(����)�ж� (USART->CR1�ĵ�4λ)

  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;           //ѡ�񴮿�3�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct); //����3�жϳ�ʼ��
  
  //ʹ�ܴ���3
  USART_Cmd(USART3, ENABLE);
}

/*****************************************************
*��������  ������3����һ���ֽڵ����ݣ�u8��
*������    ��USART3_Send_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
*           PB10     TX     ---------���
*           PB11     RX     ---------����
********************************************************/
void USART3_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET);//�ȴ����ͽ���(USART3->SR�ĵ�6λ)
	USART_SendData(USART3, data);                           //����һ���ֽڵ�����(USART3->DR)
}

/*****************************************************
*��������  ������3����һ���ֽڵ����ݣ�u8��
*������    ��USART3_Receive_Byte
*��������  ��void
*��������ֵ��u8
*����      ��
********************************************************/
u8 USART3_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=SET){};//�ȴ����ͽ���(USART3->SR�ĵ�5λ)
  data = USART_ReceiveData(USART3);                         //��ȡһ���ֽڵ�����(USART3->DR)
  
  return data;
}

/******************************************************************
*��������  ������3����һ���ַ���
*������    ��USART3_Send_Str
*��������  ��u8 *str
*��������ֵ��void
*����      ��
            PB10   TX    ���
            PB11   RX    ����
*******************************************************************/
void USART3_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART3_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*��������  ������3����һ���ַ���
*������    ��USART3_Receive_Str
*��������  ��u8 *str
*��������ֵ��void 
*����      ��
            PB10   TX    ���
            PB11   RX    ����
*******************************************************************/
void USART3_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART3_Receive_Byte();
    if(data == '#')   //�� # Ϊ������־
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}

/*****************************************************
*��������  ����USART2��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��USART2_Init
*��������  ��u32 baud
*��������ֵ��void
*����      ��
*           PA2     TX     ---------���
*           PA3     RX     ---------����
********************************************************/
void USART2_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
  USART_InitTypeDef USART_InitStruct;    //USARTx���ýṹ��
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(�ж�)���ýṹ��
  
  //ʱ��ʹ��   GPIOA   USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
  
  /*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PA2   USART2_TX ----- ���ô������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;				     //PA2 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //����������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //�����趨������ʼ��PA9
  //PA3  USART2_RX ----- ���ô�������
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;				     //PA3 �˿�����
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //��������
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //�����趨������ʼ��PA10
  
  /*USART ��ʼ������*/
  //USART2����
  USART_InitStruct.USART_BaudRate      = baud;                 //����������(USART->BRR�Ĵ���)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ:һ����ʼλ�� 8������λ�� n��ֹͣλ��(USART->CR1�Ĵ����ĵ�12λ)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //һ��ֹͣλ(USART->CR2�Ĵ����ĵ�12��13λ)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //����żУ��(USART->CR1�Ĵ����ĵ�10λ)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������(USART->CR3�Ĵ����ĵ�9λ)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ(USART->CR1�Ĵ����ĵ�2��3λ)
  USART_Init(USART2,&USART_InitStruct);                        //��ʼ��USART1
	
	/*NVIC����*/
	//ѡ���жϷ�ʽ��ʹ�ܸ÷�ʽ�ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);     //��ȡ���ݼĴ�����Ϊ��(�������)�ж� (USART->CR1�ĵ�5λ) 
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);     //��⵽������·(����)�ж� (USART->CR1�ĵ�4λ)

  //�����ж�����
  NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;           //ѡ�񴮿�2�ж�
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;		       //�����ȼ�3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStruct); //����2�жϳ�ʼ��
  
  //ʹ�ܴ���2
  USART_Cmd(USART2, ENABLE);
}

/*****************************************************
*��������  ������2����һ���ֽڵ����ݣ�u8��
*������    ��USART2_Send_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
*           PA2     TX     ---------���
*           PA3    RX     ---------����
********************************************************/
void USART2_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);//�ȴ����ͽ���(USART2->SR�ĵ�6λ)
	USART_SendData(USART2, data);                           //����һ���ֽڵ�����(USART2->DR)
}

/*****************************************************
*��������  ������2����һ���ֽڵ����ݣ�u8��
*������    ��USART2_Receive_Byte
*��������  ��void
*��������ֵ��u8
*����      ��
********************************************************/
u8 USART2_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=SET){};//�ȴ����ͽ���(USART2->SR�ĵ�5λ)
  data = USART_ReceiveData(USART2);                         //��ȡһ���ֽڵ�����(USART2->DR)
  
  return data;
}

/******************************************************************
*��������  ������2����һ���ַ���
*������    ��USART2_Send_Str
*��������  ��u8 *str
*��������ֵ��void
*����      ��
            PA2   TX    ���
            PA3  RX    ����
*******************************************************************/
void USART2_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART2_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*��������  ������2����һ���ַ���
*������    ��USART2_Receive_Str
*��������  ��u8 *str
*��������ֵ��void 
*����      ��
            PA2   TX    ���
            PA3   RX    ����
*******************************************************************/
void USART2_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART2_Receive_Byte();
    if(data == '#')   //�� # Ϊ������־
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}

























/**********************************printf����*****************************************/
#if 1
	#pragma import(__use_no_semihosting)             
	//��׼����Ҫ��֧�ֺ���                 
	struct __FILE 
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}; 
	/* FILE is typedef�� d in stdio.h. */ 
	FILE __stdout;       
	//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
  _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//�ض���fputc���� 
	int fputc(int ch, FILE *f)
	{      
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
		USART1->DR = (u8) ch;      
		return ch;
	}
	#endif 
/***********************************************************************************/








