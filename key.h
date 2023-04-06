#ifndef _KEY_H
#define _KEY_H
#include "common.h"
#define Key0 HAL_GPIO_ReadPin(Key0_GPIO_Port,Key0_Pin)
#define Key1 HAL_GPIO_ReadPin(Key1_GPIO_Port,Key1_Pin)
#define Key2 HAL_GPIO_ReadPin(Key2_GPIO_Port,Key2_Pin)
#define Key3 HAL_GPIO_ReadPin(Key3_GPIO_Port,Key3_Pin)
#define KEY0_PRES 1
#define KEY1_PRES 2
#define KEY2_PRES 3
#define KEY3_PRES 4
uint8_t KeyScan(void);

#endif 

