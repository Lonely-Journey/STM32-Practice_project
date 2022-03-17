#ifndef _MSA_H
#define _MSA_H

  #include "stm32f4xx.h"
	#include "mpu6050.h"
	#include "rtc.h"
	
	typedef struct 
	{
		float calories; //��·��
		float distance;//��··�̣���λΪm
		u32 stepCount;	//����
	}sportsInfo_t;
	
	typedef struct 
	{
		float height;  //���
		float weight;  //����
	}personInfo_t;
	
	#define 	TRUE 		    1     //��ʾ��������
	#define 	FALSE 	    0     //��ʾ�½�����
	#define   VALUE_NUM   4     //��ֵ�����С
	
	sportsInfo_t *SensorChanged(ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t *personInfo);
	sportsInfo_t *DetectorNewStep(float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo);
	u8 DetectorPeak(float newValue,float oldValue);
	float Threshold_Value(float value);
	float AverageValue(float value[], int n);

#endif




