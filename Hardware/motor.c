#include "headfile.h"
#include "ti_msp_dl_config.h"

// X 400步/圈
// Y 2170步/圈
MOTOR_t StepX;
MOTOR_t StepY;

static void StepX_Start(uint8_t dir, uint16_t steps)
{	
	StepX.tar = steps;   // 设置要走的步数
	StepX.count = 0;
	StepX.state = 0;
	if (dir)		DL_GPIO_setPins(StepX_PORT, StepX_DirX_PIN);    
	else			DL_GPIO_clearPins(StepX_PORT, StepX_DirX_PIN); 
	DL_TimerA_startCounter(TIMER_0_INST);  
}

static void StepY_Start(uint8_t dir, uint16_t steps)
{
	StepY.tar = steps;   // 设置要走的步数
    StepY.count = 0;
    StepY.state = 0;
	if (dir)		DL_GPIO_setPins(StepY_PORT, StepY_DirY_PIN); 
	else			DL_GPIO_clearPins(StepY_PORT, StepY_DirY_PIN);  
    DL_TimerG_startCounter(TIMER_1_INST);  
}

static void ChangeStepX(void)
{
	if (StepX.state == 0)
	{
		DL_GPIO_setPins(StepX_PORT, StepX_MotorX_PIN);  
		StepX.state = 1;
	}
	else
	{
		DL_GPIO_clearPins(StepX_PORT, StepX_MotorX_PIN);  
		StepX.state = 0;

		if (++StepX.count >= StepX.tar)
		{
			DL_TimerA_stopCounter(TIMER_0_INST);
			StepX.count = 0;
		}
	}
}

static void ChangeStepY(void)
{
	if (StepY.state == 0)
	{
		DL_GPIO_setPins(StepY_PORT, StepY_MotorY_PIN);  
		StepY.state = 1;
	}
	else
	{
		DL_GPIO_clearPins(StepY_PORT, StepY_MotorY_PIN); 
		StepY.state = 0;

		if (++StepY.count >= StepY.tar)
		{
			DL_TimerG_stopCounter(TIMER_1_INST);
			StepY.count = 0;
		}
	}
}

static int16_t steps_to_angle(float angle, float change_angle)
{
    return (int16_t)(angle *change_angle/ 360.0f);  // X轴 400步/圈
}

void StepXSetAngle(float target)
{
    float err = target - StepX.current;
    uint8_t dir = (err >= 0) ? 1 : 0;
    int16_t steps = steps_to_angle(fabsf(err), 400.0f);
    StepX.current = target;  
    StepX_Start(dir, steps);
}

void StepYSetAngle(float target)
{
    float err = target - StepY.current;
    uint8_t dir = (err >= 0) ? 1 : 0;
    int16_t steps = steps_to_angle(fabsf(err), 2170.0f);
    StepY.current = target;  
    StepY_Start(dir, steps);
}

void TIMER_0_INST_IRQHandler(void)
{
    if (DL_TimerA_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
    {
        ChangeStepX();
    }
}

void TIMER_1_INST_IRQHandler(void)
{
    if (DL_TimerG_getPendingInterrupt(TIMER_1_INST) == DL_TIMER_IIDX_LOAD)
    {
        ChangeStepY();
    }
}

