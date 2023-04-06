#include "key.h"

uint8_t KeyScan(void)
{
	static uint8_t keyUp=1;
	if(keyUp&&(Key0==1||Key1==1||Key2==1||Key3==1))
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		keyUp=0;
		if(Key0==1) return KEY0_PRES;
		else if(Key1==1) return KEY1_PRES;
		else if(Key2==1) return KEY2_PRES;
		else if(Key3==1) return KEY3_PRES;
	}
	else if(Key0==0&&Key1==0&&Key2==0&&Key3==0)keyUp=1;
	return 0;
}

