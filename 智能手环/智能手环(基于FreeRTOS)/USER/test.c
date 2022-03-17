#include "sys.h"
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
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/**************��ʼ����******************/
void start_task(void *pvParameters);  //��ʼ����ĺ���
#define START_TASK_SIZE 		128       //��ʼ����Ķ�ջ��С     n*4���ֽ�
#define START_TASK_PRIO		   1        //��ʼ��������ȼ�
TaskHandle_t   StartTask_Handler;     //������ʼ���������

/**************����ѡ������******************/
void pick_task(void *pvParameters);  //����ѡ��������
#define PICK_TASK_SIZE 		 50        //����ѡ�������ջ��С     n*4���ֽ�
#define PICK_TASK_PRIO		 2         //����ѡ���������ȼ�
TaskHandle_t   PickTask_Handler;     //��������ѡ�����������

/**************�ֻ���������******************/
void func_task(void *pvParameters);  //�ֻ�����������
#define FUNC_TASK_SIZE 		128        //�ֻ����������ջ��С     n*4���ֽ�
#define FUNC_TASK_PRIO		 3         //�ֻ������������ȼ�
TaskHandle_t   FuncTask_Handler;     //�����ֻ��������������

/**************ʵʱ�Ʋ�����******************/
void count_task(void *pvParameters);  //ʵʱ�Ʋ�������
#define COUNT_TASK_SIZE 	100         //ʵʱ�Ʋ������ջ��С     n*4���ֽ�
#define COUNT_TASK_PRIO		 4          //ʵʱ�Ʋ��������ȼ�
TaskHandle_t   CountTask_Handler;     //����ʵʱ�Ʋ����������

//��Ϣ���о��
QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init();		      //��ʼ����ʱ����
	Watch_Init();         //��������
	
	//������ʼ����
  xTaskCreate((TaskFunction_t) start_task,            //������
							(char *        ) "start_task",          //������
							(uint16_t      ) START_TASK_SIZE,       //�����ջ��С
							(void *        ) NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t   ) START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t *) &StartTask_Handler );  //������    
							
	vTaskStartScheduler();                             //�����������
}
 
//��ʼ����������
void start_task(void *pvParameters)
{
	//�����ٽ�����ֹ���´���ִ��ʱ�����
	taskENTER_CRITICAL();           //�����ٽ���
	
	//������Ϣ����
	//xQueueCreate�����Ĳ���
	//����1����Ϣ���г���(Ҳ������Ϣ����)
	//����2����Ϣ�ĳ���(��С)�����ֽ�Ϊ��λ
	Key_Queue=xQueueCreate(1,1);        //������ϢKey_Queue
	
	//����ѡ������
	xTaskCreate((TaskFunction_t) pick_task,            //������
							(char *        ) "pick_task",          //������
							(uint16_t      ) PICK_TASK_SIZE,       //�����ջ��С
							(void *        ) NULL,                 //���ݸ��������Ĳ���
							(UBaseType_t   ) PICK_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t *) &PickTask_Handler );  //������   
	//�ֻ���������
	xTaskCreate((TaskFunction_t) func_task,            //������
							(char *        ) "func_task",          //������
							(uint16_t      ) FUNC_TASK_SIZE,       //�����ջ��С
							(void *        ) NULL,                 //���ݸ��������Ĳ���
							(UBaseType_t   ) FUNC_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t *) &FuncTask_Handler );  //������
	//ʵʱ�Ʋ�����
	xTaskCreate((TaskFunction_t) count_task,           //������
							(char *        ) "count_task",         //������
							(uint16_t      ) COUNT_TASK_SIZE,      //�����ջ��С
							(void *        ) NULL,                 //���ݸ��������Ĳ���
							(UBaseType_t   ) COUNT_TASK_PRIO,      //�������ȼ�
							(TaskHandle_t *) &CountTask_Handler ); //������
	
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	
	taskEXIT_CRITICAL();            //�˳��ٽ���
}


/****************����ѡ������*******************/
void pick_task(void *pvParameters)
{
	u8 temp = 0;
	Watch_Menu();         //�����ʼ��
	while(1)
	{
		//����ɨ��
		temp = Select_Menu();
		
		if((Key_Queue != NULL)&&(temp != 0))
		{
			xQueueSend(Key_Queue,&temp,0);     //��Ϣ��� 
			vTaskSuspend( NULL );              //������ѡ������ֹͣ���� 
		}
		else
			vTaskDelay(50);
	}
}

/****************�ֻ���������*******************/
void func_task(void *pvParameters)
{
	u8 key = 0;
	
	while(1)
	{
		if((Key_Queue!=NULL) && (xQueueReceive(Key_Queue,&key,1)))
		{
			switch(key)
			{
				case 1:Bp_Function();         key=0;break;
				case 2:Rate_Function();       key=0;break;
				case 3:Temperature_Function();key=0;break;
				case 4:Humidity_Function();   key=0;break;
				case 5:RTC_Function();        key=0;break;
				case 6:Step_Function();       key=0;break;
			}
			vTaskResume(PickTask_Handler);   //�ָ�����ѡ������ֹͣ����        
		}
	}
}

/****************ʵʱ�Ʋ�����*******************/
void count_task(void *pvParameters)
{
	while(1)
	{
		motion_Scan();
		vTaskDelay(10);
	}
}




