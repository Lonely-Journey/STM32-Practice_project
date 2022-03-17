#include "MFRC522.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
#include "string.h"

void delay_spi(void)
{
	u8 t=5;
	while(t--);
}

/*************************************************************************
*��������  ����ʼ��MFRC��Ӧ��GPIO��
*������    ��MFRC522_IO_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            SPI2_MOSI ------- PB15  �����������
*            SPI2_MISO ------- PB14  ������������
*            SPI2_SCLK ------- PB13  ʱ����
*            RC_RES    ------- PA7   ��λ 
*            RC_CS     ------- PA6  Ƭѡѡ��
***************************************************************************/
void MFRC522_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx���ýṹ��
  
  //GPIOxʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	     //ʹ��PA�˿�ʱ��
	
	//PA�˿�
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;                 //FMRC522  PA6/7 �˿�����
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 		             //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                   //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					                       //�����趨������ʼ��PA6/7
	
	//Ƭѡ������
	GPIO_SetBits(GPIOA, GPIO_Pin_6); 
	GPIO_SetBits(GPIOA, GPIO_Pin_7); 
}

/**********************************************************************
*��������  ����MFRC522��ĳ���Ĵ�������д����
*������    ��MFRC_Write
*��������  ��u8 addr ,u8 data  
*��������ֵ��u8
*����      ��
*            ��������ѡ����    MFRC_DC_L   MFRC_DC_H
*            ʱ����            MFRC_CLK_L  MFRC_CLK_H
*            Ӳ��λMFRC       MFRC_RES_L   MFRC_RES_H
*            �����������      MFRC_MOSI_L  MFRC_MOSI_H
************************************************************************/
void MFRC_Write(u8 addr,u8 data)
{
	u8 ucAddr;

  //Ƭѡ������
	MFRC_CS_L;
	
	//��ַ��ʽ����7�ֽڣ���д�ж��ֽ�  +  ��6-1�ֽڣ���ַ  +  ��8�ֽڣ�0
	ucAddr = ((addr<<1)&0x7E);
	
	//����Ҫ�޸ĵļĴ�����ַ
	SPI2_RS_Byte(ucAddr);
	//�޸�ֵ
	SPI2_RS_Byte(data);
	
	//Ƭѡ������
	MFRC_CS_H;
}

/**********************************************************************
*��������  ����MFRC522��ĳ���Ĵ�������д����
*������    ��MFRC_Read
*��������  ��u8 addr ,u8 data  
*��������ֵ��u8
*����      ��
*            ��������ѡ����    MFRC_DC_L   MFRC_DC_H
*            ʱ����            MFRC_CLK_L  MFRC_CLK_H
*            Ӳ��λMFRC       MFRC_RES_L   MFRC_RES_H
*            �����������      MFRC_MOSI_L  MFRC_MOSI_H
************************************************************************/
u8 MFRC_Read(u8 addr)
{
	u8   ucAddr;
	u8   ucResult=0;
	
	//Ƭѡ������
	MFRC_CS_L;
	
	//��ַ��ʽ����7�ֽڣ���д�ж��ֽ�  +  ��6-1�ֽڣ���ַ  +  ��8�ֽڣ�0
	ucAddr = ((addr<<1)&0x7E)|0x80;

	SPI2_RS_Byte(ucAddr);
	ucResult=SPI2_RS_Byte(0);
	
	//Ƭѡ������
	MFRC_CS_H;
	
	return ucResult;
}

