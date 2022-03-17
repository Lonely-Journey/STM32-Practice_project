#include "stm32f4xx.h"  
#include "interface.h"
#include "stm32f4xx.h"
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
#include "MeterStepAlgorithm.h"

static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}


/*************************************************************
*��������  ����ʼ�����������ֻ�����
*������    ��Watch_Init 
*��������  ��void
*��������ֵ��void
*����      ��
**************************************************************/
void Watch_Init(void)
{
	RTC_DATES time = {19,6,27,5,6,6,20,0};
	u8 i,t=70,k;
	
  LED_Init();
	KEY_Init();
	MOTOR_Init();
	TIM10_Init(9999,9999);
	ADC1_Init_In3();
	//TIM2_PWM_Init(1000,50,pwm);
	OLED_Init();
	ADC1_Init_In3();
	SHT20_Init();
	HP6_Init();
	MPU6050_Init();
	MYRTC_Init(time);//�������ڲ�����ʱ��
	
	for(i=0;i<14;i++)
  {
		k++;
		if(k==7)k=0;
		OLED_ShowPicture(t,0,&TU[k*512]);
		delay_ms(100);
		OLED_Clear();
		t-=10;
  }
	OLED_ShowPicture(1,0,TU2);
	OLED_ShowString(60,2,16,"welcome");
	OLED_ShowString(63,4,16,"to use");
	
	while(!KEY_Scan());
	OLED_Clear();
	OLED_ShowChar(10,1,16,'*');
}

/*************************************************************
*��������  �������ֻ����˵�����
*������    ��Watch_Menu 
*��������  ��void
*��������ֵ��void
*����      ��
**************************************************************/
void Watch_Menu(void)
{
	OLED_HanziString(20,1,16,"Ѫѹ");
	OLED_HanziString(75,1,16,"����");
	OLED_HanziString(20,3,16,"�¶�");
	OLED_HanziString(75,3,16,"ʪ��");
	OLED_HanziString(20,5,16,"ʱ��");
	OLED_HanziString(75,5,16,"����");
}	

/*************************************************************
*��������  �������ֻ����ʼ�����
*������    ��Rate_HMI 
*��������  ��void
*��������ֵ��void
*����      ��
**************************************************************/
void Rate_HMI(void)
{
	OLED_ShowPicture(1,0,TU3);
	OLED_HanziString(65,1,16,"���°���");
	OLED_HanziString(65,3,16,"��ʼ����");
	OLED_HanziString(82,5,16,"���");
}

/*************************************************************
*��������  �������ֻ����ʼ�����
*������    ��Bp_HMI 
*��������  ��void
*��������ֵ��void
*����      ��
**************************************************************/
void Bp_HMI(void)
{
	OLED_ShowPicture(1,0,TU4);
	OLED_HanziString(65,1,16,"���°���");
	OLED_HanziString(65,3,16,"��ʼѪѹ");
	OLED_HanziString(82,5,16,"���");
}


