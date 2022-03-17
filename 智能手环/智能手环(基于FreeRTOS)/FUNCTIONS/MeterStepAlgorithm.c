#include "stm32f4xx.h"
#include "MeterStepAlgorithm.h"
#include "led.h"
#include "key.h"
#include "motor.h"
#include "usart.h"
#include "time.h"
#include "nvic.h"
#include "pwm.h"
#include "OLED.h"
#include "matrix.h"
#include "adc.h"
#include "stdio.h"
#include "sht20.h"
#include "hp6.h"
#include "mpu6050.h"
#include "rtc.h"
#include "interface.h"
#include "function.h"
#include "math.h"


//����װ��ǰ���ٶȾ���ʽ������ֵ  
float gravityNew = 0;
//�ϴε�xyz���ٶȾ���ʽ������ֵ 
float gravityOld = 0; 
//��ǰ�����ʱ��  
long timeOfThisPeak = 0;  
//��һ�β����ʱ��  
long timeOfLastPeak = 0; 
//��ǰ��ʱ��  
long timeOfNow = 0; 

//��һ���״̬�����������½�  
u8 lastStatus = FALSE;       //��ʼֵ������Ĭ�����½�״̬
//��ǰ��״̬�Ƿ�����״̬ 
u8 isDirectionUp = FALSE;    //��ʼֵ������Ĭ�����½�״̬
//������������  
int continueUpCount = 0;
//��¼���½�����ǰ���������Ĵ�����Ϊ�˼�¼�������������  
int continueUpFormerCount = 0; 
//����ֵ  
float peakOfWave = 0; 
//����ֵ  
float valleyOfWave = 0; 

//��ֵ  
float ThreadValue = (float) 2.0;   //��ʼ����ֵΪ2.0
//���ڴ�ż�����ֵ�Ĳ��岨�Ȳ�ֵ  
float tempValue[VALUE_NUM] ={0};
//��ǰ��ֵ����װ�˶��ٸ�Ԫ��
int tempCount = 0;
//��̬��ֵ��Ҫ��̬�����ݣ����ֵ������Щ��̬���ݵ���ֵ  
float initialValue = (float) 1.3; 

//�Ʋ�����(��¼��ǰ����)
static u8 stepTempCount =0;
//������Ϣ:��·���̡�����
static sportsInfo_t sportsInfo;

/*****************************************************************************************
*��������  ����MPU6050�ļ��ٶ�����ͨ���㷨�õ�����Ҫ�Ĳ��������߾��롢���Ŀ�·��(�������ֻ�Ǹ���װ)
*������    ��SensorChanged 
*��������  ��ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t * personInfo
*��������ֵ��sportsInfo_t
*����      ��
*           ����1��xyz�Ľ��ٶ�(�ṹ��)  
*           ����2��RTCʱ��(�ṹ��)
*           ����3��������������ߡ�����(�ṹ��)
******************************************************************************************/
sportsInfo_t *SensorChanged(ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t *personInfo)
{
//	u8 buff[20];
	
	ADD_SPEED *p = pAccValue;   //��ӱ���
	
	//�����ٶȸ��ݹ�ʽ���д���
	gravityNew = (float)sqrt(p->x_val*p->x_val + p->y_val*p->y_val + p->z_val*p->z_val);
	
//	sprintf((char *)buff,"Step:%0.2f",gravityNew);
//	OLED_ShowString(3,1,16,buff);
	
	return DetectorNewStep(gravityNew,timeStamp_p,personInfo);
}



