#include "key.h"
static uint8_t KeyNum=0;

uint8_t Key_GetState(void)
{
	if (DL_GPIO_readPins(KEY_PORT, KEY_task_PIN) == 0)
	{
		return 1;
	}
	if (DL_GPIO_readPins(KEY_PORT, KEY_start_PIN) == 0)
	{
		return 2;
	}
	if (DL_GPIO_readPins(KEY3_PORT, KEY3_KEY_3_PIN) == 0)
	{
		return 3;
	}
	return 0;
}

void Key_Scan(void)
{
	static uint8_t cnt;
	static uint8_t CurrState, PrevState;
	cnt ++;
	
	if (cnt >=3)
	{
		cnt = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		if (CurrState == 0 && PrevState != 0)
		{
			KeyNum = PrevState;
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (KeyNum)
	{
		Temp = KeyNum;
		KeyNum = 0;
		return Temp;
	}
	return 0;
}
