#include "headfile.h"

uint8_t task, start_flag;

void TaskSelect(void)
{
	uint8_t key = Key_GetNum();
		
	// ÇĞ»»ÈÎÎñ
	switch(key)
	{
		case 1:
			start_flag++;
			start_flag%=2;
			break;
		case 2:
			task++;
			task%=3;
			break;
		case 3:
			break;
	}

	if(start_flag)
	{
		switch(task)
		{
			case 0:
				
				break;
			case 1:
				
				break;
			case 2:
				
				break;
		}
	}
}