/*****************************************************************************************
*��������  �����������߾��롢���Ŀ�·��
*������    ��DetectorNewStep
*��������  ��float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo
*��������ֵ��sportsInfo_t
*����      ��
*           ����1��������ʽ������xyz���ٶȵ�ֵ 
*           ����2��RTCʱ��(�ṹ��)
*           ����3��������������ߡ�����(�ṹ��)
******************************************************************************************/
sportsInfo_t *DetectorNewStep(float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo)
{
	personInfo_t *pI = personInfo;     //��ӱ�����ȡ�����Ǵ�����������Ϣ
	timeStamp_t *time_p = timeStamp_p; //��ӱ�����ȡ�����Ǵ�����RTCʱ��
	static u32 step_2second;           //��¼ÿ�������ߵĲ���
	static u32 time_old;               //��һ�ε�ʱ��(Ҳ������һ�ν����������ʱ��)
	u32 time_now;                      //��ǰʱ��(Ҳ�������µ�ʱ��)
	float step_lenth,walk_speed,walk_distance,Calories;//���������٣����߾��룬���ĵĿ�·��
	
	if(gravityOld==0)
	{
		//��ȡ��ǰ�ľ�����ʽ������xyz���ٶ�
		gravityOld = values;
	}
	else 
  { 
		if (DetectorPeak(values, gravityOld))//��⵽���壬���ַ�ֵ��ִ������Ķ���
		{
			//��ǰ��ֵʱ�����ȥ��ֵʱ����н���
			timeOfLastPeak = timeOfThisPeak;//�����ϴβ����ʱ�� 
			time_now = timeOfNow = (((time_p->hour*60)+time_p->minute)*60+time_p->second)*1000+time_p->twentyMsCount*20; //��ȡʱ�� ,��ת��Ϊ����
			
			//�����⵽�˲��壬���ҷ���ʱ����Լ���ֵ�����������ж�Ϊ1��
			//��ǰʱ�� - ��һ�εķ�ֵʱ�� ���� 250         ����ֵ - ����ֵ ���ڵ���  ��ֵ
			if((timeOfNow - timeOfLastPeak >= 250 )&&(peakOfWave - valleyOfWave >= ThreadValue))
			{
				timeOfThisPeak = timeOfNow; //���µ�ǰ����ʱ�� 
				stepTempCount++;            //��1Ϊ����
				step_2second ++;            //��1Ϊ����(�����������㲽���õ�)
				
				//�ж��Ƿ��Ѿ�������
				if((time_now - time_old) >= 2000 ) 
				{
					//���ݲ����벽���Ĺ�ϵ��ó����¹�ϵ
					if(step_2second == 1) step_lenth = pI->height/5;
					else if(step_2second == 2) step_lenth = pI->height/4;
					else if(step_2second == 3) step_lenth = pI->height/3;
					else if(step_2second == 4) step_lenth = pI->height/2;
					else if(step_2second == 5) step_lenth = pI->height/1.2f;
					else if(step_2second == 7) step_lenth = pI->height;
					else if(step_2second == 8) step_lenth = pI->height*1.2f;
					else step_lenth = 0;
					
					//���� = ���� * ���� / 2��
					walk_speed = step_2second * step_lenth/2;        //�������
					//���߾��� = ���� * ����
					walk_distance  = step_2second * step_lenth;      //������߾���
					//��·�� = 4.5 * �����ٶ� * (���� / 2 )/1800
					Calories = 4.5f*walk_speed*(pI->weight/2)/1800;  //��·��
					
					sportsInfo.calories  += Calories;
					sportsInfo.distance  += walk_distance;		
					time_old = time_now;         //����ʱ��
					step_2second = 0;            //��¼ÿ�������ߵĲ����ı�������
				}
				
				/************������Ч�˶�*************/
				/***********************************************************
				*1.������¼5�ſ�ʼ�Ʋ� 
				*2.�����¼�Ĳ��û�ͣס����3�룬��ǰ��ļ�¼ʧЧ���´δ�ͷ��ʼ 
				*3.����4��¼�˲��û������˶���֮ǰ�����ݲ���Ч
				*************************************************************/
				//���3��ಽ������5������ʧ��
				if ((stepTempCount< 5 )&&(timeOfNow - timeOfLastPeak >= 3000))          
				{
					//�������¼���
					stepTempCount = 0;
				}
				//���3���ڣ���������5������������
				else if((stepTempCount>= 5)&&(timeOfNow - timeOfLastPeak <= 3000))
				{
					//��������
					sportsInfo.stepCount += stepTempCount;          
					stepTempCount         = 0; 				
				}
			}
			//�ж�������ֵ��ʱ�����250�룬�Ҳ����벨�Ȳ�Ҫ����1.3
			if ((timeOfNow - timeOfLastPeak >= 250) && (peakOfWave - valleyOfWave >= initialValue)) 
			{
				//����ʱ��
				timeOfThisPeak = timeOfNow;
				//��ȡ��ֵ
				ThreadValue = Threshold_Value(peakOfWave - valleyOfWave);//���·�ֵ  
			}
		}
	}
	gravityOld = values;  
  return &sportsInfo;
}

