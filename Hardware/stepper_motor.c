#include "headfile.h"
#include "ti_msp_dl_config.h"

// X 400步/圈
// Y 2170步/圈


int16_t stepX_target=0, stepX_count = 0, stepX_pin_state = 0;
int16_t stepY_target=0, stepY_count = 0, stepY_pin_state = 0;
// 
void Stepper_X_Start(uint8_t dir, uint16_t steps)
{
    if (dir)
		DL_GPIO_setPins(Stepper_Motor_X_PORT, Stepper_Motor_X_Dir_X_PIN);     // DIR = 1
	else
		DL_GPIO_clearPins(Stepper_Motor_X_PORT, Stepper_Motor_X_Dir_X_PIN);   // DIR = 0
	  // 设置方向

    stepX_target = steps;   // 设置要走的步数
    stepX_count = 0;
    stepX_pin_state = 0;

    DL_TimerA_startCounter(TIMER_0_INST);  // 启动定时器
}

void Stepper_Y_Start(uint8_t dir, uint16_t steps)
{
	if (dir)
	{
		DL_GPIO_setPins(Stepper_Motor_Y_PORT, Stepper_Motor_Y_Dir_Y_PIN);     // DIR = 1
	}
	else
	{
		DL_GPIO_clearPins(Stepper_Motor_Y_PORT, Stepper_Motor_Y_Dir_Y_PIN);   // DIR = 0
	}
	stepY_target = steps;   // 设置要走的步数
    stepY_count = 0;
    stepY_pin_state = 0;

    DL_TimerG_startCounter(TIMER_1_INST);  // 启动定时器
}

void stepX(void)
{
	LED_Green_ON();
	if (stepX_pin_state == 0)
	{
		DL_GPIO_setPins(Stepper_Motor_X_PORT, Stepper_Motor_X_Step_X_PIN);  // STEP高
		stepX_pin_state = 1;
	}
	else
	{
		DL_GPIO_clearPins(Stepper_Motor_X_PORT, Stepper_Motor_X_Step_X_PIN);  // STEP低
		stepX_pin_state = 0;

		if (++stepX_count >= stepX_target)
		{
			DL_TimerA_stopCounter(TIMER_0_INST);
			stepX_count = 0;
		}
	}
}

void stepY(void)
{
	LED_Green_OFF();
	if (stepY_pin_state == 0)
	{
		DL_GPIO_setPins(Stepper_Motor_Y_PORT, Stepper_Motor_Y_Step_Y_PIN);  // STEP高
		stepY_pin_state = 1;
	}
	else
	{
		DL_GPIO_clearPins(Stepper_Motor_Y_PORT, Stepper_Motor_Y_Step_Y_PIN);  // STEP低
		stepY_pin_state = 0;

		if (++stepY_count >= stepY_target)
		{
			DL_TimerG_stopCounter(TIMER_1_INST);
			stepY_count = 0;
		}
	}
}
float current_angle_X = 0.0f;
float current_angle_Y = 0.0f;

int16_t angle_to_steps_X(float angle_deg)
{
    return (int16_t)(angle_deg * 400.0f / 360.0f);  // X轴 400步/圈
}

int16_t angle_to_steps_Y(float angle_deg)
{
    return (int16_t)(angle_deg * 2170.0f / 360.0f);  // Y轴 2170步/圈
}

void Stepper_X_Goto_Angle(float target_angle_deg)
{
    float delta_angle = target_angle_deg - current_angle_X;
    uint8_t dir = (delta_angle >= 0) ? 1 : 0;
    int16_t steps = angle_to_steps_X(fabsf(delta_angle));

    current_angle_X = target_angle_deg;  // 更新当前角度
    Stepper_X_Start(dir, steps);
}

void Stepper_Y_Goto_Angle(float target_angle_deg)
{
    float delta_angle = target_angle_deg - current_angle_Y;
    uint8_t dir = (delta_angle >= 0) ? 1 : 0;
    int16_t steps = angle_to_steps_Y(fabsf(delta_angle));

    current_angle_Y = target_angle_deg;  // 更新当前角度
    Stepper_Y_Start(dir, steps);
}

