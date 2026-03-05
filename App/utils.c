#include "headfile.h"

uint8_t task_num, start_flag;

void System_Init(void)
{
	SYSCFG_DL_init();
	UartDeviceInit();
	TimerDeviceInit();
}

void TaskSelect(void)
{
	uint8_t key = Key_GetNum();
		
	// ÇÐ»»ÈÎÎñ
	switch(key)
	{
		case 1:
			start_flag++;
			start_flag %= 2;
			break;
		case 2:
			task_num++;
			task_num %= 3;
			break;
		case 3:
			break;
	}
}

