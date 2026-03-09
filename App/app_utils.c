#include "headfile.h"

uint8_t SoundLight_flag = 0;
uint16_t SoundLight_time = 0;
extern uint8_t lap_count;
float angle_initial = 0;

void System_Init(void)
{
//	board_init(); // 延迟 串口
//	jy901s_Init();
//	HC05_Init();
//	encoder_Init();
//	timer0_init();
//	timer1_init();
//	Ultrasonic_Init();
//	OLED_Init();
//    OLED_Clear();
//	delay_ms(100);//等待部署
//	IMU_init();
//	timer3_init();
//	delay_ms(20);
}

void Task_select(void)
{
	uint8_t Key = Key_GetNum();
		
	// 切换任务
	if(start_flag == 0)
	{
		if (Key == 1) 
		{
			LED_Green_ON();
			Task++;
		}
		if(Key == 2)
		{
			LED_Blue_ON();
			start_flag = 1;
		}
		if(Key == 3)
		{
		
		}
		if (Task > 4) Task = 0; 
	}
	
	// 执行任务
	if(start_flag == 1)
	{
		if(first_flag == 1)
		{
			switch(Task)
			{
				case 1: Task_1(); break;
				case 2: Task_2(); break;
				case 3: Task_3(); break;
				case 4: Task_4(); break;
			}
		}
	}
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

