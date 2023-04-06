#ifndef _LED_H
#define _LED_H
#include "common.h"
void appStartTask_LED(void);
static void Led0task(void *pvParameters);	 //测试任务函数
static void Led1task(void *pvParameters);
static void getTaskInfo(void *pvParameters); //任务信息获取
static void printTask(void *pvParameters);	 //队列保护串口打印任务函数
#endif 
