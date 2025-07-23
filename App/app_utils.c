#include "headfile.h"

uint8_t SoundLight_flag = 0;
uint16_t SoundLight_time = 0;
extern uint8_t lap_count;

float angle_initial = 0;

void Task_select(void)
{
	uint8_t Key = Key_GetNum();
		
	// 切换任务
	if (Key == 1) 
	{
		LED_Green_ON();
		LED_Blue_OFF();
		start_flag++;
		start_flag%=2;
	}
	if(Key == 2)
	{
		LED_Green_OFF();
		LED_Blue_ON();
		Task++;
		Task%=3;
	}
	if(Key == 3)
	{
	
	}
	
	// 执行任务
//	if(start_flag == 1)
//	{
//		if(first_flag == 1)
//		{
//			switch(Task)
//			{
//				case 1: Task_1(); break;
//				case 2: Task_2(); break;
//				case 3: Task_3(); break;
//				case 4: Task_4(); break;
//			}
//		}
//	}
}

 
void SoundLight(void)
{
	if(SoundLight_flag == 0)
	{
		Buzzer_ON();
		LED_Blue_ON();
		SoundLight_flag = 1;
	}
}

void UpdateSoundLight(void)
{
    if(SoundLight_flag)
    {
        SoundLight_time++;

		if(SoundLight_time >= 12) 
		{
			Buzzer_OFF();
			LED_Blue_OFF();
			SoundLight_time = 0;
			SoundLight_flag = 0; 
		}
        
    }
}

extern int16_t turn_time;
extern uint8_t turn_flag;

void params_clear(void)
{
}
