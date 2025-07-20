#include "headfile.h"

#define TURNTIME 11900
#define DISTACNE1 100
#define DISTACNE2 100

uint8_t workstep = 0;
int16_t turn_time = 0;
uint8_t turn_flag = 0;
uint8_t lap_count = 0;
uint8_t Line_flag = 0;
uint8_t baisetime = 0;

void Task_1(void)
{
	switch(workstep)
	{
		case 0: 
			SoundLight();
			//test();
			workstep++;
			break;
		
		case 1:  
			
			workstep++;
			break;
			
		case 2:  
			
			params_clear();
			break;
	}
}

void Task_2(void)
{
	switch(workstep)
	{
		case 0: 
			//参数初始化
			break;
		
		case 1:  
			
			workstep++;
			break;
		
		case 2: 
			SoundLight();
			workstep++;
			break;
			
		case 3: 
			SoundLight();
			break;
			
		case 4: 
			SoundLight();
			workstep++;
			break;
			
		case 5:	
			params_clear();
			break;	
	}
}

void Task_3(void)
{
	switch(workstep)
	{
		case 0: 
			//参数初始化
			workstep++;
			break;
		
		case 1: 
			// 在一段时间里执行什么动作
			while(turn_flag) 
			{
				turn_time++;
				if(turn_time > TURNTIME)
				{	
				}
			}
			workstep++;
			break;
			
		case 2:  
			SoundLight();
			while(Line_flag)
			{
			}
			workstep ++;
			break;
			
		case 3:  
			SoundLight();
			basespeed = 0;
			turn_flag = 1;
			while(turn_flag)
			{
				turn_time++;
				if(turn_time > TURNTIME)
				{	
					baisetime = 0;
					turn_time = 0;
					turn_flag = 0;
				}
			}
			basespeed = 480;
			while(Line_flag == 0)
			{
			}
			workstep++;
			break;
		
		case 4:  
			SoundLight();
			basespeed = 390;
			while(Line_flag)
			{

			}
			workstep++;
			break;
			
		case 5:  
			SoundLight(); 
			params_clear();
			break;	
	}
}

void Task_4(void)
{
	switch(workstep)
	{
		case 0: 
			//参数初始化
			Line_flag = 0;
			workstep++;
			break;
		
		case 1: 
			basespeed = 0;
			turn_flag = 1;

			while(turn_flag)
			{

				turn_time++;
				if(turn_time > TURNTIME)
				{	
					baisetime = 0;
					turn_time = 0;
					turn_flag = 0;
				}
			}
			while(Line_flag == 0)
			{
			}
			workstep++;
			break;
			
		case 2:  
			SoundLight();
			while(Line_flag)
			{
			}
			workstep ++;
			break;
			
		case 3:  
			SoundLight();
			basespeed = 0;
			turn_flag = 1;
			while(turn_flag)
			{
				turn_time++;
				if(turn_time > TURNTIME)
				{	
					baisetime = 0;
					turn_time = 0;
					turn_flag = 0;
				}
			}
			while(Line_flag == 0)
			{

			}
			workstep++;
			break;
		
		case 4:  
			SoundLight();
			while(Line_flag)
			{

			}
			workstep++;
			break;
			
		case 5:  
			SoundLight();
			lap_count++; // 圈数
			Line_flag = 0;
			if(lap_count == 1)
			{

			}
			if(lap_count == 2)
			{
			}
			if(lap_count == 3)
			{
			}
			if(lap_count < 4) 
			{   
				baisetime = 0;
				workstep = 0; 
			} 
			else if(lap_count >= 4)
			{
				params_clear();
			}
			break;	
	}
}

