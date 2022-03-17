#include "sys.h"
#include "printer.h"
#include "motor.h"
#include "w25q64.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "matrix.h"
#include "time.h"


u8 p_flag = 1;//��ӡˢ�����ݱ�־����

/*****************************************************
*��������  ���Դ�ӡͷ��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��Printer_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

	//GPIOA
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	//PA8/11/12 �˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		              //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		              //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                    //�����趨������ʼ��PA8/11/12
	
	//GPIOB
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //PB10/11/12/13/14/15 �˿�����
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                    //�����趨������ʼ��PB10/11/12/13/14/15
}


/*****************************************************
*��������  �����ӡͷ����8λ����
*������    ��Printer_Write_Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Write_Byte(u8 data)
{
	u8 i;
	CLK_L;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(data & 0x80) DI_H;
		else DI_L;
		CLK_H;
		data <<= 1;
		delay_us(1);
	}
}

/*****************************************************
*��������  ���Դ�ӡͷ����߼���
*������    ��Printer_Left
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Left(void)
{
	STB1_H ;
	STB2_H ;
	STB3_H ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*��������  ���Դ�ӡͷ���ұ߼���
*������    ��Printer_Right
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Right(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_H ;
	STB5_H ;
	STB6_H ;
}

/*****************************************************
*��������  ���رմ�ӡͷ����
*������    ��Printer_OFF
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_OFF(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*��������  ����ӡ����
*������    ��Printer_Stripe
*��������  ��void
*��������ֵ��void
*����      ��
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Stripe(void)
{
	u8 i =0;
	for(i=0; i<48;i++)
	{
		Printer_Write_Byte(0xf0);
	}
}

/**************************************************************
*��������  ����ӡһ��ͼƬ
*������    ��Printer_Picture
*��������  ��u8 *tu
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_Picture(u8 *tu)
{
	u8 i;
	static u16 line = 0;
	
	if(p_flag == 1)//�ж�����������ݱ�־λ //��λ�����ж�����1����ʼֵ����Ϊ1��������һ�����ݾ���������ʱ��ǰ׼������
	{
		p_flag = 0;   //����������ݱ�־λ ��0           //Ϊ����һ�����ж�����1
		for (i=0; i<48; i++)  //дͼƬ��һ������ // 48�ֽ�
		{
			Printer_Write_Byte(*(tu+i+line*48));//ͼƬȡģ���򣺴����ң����ϵ��£�һ��48�ֽڣ�����ÿһ������ƫ��48
		}
		line++;
	}
	
	if (line >= 473)  //ͼƬ��473 �к�0~472    ����Ҫ��472�д�ӡ�����Ե�473ʱ�ر�
	{
		line = 0; //�к����㣬��һ�δ�ӡ��0�п�ʼ
		MOTOR_OFF;
		TIM_Cmd(TIM3, DISABLE);
		Printer_OFF();
	}
}

/*****************************************************
*��������  �����ӡͷ����48���ֽ�����
*������    ��Printer_Write_48Byte
*��������  ��u8 data
*��������ֵ��void
*����      ��
********************************************************/
void Printer_Write_48Byte(u8 *buf)
{
	u8 i;
	
	for(i=0;i<48;i++)
	{
		Printer_Write_Byte(*(buf+i));
	}
}

/**************************************************************
*��������  ����ӡһ���ַ�16*16(��ϰ˼·)
*������    ��Printer_CH16
*��������  ��u8 *ch
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_CH16(u8 *str)
{
	u8 i=0;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - ' ';  //����Ŀո���ָ20�ſո񣬶������ַ��ֿ�ĵ�һ���ո�
			buf[i] = ch16[num*16+hang];
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//�����������	
		
		hang++;
		
		if(hang >= 16)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*��������  ����ӡһ���ַ�24*24(��ϰ˼·)
*������    ��Printer_CH24
*��������  ��u8 *str
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_CH24(u8 *str)
{
	u8 i=0,j=0;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - 0;
			buf[j] = ch24[num*48+hang*2];
			j++;
			buf[j] = ch24[num*48+hang*2+1];
			j++;
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//�����������	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*��������  ����ӡһ���ַ�24*24(��w25q64��������)
*������    ��Printer_Str24
*��������  ��u8 *str
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_Str24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - 0;
			addr = 0x0b0000 + num*48 + hang*2;
			W25Q64_Read_Bytes(addr,2,&buf[j]);
			j += 2;
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//�����������	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*��������  ����ӡһ������24*24(��w25q64��������)
*������    ��Printer_HZ24
*��������  ��u8 *str
*��������ֵ��void
*����      ��
****************************************************************/
void Printer_HZ24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u32 num = 0;
	static u8 hang=0;
	u32 H,L;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			//����
			H = (*(str+i) - 0xa1) * 94;
			//λ��
			L = *(str+i+1) - 0xa1;
			num = H + L;
			addr = 0x0b0000 + 0x00001806 + num*72 + hang*3;
			W25Q64_Read_Bytes(addr,3,&buf[j]);
			j += 3;
			i += 2;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//�����������	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*��������  ����ӡһ������/�ַ�24*24(��w25q64��������)
*������    ��Printer_String24
*��������  ��u8 *str
*��������ֵ��void
*����      ��
****************************************************************/
u8 Printer_String24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u32 num = 0;
	static u8 hang=0;
	u32 H,L;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			
			if((32<=*(str+i)) && (*(str+i)<=127))
			{
				num = *(str+i) - 0;
				addr = 0x0b0000 + num*48 + hang*2;
				W25Q64_Read_Bytes(addr,2,&buf[j]);
				j += 2;
				i++;
			}
			else
			{
				//����
				H = (*(str+i) - 0xa1) * 94;
				//λ��
				L = *(str+i+1) - 0xa1;
				num = H + L;
				addr = 0x0b0000 + 0x00001806 + num*72 + hang*3;
				W25Q64_Read_Bytes(addr,3,&buf[j]);
				j += 3;
				i += 2;
			}
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//�����������	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
			return 0;
		}
	}
	return 1;
}

/**************************************************************
*��������  ����ӡһ������/�ַ�24*24(��w25q64��������)(���Զ�����)
*������    ��Printer1_String24
*��������  ��u8 *str
*��������ֵ��void
*����      ��
****************************************************************/
void Printer1_String24(u8 *str)
{
	u8 i=0,j=0,k=0,key=1,t=1;
	u8 buf[51] = {0};
	
	THIS:
	while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
	{
		if((32<=*(str+i)) && (*(str+i)<=127))
		{
			if(j+2>48)
			{
				key=0;
				break;
			}
				
			buf[k] = *(str+i);
			j += 2;
			i++;
			k++;
		}
		else
		{
			if(j+2>48)
			{
				key=0;
				break;
			}
			
			buf[k] = *(str+i);
			buf[k+1] = *(str+i+1);
			j += 3;
			i += 2;
			k += 2;
		}
		key = 1;
	}
	
	buf[k] = '\r';
	buf[k+1] = '\n';
	
	TIM_Cmd(TIM3, ENABLE);
	while(t)
	{
		t = Printer_String24(buf);
	}
	if(key != 1)
	{
		t = 1;
		j = 0;
		k = 0;
		goto THIS;
	}
}






