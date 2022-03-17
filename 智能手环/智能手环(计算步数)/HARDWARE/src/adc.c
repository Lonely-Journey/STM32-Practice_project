#include "stm32f4xx.h"   
#include "stdio.h"
#include "adc.h"

/************************************
*   F407��ADCʱ��Ƶ�ʹٷ�����: 36Mhz
*   F103��ADCʱ��Ƶ�ʹٷ�����: 14Mhz
*************************************/

//��ʱ��������
static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;   //��ϵͳʱ��Ƶ�ʾ�����
	while(i)
	{
		i--;
	}
}

/******************************************
*������    ��ADC1_Init_In3
*��������  ��ADC1�ĵ�����ͨ����ʼ������
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PA3----------ADC1_IN3   ģ��ģʽ
*********************************************/
void ADC1_Init_In3(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx�ṹ������
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCxһ�����ýṹ��
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx��ʼ�����ýṹ��
  
  //ʱ��ʹ��  GPIOA  ADC1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
  /*GPIOx�˿�����*/
  //PA5
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_3;         //ѡ��PA5
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //ģ������
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //��������
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //�����趨������ʼ��PA5
	
	/*ADCx�Ĵ�������*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC->CCR�Ĵ����ĵ�0-4λ)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��(ADC->CCR�Ĵ����ĵ�8-11λ)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMAʧ��(ADC->CCR�Ĵ����ĵ�14-15λ)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR�Ĵ����ĵ�16-17λ)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCxһ�����ó�ʼ��(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12λģʽ(ADC->CR1�Ĵ����ĵ�24��25λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //��ɨ��ģʽ(ADC->CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //�ر�����ת��(ADC->CR2�Ĵ����ĵ�1λ)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ������⣬ʹ���������(ADC->CR2�Ĵ����ĵ�20-21λ/��28-29λ)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //�Ҷ���	(ADC->CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1��ת���ڹ��������� Ҳ����ֻת����������1(ADC->SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC1, &ADC_InitStruct);                                         //ADCx���ó�ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC1�ĵ�һ��ת��ʱͨ��3������ʱ��480������        (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADCx��������ADת����	
}

/******************************************
*������    ��ADC1_In3_Data
*��������  ����ȡADC1ͨ��3��ת������
*��������  ��void
*��������ֵ��u16
*��������  ������ת���ɵ���������
*********************************************/
u16 ADC1_In3_Data(void)
{
  u16 data;
  
  //��������ͨ��ת������(ADC1->CR2�Ĵ����ĵ�30λ)
	ADC_SoftwareStartConv(ADC1);    
  //�ȴ�ת�����
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//�ȴ�ת������(ADC1->SR�Ĵ����ĵ�1λ)
  //��ȡת���������(ADC1->DR)
  data = ADC_GetConversionValue(ADC1);   
  
  return data;
}

/******************************************
*������    ��ADC_KEY_Data
*��������  ����ȡ�巽�򰴼���ֵ
*��������  ��void
*��������ֵ��u16
*��������  ������ת���ɵ���������
*********************************************/
u16 ADC_KEY_Data(void)
{
	u16 data[5],temp, sum;
	u8 i,j;
	
	//��ȡ5��
	for(i=0;i<5;i++)
	{
		data[i] = ADC1_In3_Data();
	}
	
	//����
	for(i=1;i<5;i++)
	{
		for(j=0;j<5-i;j++)
		{
			if(data[j] > data[j+1])
			{
				temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}
		}
	}
	
	//ȥ�������С��ƽ��
	for(i=1;i<4;i++)
	{
		sum = sum + data[i];
	}
	
	return (sum / 3);
}

/******************************************
*������    ��ADC_KEY_Scan
*��������  ����ȡ�巽�򰴼��İ���ɨ��
*��������  ��void
*��������ֵ��u8
*��������  ������ת���ɵ���������
*********************************************/
u8 ADC_KEY_Scan(void)
{
	u16 adc_key = 0;
	static u8 temp = 0;
	static u8 key_flay = 1;
	
	adc_key = ADC_KEY_Data();
	adc_key = adc_key *0.40 / 100;
	
	if(key_flay&&adc_key)
	{
		delay_ms(10);
		if(adc_key)
		{
			key_flay = 0;
			if(adc_key==4) return (temp = 1);          //��
			else if(adc_key==5) return (temp = 2);     //��
			else if(adc_key==8) return (temp =3);      //��
			else if(adc_key==16) return (temp = 4);    //��
		}
	}
	
	if(adc_key == 0)
  {
    key_flay = 1;
		temp = 0;
  }
	
	return temp;
}