/*****************************************************************************************
*��������  ����鲨��
*������    ��DetectorPeak
*��������  ��float newValue, float oldValue
*��������ֵ��u8
*����      ��
*           ����1����ǰ������ʽ������xyz���ٶȵ�ֵ 
*           ����2����һ�ξ�����ʽ������xyz���ٶȵ�ֵ
*
*           �����ĸ������ж�Ϊ���壺
*					 ��1��Ŀǰ��Ϊ�½������ƣ�isDirectionUpΪFALSE 
*					 ��2��֮ǰ�ĵ�Ϊ���������ƣ�lastStatusΪTRUE 
*					 ��3��������Ϊֹ�������������ڵ���2��
*					 ��4������ֵ����20 //Jahol:�����ֵ�޸�Ϊ15
*           ��¼����ֵ��
*					 ��1���۲첨��ͼ�����Է����ڳ��ֲ��ӵĵط������ȵ���һ�����ǲ��壬�бȽ����Ե������Լ���ֵ 
*				   ��2������Ҫ��¼ÿ�εĲ���ֵ��Ϊ�˺��´εĲ������Ա� 	
*   
******************************************************************************************/
u8 DetectorPeak(float newValue,float oldValue)
{
	//��ǰ״̬���ȥ״̬���н���
	lastStatus = isDirectionUp;
	
	//�жϵ�ǰ��״̬����
	if (newValue >= oldValue) //����������
	{
		//��ǰ״̬��Ϊ����
		isDirectionUp = TRUE;  
		//����������1
    continueUpCount++; 
	}
	else				              //���½�����
	{
		//��ǰ״̬��Ϊ�½�
		isDirectionUp = FALSE;
		//��ȡ�½�����ǰ����������
		continueUpFormerCount = continueUpCount;
		//����������¼����
		continueUpCount = 0;
	}
	
	//�����ǰ���½����ƣ�����һ�����������ƣ������½�����ǰ��������������2���ϴξ���ʽ�����xyz���ٶ�ֵҪ����20
	if ((!isDirectionUp) && lastStatus && ((continueUpFormerCount >= 2) && (oldValue >= 20)))
	{
		//����һ�εľ���ʽ�����xyz���ٶ�ֵ��Ϊ����ֵ
		peakOfWave = oldValue;
		return 1;
	}
	//�����һ�����½����ƣ�����ǰ���������ƣ�
	else if ((!lastStatus) && isDirectionUp) 
  {  
		//����һ�εľ���ʽ�����xyz���ٶ�ֵ��Ϊ����ֵ
    valleyOfWave = oldValue;  
    return 0;  
  }
	//�����һ�κ͵�ǰ��״̬ͬΪ�������½�
	else
	{
		//�����κδ���
		return 0;
	}
	
}

/*****************************************************************************************
*��������  ����ֵ�ļ���
*������    ��Threshold_Value
*��������  ��float value
*��������ֵ��float
*����      ��
*            �����������벨�ȵĲ�
******************************************************************************************/
float Threshold_Value(float value)
{
	u8 i;
	float tempThread = ThreadValue;
	
	//�ж������Ƿ�װ��
	if (tempCount < VALUE_NUM)
  {  
    tempValue[tempCount] = value;  
    tempCount++;  
  } 
	else
	{
		tempThread = AverageValue(tempValue, VALUE_NUM);//���㷧ֵ
		
		//��������
		for ( i = 1;i < VALUE_NUM;i++)//������λ����
    {  
      tempValue[i - 1] = tempValue[i];  
    }  
    tempValue[VALUE_NUM - 1] = value; 
	}
	return tempThread;
}

/*****************************************************************************************
*��������  ���ݶȻ���ֵ
*������    ��AverageValue
*��������  ��float value[] , int n
*��������ֵ��float
*����      ��
*             ����1����ֵ����
*             ����2��Ҫ��ƽ��ֵ��Ԫ�ظ���
*
*             1.��������ľ�ֵ 
*			        2.ͨ����ֵ����ֵ�ݶȻ���һ����Χ�� 
******************************************************************************************/
float AverageValue(float value[], int n)
{
	float ave = 0;  
  u8 i =0;
	
  for (i=0;i<n;i++)
  {  
    ave += value[i];//���  
  }
	
	ave = ave/VALUE_NUM;//��ƽ��ֵ 
	
	if (ave >= 8)  
    ave = (float) 4.3; 
  else if (ave >= 7 && ave < 8)  
    ave = (float) 3.3;  
  else if (ave >= 4 && ave < 7)  
    ave = (float) 2.3;  
  else if (ave >= 3 && ave < 4)  
    ave = (float) 2.0;  
  else 
    ave = (float) 1.3;  
  
  return ave; 
}




