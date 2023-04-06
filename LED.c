#include "LED.h"

static TaskHandle_t Led0TaskHandle = NULL;	//任务句柄
static TaskHandle_t Led1TaskHandle = NULL;	//任务句柄
static TaskHandle_t TaskInfoTaskHandle = NULL;	//任务句柄
static TaskHandle_t PrintTaskHandle = NULL;	//消息守护任务句柄
static char pcToPrint[80]; //打印缓冲
float OutData[4];
xQueueHandle xQueuePrint;	//消息队列句柄
void appStartTask_LED(void)
{
	xQueuePrint = xQueueCreate(2,sizeof(pcToPrint));	//创建一个长度为2，队列大小足够的字符队列
	if(xQueuePrint != NULL)
	{
		taskENTER_CRITICAL();	//进入临界段，关中断
	   /* 创建Led0task任务 */
		xTaskCreate((TaskFunction_t )Led0task,  /* 任务入口函数 */
					(const char*    )"Led0task",/* 任务名字 */
					(uint16_t       )256,  /* 任务栈大小 */
					(void*          )NULL,/* 任务入口函数参数 */
					(UBaseType_t    )3, /* 任务的优先级 */
					(TaskHandle_t*  )&Led0TaskHandle);/* 任务控制块指针 */ 
		/* 创建Led1task任务 */
		xTaskCreate((TaskFunction_t )Led1task,  /* 任务入口函数 */
					(const char*    )"Led1task",/* 任务名字 */
					(uint16_t       )256,  /* 任务栈大小 */
					(void*          )NULL,/* 任务入口函数参数 */
					(UBaseType_t    )3, /* 任务的优先级 */
					(TaskHandle_t*  )&Led1TaskHandle);/* 任务控制块指针 */ 
					
		/* 创建getTaskInfo任务 */
		xTaskCreate((TaskFunction_t )getTaskInfo,  /* 任务入口函数 */
					(const char*    )"getTaskInfo",/* 任务名字 */
					(uint16_t       )512,  /* 任务栈大小 */
					(void*          )NULL,/* 任务入口函数参数 */
					(UBaseType_t    )3, /* 任务的优先级 */
					(TaskHandle_t*  )&TaskInfoTaskHandle);/* 任务控制块指针 */ 
					
				/* 创建getTaskInfo任务 */
		xTaskCreate((TaskFunction_t )printTask,  /* 任务入口函数 */
					(const char*    )"printTask",/* 任务名字 */
					(uint16_t       )128,  /* 任务栈大小 */
					(void*          )NULL,/* 任务入口函数参数 */
					(UBaseType_t    )3, /* 任务的优先级 */
					(TaskHandle_t*  )&PrintTaskHandle);/* 任务控制块指针 */ 
					
	//	vTaskSuspend(Led1TaskHandle);
		taskEXIT_CRITICAL();	//退出临界段，开中断
		vTaskStartScheduler();	//开启调度器	
		
	}

}

static void Led0task(void *pvParameters)	//测试任务函数
{
	uint16_t cnt = 0;
	uint32_t test[128]={0};
//	UBaseType_t uxPriority;
//	TickType_t xFirstTime;
	  while (1)
	  {
//		 xFirstTime = xTaskGetTickCount();	//获取进入时节拍
		HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
		  vTaskDelay(pdMS_TO_TICKS(500));
		  cnt+=10;
		  if(cnt>3000) cnt =0;
//		  cnt=xTaskGetTickCount()-xFirstTime;
//		taskENTER_CRITICAL() ;	//进入临界段，关中断
			sprintf(pcToPrint,"LED0闪烁任务节拍数:%d\r\n",cnt);
//		taskEXIT_CRITICAL();	//退出临界段，开中断  
		xQueueSendToBack(xQueuePrint,pcToPrint,0);	
	  }
	  vTaskDelete(NULL);
}

static void Led1task(void *pvParameters)	//测试任务函数
{
	uint16_t cnt = 0;
	uint32_t test[128]={0};
//	TickType_t xFirstTime;
	  while (1)
	  {
//		 xNextTime=xFirstTime = xTaskGetTickCount();	//获取进入时节拍
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin); 
		  vTaskDelay(pdMS_TO_TICKS(250));
		  cnt+=20;
		 if(cnt>3000) cnt =0;
//		  cnt=xTaskGetTickCount()-xFirstTime;
//		taskENTER_CRITICAL();	//进入临界段，关中断
			sprintf(pcToPrint,"LED1任务节拍数%d\r\n",cnt);
//		taskEXIT_CRITICAL();	//退出临界段，开中断  
//		if(++cnt>=10)
//		{
//			cnt=0;
//			printf("LED1任务挂起，LED0恢复");
//			vTaskResume(Led0TaskHandle);	//恢复任务led0
//			vTaskSuspend(NULL);	//挂起自己
//		}
		  xQueueSendToBack(xQueuePrint,pcToPrint,0);	
	  }
	  vTaskDelete(NULL);
}

static void getTaskInfo(void *pvParameters)	//任务信息获取
{
	uint8_t keyValue = 0;
	char pcTaskInfo[800];
	while(1)
	{
		keyValue = KeyScan(); 
		if(keyValue==KEY3_PRES)
		{
			vTaskList(pcTaskInfo);
			printf("任务名\t 任务状态\t优先级\t剩余堆栈大小\t任务号\r\n");
			printf("%s\r\n",pcTaskInfo);
		}
		else if(keyValue==KEY2_PRES)
		{
			vTaskGetRunTimeStats(pcTaskInfo);
			printf("任务名\t\t运行时间\t百分比\r\n");
			printf("%s\r\n",pcTaskInfo);
		}
			vTaskDelay(pdMS_TO_TICKS(100));
	}
}

static void printTask(void *pvParameters)	//队列保护串口打印任务函数
{
	char pcToWrite[80];
	while(1)
	{
		xQueueReceive(xQueuePrint,pcToWrite,portMAX_DELAY);	//无数据时无限等待
		printf("%s",pcToWrite);
	}
}