/**********************************************************************
*��������  ����MFRC522���и�λ
*������    ��MFRC_RES
*��������  ��void  
*��������ֵ��void
*����      ��
*             MFRC_RES_L      //��ʼ��λ
*             MFRC_RES_H      //������λ
************************************************************************/
u8 MFRC_RES(void)
{
	//��MFRC522���и�λ
	MFRC_RES_H ;
	delay_us(2);
	MFRC_RES_L;
	delay_us(2);
	MFRC_RES_H ;
	delay_us(2);
	
	//����MFRC522�ڲ��ļĴ���
	MFRC_Write(CommandReg,PCD_RESETPHASE);
	MFRC_Write(CommandReg,PCD_RESETPHASE);
	delay_us(2);
	
	MFRC_Write(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	MFRC_Write(TReloadRegL,30);           
	MFRC_Write(TReloadRegH,0);
	MFRC_Write(TModeReg,0x8D);
	MFRC_Write(TPrescalerReg,0x3E);
	
	MFRC_Write(TxAutoReg,0x40);//����Ҫ
	
	return MI_OK;
}

/**********************************************************************
*��������  �����MFRC522��ĳ���Ĵ�����λ
*������    ��MFRC_ClearBitMask
*��������  ��u8 reg,u8 mask    //Ҫ����ļĴ�����ַ     Ҫ���λ(����������Ҫ���λ��1) 
*��������ֵ��void
*����      ��
************************************************************************/
void MFRC_ClearBitMask(u8 reg,u8 mask) 
{
    char   tmp = 0x0;
    tmp = MFRC_Read(reg);
    MFRC_Write(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************************
*��������  ����1MFRC522��ĳ���Ĵ�����λ
*������    ��MFRC_SetBitMask
*��������  ��u8 reg,u8 mask    //Ҫ����ļĴ�����ַ     Ҫ���λ(����������Ҫ���λ��1) 
*��������ֵ��void
*����      ��
************************************************************************/
void MFRC_SetBitMask(u8 reg,u8 mask) 
{
    u8 tmp = 0x0;
    tmp = MFRC_Read(reg);
    MFRC_Write(reg, tmp|mask);  
}

/**********************************************************************
*��������  ����MFRC522�����߽��йر�
*������    ��MFRC_PcdAntennaOff
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
void MFRC_PcdAntennaOff(void)
{
	MFRC_ClearBitMask(TxControlReg, 0x03);
}

/**********************************************************************
*��������  ����MFRC522�����߽��п���
*������    ��MFRC_PcdAntennaON
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
void MFRC_PcdAntennaON(void)
{
	u8 i;
	
	i = MFRC_Read(TxControlReg);
	if (!(i & 0x03))
	{
			MFRC_SetBitMask(TxControlReg, 0x03);
	}
}

/**********************************************************************
*��������  ����MFRC522�Ĺ���ģʽ��������
*������    ��MFRC_M500PcdConfigISOType
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_M500PcdConfigISOType(u8 type)
{
   if (type == 'A')                     //ISO14443_A
   { 
			MFRC_ClearBitMask(Status2Reg,0x08);
			MFRC_Write(ModeReg,0x3D);//3F
			MFRC_Write(RxSelReg,0x86);//84
			MFRC_Write(RFCfgReg,0x7F);   //4F
			MFRC_Write(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			MFRC_Write(TReloadRegH,0);
			MFRC_Write(TModeReg,0x8D);
			MFRC_Write(TPrescalerReg,0x3E);
			delay_us(2);
			MFRC_PcdAntennaON();
   }
   else 
		  return 1;
   
   return 0;
}


/**********************************************************************
*��������  ����MFRC522�Ľ��г�ʼ��
*������    ��MFRC_Init
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
void MFRC_Init(void)
{
	MFRC522_IO_Init();
	MFRC_RES();             //��MFRC522���и�λ����
	MFRC_PcdAntennaOff();   //�ر�����
	MFRC_PcdAntennaON();    //��������
	MFRC_M500PcdConfigISOType('A');   //���ù���ģʽ
} 

/**********************************************************************
*��������  ��MFRC522��ISO14443��ͨѶ
*������    ��MFRC_PcdComMF522
*��������  ��
            Command[IN]     :RC522������
					  pIn [IN]        :ͨ��RC522���͵���Ƭ������
						InLenByte[IN]   :�������ݵ��ֽڳ���
						pOut [OUT]      :���յ��Ŀ�Ƭ��������
					  *pOutLenBit[OUT]:�������ݵ�λ���� 
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdComMF522(u8 Command, u8 *pIn ,u8 InLenByte,u8 *pOut ,u8 *pOutLenBit)
{
	u8 status = MI_ERR;   //ͨ���������
	u8 irqEn   = 0x00;
	u8 waitFor = 0x00;
	u8 lastBits;
	u8 n;
	u16 i;
	
	//�ж�ͨ����Ϊ����֤��Կ�����շ�����
	switch (Command)
	{
		//��֤��Կ
		case PCD_AUTHENT:   
										irqEn   = 0x12;
										waitFor = 0x10;
										break;
		//�շ�����
		case PCD_TRANSCEIVE:
										irqEn   = 0x77;
										waitFor = 0x30;
										break;
		default:	break;
	}
	
	MFRC_Write(ComIEnReg,irqEn|0x80);
	MFRC_ClearBitMask(ComIrqReg,0x80);	    //�������ж�λ
	MFRC_Write(CommandReg,PCD_IDLE);
	MFRC_SetBitMask(FIFOLevelReg,0x80);	 	  //��FIFO����
	
	//׼����Ҫ��SO14443��ͨ�ŵ�����
	for (i=0; i<InLenByte; i++) MFRC_Write(FIFODataReg, pIn [i]); 
	
  MFRC_Write(CommandReg, Command);

	//�ж��Ƿ��Ƿ��Ͳ���������ָ��
	if (Command == PCD_TRANSCEIVE) MFRC_SetBitMask(BitFramingReg,0x80); //��ʼ����
	
	i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	
	do 
	{
			n = MFRC_Read(ComIrqReg);
			i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
	MFRC_ClearBitMask(BitFramingReg,0x80);
	
	if (i!=0)
	{    
		if(!(MFRC_Read(ErrorReg)&0x1B))
		{
				status = MI_OK;
				if (n & irqEn & 0x01)  status = MI_NOTAGERR;
				if (Command == PCD_TRANSCEIVE)
				{
						n = MFRC_Read(FIFOLevelReg);
						lastBits = MFRC_Read(ControlReg) & 0x07;
						if (lastBits)  *pOutLenBit = (n-1)*8 + lastBits;  
						else  *pOutLenBit = n*8;
						if (n == 0)  n = 1;
						if (n > MAXRLEN) n = MAXRLEN;
						//��ȡ����
						for (i=0; i<n; i++) pOut[i] = MFRC_Read(FIFODataReg);
				}
		}
		else
			 status = MI_ERR;
	}
	
	MFRC_SetBitMask(ControlReg,0x80);           // stop timer now
	MFRC_Write(CommandReg,PCD_IDLE); 
	return status;
}



/**********************************************************************
*��������  ��MFRC522����Ѱ������
*������    ��MFRC_PcdRequest
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdRequest(u8 req_code,u8 *pTagType)
{
	u8 status; 
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 
	
	MFRC_ClearBitMask(Status2Reg,0x08);
	MFRC_Write(BitFramingReg,0x07);
	MFRC_SetBitMask(TxControlReg,0x03);
	
	ucComMF522Buf[0] = req_code;
	
	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
     status = MI_ERR;  
   
	return status;
}

/**********************************************************************
*��������  ��MFRC522���з���ײ����
*������    ��MFRC_PcdAnticoll
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdAnticoll(u8 *pSnr)
{
	u8 status;
	u8 i,snr_check=0;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 
    

  MFRC_ClearBitMask(Status2Reg,0x08);
  MFRC_Write(BitFramingReg,0x00);
  MFRC_ClearBitMask(CollReg,0x80);
 
	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

	if (status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			 *(pSnr+i)  = ucComMF522Buf[i];
			 snr_check ^= ucComMF522Buf[i];
		}

		if (snr_check != ucComMF522Buf[i]) status = MI_ERR;
	}
    
  MFRC_SetBitMask(CollReg,0x80);
	
	return status;
}

/**********************************************************************
*��������  ����MFRC522����CRC16����
*������    ��CalulateCRC
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
void CalulateCRC(u8 *pIn ,u8 len,u8 *pOut )
{
    u8 i,n;
	
    MFRC_ClearBitMask(DivIrqReg,0x04);
    MFRC_Write(CommandReg,PCD_IDLE);
    MFRC_SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++) MFRC_Write(FIFODataReg, *(pIn +i)); 
	
    MFRC_Write(CommandReg, PCD_CALCCRC);
	
    i = 0xFF;
	
    do 
    {
        n = MFRC_Read(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = MFRC_Read(CRCResultRegL);
    pOut [1] = MFRC_Read(CRCResultRegM);
}

/**********************************************************************
*��������  ��MFRC522����ѡ��������
*������    ��MFRC_PcdSelect
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdSelect(u8 *pSnr)
{
    u8 status;
    u8 i;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    MFRC_ClearBitMask(Status2Reg,0x08);

    status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18)) status = MI_OK; 
    else  status = MI_ERR;

    return status;
}

/**********************************************************************
*��������  ��MFRC522���п�����֤
*������    ��MFRC_PcdAuthState
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
{
	u8 status;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	
	memcpy(&ucComMF522Buf[2], pKey, 6); 
	memcpy(&ucComMF522Buf[8], pSnr, 4); 
	
	status = MFRC_PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	if ((status != MI_OK) || (!(MFRC_Read(Status2Reg) & 0x08))) status = MI_ERR;
	
	return status;
}

/**********************************************************************
*��������  ��MFRC522��ȡ����һ���������
*������    ��MFRC_PcdRead
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdRead(u8 addr,u8 *p)
{
    u8 status;
    u8 unLen;
    u8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
           *(p +i) = ucComMF522Buf[i]; 
    }
    else
         status = MI_ERR; 
    
    return status;
}

/**********************************************************************
*��������  ��MFRC522д���ݵ�һ������
*������    ��MFRC_PcdWrite
*��������  ��void  
*��������ֵ��void
*����      ��
************************************************************************/
u8 MFRC_PcdWrite(u8 addr,u8 *p)
{
	u8 status;
	u8 unLen;
	u8 i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A)) status = MI_ERR;
			
	if (status == MI_OK)
	{
			for (i=0; i<16; i++)
			{    
				ucComMF522Buf[i] = *(p +i);   
			}
			CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

			status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
			
			if ((status != MI_OK)||(unLen != 4)||((ucComMF522Buf[0]&0x0F) != 0x0A)) status = MI_ERR;
	}

	return status;
}










