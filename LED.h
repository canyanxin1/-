#ifndef _LED_H
#define _LED_H
#include "common.h"
void appStartTask_LED(void);
static void Led0task(void *pvParameters);	 //����������
static void Led1task(void *pvParameters);
static void getTaskInfo(void *pvParameters); //������Ϣ��ȡ
static void printTask(void *pvParameters);	 //���б������ڴ�ӡ������
#endif 
