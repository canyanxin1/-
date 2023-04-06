#include "LED.h"

static TaskHandle_t Led0TaskHandle = NULL;	//������
static TaskHandle_t Led1TaskHandle = NULL;	//������
static TaskHandle_t TaskInfoTaskHandle = NULL;	//������
static TaskHandle_t PrintTaskHandle = NULL;	//��Ϣ�ػ�������
static char pcToPrint[80]; //��ӡ����
float OutData[4];
xQueueHandle xQueuePrint;	//��Ϣ���о��
void appStartTask_LED(void)
{
	xQueuePrint = xQueueCreate(2,sizeof(pcToPrint));	//����һ������Ϊ2�����д�С�㹻���ַ�����
	if(xQueuePrint != NULL)
	{
		taskENTER_CRITICAL();	//�����ٽ�Σ����ж�
	   /* ����Led0task���� */
		xTaskCreate((TaskFunction_t )Led0task,  /* ������ں��� */
					(const char*    )"Led0task",/* �������� */
					(uint16_t       )256,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )3, /* ��������ȼ� */
					(TaskHandle_t*  )&Led0TaskHandle);/* ������ƿ�ָ�� */ 
		/* ����Led1task���� */
		xTaskCreate((TaskFunction_t )Led1task,  /* ������ں��� */
					(const char*    )"Led1task",/* �������� */
					(uint16_t       )256,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )3, /* ��������ȼ� */
					(TaskHandle_t*  )&Led1TaskHandle);/* ������ƿ�ָ�� */ 
					
		/* ����getTaskInfo���� */
		xTaskCreate((TaskFunction_t )getTaskInfo,  /* ������ں��� */
					(const char*    )"getTaskInfo",/* �������� */
					(uint16_t       )512,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )3, /* ��������ȼ� */
					(TaskHandle_t*  )&TaskInfoTaskHandle);/* ������ƿ�ָ�� */ 
					
				/* ����getTaskInfo���� */
		xTaskCreate((TaskFunction_t )printTask,  /* ������ں��� */
					(const char*    )"printTask",/* �������� */
					(uint16_t       )128,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )3, /* ��������ȼ� */
					(TaskHandle_t*  )&PrintTaskHandle);/* ������ƿ�ָ�� */ 
					
	//	vTaskSuspend(Led1TaskHandle);
		taskEXIT_CRITICAL();	//�˳��ٽ�Σ����ж�
		vTaskStartScheduler();	//����������	
		
	}

}

static void Led0task(void *pvParameters)	//����������
{
	uint16_t cnt = 0;
	uint32_t test[128]={0};
//	UBaseType_t uxPriority;
//	TickType_t xFirstTime;
	  while (1)
	  {
//		 xFirstTime = xTaskGetTickCount();	//��ȡ����ʱ����
		HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
		  vTaskDelay(pdMS_TO_TICKS(500));
		  cnt+=10;
		  if(cnt>3000) cnt =0;
//		  cnt=xTaskGetTickCount()-xFirstTime;
//		taskENTER_CRITICAL() ;	//�����ٽ�Σ����ж�
			sprintf(pcToPrint,"LED0��˸���������:%d\r\n",cnt);
//		taskEXIT_CRITICAL();	//�˳��ٽ�Σ����ж�  
		xQueueSendToBack(xQueuePrint,pcToPrint,0);	
	  }
	  vTaskDelete(NULL);
}

static void Led1task(void *pvParameters)	//����������
{
	uint16_t cnt = 0;
	uint32_t test[128]={0};
//	TickType_t xFirstTime;
	  while (1)
	  {
//		 xNextTime=xFirstTime = xTaskGetTickCount();	//��ȡ����ʱ����
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin); 
		  vTaskDelay(pdMS_TO_TICKS(250));
		  cnt+=20;
		 if(cnt>3000) cnt =0;
//		  cnt=xTaskGetTickCount()-xFirstTime;
//		taskENTER_CRITICAL();	//�����ٽ�Σ����ж�
			sprintf(pcToPrint,"LED1���������%d\r\n",cnt);
//		taskEXIT_CRITICAL();	//�˳��ٽ�Σ����ж�  
//		if(++cnt>=10)
//		{
//			cnt=0;
//			printf("LED1�������LED0�ָ�");
//			vTaskResume(Led0TaskHandle);	//�ָ�����led0
//			vTaskSuspend(NULL);	//�����Լ�
//		}
		  xQueueSendToBack(xQueuePrint,pcToPrint,0);	
	  }
	  vTaskDelete(NULL);
}

static void getTaskInfo(void *pvParameters)	//������Ϣ��ȡ
{
	uint8_t keyValue = 0;
	char pcTaskInfo[800];
	while(1)
	{
		keyValue = KeyScan(); 
		if(keyValue==KEY3_PRES)
		{
			vTaskList(pcTaskInfo);
			printf("������\t ����״̬\t���ȼ�\tʣ���ջ��С\t�����\r\n");
			printf("%s\r\n",pcTaskInfo);
		}
		else if(keyValue==KEY2_PRES)
		{
			vTaskGetRunTimeStats(pcTaskInfo);
			printf("������\t\t����ʱ��\t�ٷֱ�\r\n");
			printf("%s\r\n",pcTaskInfo);
		}
			vTaskDelay(pdMS_TO_TICKS(100));
	}
}

static void printTask(void *pvParameters)	//���б������ڴ�ӡ������
{
	char pcToWrite[80];
	while(1)
	{
		xQueueReceive(xQueuePrint,pcToWrite,portMAX_DELAY);	//������ʱ���޵ȴ�
		printf("%s",pcToWrite);
	}
}
